/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [Tree.c]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/structures/src/Tree.c $
    
    3     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include <charpipeline\structures\tree.h>

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Removes a specified branch from a tree.  If the branch has children
//	they are added back into the tree at the level from which the 
//	branch was removed
/*>-----------------------------------------------------------------<*/
void	DSExtractBranch ( DSTreePtr tree, Ptr obj )
{
	DSBranchPtr branch = (DSBranchPtr) ((u32)obj + tree->Offset);
	Ptr cursor, next;
	
	cursor = branch->Children;
	while(cursor)							// WHILE THE BRANCH STILL HAS CHILDREN
	{										// INSERT THEM BACK INTO THE TREE AT THE CURRENT LEVEL
		next = ((DSBranchPtr)((u32)cursor + tree->Offset))->Next;
		DSInsertBranchBelow(tree, branch->Parent, cursor);
		cursor = next;
	}

	DSRemoveBranch(tree, obj);
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Sets the tree's root to 0 and initializes the offset of the branch
//	in the obj structure.
/*>-----------------------------------------------------------------<*/
void	DSInitTree ( DSTreePtr tree, Ptr obj, DSBranchPtr branch )
{
	tree->Root = 0;
	tree->Offset = (u32)branch - (u32)obj;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Inserts the branch into the tree as a child of the object pointed
//	to by cursor.  if cursor is 0, obj gets inserted at the root level 
//	of the tree
/*>-----------------------------------------------------------------<*/
void	DSInsertBranchBelow ( DSTreePtr tree, Ptr cursor, Ptr obj )
{
	DSBranchPtr branch;
	DSBranchPtr objBranch = (DSBranchPtr)((u32)obj + tree->Offset);
	Ptr tail = 0;

	if(cursor)											// IF CURSOR POINTS TO SOMETHING...
	{						
		branch = (DSBranchPtr)((u32)cursor + tree->Offset);

		if(branch->Children)							// IF CURSOR HAS CHILDREN
			tail = branch->Children;					// SET TAIL EQUAL TO THE CHILD LIST
		else											// ELSE
			branch->Children = obj;						// MAKE OBJ BRANCH'S ONLY CHILD
	}
	else												// ELSE CURSOR IS 0...
	{
		if(tree->Root)									// IF TREE HAS A ROOT
			tail = tree->Root;							// SET TAIL EQUAL TO THE ROOT LIST
		else											// ELSE
			tree->Root = obj;							// MAKE OBJ TREE'S ROOT
	}

	if(tail)											// IF WE NEED TO INSERT OBJ INTO A LIST
	{													// FIND THE END OF THE LIST
		while(((DSBranchPtr)((u32)tail + tree->Offset))->Next)
		{
			tail = ((DSBranchPtr)((u32)tail + tree->Offset))->Next;
		}
														// INSERT OBJ AT THE END OF THE LIST
		((DSBranchPtr)((u32)tail + tree->Offset))->Next = obj;
		objBranch->Prev = tail;
	}
	else												//ELSE OBJ IS THE ONLY ELEMENT IN THE LIST
		objBranch->Prev = 0;
		
	objBranch->Next = 0;
	objBranch->Parent = cursor;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Inserts an object at the end of the list specified by cursor.  If
//	cursor is 0 obj is inserted at the root level of the tree.
/*>-----------------------------------------------------------------<*/
void	DSInsertBranchBeside ( DSTreePtr tree, Ptr cursor, Ptr obj )
{
	DSBranchPtr objBranch = (DSBranchPtr)((u32)obj + tree->Offset);
	DSBranchPtr prevBranch;

	if(!cursor)								// IF CURSOR IS 0...
	{
		if(!tree->Root)						// IF TREE HAS NO ROOT
		{									// MAKE OBJ TREE'S ROOT
			tree->Root = obj;
			objBranch->Next = 0;
			objBranch->Prev = 0;
			objBranch->Children = 0;
			objBranch->Parent = 0;
			return;
		}
		else								// ELSE 
			cursor = tree->Root;			// SET CURSOR TO BE THE START OF THE ROOT LIST 
	}

	while(((DSBranchPtr)((u32)cursor + tree->Offset))->Next)	// FIND THE END OF THE LIST
	{
		cursor = ((DSBranchPtr)((u32)cursor + tree->Offset))->Next;
	}
	
	prevBranch = (DSBranchPtr)((u32)cursor + tree->Offset);	// INSERT THE BRANCH
	prevBranch->Next = obj;
	objBranch->Prev = cursor;
	objBranch->Next = 0;
	objBranch->Parent = prevBranch->Parent;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Extracts a branch and all of its children from a tree.
/*>-----------------------------------------------------------------<*/
void	DSRemoveBranch ( DSTreePtr tree, Ptr obj )
{
	DSBranchPtr branch = (DSBranchPtr) ((u32)obj + tree->Offset);
	DSBranchPtr parent;

	if(branch->Parent)							// IF OBJ HAS A PARENT...
	{
		parent = (DSBranchPtr) ((u32)branch->Parent + tree->Offset);

		if(parent->Children == obj)				// IF OBJ IS THE HEAD OF THE CILD LIST
			parent->Children = branch->Next;	// SET HEAD OF THE CHILD LIST TO BRANCH->NEXT
	}
	else										// ELSE OBJ IS AT THE ROOT LEVEL OF THE TREE
	{
		if(tree->Root == obj)					// IF OBJ IS THE HEAD OF THE ROOT LIST
			tree->Root = branch->Next;			// SET HEAD OF THE ROOT LIST TO BRANCH->NEXT 
	}

	if(branch->Prev)											//IF BRANCH HAS A PREV 
		((DSBranchPtr)((u32)branch->Prev + tree->Offset))->Next	//SET PREV->NEXT TO BRANCH->NEXT
		= branch->Next;
	if(branch->Next)											//IF BRANCH HAS A NEXT
		((DSBranchPtr)((u32)branch->Next + tree->Offset))->Prev	//SET NEXT->PREV TO BRANCH->PREV
		= branch->Prev;

	branch->Prev = 0;
	branch->Next = 0;
	branch->Parent = 0;
}
