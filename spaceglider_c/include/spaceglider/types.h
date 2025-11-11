#ifndef SPACEGLIDER_TYPES_H
#define SPACEGLIDER_TYPES_H

#include <stdbool.h>
#include <math.h>

// Vector types
typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z, w;
} vec4;

// 3x3 matrix type
typedef struct {
    float m[3][3];
} mat3;

// 2x3 matrix type
typedef struct {
    vec3 m[2];
} mat2x3;

// Game state
typedef struct {
    vec3 campos;          // Camera position
    mat3 camframe;        // Camera frame (orientation matrix)
    float camzoom;        // Camera zoom factor
    vec3 waypoint;        // Waypoint in world coordinates
    vec3 mapmarker;       // Map marker position
    vec4 exposure;        // Exposure parameters
    float hudbright;      // HUD brightness
    unsigned int switches; // Game switches
    int stage;            // Game stage
    int ipage;            // Info page
    int hmdmode;          // HMD mode
    int aeromode;         // Aero mode
    int rcsmode;          // RCS mode
    int engmode;          // Engine mode
    float time;           // Game time
} GameState;

// Vehicle state
typedef struct {
    vec3 localr;          // Local position
    vec3 localv;          // Local velocity
    vec3 orbitr;          // Orbital position
    vec3 orbitv;          // Orbital velocity
    mat3 localB;          // Local orientation matrix
    vec3 B[3];            // Basis vectors (forward, right, up)
    float throttle;       // Throttle setting
    float tvec;           // Thrust vector angle
    float rho;            // Air density
    float mach;           // Mach number
    float dynpres;        // Dynamic pressure
    vec4 modes;           // Mode settings (HMD, aero, RCS, engine)
} VehicleState;

// Planet data
typedef struct {
    float radius;         // Planet radius
    float GM;            // Gravitational parameter
    float omega;         // Planet rotation rate
} PlanetData;

// Constants
#define ZERO ((vec3){0, 0, 0})
#define UNIT_X ((vec3){1, 0, 0})
#define UNIT_Y ((vec3){0, 1, 0})
#define UNIT_Z ((vec3){0, 0, 1})

// HMD modes
#define VS_HMD_OFF 0
#define VS_HMD_SUR 1
#define VS_HMD_ORB 2

// Aero modes
#define VS_AERO_OFF 0
#define VS_AERO_MAN 1
#define VS_AERO_FBW 2

// RCS modes
#define VS_RCS_OFF 0
#define VS_RCS_MAN 1
#define VS_RCS_RR 2
#define VS_RCS_LVLH 3

// Engine modes
#define VS_ENG_OFF 0
#define VS_ENG_DRV 1
#define VS_ENG_IMP 2
#define VS_ENG_NOV 3

// Game switches
#define GS_IRCAM 0x01
#define GS_TRMAP 0x02
#define GS_MMODE_MASK 0x1C
#define GS_MMODE_SHIFT 2

// Game stages
#define GS_RUNNING 0
#define GS_PAUSED 1
#define GS_COMPLETED 2

#endif // SPACEGLIDER_TYPES_H