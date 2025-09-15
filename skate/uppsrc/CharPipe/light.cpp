/*---------------------------------------------------------------------------*
  Project:  Character Pipeline
  File:     light.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/lighting/src/light.c $
    
    5     8/14/00 6:25p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    4     8/04/00 5:09p John
    Renamed forwardMatrix to forwardMtx.
    
    3     7/18/00 7:30p John
    Modified to use new CTRL library.
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     11/02/99 6:14p Ryan
    Update for new animation format
    
    1     10/11/99 12:59p Ryan
    

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <CharPipe/light.h>

/*>*******************************(*)*******************************<*/
void LITAlloc		( LITLightPtr *light )
{	
	(*light) = (LITLightPtr)OSAlloc(sizeof(LITLight));

	(*light)->position.x = (*light)->position.y = (*light)->position.z = 0.0F;
	(*light)->direction.x = (*light)->direction.y = 0.0F;
	(*light)->direction.z = 1.0F;
	(*light)->parent = 0;
	(*light)->animPipe = 0;

	CTRLInit(&((*light)->control));
}

/*>*******************************(*)*******************************<*/
void LITFree		( LITLightPtr *light )
{	
	OSFree(*light);

	*light = 0;
}

/*>*******************************(*)*******************************<*/
void LITInitAttn ( LITLightPtr light, f32 a0, f32 a1, f32 a2, 
					 f32 k0, f32 k1, f32 k2 )
{
	GXInitLightAttn(&(light->lt_obj), a0, a1, a2, k0, k1, k2);
}

/*>*******************************(*)*******************************<*/
void LITInitSpot ( LITLightPtr light, f32 cutoff, GXSpotFn spot_func )
{
	GXInitLightSpot(&(light->lt_obj), cutoff, spot_func);
}

/*>*******************************(*)*******************************<*/
void LITInitDistAttn ( LITLightPtr light, f32 ref_distance, f32 ref_brightness,
						 GXDistAttnFn dist_func )
{
	GXInitLightDistAttn(&(light->lt_obj), ref_distance, ref_brightness, dist_func);
}

/*>*******************************(*)*******************************<*/
void LITInitPos 	( LITLightPtr light, f32 x, f32 y, f32 z )
{
	light->position.x = x;
	light->position.y = y;
	light->position.z = z;
}

/*>*******************************(*)*******************************<*/
void LITInitDir 	( LITLightPtr light, f32 nx, f32 ny, f32 nz )
{
	light->direction.x = nx;
	light->direction.y = ny;
	light->direction.z = nz;
}

/*>*******************************(*)*******************************<*/
void LITInitColor ( LITLightPtr light, GXColor color )
{
	light->color = color;
}

/*>*******************************(*)*******************************<*/
void LITXForm		( LITLightPtr light, Mtx view )
{
	Mtx m;

	CTRLBuildMatrix(&(light->control), m);

	if(light->parent)
		MTXConcat(light->parent, m, m);

	MTXConcat(view, m, m);

	MTXMultVec(m, &light->position, &light->worldPosition);

	MTXInverse(m, m);
	MTXTranspose(m, m);

	MTXMultVec(m, &light->direction, &light->worldDirection);
}

/*>*******************************(*)*******************************<*/
void LITAttach	( LITLightPtr light, Ptr parent, CPParentType type )
{
	switch(type)
	{
		case PARENT_BONE:
			light->parent = ((ACTBonePtr)parent)->forwardMtx;
			break;
		case PARENT_DISP_OBJ:
			light->parent = ((DODisplayObjPtr)parent)->worldMatrix;
			break;
		case PARENT_MTX:
			light->parent = (MtxPtr)parent;
			break;
	}
}

/*>*******************************(*)*******************************<*/
void LITDetach	( LITLightPtr light )
{
	light->parent = 0;
}

/*>*******************************(*)*******************************<*/
CTRLControlPtr	LITGetControl	( LITLightPtr light )
{
	return &(light->control);
}

/*>*******************************(*)*******************************<*/
void LITSetAnimSequence	( LITLightPtr light, ANIMBankPtr animBank, 
						  char *seqName, float time )
{
    ANIMSequencePtr seq = ANIMGetSequence(animBank, seqName, 0);
	ANIMTrackPtr track = &(seq->trackArray[0]);

    if(!light->animPipe)
    {
		light->animPipe = OSAlloc(sizeof(ANIMPipe));
		light->animPipe->time = 0.0F;
		light->animPipe->speed = 1.0F;
		light->animPipe->currentTrack = NULL;
		light->animPipe->control = NULL;
	}

	ANIMBind(light->animPipe, &light->control, track, time);
}

/*>*******************************(*)*******************************<*/
void LITTick		( LITLightPtr light )
{
	ANIMTick(light->animPipe);
}

/*>*******************************(*)*******************************<*/