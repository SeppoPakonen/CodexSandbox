/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     actor.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/actor/src/actor.c $
    
    16    11/13/00 1:21p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    Hierarchy transformation is no longer a mtx in the layout, but a
    CTRLControl structure of translation, rotation, and scale.
    
    15    8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    14    8/04/00 5:04p John
    Optimized ACTBuildMatrices more to eliminate recursion, precompute some
    matrices, and not compute others when not needed.
    
    13    7/18/00 7:22p John
    Modified to use new CTRL library.
    Currently backward compatible, but hierarchy control must be a matrix.
    Should change ACT version number later.
    
    12    7/19/00 4:24p Ryan
    Update to work with precompiled shaders.
    
    11    6/08/00 3:50p Mikepc
    line 227:  fixed memory leak bug for runtime animation structures.
    
    10    4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    9     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <Dolphin/os.h>
#include <Dolphin/dvd.h>
#include <Dolphin/platform.h>

#include <CharPipe/actor.h>

/********************************/
static void InitActorWithLayout ( ACTActorPtr actor, ACTLayoutPtr layout );
static void InitBone            ( ACTBone *parent, ACTBoneLayoutPtr layout, ACTActorPtr actor, u16 *numBones );
static void LoadActorLayout     ( ACTLayoutPtr *layout, char *name );

static void BuildBoneSkinOrientationMatrix( ACTBone *bone, Mtx orientation );
static void FreeActorLayout     ( ACTLayoutPtr *layout );

/*>*******************************(*)*******************************<*/
//  ACTOR LOAD ROUTINES
/*>*******************************(*)*******************************<*/
static void InitActorWithLayout ( ACTActorPtr actor, ACTLayoutPtr layout )
{
    ACTBone *temp = 0; 
    u16 numBones = 0;
    Mtx identity;

    MTXIdentity(identity);

    actor->actorID = layout->actorID;
    actor->layout = layout;
    actor->totalBones = layout->totalBones;

    DSInitTree(&(actor->hierarchy), (Ptr)temp, &(temp->branch));
    DSInitList(&(actor->drawPriorityList), (Ptr)temp, &(temp->drawPriorityLink));
    
    // Get the geometry palette (GPL) with all of the display objects
    actor->pal = 0;
    if(layout->geoPaletteName)
    {
        GEOGetPalette(&actor->pal, layout->geoPaletteName);
    }

    // Allocate runtime bone structures and matrices
    actor->boneArray = (ACTBone**)OSAlloc(sizeof(ACTBone*) * layout->totalBones);
    actor->forwardMtxArray = (MtxPtr)OSAlloc(sizeof(Mtx) * layout->totalBones);

    // If there is a stitched display object, initialize and
    // allocate stitching arrays 
    if(layout->skinFileID != ACT_NO_DISPLAY_OBJECT)
    {
        DOGet(&actor->skinObject, actor->pal, layout->skinFileID, 0);
        DOSetWorldMatrix(actor->skinObject, identity);

        actor->skinMtxArray = (MtxPtr)OSAlloc(sizeof(Mtx) * layout->totalBones);
        actor->orientationInvMtxArray = (MtxPtr)OSAlloc(sizeof(Mtx) * layout->totalBones);
        if( actor->skinObject->lightingData )
            actor->skinInvTransposeMtxArray = (MtxPtr)OSAlloc(sizeof(Mtx) * layout->totalBones);
        else
            actor->skinInvTransposeMtxArray = 0;
    }
    else
    {
        actor->skinObject = 0;
        actor->skinMtxArray = 0;
        actor->orientationInvMtxArray = 0;
        actor->skinInvTransposeMtxArray = 0;
    }

    // Initialize the actor world control
    CTRLInit(&actor->worldControl);

    // Initialize the bones
    if(layout->hierarchy.Root)
    {
        InitBone(0, (ACTBoneLayoutPtr)layout->hierarchy.Root, actor, &numBones);
    }

    // Double check the number of bones in ACT file is correct
    if(numBones != actor->totalBones)
        OSHalt("Incorrect number of bones in actor");

    // Build inverse orientation matrices for stitching the first time
    if(actor->skinObject)
    {
        BuildBoneSkinOrientationMatrix((ACTBone*)actor->hierarchy.Root, identity);
    }

    // Sort the actor bones so they are drawn in the proper order
    ACTSort(actor);
}

/*>*******************************(*)*******************************<*/
static void InitBone    ( ACTBone *parent, ACTBoneLayoutPtr layout, ACTActorPtr actor, u16 *numBones )
{
    ACTBone *newBone;

    while(layout)
    {
        // Allocate the bone
        newBone = (ACTBone*)OSAlloc(sizeof(ACTBone));

        // Add the bone to the hierarchy
        newBone->branch.Children = 0;
        DSInsertBranchBelow(&actor->hierarchy, (Ptr)parent, (Ptr)newBone);

        // Create the boneArray in order such that we traverse the hierarchy
        // in a depth-first manner
        actor->boneArray[*numBones] = newBone;

        // Initialize boneID (used to bind animation data)
        newBone->boneID = layout->boneID;

        // Set the inheritance flag
        newBone->inheritanceFlag = layout->inheritanceFlag;

        // Set the order in which the bones should be drawn
        newBone->drawingPriority = layout->drawingPriority;

        // Set the orientation control
        CTRLInit( &newBone->orientationCtrl );
        if(layout->orientationCtrl)
        {
            newBone->orientationCtrl = *layout->orientationCtrl;
        }

        // Attach a display object if it exists
        newBone->dispObj = 0;
        if(layout->geoFileID != ACT_NO_DISPLAY_OBJECT)
            DOGet(&newBone->dispObj, actor->pal, layout->geoFileID, 0);

        // No animation yet (set in ACTSetBoneTrack)
        newBone->animPipe = 0;
        CTRLInit(&newBone->animationCtrl);

        // Initialize matrix pointers to point to matrix arrays in the actor
        newBone->forwardMtx = &(actor->forwardMtxArray[*numBones * MTX_PTR_OFFSET]);    
        MTXIdentity(newBone->forwardMtx);

        if(actor->skinObject)
        {
            newBone->skinMtx = &(actor->skinMtxArray[*numBones * MTX_PTR_OFFSET]);
            MTXIdentity(newBone->skinMtx);

            newBone->orientationInvMtx = &(actor->orientationInvMtxArray[*numBones * MTX_PTR_OFFSET]);  
            MTXIdentity(newBone->orientationInvMtx);

            if( actor->skinObject->lightingData )
            {
                newBone->skinInvTransposeMtx = &(actor->skinInvTransposeMtxArray[*numBones * MTX_PTR_OFFSET]);
                MTXIdentity(newBone->skinInvTransposeMtx);
            }
            else
            {
                newBone->skinInvTransposeMtx = 0;
            }
        }
        else
        {
            newBone->skinMtx = 0;
            newBone->orientationInvMtx = 0;
            newBone->skinInvTransposeMtx = 0;
        }

        (*numBones)++;

        // Traverse to the children first (depth first)
        if(layout->branch.Children)
        {
            InitBone(newBone, (ACTBoneLayout*)layout->branch.Children, actor, numBones);
        }

        // Then traverse to the siblings
        layout = (ACTBoneLayoutPtr)layout->branch.Next;
    }
}

/*>*******************************(*)*******************************<*/
static void LoadActorLayout     ( ACTLayoutPtr *layout, char *name )
{
    ACTLayoutPtr     actLayout;
    ACTBoneLayoutPtr bone;
    DVDFileInfo      dfi;
    u32              i;

    if(!DVDOpen(name, &dfi))
    {
        OSReport("LoadActorLayout: Could not find file %s", name );
        OSHalt("");
    }
    actLayout = (ACTLayoutPtr)OSAlloc(OSRoundUp32B(dfi.length));
    DVDRead(&dfi, actLayout, (s32)OSRoundUp32B(dfi.length), 0);
    DVDClose(&dfi); 

    //unpack it!
    if(actLayout->versionNumber != ACT_VERSION)
    {
        OSReport("LoadActorLayout: Incompatible version number %d for %s, since\n", actLayout->versionNumber, name);
        OSReport("                 the library version number is %d.\n", ACT_VERSION );
        OSHalt("");
    }

    if(actLayout->userDataSize > 0 && actLayout->userData)
        actLayout->userData = (Ptr)((u32)(actLayout->userData) + (u32)actLayout);

    if(actLayout->hierarchy.Root)
        actLayout->hierarchy.Root = (Ptr)((u32)(actLayout->hierarchy.Root) + (u32)actLayout);
        
    if(actLayout->geoPaletteName)
        actLayout->geoPaletteName = (Ptr)((u32)(actLayout->geoPaletteName) + (u32)actLayout);

    bone = (ACTBoneLayoutPtr)(((u32)actLayout) + sizeof(ACTLayout));

    for(i = 0; i < actLayout->totalBones; i ++)
    {
        if(bone[i].orientationCtrl)
            bone[i].orientationCtrl = (CTRLControlPtr)((u32)(bone[i].orientationCtrl) + (u32)actLayout);

        if(bone[i].branch.Prev)
            bone[i].branch.Prev = (Ptr)((u32)(bone[i].branch.Prev) + (u32)actLayout);

        if(bone[i].branch.Next)
            bone[i].branch.Next = (Ptr)((u32)(bone[i].branch.Next) + (u32)actLayout);

        if(bone[i].branch.Parent)
            bone[i].branch.Parent = (Ptr)((u32)(bone[i].branch.Parent) + (u32)actLayout);

        if(bone[i].branch.Children)
            bone[i].branch.Children = (Ptr)((u32)(bone[i].branch.Children) + (u32)actLayout);
    }

    *layout = actLayout;
} 

/*>*******************************(*)*******************************<*/
void    ACTGet          ( ACTActorPtr *actor, char *name )
{
    ACTLayoutPtr    layout = 0;

    if(*actor) ACTRelease(actor);

    (*actor) = (ACTActorPtr) OSAlloc(sizeof(ACTActor));

    if(DOCacheInitialized) layout = (ACTLayoutPtr)DSGetCacheObj(&DODisplayCache, name);
    if(!layout) //file was not found in cache so load it!!!
    {
        LoadActorLayout(&layout, name);
        if(DOCacheInitialized) 
        {
            DSAddCacheNode(&DODisplayCache, name, (Ptr)layout, (Ptr)FreeActorLayout);
            DSGetCacheObj(&DODisplayCache, name); //Increment reference count
        }
    }

    InitActorWithLayout(*actor, layout);
}

/*>*******************************(*)*******************************<*/
void    ACTRelease      ( ACTActorPtr *actor )
{
    u16 numBones = 0;

    while(numBones < (*actor)->totalBones)
    {
        if(((*actor)->boneArray[numBones])->dispObj)
            DORelease(&(((*actor)->boneArray[numBones])->dispObj));

        if( (*actor)->boneArray[numBones]->animPipe )
        {
            OSFree( (*actor)->boneArray[numBones]->animPipe );
            (*actor)->boneArray[numBones]->animPipe = NULL;
        }

        OSFree((*actor)->boneArray[numBones]);
        numBones ++;
    }

    if((*actor)->skinObject)
    {
        OSFree((*actor)->skinMtxArray);
        OSFree((*actor)->orientationInvMtxArray);
        if( (*actor)->skinObject->lightingData )
            OSFree((*actor)->skinInvTransposeMtxArray);
        DORelease(&((*actor)->skinObject));
    }

    if((*actor)->pal) GEOReleasePalette(&((*actor)->pal));

    OSFree((*actor)->forwardMtxArray);
    OSFree((*actor)->boneArray);
    
    if(DOCacheInitialized)
    {
        if((*actor)->layout) 
            DSReleaseCacheObj(&DODisplayCache, (Ptr)(*actor)->layout);
    }
    else
    {
        if((*actor)->layout)
        {
            OSFree((*actor)->layout);
            (*actor)->layout = 0;
        }
    }
    
    OSFree(*actor);
    *actor = 0;
}

/*>*******************************(*)*******************************<*/
//  ACTOR MANIPULATION ROUTINES
/*>*******************************(*)*******************************<*/
void    ACTRender       ( ACTActorPtr actor, Mtx camera, u8 numLights, ... )
{
    ACTBone *temp = 0; 
    va_list  ptr;

    if(actor->skinObject)
    {
        va_start(ptr, numLights);

        DOVARenderSkin(actor->skinObject, camera, actor->skinMtxArray, 
                       actor->skinInvTransposeMtxArray, numLights, ptr);
        va_end(ptr);
    }

    temp = (ACTBone*)(actor->drawPriorityList.Head);

    while(temp)
    {
        va_start(ptr, numLights);
        DOVARender(temp->dispObj, camera, numLights, ptr );
        temp = (ACTBone*)(temp->drawPriorityLink.Next);
        va_end(ptr);
    }
}

/*>*******************************(*)*******************************<*/
void    ACTHide         ( ACTActorPtr actor )
{
    u16 numBones;

    if(actor->skinObject)
        DOHide(actor->skinObject);

    for(numBones = 0; numBones < actor->totalBones; numBones++)
    {
        if(actor->boneArray[numBones]->dispObj)
            DOHide(actor->boneArray[numBones]->dispObj);
    }
}

/*>*******************************(*)*******************************<*/
void    ACTShow         ( ACTActorPtr actor )
{
    u16 numBones;

    if(actor->skinObject)
        DOShow(actor->skinObject);

    for(numBones = 0; numBones < actor->totalBones; numBones++)
    {
        if(actor->boneArray[numBones]->dispObj)
            DOShow(actor->boneArray[numBones]->dispObj);
    }
}

/*>*******************************(*)*******************************<*/
static void BuildBoneSkinOrientationMatrix( ACTBone *bone, Mtx orientation )
{
    Mtx boneMtx;
    Mtx newOrientation;

    while( bone )
    {
        if( bone->inheritanceFlag == ACT_INHERIT_ALL )
        {
            CTRLBuildMatrix( &bone->orientationCtrl, boneMtx );
            MTXConcat( orientation, boneMtx, newOrientation );
            MTXInverse( newOrientation, bone->orientationInvMtx );
        }
        else
            CTRLBuildInverseMatrix( &bone->orientationCtrl, bone->orientationInvMtx );

        if(bone->branch.Children)
            BuildBoneSkinOrientationMatrix((ACTBone*)bone->branch.Children, newOrientation);

        bone = (ACTBone*)bone->branch.Next;
    }
}

/*>*******************************(*)*******************************<*/
void    ACTBuildMatrices    ( ACTActorPtr actor )
{
    Mtx       actorMtx;
    Mtx       animatedBoneMtx;
    Mtx       orientationMtx;
    ACTBone  *bone;
    ACTBone  *parent;
    u16       boneIndex;

    // Build the world control for the actor
    CTRLBuildMatrix(&actor->worldControl, actorMtx);
    
    // We traverse hierarchy in depth first manner, since
    // boneArray is in such an order when it was created through InitBone
    for(boneIndex = 0; boneIndex < actor->totalBones; boneIndex++)
    {
        bone = actor->boneArray[boneIndex];

        if( bone->animationCtrl.type != CTRL_NONE )
        {
            CTRLBuildMatrix(&bone->animationCtrl, animatedBoneMtx);

            if( !bone->animPipe->replaceHierarchyCtrl )
            {
                // Add in the animation matrix
                CTRLBuildMatrix(&bone->orientationCtrl, orientationMtx);
                MTXConcat(orientationMtx, animatedBoneMtx, animatedBoneMtx);
            }
        }
        else
            // No animation so assume identity animation matrix
            CTRLBuildMatrix(&bone->orientationCtrl, animatedBoneMtx );

        // Concatenate the appropriate parent depending on inheritance
        parent = (ACTBone*)bone->branch.Parent;
        if(bone->inheritanceFlag == ACT_INHERIT_ALL && parent)
            MTXConcat(parent->forwardMtx, animatedBoneMtx, bone->forwardMtx);
        else
            MTXConcat(actorMtx, animatedBoneMtx, bone->forwardMtx);

        // Set the world matrix of the attached display object
        if(bone->dispObj)
            DOSetWorldMatrix(bone->dispObj, bone->forwardMtx);

        // Build the skin matrices
        if(actor->skinObject)
        {
            // Inverse orientation is needed since all vertices are in one coordinate space
            // This was precomputed in InitActorWithLayout
            MTXConcat(bone->forwardMtx, bone->orientationInvMtx, bone->skinMtx);

            if(actor->skinObject->lightingData)
            {
                MTXInverse(bone->skinMtx, animatedBoneMtx);
                MTXTranspose(animatedBoneMtx, bone->skinInvTransposeMtx);
            }
        }
    }
}

/*>*******************************(*)*******************************<*/
CTRLControlPtr  ACTGetControl       ( ACTActorPtr actor )
{
    return &(actor->worldControl);
}

/*>*******************************(*)*******************************<*/
void    ACTSetInheritance ( ACTActorPtr actor, u8 inheritanceFlag )
{
    u16 i;

    for(i = 0; i < actor->totalBones; i++)
    {
        actor->boneArray[i]->inheritanceFlag = inheritanceFlag;
    }
}

/*>*******************************(*)*******************************<*/
static void FreeActorLayout ( ACTLayoutPtr *layout )
{
    OSFree((*layout));
    *layout = 0;
}

/*>*******************************(*)*******************************<*/
void    ACTSetAmbientPercentage ( ACTActorPtr actor, f32 percent )
{
    u16 numBones = 0;

    if(actor->skinObject)
        DOSetAmbientPercentage(actor->skinObject, percent);

    while(numBones < actor->totalBones)
    {
        if((actor->boneArray[numBones])->dispObj)
            DOSetAmbientPercentage((actor->boneArray[numBones])->dispObj, percent);
        numBones ++;
    }
}

/*>*******************************(*)*******************************<*/
void    ACTSort ( ACTActorPtr actor )
{
    u16 numBones = 0;
    ACTBone *temp = 0; 

    actor->drawPriorityList.Head = 0;
    actor->drawPriorityList.Tail = 0;

    while(numBones < actor->totalBones)
    {
        if((actor->boneArray[numBones])->dispObj)
        {
            temp = (ACTBone*)(actor->drawPriorityList.Head);

            while(temp)
            {
                if(temp->drawingPriority > (actor->boneArray[numBones])->drawingPriority)
                    break;

                temp = (ACTBone*)(temp->drawPriorityLink.Next);
            }

            DSInsertListObject(&(actor->drawPriorityList), (Ptr)temp, (Ptr)(actor->boneArray[numBones]));
        }

        numBones ++;
    }
}

/*>*******************************(*)*******************************<*/
void    ACTSetEffectsShader     ( ACTActorPtr actor, Ptr shaderFunc, Ptr data )
{
    u16 numBones = 0;

    if(actor->skinObject)
        DOSetEffectsShader(actor->skinObject, shaderFunc, data);

    while(numBones < actor->totalBones)
    {
        if((actor->boneArray[numBones])->dispObj)
            DOSetEffectsShader((actor->boneArray[numBones])->dispObj, shaderFunc, data);
        numBones ++;
    }
}

/*>*******************************(*)*******************************<*/
u32 ACTGetUserDataSize      ( ACTActorPtr actor )
{
    ASSERT( actor && actor->layout );
    return actor->layout->userDataSize;
}

/*>*******************************(*)*******************************<*/
Ptr ACTGetUserData          ( ACTActorPtr actor )
{
    ASSERT( actor && actor->layout );
    return actor->layout->userData;
}
