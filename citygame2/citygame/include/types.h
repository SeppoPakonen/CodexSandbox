#ifndef TYPES_H
#define TYPES_H

// Basic types and structures for the city game
typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y, z, w;
} Vec4;

typedef struct {
    int vtype;         // 0=none 1=car,2=slowcar
    int dir_in;        // 0..3 DIRS[i]
    int dir_out;       // 0..3 DIRS[i]
    float offset;      // -.5 ... +.5
    float vel;         // 0. ...  +1.
    int brake;
    int lane;          // lane
    #ifdef DOUBLE_LANES
    int lane_out;
    #endif
} Vehicle;

#define MAX_LANES 2
#define LANES 1  // Single lane for now

typedef struct {
    Vec2 block_pos;    // block xy
    int pos;           // position within block
    int road;
    int shift;
    Vehicle v[MAX_LANES];  // vehicles
} Cell;

typedef struct {
    Vec2 block_pos;
    int btype;         // 0=none 1=roundabout 2=semaphore 4=street 5=overpass
    int ltype;         // 0=none  2=building 3=center
    int bh;            // building height
    int th;            // terrain height
    int conns;         // current connections (bitmask)
    float counter;
    int dist;          // distance from center
    int dir;           // direction to center
    int aconns;        // allowed connections (bitmask)
    int semaphore;     // 0:YELLOW 1:HORIZONTAL 2:VERTICAL
} Block;

#endif