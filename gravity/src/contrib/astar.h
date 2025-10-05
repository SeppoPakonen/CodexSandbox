
/*
;===================================================================
;A* Pathfinder (Version 1.71a) by Patrick Lester. Used by permission.
;===================================================================
;Last updated 06/16/03 -- Visual C++ version
 */
#ifndef _ASTAR_
#define _ASTAR_

class AStar
{
  private:
  	//Declare constants
    enum { 
      mapWidth = 100, mapHeight = 100, tileSize = 48,
  	  notfinished = 0, notStarted = 0, // path-related constants
  	  found = 1, nonexistent = 2,
  	  walkable = 0, unwalkable = 1 // walkability array constants
    };
 	  int onClosedList;

  	//Create needed arrays
  	char walkability [mapWidth][mapHeight];
  	int openList[mapWidth*mapHeight+2]; //1 dimensional array holding ID# of open list items
  	int whichList[mapWidth+1][mapHeight+1];  //2 dimensional array used to record 
    // 		whether a cell is on the open list or on the closed list.
  	int openX[mapWidth*mapHeight+2]; //1d array stores the x location of an item on the open list
  	int openY[mapWidth*mapHeight+2]; //1d array stores the y location of an item on the open list
    int parentX[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (x)
    int parentY[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (y)
	  int Fcost[mapWidth*mapHeight+2];	//1d array to store F cost of a cell on the open list
	  int Gcost[mapWidth+1][mapHeight+1]; 	//2d array to store G cost for each cell.
	  int Hcost[mapWidth*mapHeight+2];	//1d array to store H cost of a cell on the open list
	  int pathLength;     //stores length of the found path for critter
	  int pathLocation;   //stores current position along the chosen path for critter		
	  int* pathBank;

  	//Path reading variables
	  int pathStatus;
	  int xPath;
	  int yPath;

  public:
    AStar();
    ~AStar();

    // GS functions (JHA)
    void create_from_playmap(class gsMap *map);
    int  get_path_length()                            { return pathLength; }

    //-----------------------------------------------------------------------------
    // Function Prototypes: where needed
    //-----------------------------------------------------------------------------
    int  FindPath(int startingX, int startingY, int targetX, int targetY);
    void ReadPath(int currentX,int currentY, int pixelsPerFrame);
    int  ReadPathX(int pathLocation);
    int  ReadPathY(int pathLocation);

    // other functions
    void InitializePathfinder (void);
    void EndPathfinder (void);

};


#endif