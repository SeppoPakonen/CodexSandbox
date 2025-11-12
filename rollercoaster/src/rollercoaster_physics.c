#include "../include/rollercoaster.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Vector operations
vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3_mul(vec3 a, float s) {
    return (vec3){a.x * s, a.y * s, a.z * s};
}

vec3 vec3_div(vec3 a, float s) {
    return (vec3){a.x / s, a.y / s, a.z / s};
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3_normalize(vec3 v) {
    float len = vec3_length(v);
    if (len > 0.0f) {
        return vec3_div(v, len);
    }
    return (vec3){0.0f, 0.0f, 0.0f};
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

vec3 vec3_abs(vec3 v) {
    return (vec3){fabsf(v.x), fabsf(v.y), fabsf(v.z)};
}

vec3 vec3_max(vec3 a, vec3 b) {
    return (vec3){
        fmaxf(a.x, b.x),
        fmaxf(a.y, b.y),
        fmaxf(a.z, b.z)
    };
}

vec3 vec3_min(vec3 a, vec3 b) {
    return (vec3){
        fminf(a.x, b.x),
        fminf(a.y, b.y),
        fminf(a.z, b.z)
    };
}

float fmax3(float a, float b, float c) {
    return fmaxf(a, fmaxf(b, c));
}

float fmin3(float a, float b, float c) {
    return fminf(a, fminf(b, c));
}

// Math utilities
float clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

float smoothstep(float edge0, float edge1, float x) {
    if (x <= edge0) return 0.0f;
    if (x >= edge1) return 1.0f;

    float t = (x - edge0) / (edge1 - edge0);
    return t * t * (3.0f - 2.0f * t);
}

float mix(float x, float y, float a) {
    return x * (1.0f - a) + y * a;
}

// Noise functions
float dot_vec2(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

vec4 hashv4f(float p) {
    const float cHashA4[4] = {0.0f, 1.0f, 57.0f, 58.0f};
    const float cHashM = 43758.54f;

    vec4 result;
    result.x = fmodf(sinf(p + cHashA4[0]) * cHashM, 1.0f);
    result.y = fmodf(sinf(p + cHashA4[1]) * cHashM, 1.0f);
    result.z = fmodf(sinf(p + cHashA4[2]) * cHashM, 1.0f);
    result.w = fmodf(sinf(p + cHashA4[3]) * cHashM, 1.0f);

    return result;
}

float noisefv2(vec2 p) {
    vec2 ip = {(float)(int)floorf(p.x), (float)(int)floorf(p.y)};
    vec2 fp = {p.x - floorf(p.x), p.y - floorf(p.y)};

    // Interpolate
    fp.x = fp.x * fp.x * (3.0f - 2.0f * fp.x);
    fp.y = fp.y * fp.y * (3.0f - 2.0f * fp.y);

    vec4 t = hashv4f(dot_vec2(ip, (vec2){1.0f, 57.0f}));

    float a = mix(t.x, t.y, fp.x);
    float b = mix(t.z, t.w, fp.x);
    return mix(a, b, fp.y);
}

float fbm(vec2 p) {
    float f = 0.0f;
    float a = 1.0f;

    for (int i = 0; i < 5; i++) {
        f += a * noisefv2(p);
        a *= 0.5f;
        p.x *= 2.0f;
        p.y *= 2.0f;
    }

    return f;
}

// vec2 utility functions
vec2 vec2_scale(vec2 v, float s) {
    return (vec2){v.x * s, v.y * s};
}

// Geometry functions
float sdBox(vec3 p, vec3 b) {
    vec3 d = vec3_abs(p);
    d.x -= b.x;
    d.y -= b.y;
    d.z -= b.z;
    vec3 max_d = vec3_max(d, (vec3){0.0f, 0.0f, 0.0f});
    float max_val = fmax3(d.x, d.y, d.z);
    return fminf(max_val, 0.0f) + vec3_length(max_d);
}

float sdSphere(vec3 p, float s) {
    return vec3_length(p) - s;
}

float sdCylinder(vec3 p, float r, float h) {
    float xy_len = sqrtf(p.x * p.x + p.y * p.y);
    float d1 = xy_len - r;
    float d2 = fabsf(p.z) - h;
    return fmaxf(d1, d2);
}

float sdCapsule(vec3 p, float r, float h) {
    vec3 q = p;
    q.z -= h * clamp(p.z / h, -1.0f, 1.0f);
    return vec3_length(q) - r;
}

float sdCylinderAn(vec3 p, float r, float w, float h) {
    float xy_len = sqrtf(p.x * p.x + p.y * p.y);
    float d1 = fabsf(xy_len - r) - w;
    float d2 = fabsf(p.z) - h;
    return fmaxf(d1, d2);
}

vec2 rot2D(vec2 q, float a) {
    float cos_a = cosf(a);
    float sin_a = sinf(a);
    return (vec2){
        q.x * cos_a - q.y * sin_a,
        q.x * sin_a + q.y * cos_a
    };
}

mat3 axToRMat(vec3 vz, vec3 vy) {
    vz = vec3_normalize(vz);
    vec3 vx = vec3_normalize(vec3_cross(vy, vz));
    vy = vec3_cross(vz, vx);

    mat3 result;
    result.m[0][0] = vx.x; result.m[1][0] = vy.x; result.m[2][0] = vz.x;
    result.m[0][1] = vx.y; result.m[1][1] = vy.y; result.m[2][1] = vz.y;
    result.m[0][2] = vx.z; result.m[1][2] = vy.z; result.m[2][2] = vz.z;

    return result;
}

// Initialize the roller coaster
void initRollerCoaster(RollerCoasterState* state) {
    memset(state, 0, sizeof(RollerCoasterState));
    trackSetup(state);
}

// Update the roller coaster state
void updateRollerCoaster(RollerCoasterState* state, float time) {
    state->tCur = time;

    vec3 oDir, oNorm;
    float vel = 0.8f;

    for (int k = 0; k < NCAR; k++) {
        float t = vel * time - 2.0f + state->tLen[NSEG] * (float)(NCAR - 1 - k) / (float)NCAR;
        state->carPos[k] = trackPath(state, t, &oDir, &oNorm);
        state->carMat[k] = axToRMat(oDir, oNorm);
    }
}

// Track setup
void trackSetup(RollerCoasterState* state) {
    vec3 cPtOrg = {2.0f, 0.0f, -3.0f};
    float hTop = 1.5f;
    float rLoop = 2.2f;
    float sLoop = 0.3f;
    float sHzRamp = 0.5f;
    float rDnCirc = 2.0f;
    float rUpCirc = rDnCirc + sLoop;
    float sDnRamp = 1.5f;
    float sUpRamp = 1.3f * sDnRamp;
    float sHzStr = sDnRamp - sUpRamp + 3.0f * sHzRamp;
    float tWait = 2.0f;
    float vfFast = 5.0f;
    float vfLoop = 0.6f;

    vec3 cDnCirc = vec3_add(cPtOrg, (vec3){-rDnCirc - sLoop, 0.0f, -2.0f * sHzRamp});
    vec3 cUpCirc = vec3_add(cPtOrg, (vec3){-rUpCirc + sLoop, 2.0f * hTop, 2.0f * sDnRamp + 6.0f * sHzRamp});

    state->cPt[0] = vec3_add(cDnCirc, (vec3){-rDnCirc, 0.0f, sHzStr});
    state->cPt[1] = vec3_add(state->cPt[0], (vec3){0.0f, 0.0f, sHzStr});
    state->cPt[3] = vec3_sub(cUpCirc, vec3_mul((vec3){rUpCirc, 0.0f, 0.0f}, 1.0f));
    state->cPt[4] = vec3_add(cUpCirc, vec3_mul((vec3){rUpCirc, 0.0f, 0.0f}, 1.0f));
    state->cPt[2] = vec3_sub(state->cPt[3], vec3_mul((vec3){0.0f, 0.0f, 2.0f * sHzRamp}, 1.0f));
    state->cPt[5] = vec3_sub(state->cPt[4], vec3_mul((vec3){0.0f, 0.0f, 2.0f * sHzRamp}, 1.0f));
    state->cPt[7] = vec3_add(cPtOrg, vec3_mul((vec3){sLoop, 0.0f, 0.0f}, 1.0f));
    state->cPt[8] = vec3_sub(cPtOrg, vec3_mul((vec3){sLoop, 0.0f, 0.0f}, 1.0f));
    state->cPt[6] = vec3_add(state->cPt[7], vec3_mul((vec3){0.0f, 0.0f, 4.0f * sHzRamp}, 1.0f));
    state->cPt[9] = vec3_add(cDnCirc, vec3_mul((vec3){rDnCirc, 0.0f, 0.0f}, 1.0f));
    state->cPt[10] = vec3_sub(cDnCirc, vec3_mul((vec3){rDnCirc, 0.0f, 0.0f}, 1.0f));
    state->cPt[NSEG - 1] = state->cPt[0];

    state->tLen[0] = 0.0f;
    for (int k = 1; k < NSEG; k++) {
        state->tLen[k] = vec3_length(vec3_sub(state->cPt[k], state->cPt[k - 1]));
    }

    state->tLen[4] = PI * rUpCirc;
    state->tLen[6] /= 0.5f * (1.0f + vfFast);
    state->tLen[8] = sqrtf(2.0f * PI * rLoop * 2.0f * rLoop + 2.0f * sLoop * 2.0f * sLoop) * (1.0f + vfLoop);
    state->tLen[10] = PI * rDnCirc;

    for (int k = 7; k < NSEG - 1; k++) {
        state->tLen[k] /= vfFast;
    }

    for (int k = 1; k < NSEG; k++) {
        state->tLen[k] += state->tLen[k - 1];
    }

    state->tLen[NSEG] = state->tLen[NSEG - 1] + tWait;
}

// Track path calculation
vec3 trackPath(RollerCoasterState* state, float t, vec3* oDir, vec3* oNorm) {
    vec3 p, p1, p2, u;
    float w, ft, s;
    int ik;
    
    // Define constants
    float hTop = 1.5f;
    float rLoop = 2.2f;
    float sLoop = 0.3f;
    float sHzRamp = 0.5f;
    float sDnRamp = 1.5f;  // Needed for cUpCirc calculation
    float rDnCirc = 2.0f;
    float rUpCirc = rDnCirc + sLoop;
    float vfFast = 5.0f;
    float vfLoop = 0.6f;

    t = fmodf(t, state->tLen[NSEG]);
    ik = -1;

    for (int k = 1; k < NSEG; k++) {
        if (t < state->tLen[k]) {
            t -= state->tLen[k - 1];
            p1 = state->cPt[k - 1];
            p2 = state->cPt[k];
            w = state->tLen[k] - state->tLen[k - 1];
            ik = k;
            break;
        }
    }

    *oNorm = (vec3){0.0f, 1.0f, 0.0f};
    ft = t / w;

    if (ik < 0) {
        p = state->cPt[0];
        *oDir = (vec3){0.0f, 0.0f, 1.0f};
    } else if (ik == 2 || ik == 6) {
        *oDir = vec3_sub(p2, p1);
        if (ik == 6) ft *= (2.0f + (vfFast - 1.0f) * ft) / (vfFast + 1.0f);
        p = vec3_add(p1, vec3_mul(vec3_sub(p2, p1), ft));
        float y_offset = p1.y + (p2.y - p1.y) * smoothstep(0.0f, 1.0f, ft);
        p.y = y_offset;
        oDir->y *= 6.0f * ft * (1.0f - ft);
        *oDir = vec3_normalize(*oDir);
    } else if (ik == 4) {
        ft *= PI;
        // Calculate cUpCirc equivalent based on trackSetup
        vec3 cPtOrg = {2.0f, 0.0f, -3.0f};
        vec3 cUpCirc = vec3_add(cPtOrg, (vec3){-rUpCirc + sLoop, 2.0f * hTop, 2.0f * sDnRamp + 6.0f * sHzRamp});
        p = cUpCirc;
        float cos_ft = cosf(ft);
        float sin_ft = sinf(ft);
        u = (vec3){-cos_ft, 0.0f, sin_ft};
        p.x += rUpCirc * u.x;
        p.z += rUpCirc * u.z;
        *oDir = vec3_cross(*oNorm, u);
    } else if (ik == 8) {
        float ft_orig = ft;
        if (ft < 0.5f) {
            ft = ft_orig * (1.0f + vfLoop * (1.0f - 2.0f * ft_orig));
        } else {
            ft = ft_orig * (1.0f + 2.0f * vfLoop * (ft_orig - 1.5f)) + vfLoop;
        }

        p = vec3_mul(vec3_add(state->cPt[7], state->cPt[8]), 0.5f);
        p.x += sLoop * (1.0f - 2.0f * ft);

        ft *= 2.0f * PI;
        float cos_ft = cosf(ft);
        float sin_ft = sinf(ft);

        u = (vec3){0.0f, cos_ft, sin_ft};
        p.y += rLoop * (1.0f - u.y);
        p.z += rLoop * (0.0f - u.z);

        *oNorm = u;
        *oDir = (vec3){
            -2.0f * sLoop,
            2.0f * PI * rLoop * sin_ft,
            2.0f * PI * rLoop * (-cos_ft)
        };
        *oDir = vec3_normalize(*oDir);
    } else if (ik == 10) {
        ft *= PI;
        // Calculate cDnCirc equivalent based on trackSetup
        vec3 cPtOrg = {2.0f, 0.0f, -3.0f};
        vec3 cDnCirc = vec3_add(cPtOrg, (vec3){-rDnCirc - sLoop, 0.0f, -2.0f * sHzRamp});
        p = cDnCirc;
        float cos_ft = cosf(ft);
        float sin_ft = sinf(ft);
        u = (vec3){cos_ft, 0.0f, -sin_ft};
        p.x += rDnCirc * u.x;
        p.z += rDnCirc * u.z;
        *oDir = vec3_cross(*oNorm, u);
    } else if (ik < NSEG) {
        *oDir = vec3_sub(p2, p1);
        p = vec3_add(p1, vec3_mul(*oDir, ft));
        *oDir = vec3_normalize(*oDir);
    }

    return p;
}

// Track distance function
float trackDf(RollerCoasterState* state, vec3 p, float dMin) {
    vec3 q;
    vec2 csTrk, tr;
    float d, f;
    float wTrk = 0.015f;
    float hTrk = 0.025f;
    csTrk = (vec2){wTrk, hTrk};

    vec3 cPtOrg = {2.0f, 0.0f, -3.0f};
    q = vec3_sub(p, cPtOrg);
    q.y -= 2.2f; // rLoop

    f = smoothstep(0.0f, 1.0f, atan2f(fabsf(q.z), -q.y) / PI);

    tr = (vec2){
        sqrtf(q.y * q.y + q.z * q.z) - 2.2f, // length(q.yz) - rLoop
        q.x - ((q.z >= 0) ? 1 : -1) * 0.3f * f // sign(q.z) * sLoop * f, sLoop=0.3f
    };

    // Simplified track collision - a full implementation would be more complex
    d = sdBox(p, (vec3){0.1f, 0.1f, 0.1f}); // Placeholder

    if (d < dMin) {
        dMin = d;
        state->idObj = 1;
    }

    return dMin;
}

// Object distance function
float objDf(RollerCoasterState* state, vec3 p) {
    float dMin = 60.0f; // dstFar
    dMin = trackDf(state, p, dMin);

    // Add other objects (ground, supports, etc.)
    float d_ground = p.y; // Ground plane at y=0
    if (d_ground < dMin) {
        dMin = d_ground;
        state->idObj = 6; // Ground
    }

    return dMin;
}

// Ray-object intersection
float objRay(RollerCoasterState* state, vec3 ro, vec3 rd) {
    float dHit = 0.0f;
    for (int j = 0; j < 150; j++) {
        float d = objDf(state, vec3_add(ro, vec3_mul(rd, dHit)));
        dHit += d;
        if (d < 0.0005f || dHit > 60.0f) break; // dstFar = 60.0f
    }
    return dHit;
}

// Normal calculation
vec3 objNf(RollerCoasterState* state, vec3 p) {
    const float e = 0.0002f;
    float vx = objDf(state, (vec3){p.x + e, p.y + e, p.z + e}) -
               objDf(state, (vec3){p.x + e, p.y - e, p.z - e}) -
               objDf(state, (vec3){p.x - e, p.y + e, p.z - e}) -
               objDf(state, (vec3){p.x - e, p.y - e, p.z + e});

    float vy = objDf(state, (vec3){p.x + e, p.y + e, p.z - e}) +
               objDf(state, (vec3){p.x - e, p.y + e, p.z + e}) -
               objDf(state, (vec3){p.x + e, p.y - e, p.z + e}) -
               objDf(state, (vec3){p.x - e, p.y - e, p.z - e});

    float vz = objDf(state, (vec3){p.x + e, p.y - e, p.z + e}) +
               objDf(state, (vec3){p.x - e, p.y + e, p.z + e}) -
               objDf(state, (vec3){p.x + e, p.y + e, p.z - e}) -
               objDf(state, (vec3){p.x - e, p.y - e, p.z - e});

    vec3 n = {vx, vy, vz};
    return vec3_normalize(n);
}

// Shadow calculation
float objSShadow(RollerCoasterState* state, vec3 ro, vec3 rd) {
    float sh = 1.0f;
    float d = 0.03f;
    for (int j = 0; j < 25; j++) {
        float h = objDf(state, vec3_add(ro, vec3_mul(rd, d)));
        sh = fminf(sh, smoothstep(0.0f, 1.0f, 30.0f * h / d));
        d += fminf(0.25f, 2.0f * h);
        if (h < 0.001f) break;
    }
    return 0.8f + 0.2f * sh;
}

// Background color
vec3 bgCol(RollerCoasterState* state, vec3 ro, vec3 rd, vec3 sunDir) {
    vec3 col;
    vec2 w;
    float sd, f;

    vec2 e = {0.01f, 0.0f};

    if (rd.y >= 0.0f) {
        ro.x += 2.0f * state->tCur;
        ro.z += 2.0f * state->tCur;

        sd = fmaxf(vec3_dot(rd, sunDir), 0.0f);
        col = (vec3){0.1f, 0.2f, 0.4f};
        col = vec3_add(col, vec3_mul((vec3){0.2f, 0.2f, 0.2f}, (1.0f - fmaxf(rd.y, 0.0f))));
        col = vec3_add(col, vec3_mul((vec3){0.1f, 0.1f, 0.1f}, powf(sd, 16.0f)));
        col = vec3_add(col, vec3_mul((vec3){0.2f, 0.2f, 0.2f}, powf(sd, 256.0f)));

        float t = (100.0f - ro.y) / fmaxf(rd.y, 0.001f);
        w.x = 0.02f * (ro.x + rd.x * t);
        w.y = 0.02f * (ro.z + rd.z * t);
        f = fbm(w);

        float mix_val = clamp(0.2f + 0.8f * f * rd.y, 0.0f, 1.0f);
        col.x = mix(col.x, 1.0f, mix_val);
        col.y = mix(col.y, 1.0f, mix_val);
        col.z = mix(col.z, 1.0f, mix_val);
    } else {
        float t = -ro.y / rd.y;
        ro = vec3_add(ro, vec3_mul(rd, t));

        w.x = (state->riding ? 5.0f : 0.8f) * ro.x;
        w.y = (state->riding ? 5.0f : 0.8f) * ro.z;

        f = fbm(w);
        vec3 vn = vec3_normalize((vec3){
            f - fbm((vec2){w.x + e.x, w.y}),
            0.1f,
            f - fbm((vec2){w.x, w.y + e.x})
        });

        col = vec3_mul((vec3){0.4f, 0.3f, 0.1f}, (1.0f - 0.1f * noisefv2(w)));
        col = vec3_add(col, vec3_mul((vec3){0.0f, 0.2f, 0.1f}, f));

        float dist_factor = 0.15f * sqrtf(ro.x * ro.x * 2.8f + ro.z * ro.z * 1.0f);
        float smooth_val = smoothstep(7.0f, 8.0f, dist_factor);
        vec3 mixed_col = vec3_add(
            vec3_mul((vec3){0.6f, 0.5f, 0.3f}, (1.0f - 0.2f * fbm(vec2_scale(w, 137.1f)))),
            vec3_mul(vec3_mul((vec3){0.4f, 0.5f, 0.2f}, 0.8f), f)
        );
        col = vec3_add(
            vec3_mul(col, 1.0f - smooth_val),
            vec3_mul(mixed_col, smooth_val)
        );

        float light = fmaxf(vec3_dot(vn, sunDir), 0.0f);
        col = vec3_mul(col, (1.0f - 0.1f * light));
        col = vec3_add(col, vec3_mul(vec3_mul(col, 0.1f), light));

        float blend = powf(1.0f + rd.y, 64.0f);
        col = vec3_add(
            vec3_mul(col, 1.0f - blend),
            vec3_mul((vec3){0.45f, 0.55f, 0.7f}, blend)
        );
    }

    return col;
}

// Scene rendering
vec3 showScene(RollerCoasterState* state, vec3 ro, vec3 rd) {
    vec3 objCol, col, vn;
    float dstHit;
    int idObjT;

    dstHit = objRay(state, ro, rd);

    if (dstHit < 60.0f) { // dstFar
        ro = vec3_add(ro, vec3_mul(rd, dstHit));
        idObjT = state->idObj;
        vn = (state->idObj == 6) ? (vec3){0.0f, 1.0f, 0.0f} : objNf(state, ro);
        state->idObj = idObjT;

        if (state->idObj != 6) {
            switch (state->idObj) {
                case 1: // Track
                    objCol = (vec3){0.9f, 0.9f, 1.0f};
                    break;
                case 2: // Support structure
                    objCol = (vec3){0.8f, 0.5f, 0.2f};
                    objCol = vec3_mul(objCol, (1.0f - 0.2f * fbm(vec2_scale((vec2){ro.x, ro.z}, 100.0f))));
                    break;
                case 3: // Cars
                    objCol = (vec3){1.0f, 0.0f, 0.0f};
                    break;
                case 4: // Other
                    objCol = (vec3){0.5f, 0.3f, 0.0f};
                    break;
                case 5: // More objects
                    objCol = (vec3){0.0f, 1.0f, 0.0f};
                    break;
                default:
                    objCol = (vec3){0.7f, 0.7f, 0.7f};
                    break;
            }

            float diffuse = 0.3f + 0.7f * fmaxf(vec3_dot(vn, state->sunDir), 0.0f);
            col = vec3_mul(objCol, diffuse);

            // Specular highlight
            vec3 reflect_dir = vec3_normalize(vec3_sub(state->sunDir, vec3_mul(vn, 2.0f * vec3_dot(vn, state->sunDir))));
            float specular = powf(fmaxf(0.0f, vec3_dot(state->sunDir, reflect_dir)), 32.0f);
            col = vec3_add(col, vec3_mul((vec3){specular, specular, specular}, 0.5f));

            if (!state->riding) {
                col = vec3_mul(col, objSShadow(state, ro, state->sunDir));
            }
        } else {
            col = bgCol(state, ro, rd, state->sunDir);
        }
    } else {
        col = bgCol(state, ro, rd, state->sunDir);
    }

    // Clamp color values
    col.x = fminf(fmaxf(col.x, 0.0f), 1.0f);
    col.y = fminf(fmaxf(col.y, 0.0f), 1.0f);
    col.z = fminf(fmaxf(col.z, 0.0f), 1.0f);

    return col;
}