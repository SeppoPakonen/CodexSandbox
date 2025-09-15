#include <Dolphin/dolphin.h>
#include <stdio.h>
#include "list.h"

//---------------------------------------------------------------------------
// Construct a Doubley Linked List
//---------------------------------------------------------------------------
DLList * DLListConstructor(void) {
	DLList * listP;

	// Create 
	listP = (DLList *)OSAlloc(sizeof(DLList));
	// Zero Everthing Out
	memset((char *)listP, 0, sizeof(DLList));

	return listP;
}

//---------------------------------------------------------------------------
// Construct a Doubley Linked List Node
//---------------------------------------------------------------------------
DLNode * DLNodeConstructor(void * infoP) {
	DLNode * nodeP;

	// Create 
	nodeP = (DLNode *)OSAlloc(sizeof(DLNode));
	// Zero Everthing Out
	memset((char *)nodeP, 0, sizeof(DLNode));

	nodeP->infoP = infoP;

	return nodeP;
}

//---------------------------------------------------------------------------
// Add a Doubley Linked List Node
//---------------------------------------------------------------------------
void DLAddNode(DLList * listP, DLNode * nodeP) {
	// Check to see if the node is empty
	if (nodeP == NULL) return;

	// Check to see if the list is empty
	if (listP->headP == NULL) {
		// Make it the first node
		nodeP->nextP = NULL;
		nodeP->prevP = NULL;
		listP->headP = nodeP;
		listP->tailP = nodeP;
	}
	// Add to the end of the list
	else {
		nodeP->prevP = listP->tailP;
		nodeP->nextP = NULL;
		listP->tailP->nextP = nodeP;
		listP->tailP = nodeP;
	}
	listP->cnt++;
}

//---------------------------------------------------------------------------
// Add a Doubley Linked List Node Sroted!
//---------------------------------------------------------------------------
void DLAddSortNode(DLList * listP, DLNode * nodeP) {
	DLNode * travP;

	// Check to see if the list is empty
	if (listP->headP == NULL) {
		listP->headP = nodeP;
		listP->tailP = nodeP;
		nodeP->nextP = NULL;
		nodeP->prevP = NULL;
	}
	// Check to see if the node goes at the beginning of the list
	else if (nodeP->value < listP->headP->value) {
		listP->headP->prevP = nodeP;
		nodeP->nextP = listP->headP;
		nodeP->prevP = NULL;
		listP->headP = nodeP;
	}
	// Add to the end of the list
	else if (nodeP->value >= listP->tailP->value) {
		listP->tailP->nextP = nodeP;
		nodeP->prevP = listP->tailP;
		nodeP->nextP = NULL;
		listP->tailP = nodeP;
	}
	// Add it in place
	else {
		for(travP = listP->headP->nextP; travP; travP = travP->nextP) {
			// Look for the node before nodeP
			if (nodeP->value < travP->value) {
				// Insert nodeP before travP 
				nodeP->nextP = travP;
				nodeP->prevP = travP->prevP;
				travP->prevP->nextP = nodeP;
				travP->prevP = nodeP;
				// Done. Need to do this!
				break;
			}
		}
	}
	listP->cnt++;
}

//---------------------------------------------------------------------------
// Delete a Doubley Linked List Node
//---------------------------------------------------------------------------
void DLDeleteNode(DLList * listP, DLNode * nodeP) {
	DLNode * nodePrevP;
	DLNode * nodeNextP;

	// Check to see if the list is empty
	if (listP->headP == NULL) {
		return;
	}

	// Check to see if the node is empty
	if (nodeP == NULL) {
		return;
	}

	// Snag Pointers
	nodeNextP = nodeP->nextP;
	nodePrevP = nodeP->prevP;

	// Delete the node from List
	if (nodePrevP) nodePrevP->nextP = nodeNextP;
	if (nodeNextP) nodeNextP->prevP = nodePrevP;

	// If the Node was the Head
	if (listP->headP == nodeP) listP->headP = nodeNextP;
	if (listP->headP == NULL) listP->tailP = NULL;

	// If the Node was the Tail
	if (listP->tailP == nodeP) listP->tailP = nodePrevP;
	if (listP->tailP == NULL) listP->headP = NULL;

	listP->cnt--;
}

//---------------------------------------------------------------------------
// Get Pointer to first Node
//---------------------------------------------------------------------------
DLNode * DLGetNode(DLList * listP) {
	return listP->headP;
}

//---------------------------------------------------------------------------
// Delete and return first Node
//---------------------------------------------------------------------------
DLNode * DLPopNode(DLList * listP) {
	DLNode * retP;

	retP = listP->headP;
	DLDeleteNode(listP, retP);

	return retP;
}

