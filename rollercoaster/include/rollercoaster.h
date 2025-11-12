#ifndef ROLLERCOASTER_H
#define ROLLERCOASTER_H

#include <math.h>
#include <stdbool.h>

// Constants
#define PI 3.14159f
#define NCAR 5
#define NSEG 12

// Vector structures
typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;

typedef struct {
    float m[3][3];  // 3x3 matrix
} mat3;

// Roller coaster state
typedef struct {
    vec3 carPos[NCAR];
    mat3 carMat[NCAR];
    vec3 cPt[NSEG];
    float tLen[NSEG + 1];
    float tCur;
    bool riding;
    int idObj;
    vec3 sunDir;
} RollerCoasterState;

// Physics functions
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float s);
vec3 vec3_div(vec3 a, float s);
float vec3_dot(vec3 a, vec3 b);
float vec3_length(vec3 v);
vec3 vec3_normalize(vec3 v);
vec3 vec3_cross(vec3 a, vec3 b);
vec3 vec3_abs(vec3 v);
vec3 vec3_max(vec3 a, vec3 b);  // Component-wise max
vec3 vec3_min(vec3 a, vec3 b);  // Component-wise min
float fmax3(float a, float b, float c);  // Max of 3 values
float fmin3(float a, float b, float c);  // Min of 3 values

// Math utilities
float clamp(float value, float min_val, float max_val);
float smoothstep(float edge0, float edge1, float x);
float mix(float x, float y, float a);

// Noise functions
float dot_vec2(vec2 a, vec2 b);
float noisefv2(vec2 p);
float fbm(vec2 p);
vec2 vec2_scale(vec2 v, float s);

// Geometry functions
float sdBox(vec3 p, vec3 b);
float sdSphere(vec3 p, float s);
float sdCylinder(vec3 p, float r, float h);
float sdCapsule(vec3 p, float r, float h);
float sdCylinderAn(vec3 p, float r, float w, float h);

// Track functions
void trackSetup(RollerCoasterState* state);
vec3 trackPath(RollerCoasterState* state, float t, vec3* oDir, vec3* oNorm);
float trackDf(RollerCoasterState* state, vec3 p, float dMin);
float objDf(RollerCoasterState* state, vec3 p);
float objRay(RollerCoasterState* state, vec3 ro, vec3 rd);
vec3 objNf(RollerCoasterState* state, vec3 p);
float objSShadow(RollerCoasterState* state, vec3 ro, vec3 rd);
vec3 showScene(RollerCoasterState* state, vec3 ro, vec3 rd);
vec3 bgCol(RollerCoasterState* state, vec3 ro, vec3 rd, vec3 sunDir);
vec2 rot2D(vec2 q, float a);
mat3 axToRMat(vec3 vz, vec3 vy);

// Main functions
void initRollerCoaster(RollerCoasterState* state);
void updateRollerCoaster(RollerCoasterState* state, float time);

#endif