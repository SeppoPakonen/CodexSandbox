/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderPruneTree.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderPruneTree.c $    
    
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

#include "shaderPruneTree.h"
#include "shaderResManage.h"
#include "shaderFlattenTree.h"

/*>*******************************(*)*******************************<*/
ShaderTEVStage	CollapsedTEVPool[33];
u8              OptimalTreeSize;

/*>*******************************(*)*******************************<*/
static ShaderTEVStage*  Children[32];
static ShaderTEVStage*  Parents[32];
static u8               CollapseIndex[32];
static u8               NumCandidates = 0;

#ifdef MAC
static u32              PermutationCounter = 0;
#endif

static u8 BestAlpha;
static u8 BestColor;

static ShaderTEVStage   *OptimalColorStages[16];
static ShaderTEVStage   *OptimalAlphaStages[16];

static ShaderTEVStage   *CopyList[33];

static ShaderTEVStage   *AddList[32];
static u8               AddListCounter;

/*>*******************************(*)*******************************<*/
static void PruneTEVStages	( ShaderTEVStage *stage );

static void FindLERPTypes	( ShaderTEVStage *stage, u8 recursion );

static void CollapseComplexTEVStages	( ShaderTEVStage *stage, 
										  ShaderTEVStage *parent, u8 index );

static u8 DetermineNonTrivialLERParams			( ShaderTEVStage *stage );
static u8 DetermineSimpleTEVType				( ShaderTEVStage *stage );
static SHADER_TEVArg DetermineTrivialTEVType	( ShaderTEVStage *stage );

static void ReParentTEVStage    ( ShaderTEVStage *oldParent, ShaderTEVStage *newParent, 
                                  ShaderTEVStage *current );

static void CreateCollapseList  ( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index );

static void CollapseRecursive   ( void );

static void CollapseTEVStageTRIVIAL	( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index );
static void CollapseTEVStageSIMPLE	( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index );
static void CollapseTEVStageLERP	( ShaderTEVStage *stage, ShaderTEVStage *parent );

static void RemoveStageFromLists	( u8 index );
static void AddStageToLists	        ( u8 index, ShaderTEVStage *stage, ShaderTEVStage *parent,
                                      u8 newIndex );

static void	AssignMiscParameters	( ShaderTEVStage *stage, ShaderTEVStage *parent );

static void DetachStage 	( ShaderTEVStage *stage, ShaderTEVStage *parent );
static void AttachStage 	( ShaderTEVStage *stage, ShaderTEVStage *parent );

static u8 ReParentTEVStageInCollapseList  ( ShaderTEVStage *oldParent, ShaderTEVStage *newParent, 
                                            ShaderTEVStage *current, u8 newIndex );

static void FindNumOptimalTEVStages ( ShaderTEVStage *stage );

//static void CopyOptimalTree ( void );
//static ShaderTEVStage *CopyStage	( ShaderTEVStage *stage, u8 *copied );

static void CreateAddList   ( ShaderTEVStage *stage );

static void PermuteTree ( void );

static void SetUpFlatten    ( ShaderTEVStage *stage );


/*>*******************************(*)*******************************<*/
void PruneTEVTree	( void )
{
    u8 i;

	PruneTEVStages(&(TEVPool[0]));

	FindLERPTypes(&(TEVPool[0]), 1);

    //create a list of all addition shaders

    

    //create a list of all add shaders in the tree
    //reset lists
    AddListCounter = 0;
    for ( i = 0; i < 32; i ++ )
    {
        AddList[i] = 0;
    }
    CreateAddList(&(TEVPool[0]));

    //reset the optimal tree size to 99
    OptimalTreeSize = 99;

    //start at front of list and collapse in all possible combinations
    //when there are no more candidates left in the list, check to see
    //if the tree is the most optimized yet.
    //permute addition shaders and collapse
    PermuteTree();
    
	//CollapseComplexTEVStages(&(TEVPool[0]), 0, 0);
}

/*>*******************************(*)*******************************<*/
static void PruneTEVStages	( ShaderTEVStage *stage )
{
	u8 i;

	if(stage->TEVArg[2] == SHADER_ZERO)
	{
		stage->TEVArg[1] = SHADER_ZERO;
		stage->tevStage[1] = 0;
	}
	else if(stage->TEVArg[2] == SHADER_ONE)
	{
		stage->TEVArg[0] = SHADER_ZERO;
		stage->tevStage[0] = 0;
	}
	else if(stage->TEVArg[0] == SHADER_ZERO && 
			stage->TEVArg[1] == SHADER_ZERO)
	{
		stage->TEVArg[2] = SHADER_ZERO;
		stage->tevStage[2] = 0;
	}
	else if(stage->TEVArg[0] == SHADER_ONE && 
			stage->TEVArg[1] == SHADER_ONE)
	{
		stage->TEVArg[2] = SHADER_ZERO;
		stage->tevStage[2] = 0;
	}

	for ( i = 0; i < 4; i ++ )
	{
		if(stage->tevStage[i])
			PruneTEVStages(stage->tevStage[i]);
	}
}

/*>*******************************(*)*******************************<*/
static void FindLERPTypes	( ShaderTEVStage *stage, u8 recursion )
{
	u8 i;
	u8 numNonTrivialParams;

	if(stage->type == SHADER_TYPE_COMPLEX)
	{
		//determine LERP type
		numNonTrivialParams = DetermineNonTrivialLERParams(stage);

		if(numNonTrivialParams > 1)
			stage->LERPType = SHADER_LERP;
		else if(numNonTrivialParams > 0)
		{
			stage->LERPType = SHADER_SIMPLE;
			stage->outputIndex = DetermineSimpleTEVType(stage);
		}
		else
		{
			stage->LERPType = SHADER_TRIVIAL;
			stage->outputArg = DetermineTrivialTEVType(stage);
		}
    }

	if(recursion)
	{
		for ( i = 0; i < 4; i ++ )
		{
			if(stage->tevStage[i])
			{
				FindLERPTypes(stage->tevStage[i], 1);
			}
		}
	}
}

/*>*******************************(*)*******************************<*
static void CollapseComplexTEVStages	( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index )
{
	u8 i;

	for ( i = 0; i < 4; i ++ )
	{
		if(stage->tevStage[i])
		{
			if(stage->tevStage[i]->type == SHADER_TYPE_COMPLEX)
				CollapseComplexTEVStages(stage->tevStage[i], stage, i);
		}
	}
	
	//check to see if this stage can collapse into its parent

	//check if LERP == 0 and d == 0

	if(stage->op == GX_TEV_ADD && parent  && parent->type != SHADER_TYPE_EMPTY)
	{
		switch(stage->LERPType)
		{
			case SHADER_TRIVIAL:
				if(stage->outputArg == SHADER_ZERO)
				{
					if(CheckResourceCollision(stage, parent))
					{
                        ReParentTEVStage(stage, parent, stage->tevStage[3]);

						parent->TEVArg[index] = stage->TEVArg[3];
						parent->tevStage[index] = stage->tevStage[3];
						FindLERPTypes(parent, 0);
					}
					break;
				}
				if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
				{
					if(CheckResourceCollision(stage, parent))
					{
						parent->TEVArg[index] = stage->outputArg;
						parent->tevStage[index] = 0;
						FindLERPTypes(parent, 0);
					}
				}
				break;
			case SHADER_SIMPLE:
				if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
				{
					if(CheckResourceCollision(stage, parent))
					{
                        ReParentTEVStage(stage, parent, stage->tevStage[stage->outputIndex]);

						parent->TEVArg[index] = stage->TEVArg[stage->outputIndex];
						parent->tevStage[index] = stage->tevStage[stage->outputIndex];
						FindLERPTypes(parent, 0);
					}
				}
				break;
			case SHADER_LERP:
				if(stage->TEVArg[3] == SHADER_ZERO && // d == 0
				   parent->LERPType == SHADER_SIMPLE)
				{
					if(CheckResourceCollision(stage, parent))
					{
                        ReParentTEVStage(stage, parent, stage->tevStage[0]);
                        ReParentTEVStage(stage, parent, stage->tevStage[1]);
                        ReParentTEVStage(stage, parent, stage->tevStage[2]);

						parent->TEVArg[0] = stage->TEVArg[0];
						parent->tevStage[0] = stage->tevStage[0];
						parent->TEVArg[1] = stage->TEVArg[1];
						parent->tevStage[1] = stage->tevStage[1];
						parent->TEVArg[2] = stage->TEVArg[2];
						parent->tevStage[2] = stage->tevStage[2];
						parent->LERPType = SHADER_LERP;

                        FindLERPTypes(parent, 0);
					}
				}
				break;
		}
	}
}

/*>*******************************(*)*******************************<*/
static u8 DetermineNonTrivialLERParams	( ShaderTEVStage *stage )
{
	u8 count = 0;
	u8 i;

	for ( i = 0; i < 3; i ++ )
	{
		if(stage->tevStage[i])
		{
			count ++;
			continue;
		}

		if(stage->TEVArg[i] != SHADER_ONE && stage->TEVArg[i] != SHADER_ZERO)
			count ++;
	}

	return count;
}

/*>*******************************(*)*******************************<*/
static u8 DetermineSimpleTEVType	( ShaderTEVStage *stage )
{
	u8 i;

	for ( i = 0; i < 3; i ++ )
	{
		if(stage->TEVArg[i] < SHADER_NONTRIVIAL)
			return i;
	}

	//never reached
	return 0;
}

/*>*******************************(*)*******************************<*/
static SHADER_TEVArg DetermineTrivialTEVType	( ShaderTEVStage *stage )
{
	if(stage->TEVArg[0] == SHADER_ZERO &&
	   stage->TEVArg[1] == SHADER_ZERO)
	   return SHADER_ZERO;

	return SHADER_ONE;
}

/*>*******************************(*)*******************************<*/
//This function is called when a TEV stage is collapsed over an existing
//TEV stage.  The child TEV stages need to be reparented to the new TEV
//stage.
/*>*******************************(*)*******************************<*/
static void ReParentTEVStage    ( ShaderTEVStage *oldParent, ShaderTEVStage *newParent, 
                                  ShaderTEVStage *current )
{
    u32 i;

    if(!current) return;

    for ( i = 0; i < 8; i ++ )
    {
        if(current->parent[i] == oldParent)
        {
            current->parent[i] = newParent;
            return;
        }
    }

    OSHalt("parent never found in shader prune TEV tree!!!");
}

/*>*******************************(*)*******************************<*/
//This function creates a list of all the collapseable TEV stages and 
//their parents.
/*>*******************************(*)*******************************<*/
static void CreateCollapseList  ( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index  )
{
    u8 i;

	for ( i = 0; i < 4; i ++ )
	{
		if(stage->tevStage[i])
		{
			if(stage->tevStage[i]->type == SHADER_TYPE_COMPLEX)
				CreateCollapseList(stage->tevStage[i], stage, i);
		}
	}

    if(parent && parent->type != SHADER_TYPE_EMPTY)
    {
           //check to make sure that stage CAN collapse into parent - if not, don't add it!

        //cannot collapse if op is not add
        if(stage->op == GX_TEV_ADD)
	    {
		    switch(stage->LERPType)
		    {
			    case SHADER_TRIVIAL:
				    if(stage->outputArg == SHADER_ZERO)//move up d parameter
	                {
		                if(!(CheckResourceCollision(stage, parent))) return;
	                }
	                else if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
	                {
		                if(!(CheckResourceCollision(stage, parent))) return;
	                }  
                    else
                        return;
				    break;
			    case SHADER_SIMPLE:
				    if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
	                {
		                if(!(CheckResourceCollision(stage, parent))) return;
                    }
                    else 
                        return;
				    break;
			    case SHADER_LERP:
				    if(stage->TEVArg[3] == SHADER_ZERO && // d == 0
	                   parent->LERPType == SHADER_SIMPLE &&
                       parent->tevStage[3] != stage) // make sure stage is not in the d position of parent
	                {
		                if(!(CheckResourceCollision(stage, parent))) return;
                    }
                    else 
                        return;
				    break;
		    }
	    }

        //check to see if stage and parent are already in the tree
        for ( i = 0; i < NumCandidates; i ++ )
        {
            if(Children[i] == stage && Parents[i] == parent) return;
        }

        Children[NumCandidates] = stage;
        Parents[NumCandidates] = parent;
        CollapseIndex[NumCandidates] = index;
        NumCandidates ++;

        ASSERTMSG(NumCandidates < 32, "collapse problems in CreateCollapseList");
    }
}

/*>*******************************(*)*******************************<*/
//This function collapses the collapse lists and tries to flatten
//the TEV tree in the most efficient manner
/*>*******************************(*)*******************************<*/
static void CollapseRecursive ( void )
{
    u8 i;

    ShaderTEVStage *stage; 
    ShaderTEVStage *parent; 
    u8 index;

    if(NumCandidates)
    {
        for ( i = 0; i < NumCandidates; i ++ )
        {
            //set up temporary variables
            stage = Children[i];
            parent = Parents[i];
            index = CollapseIndex[i];

            //remove ith candidate from list
            RemoveStageFromLists(i);

            //cannot collapse if op is not add
            if(stage->op == GX_TEV_ADD)
	        {
		        switch(stage->LERPType)
		        {
			        case SHADER_TRIVIAL:
				        CollapseTEVStageTRIVIAL(stage, parent, index);   
				        break;
			        case SHADER_SIMPLE:
				        CollapseTEVStageSIMPLE(stage, parent, index);   
				        break;
			        case SHADER_LERP:
				        CollapseTEVStageLERP(stage, parent);   
				        break;
		        }
	        }

            //add ith candidate back into list
            AddStageToLists(i, stage, parent, index);
        }
    }
    else
    {
#ifdef MAC
        OSReport("trying another collapse permutation %d\n", PermutationCounter);
        PermutationCounter ++;
#endif

        //check number of stages in collapsed tree
        //find the number of unique TEV stages in this shader

        BestColor = BestAlpha = 0;
        //reset the optimal TEV stage counters
        for ( i = 0; i < 16; i ++ )
        {
            OptimalColorStages[i] = 0;
            OptimalAlphaStages[i] = 0;
        }

	    FindNumOptimalTEVStages(&(TEVPool[0]));
	    if(BestAlpha > BestColor)
	    	BestColor = BestAlpha;

        OptimalTreeSize = BestColor;

        //check to see if this permutation can possibly collapse further
        //than previous collapses

        if(OptimalTreeSize < BestNumTEVStages)
        {
            SetUpFlatten(&(TEVPool[0]));
            FlattenTEVTree();
        }

/*
#ifdef MAC
        OSReport("cost is %d ", BestColor);
#endif

        //if stages < optimal
        if(BestColor < 33 && BestColor < OptimalTreeSize)
        {//save current tree
            OptimalTreeSize = BestColor;
#ifdef MAC
            OSReport("copying tree");
#endif
            CopyOptimalTree();
        }
#ifdef MAC        
        OSReport("\n");
#endif*/
    }
}

/*>*******************************(*)*******************************<*/
static void CollapseTEVStageTRIVIAL	( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index )
{
    u8 whichCollapse = 0;

    SHADER_TEVArg       oldARG;
    ShaderTEVStage*     oldStage;
    SHADER_LERPTypes    oldLERPType;

    u8                  oldCoord;
    SHADER_TexInput     oldMap;
    SHADER_RasInput     oldColor;

    u8 oldIdx;

    //collapse stage
	if(stage->outputArg == SHADER_ZERO)//move up d parameter
	{
		if(CheckResourceCollision(stage, parent))
		{
            oldARG = parent->TEVArg[index];
            oldStage = parent->tevStage[index];
            oldLERPType = parent->LERPType;

            ReParentTEVStage(stage, parent, stage->tevStage[3]);

            oldIdx = ReParentTEVStageInCollapseList(stage, parent, stage->tevStage[3], index);

			parent->TEVArg[index] = stage->TEVArg[3];
			parent->tevStage[index] = stage->tevStage[3];
			FindLERPTypes(parent, 0);

            //remove stage from parent
            DetachStage(stage, parent);

            whichCollapse = 1;
		}
	}
	else if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
	{
		if(CheckResourceCollision(stage, parent))
		{
            oldARG = parent->TEVArg[index];
            oldStage = parent->tevStage[index];
            oldLERPType = parent->LERPType;

			parent->TEVArg[index] = stage->outputArg;
			parent->tevStage[index] = 0;
			FindLERPTypes(parent, 0);

            //remove stage from parent
            DetachStage(stage, parent);

            whichCollapse = 2;
		}
	}

    //deal with textures, coordinates, colors
    if(whichCollapse)
    {
        oldCoord = parent->texGenIdx;
        oldMap = parent->texInput;
        oldColor = parent->rasInput;

        AssignMiscParameters(stage, parent);
    }

    //recur
    CollapseRecursive();

    //un-collapse stage
    if(whichCollapse)
    {
        if(whichCollapse == 1)
	    {
            ReParentTEVStage(parent, stage, stage->tevStage[3]);
            if(oldIdx != 99)
                ReParentTEVStageInCollapseList(parent, stage, stage->tevStage[3], oldIdx);
        }

    	parent->TEVArg[index] = oldARG;
    	parent->tevStage[index] = oldStage;
    	parent->LERPType = oldLERPType;

        //deal with textures, coordinates, colors
        parent->texGenIdx = oldCoord;
        parent->texInput = oldMap;
        parent->rasInput = oldColor;

        //reattach stage to parent
        AttachStage(stage, parent);
    }
}

/*>*******************************(*)*******************************<*/
static void CollapseTEVStageSIMPLE	( ShaderTEVStage *stage, ShaderTEVStage *parent, u8 index )
{			
    u8 whichCollapse = 0;

    SHADER_TEVArg       oldARG;
    ShaderTEVStage*     oldStage;
    SHADER_LERPTypes    oldLERPType;

    u8                  oldCoord;
    SHADER_TexInput     oldMap;
    SHADER_RasInput     oldColor;

    u8 oldIdx;

	if(stage->TEVArg[3] == SHADER_ZERO) // d == 0
	{
		if(CheckResourceCollision(stage, parent))
		{
            oldARG = parent->TEVArg[index];
            oldStage = parent->tevStage[index];
            oldLERPType = parent->LERPType;

            oldCoord = parent->texGenIdx;
            oldMap = parent->texInput;
            oldColor = parent->rasInput;

            AssignMiscParameters(stage, parent);

            ReParentTEVStage(stage, parent, stage->tevStage[stage->outputIndex]);

            oldIdx = ReParentTEVStageInCollapseList(stage, parent, stage->tevStage[stage->outputIndex], index);

            //remove stage from parent
            DetachStage(stage, parent);

			parent->TEVArg[index] = stage->TEVArg[stage->outputIndex];
			parent->tevStage[index] = stage->tevStage[stage->outputIndex];
			FindLERPTypes(parent, 0);

            whichCollapse = 1;
		}
	}

    //recur
    CollapseRecursive();

    //un-collapse stage
    if(whichCollapse)
	{
        ReParentTEVStage(parent, stage, stage->tevStage[stage->outputIndex]);

        if(oldIdx != 99)
                ReParentTEVStageInCollapseList(parent, stage, stage->tevStage[stage->outputIndex], oldIdx);

		parent->TEVArg[index] = oldARG;
		parent->tevStage[index] = oldStage;
		parent->LERPType = oldLERPType;	

        parent->texGenIdx = oldCoord;
        parent->texInput = oldMap;
        parent->rasInput = oldColor;

        //reattach stage to parent
        AttachStage(stage, parent);
	}
}

/*>*******************************(*)*******************************<*/
static void CollapseTEVStageLERP	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
    u8 whichCollapse = 0;

    SHADER_TEVArg       oldARG[3];
    ShaderTEVStage*     oldStage[3];
    SHADER_LERPTypes    oldLERPType;

    u8                  oldCoord;
    SHADER_TexInput     oldMap;
    SHADER_RasInput     oldColor;

    u8 oldIdx[3];

	if(stage->TEVArg[3] == SHADER_ZERO && // d == 0
	   parent->LERPType == SHADER_SIMPLE &&
       parent->tevStage[3] != stage) // make sure stage is not in the d position of parent
	{
		if(CheckResourceCollision(stage, parent))
		{
            oldARG[0] = parent->TEVArg[0];
            oldStage[0] = parent->tevStage[0];
            oldARG[1] = parent->TEVArg[1];
            oldStage[1] = parent->tevStage[1];
            oldARG[2] = parent->TEVArg[2];
            oldStage[2] = parent->tevStage[2];
            oldLERPType = parent->LERPType;

            oldCoord = parent->texGenIdx;
            oldMap = parent->texInput;
            oldColor = parent->rasInput;

            AssignMiscParameters(stage, parent);

            ReParentTEVStage(stage, parent, stage->tevStage[0]);
            ReParentTEVStage(stage, parent, stage->tevStage[1]);
            ReParentTEVStage(stage, parent, stage->tevStage[2]);

            oldIdx[0] = ReParentTEVStageInCollapseList(stage, parent, stage->tevStage[0], 0);
            oldIdx[1] = ReParentTEVStageInCollapseList(stage, parent, stage->tevStage[1], 1);
            oldIdx[2] = ReParentTEVStageInCollapseList(stage, parent, stage->tevStage[2], 2);

			parent->TEVArg[0] = stage->TEVArg[0];
			parent->tevStage[0] = stage->tevStage[0];
			parent->TEVArg[1] = stage->TEVArg[1];
			parent->tevStage[1] = stage->tevStage[1];
			parent->TEVArg[2] = stage->TEVArg[2];
			parent->tevStage[2] = stage->tevStage[2];

            FindLERPTypes(parent, 0);

            //remove stage from parent
            DetachStage(stage, parent);

            whichCollapse = 1;
		}
	}

    //recur
    CollapseRecursive();

    //un-collapse stage
    if(whichCollapse)
	{
        ReParentTEVStage(parent, stage, stage->tevStage[0]);
        ReParentTEVStage(parent, stage, stage->tevStage[1]);
        ReParentTEVStage(parent, stage, stage->tevStage[2]);

        if(oldIdx[0] != 99)
                ReParentTEVStageInCollapseList(parent, stage, stage->tevStage[0], oldIdx[0]);
        if(oldIdx[1] != 99)
                ReParentTEVStageInCollapseList(parent, stage, stage->tevStage[1], oldIdx[1]);
        if(oldIdx[2] != 99)
                ReParentTEVStageInCollapseList(parent, stage, stage->tevStage[2], oldIdx[2]);


		parent->TEVArg[0] = oldARG[0];
		parent->tevStage[0] = oldStage[0];
        parent->TEVArg[1] = oldARG[1];
		parent->tevStage[1] = oldStage[1];
        parent->TEVArg[2] = oldARG[2];
		parent->tevStage[2] = oldStage[2];
		parent->LERPType = oldLERPType;	

        parent->texGenIdx = oldCoord;
        parent->texInput = oldMap;
        parent->rasInput = oldColor;

        //reattach stage to parent
        AttachStage(stage, parent);
	}
}

/*>*******************************(*)*******************************<*/
static void RemoveStageFromLists	( u8 index )
{
	u8 i; 

	for ( i = index; i < NumCandidates - 1; i ++ )
	{
		Children[i] = Children[i + 1];
        Parents[i] = Parents[i + 1];
        CollapseIndex[i] = CollapseIndex[i + 1];
	}

	NumCandidates--;
}

/*>*******************************(*)*******************************<*/
static void AddStageToLists	( u8 index, ShaderTEVStage *stage, ShaderTEVStage *parent,
                              u8 newIndex )
{
	ShaderTEVStage *tempStage; 
    ShaderTEVStage *tempParent; 
    u8 tempIndex;

	u8 i; 

	NumCandidates++;

	for ( i = index; i < NumCandidates; i ++ )
	{
        tempStage = Children[i];
        tempParent = Parents[i];
        tempIndex = CollapseIndex[i];

        Children[i] = stage;
        Parents[i] = parent;
        CollapseIndex[i] = newIndex;
		
		stage = tempStage;
        parent = tempParent;
        newIndex = tempIndex;		
	}
}

/*>*******************************(*)*******************************<*/
static void	AssignMiscParameters	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
	if(stage->texGenIdx != NO_TEXTURE_COORD_EXP)
		parent->texGenIdx = stage->texGenIdx;
	
	if(stage->texInput != SHADER_TEXNONE)
		parent->texInput = stage->texInput;

	if(stage->rasInput != SHADER_RASNONE)
		parent->rasInput = stage->rasInput;
}

/*>*******************************(*)*******************************<*/
static void DetachStage 	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
    u32 i;

    //find parent in parent list
    for ( i = 0; i < stage->numParents; i ++ )
    {
        if(stage->parent[i] == parent)
            break;
    }
    
    //shift parents back over parent
    for ( i; i < (stage->numParents) - 1; i ++ )
    {
        stage->parent[i] = stage->parent[i + 1];
    }

    (stage->numParents)--;
    stage->parent[stage->numParents] = 0;
}

/*>*******************************(*)*******************************<*/
static void AttachStage 	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
    stage->parent[stage->numParents] = parent;
    (stage->numParents)++;
}

/*>*******************************(*)*******************************<*/
static u8 ReParentTEVStageInCollapseList  ( ShaderTEVStage *oldParent, ShaderTEVStage *newParent, 
                                            ShaderTEVStage *current, u8 newIndex )
{
    u32 i;
    u8 temp;

    if(!current) return 99;

    for ( i = 0; i < 32; i ++ )
    {
        //see if current is still in the list
        if(Children[i] == current && Parents[i] == oldParent)
        {                     
            Parents[i] = newParent;
            temp = CollapseIndex[i];
            CollapseIndex[i] = newIndex;
            return temp;
        }

        if(!(Children[i])) return 99;
    }

    //never reached
    return 99;
}

/*>*******************************(*)*******************************<*/
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

/*>*******************************(*)*******************************<*
static void CopyOptimalTree ( void )
{
    u8 i;
    u8 copied = 0;

    for ( i = 0; i < 33; i ++ )
    {
        CopyList[i] = 0;
    }

    CopyStage(&(TEVPool[0]), &copied);
}

/*>*******************************(*)*******************************<*
static ShaderTEVStage *CopyStage	( ShaderTEVStage *stage, u8 *copied )
{
    u8 i;
    ShaderTEVStage *copy = 0;

    //check to see if stage has alreday been copied
    for ( i = 0; i < 33; i ++ )
    {
        if(!(CopyList[i])) break;

        if(CopyList[i] == stage)
            return &(CollapsedTEVPool[i]);
    }

    //copy stage
    copy = &(CollapsedTEVPool[*copied]);
    CopyList[*copied] = stage;
    (*copied)++;

    copy->numParents = 0;
    for ( i = 0; i < 8; i ++ )
    {   
        copy->parent[i] = 0;
    }

    copy->channel = stage->channel;
    copy->type = stage->type;

    copy->LERPType = stage->LERPType;
    copy->outputIndex = stage->outputIndex;
    copy->outputArg = stage->outputArg;

    copy->op = stage->op;
    copy->bias = stage->bias;
    copy->scale = stage->scale;
    copy->clamp = stage->clamp;
    copy->mode = stage->mode;
    copy->out_reg = stage->out_reg;

    copy->coord = stage->coord;
    copy->map = stage->map;
    copy->color = stage->color;

    copy->numNonAllocatedChildren = stage->numNonAllocatedChildren;
    
    for ( i = 0; i < 4; i ++ )
    {
        copy->TEVArg[i] = stage->TEVArg[i];

        if(stage->tevStage[i])
        {
            copy->tevStage[i] = CopyStage(stage->tevStage[i], copied);

            //parent tev stage
            copy->tevStage[i]->parent[copy->tevStage[i]->numParents] = copy;
            (copy->tevStage[i]->numParents) ++;
        }
        else
            copy->tevStage[i] = 0;    
    }

    return copy;
}

/*>*******************************(*)*******************************<*/
static void CreateAddList   ( ShaderTEVStage *stage )
{
    u8 i;

    //check child stages
    for ( i = 0; i < 4; i ++ )
    {
        if(stage->tevStage[i])
            CreateAddList(stage->tevStage[i]);
    }
    
    //figure out if stage is an add
    if(stage->op == GX_TEV_ADD &&
       stage->TEVArg[0] != SHADER_ZERO &&
       stage->TEVArg[3] != SHADER_ZERO &&
       stage->TEVArg[1] == SHADER_ZERO &&
       stage->TEVArg[2] == SHADER_ZERO)
    {
        //check to see if stage is already in the add list
        for ( i = 0; i < AddListCounter; i ++ )
        {
            if(AddList[i] == stage)
                return;
        }
        
        AddList[AddListCounter] = stage;
        AddListCounter++;
    }
}

/*>*******************************(*)*******************************<*/
static void PermuteTree ( void )
{
    SHADER_TEVArg   tempArg;
    ShaderTEVStage	*tempStage;
    ShaderTEVStage	*stage;

    if(AddListCounter)
    {
        AddListCounter --;

        PermuteTree();

        //swap add parameters
        stage = AddList[AddListCounter];

        tempArg = stage->TEVArg[0];
        tempStage = stage->tevStage[0];
        stage->TEVArg[0] = stage->TEVArg[3];
        stage->tevStage[0] = stage->tevStage[3];
        stage->TEVArg[3] = tempArg;
        stage->tevStage[3] = tempStage;

#ifdef MAC
        OSReport("Swapping adds!!!!\n");
#endif

        PermuteTree();

        AddListCounter ++;
    }
    else
    {
        //reset the number of possible collapse candidates to 0
        NumCandidates = 0;

        //create a list of all collapseable TEV stages and their parents
        CreateCollapseList(&(TEVPool[0]), 0, 0);

        CollapseRecursive();
    }
}

/*>*******************************(*)*******************************<*/
static void SetUpFlatten    ( ShaderTEVStage *stage )
{
    u8 i;

    if(!(stage))
        return;

    stage->numNonAllocatedChildren = 0;

    for ( i = 0; i < 4; i ++ )
    {
        SetUpFlatten(stage->tevStage[i]);
    }
}

/*>*******************************(*)*******************************<*/

