/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderFlattenTree.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderFlattenTree.c $    
    
    7     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    6     6/13/00 11:09a Ryan
    bug fixes found while working on cardemo
    
    5     6/07/00 1:08p Ryan
    checkin for shader optimization v1.0
    
    4     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    3     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>

#include "shaderFlattenTree.h"
#include "shaderResManage.h"
#include "shaderPruneTree.h"

/*>*******************************(*)*******************************<*/
static void FindSchedulableInstructions	( ShaderTEVStage *stage, 
										  ShaderTEVStage **SIList, 
										  u8 *numSI );

static u8 IsInstructionInSIList	( ShaderTEVStage *stage, 
								  ShaderTEVStage **SIList, u8 numSI );

static u8	Flatten				( ShaderTEVStage **SIList, u8 numSI );
static u8	AddInstruction		( ShaderTEVStage *stage );
static void RemoveInstruction	( ShaderTEVStage *stage, u8 index );
static u8	FindStartPoint		( ShaderTEVStage *stage );

static void RemoveInstructionFromSIList	( u8 index, 
										  ShaderTEVStage **SIList, 
										  u8 *numSI );

static void RemoveInstructionFromSIList2	( ShaderTEVStage *stage, 
											  ShaderTEVStage **SIList, 
											  u8 *numSI );

static u8 CheckParallelList	( ShaderTEVStage *stage1, ShaderTEVStage *stage2 );

static void AddInstructionToSIList	( ShaderTEVStage *instruction, 
									  ShaderTEVStage **SIList, 
									  u8 *numSI );

static void AddInstructionToSIList2	( ShaderTEVStage *instruction, 
									  u8 index, 
									  ShaderTEVStage **SIList, 
									  u8 *numSI );

//static void FindNumOptimalTEVStages ( ShaderTEVStage *stage );

static void PadInstructionList		( void );
static void UnPadInstructionList	( void );

static u8 AddRootAlphaInstruction	( ShaderTEVStage *stage );
static u8 FindLastAlphaInstruction	( void );

/*>*******************************(*)*******************************<*/
ShaderTEVStage EmptyAlphaInstruction = 
{
	SHADER_TEV_ALPHA,
	SHADER_TYPE_EMPTY,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0,
	0,
	SHADER_APREV, SHADER_ZERO, SHADER_ZERO, SHADER_ZERO,
	0, 0, 0, 0,
    GX_TEV_ADD,
	GX_TB_ZERO,
	GX_CS_SCALE_1,
    GX_FALSE,
	GX_TC_LINEAR,
    SHADER_APREV,
    SHADER_RASNONE,
	NO_TEXTURE_COORD_EXP,
	SHADER_TEXNONE,
	SHADER_SIMPLE,
	0,
	SHADER_APREV
};

ShaderTEVStage EmptyColorInstruction = 
{
	SHADER_TEV_COLOR,
	SHADER_TYPE_EMPTY,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0,
	0,
	SHADER_CPREV, SHADER_ZERO, SHADER_ZERO, SHADER_ZERO,
	0, 0, 0, 0,
    GX_TEV_ADD,
	GX_TB_ZERO,
	GX_CS_SCALE_1,
    GX_FALSE,
	GX_TC_LINEAR,
    SHADER_CPREV,
    SHADER_RASNONE,
	NO_TEXTURE_COORD_EXP,
	SHADER_TEXNONE,
	SHADER_SIMPLE,
	0,
	SHADER_CPREV
};

static ShaderTEVStage *AllocationList[32];

/*>*******************************(*)*******************************<*/
void FlattenTEVTree	( void )
{
	ShaderTEVStage *SIList[32];
	u8 numSI = 0;
    u8 i;

	//BestAlpha = 0;
	//BestColor = 0;

    //reset the optimal TEV stage counters
    //for ( i = 0; i < 16; i ++ )
    //{
    //    OptimalColorStages[i] = 0;
    //    OptimalAlphaStages[i] = 0;
    //}

    //reset instruction lists
    for ( i = 0; i < 16; i ++ )
	{
		AlphaInstructions[i] = 0;
		ColorInstructions[i] = 0;
	}

    //find the number of unique TEV stages in this shader
	//FindNumOptimalTEVStages(&(TEVPool[0]));

	//if(BestAlpha > BestColor)
	//	BestColor = BestAlpha;

    //BestColor = OptimalTreeSize;

    //increment root Alpha node so that it is always allocated last
    //TEVPool[0].tevStage[1]->numNonAllocatedChildren = 1;
    TEVPool[0].tevStage[1]->numNonAllocatedChildren = 1;

	//FindSchedulableInstructions(&(TEVPool[0]), SIList, &numSI);
    for ( i = 0; i < 32; i ++ )
	{
		AllocationList[i] = 0;
	}
    FindSchedulableInstructions(&(TEVPool[0]), SIList, &numSI);

	Flatten(SIList, numSI);
}

/*>*******************************(*)*******************************<*/
static void FindSchedulableInstructions	( ShaderTEVStage *stage, ShaderTEVStage **SIList, u8 *numSI )
{
	u8 i, j;

    //stage->numNonAllocatedChildren = 0;

    //check to see if stage has already been checked
    for ( j = 0; j < 32; j ++ )
	{
        if(AllocationList[j] == stage) 
            return; //stage has already been checked - return

        if(!(AllocationList[j]))
        {
            AllocationList[j] = stage;
            
            for ( i = 0; i < 4; i ++ )
	        {
		        if(stage->tevStage[i])
		        {
			        FindSchedulableInstructions(stage->tevStage[i], SIList, numSI);
			        stage->numNonAllocatedChildren ++;
		        }
	        }

	        if(!(stage->numNonAllocatedChildren)) // Leaf node so it is schedulable
	        {
		        //check if stage is already in SIList
		        if(IsInstructionInSIList(stage, SIList, *numSI))
		        {
			        SIList[*numSI] = stage;
			        (*numSI)++;
		        }
	        }

            break;
        }
    }

	
}

/*>*******************************(*)*******************************<*/
static u8 IsInstructionInSIList	( ShaderTEVStage *stage, ShaderTEVStage **SIList, u8 numSI )
{
	u8 i; 

	for ( i = 0; i < numSI; i ++ )
	{
		if(SIList[i] == stage)
			return 0;
	}

	return 1;
}

/*>*******************************(*)*******************************<*/
static u8 Flatten	( ShaderTEVStage **SIList, u8 numSI )
{
	u8 i, j;
	ShaderTEVStage *currentInstruction;
	u8 instrIndex;
	u8 cost;

	if(numSI)
	{
		for ( i = 0; i < numSI; i ++ )
		{
			currentInstruction = SIList[i];

			instrIndex = AddInstruction(currentInstruction);

			if(instrIndex != 99) // if instruction was placed into the list
			{
				RemoveInstructionFromSIList(i, SIList, &numSI);
				for ( j = 0; j < currentInstruction->numParents; j ++ )
				{
					//decrement parent nodes
					(currentInstruction->parent[j])->numNonAllocatedChildren --;

					//add parents into SIList if necessary
					if((!((currentInstruction->parent[j])->numNonAllocatedChildren)) && 
                        (currentInstruction->parent[j])->type != SHADER_TYPE_EMPTY)
						AddInstructionToSIList(currentInstruction->parent[j], SIList, &numSI);
				}
				
				// call flatten again to add another instr. into the list
				if(Flatten(SIList, numSI)) // we've realized the best possible solution, so get out
					return 1;

				for ( j = 0; j < currentInstruction->numParents; j ++ )
				{
					//increment parent nodes
					(currentInstruction->parent[j])->numNonAllocatedChildren ++;

					//remove from SIList if necessary
					if((currentInstruction->parent[j])->numNonAllocatedChildren == 1)
						RemoveInstructionFromSIList2(currentInstruction->parent[j], SIList, &numSI);
				}
				
				//remove currentInstruction from list
				RemoveInstruction(currentInstruction, instrIndex);

				//add currentInstruction to SIList at i
				AddInstructionToSIList2(currentInstruction, i, SIList, &numSI);
				
			}
		}
	}
	else
	{
        // add root alpha stage to the end of the list
        //instrIndex = AddRootAlphaInstruction(TEVPool[0].tevStage[1]);
        instrIndex = AddRootAlphaInstruction(TEVPool[0].tevStage[1]);

        if(instrIndex != 99) // if instruction was placed into the list
        {
		    // fill up empty holes in the instruction list with empty TEV stages
		    PadInstructionList();

		    // allocate resources
		    cost = CheckResourceAllocation();

		    if(cost < BestNumTEVStages)
		    {
			    // everything worked!  Copy this method!
			    BestNumTEVStages = cost;
			    for ( i = 0; i < BestNumTEVStages; i ++ )
			    {
				    if(AlphaInstructions[i])
					    BestAlphaInstructions[i] = *(AlphaInstructions[i]);
				    else
					    BestAlphaInstructions[i] = EmptyAlphaInstruction;

				    if(ColorInstructions[i])
					    BestColorInstructions[i] = *(ColorInstructions[i]);
				    else
					    BestColorInstructions[i] = EmptyColorInstruction;
			    }
		    }

            if(cost == OptimalTreeSize) // we've realized the best possible solution
			    return 1;
            if(cost <= OptimalTreeSize)
                OSHalt("cost is fewer than optimal - error in compile!");

		    // remove empty TEV stages from the list
		    UnPadInstructionList();

            //remove root alpha node
            //RemoveInstruction(TEVPool[0].tevStage[1], instrIndex);
            RemoveInstruction(TEVPool[0].tevStage[1], instrIndex);
        }
	}

	return 0;
}

/*>*******************************(*)*******************************<*/
static u8 AddInstruction	( ShaderTEVStage *stage )
{
	ShaderTEVStage **currentList, **parallelList;
	u8 startPoint;
	u8 i;
	
	if(stage->channel == SHADER_TEV_COLOR)
	{
		currentList = ColorInstructions;
		parallelList = AlphaInstructions;
	}
	else
	{
		currentList = AlphaInstructions;
		parallelList = ColorInstructions;
	}

	startPoint = FindStartPoint(stage);

	//start at startpoint and see if we can fit this node into the list
	for ( i = startPoint; i < 16; i ++ )
	{
		if(!(currentList[i]))
		{
			if(CheckParallelList(stage, parallelList[i]))
			{
				currentList[i] = stage;
				return i;
			}
		}
	}

	return 99;
}

/*>*******************************(*)*******************************<*/
static void RemoveInstruction	( ShaderTEVStage *stage, u8 index )
{
	ShaderTEVStage **currentList;
	
	if(stage->channel == SHADER_TEV_COLOR)
		currentList = ColorInstructions;
	else
		currentList = AlphaInstructions;
	
	currentList[index] = 0;
}

/*>*******************************(*)*******************************<*/
static u8 FindStartPoint	( ShaderTEVStage *stage )
{
	u8 i, j;
	u8 numChildren = 0;
	u8 numNotFound;
	ShaderTEVStage	*tevStage[4];

	for ( i = 0; i < 4; i ++ )
	{
		if(stage->tevStage[i])
		{
			tevStage[numChildren] = stage->tevStage[i];
			numChildren ++;
		}
	}

	if(numChildren)
	{
		numNotFound = numChildren;

		for ( i = 0; i < 16; i ++)
		{
			for ( j = 0; j < numChildren; j ++ )
			{
				if(ColorInstructions[i] == tevStage[j] || 
				   AlphaInstructions[i] == tevStage[j])
				{
					numNotFound --;
					if(!(numNotFound))
						return (u8)(i + 1);
				}
			}
		}
	}

	return 0;
}

/*>*******************************(*)*******************************<*/
static void RemoveInstructionFromSIList	( u8 index, ShaderTEVStage **SIList, u8 *numSI )
{
	u8 i; 

	for ( i = index; i < (*numSI) - 1; i ++ )
	{
		SIList[i] = SIList[i + 1];
	}

	(*numSI)--;
}

/*>*******************************(*)*******************************<*/
static void RemoveInstructionFromSIList2	( ShaderTEVStage *stage, ShaderTEVStage **SIList, u8 *numSI )
{
	u8 i; 

	for ( i = 0; i < (*numSI); i ++ )
	{
		if(SIList[i] == stage)
			break;
	}

	for ( i; i < (*numSI) - 1; i ++ )
	{
		SIList[i] = SIList[i + 1];
	}

	(*numSI)--;
}

/*>*******************************(*)*******************************<*/
static void AddInstructionToSIList	( ShaderTEVStage *instruction, ShaderTEVStage **SIList, u8 *numSI )
{
	SIList[*numSI] = instruction;
	
	(*numSI)++;
}

/*>*******************************(*)*******************************<*/
static void AddInstructionToSIList2	( ShaderTEVStage *instruction, u8 index, ShaderTEVStage **SIList, u8 *numSI )
{
	ShaderTEVStage *temp;
	u8 i; 

	(*numSI)++;

	for ( i = index; i < *numSI; i ++ )
	{
		temp = SIList[i];
		SIList[i] = instruction;
		instruction = temp;
		
	}
}

/*>*******************************(*)*******************************<*/
static u8 CheckParallelList	( ShaderTEVStage *stage1, ShaderTEVStage *stage2 )
{

	if(!(stage2))
		return 1;

	if(!(stage1->texGenIdx == NO_TEXTURE_COORD_EXP || 
	     stage2->texGenIdx == NO_TEXTURE_COORD_EXP || 
	     stage2->texGenIdx == stage1->texGenIdx))
		return 0;

	if(!(stage1->texInput == SHADER_TEXNONE || 
		 stage2->texInput == SHADER_TEXNONE || 
		 stage2->texInput == stage1->texInput))
		return 0;

	if(!(stage1->rasInput == SHADER_RASNONE || 
		 stage2->rasInput == SHADER_RASNONE || 
		 stage2->rasInput == stage1->rasInput))
		return 0;

	if(stage1->mode != stage2->mode)
		return 0;	

	return 1;
}

/*>*******************************(*)*******************************<*
static void FindNumOptimalTEVStages ( ShaderTEVStage *stage )
{
	u8 i;

    if(stage->type != SHADER_TYPE_EMPTY)
    {
	    if(stage->channel == SHADER_TEV_COLOR)
        {
            for ( i = 0; i < 16; i ++ )
            {
                if(stage == OptimalColorStages[i])
                    break;
                if(!(OptimalColorStages[i]))
                {
                    OptimalColorStages[i] = stage;
		            BestColor ++;
                    break;
                }
            }
        }
	    else
        {
            for ( i = 0; i < 16; i ++ )
            {
                if(stage == OptimalAlphaStages[i])
                    break;
                if(!(OptimalAlphaStages[i]))
                {
                    OptimalAlphaStages[i] = stage;
		            BestAlpha ++;
                    break;
                }
            }
        }
    }

	if(stage->type == SHADER_TYPE_COMPLEX || stage->type == SHADER_TYPE_EMPTY)
	{
		for(i = 0; i < 4; i ++ )
		{
			if(stage->tevStage[i])
				FindNumOptimalTEVStages(stage->tevStage[i]);
		}
	}
}

/*>*******************************(*)*******************************<*/
static void PadInstructionList ( void )
{
	u8 i;

	for ( i = 0; i < 16; i ++ )
	{
		if(!(ColorInstructions[i]) && !(AlphaInstructions[i]))
			break;

		if(!(ColorInstructions[i]))
		{
			//if the parallel stage uses a weird clamp mode, allocate an empty stage.
			if(AlphaInstructions[i]->mode != GX_TC_LINEAR)
				ColorInstructions[i] = &(TEVPool[TEVCounter++]);
		}
		if(!(AlphaInstructions[i]))
		{
			//if the parallel stage uses a weird clamp mode, allocate an empty stage.
			if(ColorInstructions[i]->mode != GX_TC_LINEAR)
				AlphaInstructions[i] = &(TEVPool[TEVCounter++]);
		}
	}
}

/*>*******************************(*)*******************************<*/
static void UnPadInstructionList ( void )
{
	u8 i;

	for ( i = 0; i < 16; i ++ )
	{
		if(!(ColorInstructions[i]) && !(AlphaInstructions[i]))
			break;

		if(ColorInstructions[i] && ColorInstructions[i]->type == SHADER_TYPE_EMPTY)
		{
			ColorInstructions[i] = 0;
			TEVCounter--;
		}
		if(AlphaInstructions[i] && AlphaInstructions[i]->type == SHADER_TYPE_EMPTY)
		{
			AlphaInstructions[i] = 0;
			TEVCounter--;
		}
	}
}

/*>*******************************(*)*******************************<*/
static u8 AddRootAlphaInstruction	( ShaderTEVStage *stage )
{
	u8 startPoint;
	u8 i;

	startPoint = FindLastAlphaInstruction();
    if(startPoint == 99)
        startPoint = 0;
    else
        startPoint ++; //increment startpoint since it returns the index of the last instr.

	//start at startpoint and see if we can fit this node into the list
	for ( i = startPoint; i < 16; i ++ )
	{
		if(!(AlphaInstructions[i]))
		{
			if(CheckParallelList(stage, ColorInstructions[i]))
			{
				AlphaInstructions[i] = stage;
				return i;
			}
		}
	}

	return 99;
}

/*>*******************************(*)*******************************<*/
static u8 FindLastAlphaInstruction	( void )
{
	u8 last = 99;
	u8 i;

	for ( i = 0; i < 16; i ++ )
	{
		if(AlphaInstructions[i])
            last = i;
    }

	return last;
}

/*>*******************************(*)*******************************<*/
void CopyCompiledTEVStages  ( CompiledTEVStage *compiled,
                              ShaderTEVStage *instructions, 
                              u32 numStages, 
                              CompiledShaderResources *resources )
{
    u32 i, j;

    for ( i = 0; i < numStages; i ++ )
    {
        for ( j = 0; j < 4; j ++ )
        {
            compiled[i].TEVArg[j] = instructions[i].TEVArg[j];

            if((u32)(compiled[i].TEVArg[j]) >= SHADER_COMPLEXINPUT0_RGB &&
               (u32)(compiled[i].TEVArg[j]) <= SHADER_COMPLEXINPUT7_A)
            {
                if((u32)(compiled[i].TEVArg[j]) <= SHADER_COMPLEXINPUT7_RGB)
                    resources->complexUsed[((u32)(compiled[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_RGB))] = 1;
                else   
                    resources->complexUsed[((u32)(compiled[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_A))] = 1;
            }
        }
	
        compiled[i].op = instructions[i].op;
        compiled[i].bias = instructions[i].bias;
        compiled[i].scale = instructions[i].scale;
        compiled[i].clamp = instructions[i].clamp;
	    compiled[i].mode = instructions[i].mode;
        compiled[i].out_reg = instructions[i].out_reg;

        compiled[i].rasInput = instructions[i].rasInput;
        compiled[i].texGenIdx = instructions[i].texGenIdx;
        compiled[i].texInput = instructions[i].texInput;

        //set up resources
        if(compiled[i].rasInput != SHADER_RASNONE)
            resources->rasUsed[(u32)(compiled[i].rasInput)] = 1;
        if(compiled[i].texInput != SHADER_TEXNONE)
            resources->textureUsed[(u32)(compiled[i].texInput)] = 1;
    }
}

/*>*******************************(*)*******************************<*/