/*
 * boats_physics.c
 * 
 * Physics functions implementation for the boats game
 */

#include "boats_physics.h"
#include <stdlib.h>
#include <stdio.h>

// Hash constants
static const float cHashA4[4] = {0.0f, 1.0f, 57.0f, 58.0f};
static const float cHashA3[3] = {1.0f, 57.0f, 113.0f};
static const float cHashM = 43758.54f;

// Hash functions
static float* hash_v2f(float p) {
    static float result[2];
    result[0] = fmodf(sinf(p + cHashA4[0]) * cHashM, 1.0f);
    result[1] = fmodf(sinf(p + cHashA4[1]) * cHashM, 1.0f);
    return result;
}

static float* hash_v4f(float p) {
    static float result[4];
    result[0] = fmodf(sinf(p + cHashA4[0]) * cHashM, 1.0f);
    result[1] = fmodf(sinf(p + cHashA4[1]) * cHashM, 1.0f);
    result[2] = fmodf(sinf(p + cHashA4[2]) * cHashM, 1.0f);
    result[3] = fmodf(sinf(p + cHashA4[3]) * cHashM, 1.0f);
    return result;
}

static float* hash_v4v3(float px, float py, float pz) {
    static float result[4];
    const float cHashVA3[3] = {37.1f, 61.7f, 12.4f};
    float dot1 = (px + 0.0f) * cHashVA3[0] + (py + 0.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot2 = (px + 1.0f) * cHashVA3[0] + (py + 0.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot3 = (px + 0.0f) * cHashVA3[0] + (py + 1.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot4 = (px + 1.0f) * cHashVA3[0] + (py + 1.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    
    result[0] = fmodf(sinf(dot1) * cHashM, 1.0f);
    result[1] = fmodf(sinf(dot2) * cHashM, 1.0f);
    result[2] = fmodf(sinf(dot3) * cHashM, 1.0f);
    result[3] = fmodf(sinf(dot4) * cHashM, 1.0f);
    
    return result;
}

// Noise functions
float noise_ff(float p) {
    float ip = floorf(p);
    float fp = fmodf(p, 1.0f);
    fp = fp * fp * (3.0f - 2.0f * fp);  // smoothstep-like
    
    float* t = hash_v2f(ip);
    return t[0] + fp * (t[1] - t[0]);
}

float noise_fv2(float x, float y) {
    float ipx = floorf(x);
    float ipy = floorf(y);
    float fpx = fmodf(x, 1.0f);
    float fpy = fmodf(y, 1.0f);
    
    fpx = fpx * fpx * (3.0f - 2.0f * fpx);
    fpy = fpy * fpy * (3.0f - 2.0f * fpy);
    
    float dot_ip = ipx * cHashA3[0] + ipy * cHashA3[1];
    float* t = hash_v4f(dot_ip);
    
    float a = t[0] + fpx * (t[1] - t[0]);
    float b = t[2] + fpx * (t[3] - t[2]);
    
    return a + fpy * (b - a);
}

float noise_fv3a(float x, float y, float z) {
    float ipx = floorf(x);
    float ipy = floorf(y);
    float ipz = floorf(z);
    float fpx = fmodf(x, 1.0f);
    float fpy = fmodf(y, 1.0f);
    float fpz = fmodf(z, 1.0f);
    
    fpx = fpx * fpx * (3.0f - 2.0f * fpx);
    fpy = fpy * fpy * (3.0f - 2.0f * fpy);
    fpz = fpz * fpz * (3.0f - 2.0f * fpz);
    
    float* t1 = hash_v4v3(ipx, ipy, ipz);
    float* t2 = hash_v4v3(ipx, ipy, ipz + 1.0f);
    
    float a1 = t1[0] + fpx * (t1[1] - t1[0]);
    float b1 = t1[2] + fpx * (t1[3] - t1[2]);
    float c1 = a1 + fpy * (b1 - a1);
    
    float a2 = t2[0] + fpx * (t2[1] - t2[0]);
    float b2 = t2[2] + fpx * (t2[3] - t2[2]);
    float c2 = a2 + fpy * (b2 - a2);
    
    return c1 + fpz * (c2 - c1);
}

float fbm1(float p) {
    float f = 0.0f;
    float a = 1.0f;
    for (int i = 0; i < 5; i++) {
        f += a * noise_ff(p);
        a *= 0.5f;
        p *= 2.0f;
    }
    return f;
}

float fbm2(float x, float y) {
    float f = 0.0f;
    float a = 1.0f;
    for (int i = 0; i < 5; i++) {
        f += a * noise_fv2(x, y);
        a *= 0.5f;
        x *= 2.0f;
        y *= 2.0f;
    }
    return f;
}

float fbm3(float x, float y, float z) {
    // Rotation matrix for fractal noise
    float mr[9] = {0.0f, 0.8f, 0.6f, -0.8f, 0.36f, -0.48f, -0.6f, -0.48f, 0.64f};
    float f = 0.0f;
    float a = 1.0f;
    float px = x, py = y, pz = z;
    
    for (int i = 0; i < 5; i++) {
        f += a * noise_fv3a(px, py, pz);
        a *= 0.5f;
        
        // Apply rotation matrix
        float new_x = px * mr[0] + py * mr[1] + pz * mr[2];
        float new_y = px * mr[3] + py * mr[4] + pz * mr[5];
        float new_z = px * mr[6] + py * mr[7] + pz * mr[8];
        
        px = new_x * 4.0f;
        py = new_y * 4.0f;
        pz = new_z * 4.0f;
    }
    return f;
}

float fbm_n(float x, float y, float z, float nx, float ny, float nz) {
    float s[3] = {0.0f, 0.0f, 0.0f};
    float a = 1.0f;
    
    for (int i = 0; i < 5; i++) {
        float* n1 = hash_v4f(noise_fv2(y, z));
        float* n2 = hash_v4f(noise_fv2(z, x));
        float* n3 = hash_v4f(noise_fv2(x, y));
        
        s[0] += a * n1[0];
        s[1] += a * n2[0];
        s[2] += a * n3[0];
        
        a *= 0.5f;
        x *= 2.0f;
        y *= 2.0f;
        z *= 2.0f;
    }
    
    return s[0] * fabsf(nx) + s[1] * fabsf(ny) + s[2] * fabsf(nz);
}

// Wave functions
float wave_height(float x, float z, float time) {
    float qRot[4] = {1.6f, -1.2f, 1.2f, 1.6f}; // 2x2 rotation matrix
    
    float wFreq = 0.2f;
    float wAmp = 0.3f;
    float t4o_x = time;
    float t4o_z = -time;
    float q2_x = x;
    float q2_z = z;
    
    float ht = 0.0f;
    
    for (int j = 0; j < 3; j++) {
        float t4_x = (t4o_x + q2_x) * wFreq;
        float t4_z = (t4o_z + q2_z) * wFreq;
        
        float* t2 = hash_v4f(t4_x + t4_z); // Simplified for this implementation
        float t4_x_new = t4_x + 2.0f * t2[0] - 1.0f;
        float t4_z_new = t4_z + 2.0f * t2[1] - 1.0f;
        
        float ta_x = fabsf(sinf(t4_x_new));
        float ta_z = fabsf(sinf(t4_z_new));
        
        float v4_x = (1.0f - ta_x) * (ta_x + sqrtf(1.0f - ta_x * ta_x));
        float v4_z = (1.0f - ta_z) * (ta_z + sqrtf(1.0f - ta_z * ta_z));
        
        float v2_x = powf(1.0f - powf(v4_x * v4_x, 0.65f), 8.0f);
        float v2_z = powf(1.0f - powf(v4_z * v4_z, 0.65f), 8.0f);
        
        ht += (v2_x + v2_z) * wAmp;
        
        // Apply rotation
        float new_x = q2_x * qRot[0] + q2_z * qRot[1];
        float new_z = q2_x * qRot[2] + q2_z * qRot[3];
        q2_x = new_x;
        q2_z = new_z;
        
        wFreq *= 2.0f;
        wAmp *= 0.2f;
    }
    
    return ht;
}

float wave_ray(float ro_x, float ro_y, float ro_z, 
               float rd_x, float rd_y, float rd_z) {
    float dstFar = 70.0f;
    float s = 0.0f;
    float sLo = 0.0f;
    float dHit = dstFar;
    
    for (int j = 0; j < 50; j++) {
        float p_x = ro_x + s * rd_x;
        float p_y = ro_y + s * rd_y;
        float p_z = ro_z + s * rd_z;
        
        float h = p_y - wave_height(p_x, p_z, 0.0f); // Simplified time parameter
        
        if (h < 0.0f) break;
        
        sLo = s;
        s += fmaxf(0.5f, 1.3f * h) + 0.01f * s;
        
        if (s > dstFar) break;
    }
    
    if (s > sLo) {  // Wave was hit
        float sHi = s;
        for (int j = 0; j < 5; j++) {
            s = 0.5f * (sLo + sHi);
            float p_x = ro_x + s * rd_x;
            float p_y = ro_y + s * rd_y;
            float p_z = ro_z + s * rd_z;
            
            float h = (p_y - wave_height(p_x, p_z, 0.0f)) >= 0.0f ? 1.0f : 0.0f;
            
            sLo += h * (s - sLo);
            sHi += (1.0f - h) * (s - sHi);
        }
        dHit = sHi;
    }
    
    return dHit;
}

float wave_normal_x(float x, float y, float z, float distance) {
    float e = fmaxf(0.1f, 1e-4f * distance * distance);
    float h = wave_height(x, z, 0.0f);
    return (h - wave_height(x + e, z, 0.0f)) / e;
}

float wave_normal_y(float x, float y, float z, float distance) {
    return 1.0f;  // Simplified
}

float wave_normal_z(float x, float y, float z, float distance) {
    float e = fmaxf(0.1f, 1e-4f * distance * distance);
    float h = wave_height(x, z, 0.0f);
    return (h - wave_height(x, z + e, 0.0f)) / e;
}

// Utility functions
float smooth_min(float a, float b, float r) {
    float h = fminf(fmaxf(0.5f + 0.5f * (b - a) / r, 0.0f), 1.0f);
    return h * (1.0f - h) * r + b * h + a * (1.0f - h);
}

float smooth_bump(float lo, float hi, float w, float x) {
    float hi_high = hi + w;
    float hi_low = hi - w;
    float lo_high = lo + w;
    float lo_low = lo - w;
    
    float term1 = (x < hi_high) ? ((hi_high - x) / (2.0f * w)) : 0.0f;
    term1 = (x > hi_low) ? ((x - hi_low) / (2.0f * w)) : term1;
    
    float term2 = (x > lo_low) ? ((x - lo_low) / (2.0f * w)) : 0.0f;
    term2 = (x < lo_high) ? ((lo_high - x) / (2.0f * w)) : term2;
    
    return term1 * term2;
}

float* rotate_2d(float x, float y, float angle) {
    static float result[2];
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    
    result[0] = x * cos_a - y * sin_a;
    result[1] = x * sin_a + y * cos_a;
    
    return result;
}

// Distance functions for shapes
float pr_round_box_df(float px, float py, float pz, float bx, float by, float bz, float r) {
    float q[3] = {fabsf(px) - bx, fabsf(py) - by, fabsf(pz) - bz};
    float max_q = fmaxf(fmaxf(q[0], q[1]), q[2]);
    return fminf(max_q, 0.0f) + sqrtf(
        powf(fmaxf(q[0], 0.0f), 2) + 
        powf(fmaxf(q[1], 0.0f), 2) + 
        powf(fmaxf(q[2], 0.0f), 2)
    ) - r;
}

float pr_sph_df(float px, float py, float pz, float s) {
    return sqrtf(px*px + py*py + pz*pz) - s;
}

float pr_cyl_df(float px, float py, float pz, float r, float h) {
    float d[2] = {sqrtf(px*px + pz*pz) - r, fabsf(py) - h};
    float max_d = fmaxf(d[0], d[1]);
    return max_d;
}

float pr_caps_df(float px, float py, float pz, float r, float h) {
    float p[3] = {px, py, pz};
    float h_clamped = h * fmaxf(-1.0f, fminf(1.0f, p[2] / h));
    float dist[3] = {p[0], p[1], p[2] - h_clamped};
    return sqrtf(dist[0]*dist[0] + dist[1]*dist[1] + dist[2]*dist[2]) - r;
}

float pr_cyl_an_df(float px, float py, float pz, float r, float w, float h) {
    float d[2] = {fabsf(sqrtf(px*px + pz*pz) - r) - w, fabsf(py) - h};
    return fmaxf(d[0], d[1]);
}

float pr_r_cyl_df(float px, float py, float pz, float r, float rt, float h) {
    float dxy = sqrtf(px*px + pz*pz) - r;
    float dz = fabsf(py) - h;
    
    float dc[2] = {dxy + rt, dz + rt};
    float min1 = fminf(fmaxf(dc[0], dz), fmaxf(dc[1], dxy));
    float min2 = sqrtf(dc[0]*dc[0] + dc[1]*dc[1]) - rt;
    
    return fminf(fminf(min1, min2), sqrtf(dxy*dxy + dz*dz));
}

// Physics updates
void boat_physics_matrix(Boat* boat) {
    float bAz = 0.5f * PI - boat->angle;
    float bd_x = 0.0f, bd_y = 0.0f, bd_z = 1.0f;
    
    // Rotate bd vector
    float* rotated_bd = rotate_2d(bd_x, bd_z, bAz);
    bd_x = rotated_bd[0];
    bd_z = rotated_bd[1];
    
    // Calculate wave heights at various points around the boat
    float h[5];
    h[0] = wave_height(boat->x, boat->z, 0.0f);
    h[1] = wave_height(boat->x + 0.5f * bd_x, boat->z + 0.5f * bd_z, 0.0f);
    h[2] = wave_height(boat->x - 0.5f * bd_x, boat->z - 0.5f * bd_z, 0.0f);
    
    // Rotate bd vector 90 degrees
    float* rotated_bd2 = rotate_2d(bd_x, bd_z, -0.5f * PI);
    float bd_x2 = rotated_bd2[0];
    float bd_z2 = rotated_bd2[1];
    
    h[3] = wave_height(boat->x + 1.3f * bd_x2, boat->z + 1.3f * bd_z2, 0.0f);
    h[4] = wave_height(boat->x - 1.3f * bd_x2, boat->z - 1.3f * bd_z2, 0.0f);
    
    // Update boat position
    boat->y = 0.13f + (2.0f * h[0] + h[1] + h[2] + h[3] + h[4]) / 6.0f;
    
    // Update boat orientation
    float c = cosf(bAz);
    float s = sinf(bAz);
    
    // Set the rotation matrix
    boat->rotation_matrix[0] = c;           boat->rotation_matrix[1] = 0.0f;  boat->rotation_matrix[2] = s;
    boat->rotation_matrix[3] = 0.0f;        boat->rotation_matrix[4] = 1.0f;  boat->rotation_matrix[5] = 0.0f;
    boat->rotation_matrix[6] = -s;          boat->rotation_matrix[7] = 0.0f;  boat->rotation_matrix[8] = c;
}

void update_boat_position(Boat* boat, float time) {
    // Apply physics
    boat_physics_matrix(boat);
    
    // Simple movement
    boat->x += cosf(boat->angle) * boat->speed * 0.016f; // Assuming ~60 FPS
    boat->z += sinf(boat->angle) * boat->speed * 0.016f;
    
    // Add some wave motion
    boat->angle += sinf(time) * 0.001f;
}

void update_drone_position(Drone* drone, float time) {
    // Circular movement for demonstration
    drone->x = 5.0f * cosf(time * 0.5f);
    drone->z = 5.0f * sinf(time * 0.5f);
    
    // Adjust height based on waves
    float wave_h = wave_height(drone->x, drone->z, time);
    drone->y = 8.0f + wave_h * 0.5f + sinf(time * 2.0f) * 1.0f;
    
    // Update rotation
    drone->pitch = sinf(time * 3.0f) * 0.1f;
    drone->roll = cosf(time * 2.5f) * 0.05f;
    drone->yaw = time * 0.2f; // Slowly rotate around vertical axis
}