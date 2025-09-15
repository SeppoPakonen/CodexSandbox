#ifndef _LIST_H
#define _LIST_H

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
struct DLList_Str;
struct DLNode_Str;

struct DLList_Str {
	int cnt;
	struct DLNode_Str * headP;
	struct DLNode_Str * tailP;
};

struct DLNode_Str {
	struct DLNode_Str * nextP;
	struct DLNode_Str * prevP;
	void * infoP;
	int index;
	float value;
};

typedef struct DLList_Str DLList;
typedef struct DLNode_Str DLNode;

#define DLListNull(listP) \
	listP->headP = NULL; \
	listP->tailP = NULL; \
	listP->cnt = 0;

#define DLTraverseList(listP, nodeP) \
	for(nodeP = listP->headP; nodeP; nodeP = nodeP->nextP)

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------
DLList * DLListConstructor(void);
DLNode * DLNodeConstructor(void * infoP);
void DLAddNode(DLList * listP, DLNode * nodeP);
void DLAddSortNode(DLList * listP, DLNode * nodeP);
void DLDeleteNode(DLList * listP, DLNode * nodeP);
DLNode * DLGetNode(DLList * listP);
DLNode * DLPopNode(DLList * listP);

#endif
