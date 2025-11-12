#include "billiard.h"
#include <stdlib.h>
#include <time.h>

#define C_HASH_M 43758.54f
#define F_OVERLAP 1000.0f
#define FRIC 0.015f
#define BALL_RADIUS 0.46f

// Vector operations
vec2 vec2_add(vec2 a, vec2 b) {
    return (vec2){a.x + b.x, a.y + b.y};
}

vec2 vec2_sub(vec2 a, vec2 b) {
    return (vec2){a.x - b.x, a.y - b.y};
}

vec2 vec2_mul(vec2 a, float s) {
    return (vec2){a.x * s, a.y * s};
}

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3_mul(vec3 a, float s) {
    return (vec3){a.x * s, a.y * s, a.z * s};
}

vec4 vec4_mul(vec4 a, float s) {
    return (vec4){a.x * s, a.y * s, a.z * s, a.w * s};
}

float dot_vec2(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float dot_vec3(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross_vec3(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float length_vec2(vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

float distance_vec2(vec2 a, vec2 b) {
    vec2 diff = vec2_sub(a, b);
    return length_vec2(diff);
}

vec2 normalize_vec2(vec2 v) {
    float len = length_vec2(v);
    if (len > 0.00001f) {
        return vec2_mul(v, 1.0f / len);
    }
    return (vec2){0.0f, 0.0f};
}

vec3 normalize_vec3(vec3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.00001f) {
        return vec3_mul(v, 1.0f / len);
    }
    return (vec3){0.0f, 0.0f, 0.0f};
}

float clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float mix(float x, float y, float a) {
    return x * (1.0f - a) + y * a;
}

// Hash function
float hash(float p) {
    return fmodf(sinf(p) * C_HASH_M, 1.0f);
}

float noise(vec2 p) {
    vec2 i = (vec2){floorf(p.x), floorf(p.y)};
    vec2 f = (vec2){fmodf(p.x, 1.0f), fmodf(p.y, 1.0f)};
    
    // Fade function
    f.x = f.x * f.x * (3.0f - 2.0f * f.x);
    f.y = f.y * f.y * (3.0f - 2.0f * f.y);
    
    // Hash values for 4 corners
    float a = hash(i.x + 0.0f + 1.0f * (i.y + 0.0f));
    float b = hash(i.x + 1.0f + 1.0f * (i.y + 0.0f));
    float c = hash(i.x + 0.0f + 1.0f * (i.y + 1.0f));
    float d = hash(i.x + 1.0f + 1.0f * (i.y + 1.0f));
    
    // Interpolate
    float u = mix(a, b, f.x);
    float v = mix(c, d, f.x);
    return mix(u, v, f.y);
}

// Quaternion and matrix operations
mat3 quat_to_rotation_matrix(vec4 q) {
    mat3 m;
    float a1, a2, s;
    
    s = q.w * q.w - 0.5f;
    m.col0.x = q.x * q.x + s;  m.col1.y = q.y * q.y + s;  m.col2.z = q.z * q.z + s;
    a1 = q.x * q.y;  a2 = q.z * q.w;  m.col0.y = a1 + a2;  m.col1.x = a1 - a2;
    a1 = q.x * q.z;  a2 = q.y * q.w;  m.col2.x = a1 + a2;  m.col0.z = a1 - a2;
    a1 = q.y * q.z;  a2 = q.x * q.w;  m.col1.z = a1 + a2;  m.col2.y = a1 - a2;
    
    m.col0 = vec3_mul(m.col0, 2.0f);
    m.col1 = vec3_mul(m.col1, 2.0f);
    m.col2 = vec3_mul(m.col2, 2.0f);
    
    return m;
}

vec4 rotation_matrix_to_quat(mat3 m) {
    vec4 q;
    const float tol = 1e-6f;
    
    q.w = 0.5f * sqrtf(fmaxf(1.0f + m.col0.x + m.col1.y + m.col2.z, 0.0f));
    if (fabsf(q.w) > tol) {
        q.x = (m.col1.z - m.col2.y) / (4.0f * q.w);
        q.y = (m.col2.x - m.col0.z) / (4.0f * q.w);
        q.z = (m.col0.y - m.col1.x) / (4.0f * q.w);
    } else {
        q.x = sqrtf(fmaxf(0.5f * (1.0f + m.col0.x), 0.0f));
        if (fabsf(q.x) > tol) {
            q.y = m.col0.y / q.x;
            q.z = m.col0.z / q.x;
        } else {
            q.y = sqrtf(fmaxf(0.5f * (1.0f + m.col1.y), 0.0f));
            if (fabsf(q.y) > tol) {
                q.z = m.col1.z / q.y;
            } else {
                q.z = 1.0f;
            }
        }
    }
    
    // Normalize the quaternion
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (len > 0.0f) {
        q.x /= len;
        q.y /= len;
        q.z /= len;
        q.w /= len;
    }
    
    return q;
}

mat3 axis_angle_to_rotation_matrix(vec3 axis, float angle) {
    mat3 m;
    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1.0f - c;
    
    vec3 normalized_axis = normalize_vec3(axis);
    float x = normalized_axis.x;
    float y = normalized_axis.y;
    float z = normalized_axis.z;
    
    m.col0.x = t * x * x + c;
    m.col0.y = t * x * y - s * z;
    m.col0.z = t * x * z + s * y;
    
    m.col1.x = t * x * y + s * z;
    m.col1.y = t * y * y + c;
    m.col1.z = t * y * z - s * x;
    
    m.col2.x = t * x * z - s * y;
    m.col2.y = t * y * z + s * x;
    m.col2.z = t * z * z + c;
    
    return m;
}

vec4 euler_to_quat(vec3 e) {
    float a1 = 0.5f * e.y;
    float a2 = 0.5f * (e.x - e.z);
    float a3 = 0.5f * (e.x + e.z);
    float s1 = sinf(a1);
    float c1 = cosf(a1);
    
    vec4 q;
    q.x = s1 * cosf(a2);
    q.y = s1 * sinf(a2);
    q.z = c1 * sinf(a3);
    q.w = c1 * cosf(a3);
    
    // Normalize
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (len > 0.0f) {
        q.x /= len;
        q.y /= len;
        q.z /= len;
        q.w /= len;
    }
    
    return q;
}

// Initialize game state
void init_game_state(GameState *state) {
    state->hb_len = HB_LEN;
    state->dt = 0.03f;
    state->t_cur = 0.0f;
    state->a_cue = 0.0f;
    state->n_play = 0.0f;
    state->n_step = 0.0f;
    state->run_state = 0.0f;
    state->d_cue = 0.0f;
    state->show_cue = true;
    
    // Initialize light direction
    state->lt_dir = normalize_vec3((vec3){1.0f, 3.0f, 1.0f});
    
    // Set initial camera parameters
    state->az = 0.0f;
    state->el = 0.4f;
    state->zm_fac = 15.0f;
    
    init_balls(state);
}

void init_balls(GameState *state) {
    vec3 e;
    vec2 r, v;
    float s, fm;
    
    for (int mId = 0; mId < NBALL; mId++) {
        if (mId == 0) {  // Cue ball
            r.x = 0.0f;
            r.y = -0.6f * state->hb_len;
        } else if (mId < NBALL) {
            fm = (float)mId;
            if (mId == 1) {
                r.x = 0.0f;
                r.y = 0.0f;
            } else if (mId <= 3) {
                r.x = fm - 2.5f;
                r.y = 1.0f;
            } else if (mId <= 6) {
                r.x = fm - 5.0f;
                r.y = 2.0f;
            } else if (mId <= 10) {
                r.x = fm - 8.5f;
                r.y = 3.0f;
            } else {
                r.x = fm - 13.0f;
                r.y = 4.0f;
            }
            r.x *= 1.1f;
            r.y += 0.2f * state->hb_len;
        }
        
        // Set velocity
        if (mId == 0) {
            // Initially no velocity for cue ball
            v.x = 0.0f;
            v.y = 0.0f;
        } else {
            v.x = 0.0f;
            v.y = 0.0f;
        }
        
        state->balls[mId].position = r;
        state->balls[mId].velocity = v;
        
        // Initialize rotation quaternion
        s = 7.7f * (state->n_play + (float)mId) / (float)NBALL;
        e.x = normalize_vec3((vec3){
            hash(fmodf(s, 1.0f)),
            hash(fmodf(s + 0.2f, 1.0f)),
            hash(fmodf(s + 0.4f, 1.0f))
        }).x;
        e.y = normalize_vec3((vec3){
            hash(fmodf(s, 1.0f)),
            hash(fmodf(s + 0.2f, 1.0f)),
            hash(fmodf(s + 0.4f, 1.0f))
        }).y;
        e.z = normalize_vec3((vec3){
            hash(fmodf(s, 1.0f)),
            hash(fmodf(s + 0.2f, 1.0f)),
            hash(fmodf(s + 0.4f, 1.0f))
        }).z;
        
        e = normalize_vec3(e);
        state->balls[mId].rotation_quat = euler_to_quat((vec3){
            atan2f(e.x, e.y), acosf(e.z), 2.0f * PI * hash(fmodf(s + 0.6f, 1.0f))
        });
    }
}

// Update physics for a single ball
void step_ball(GameState *state, int mId) {
    if (mId >= NBALL) return;
    
    Ball *ball = &state->balls[mId];
    
    // Skip if ball is out of bounds
    if (ball->position.x >= 2.0f * state->hb_len) return;
    
    // Calculate forces
    vec2 force = {0.0f, 0.0f};
    vec2 r = ball->position;
    vec2 v = ball->velocity;
    
    // Collision forces with other balls
    for (int n = 0; n < NBALL; n++) {
        if (n == mId) continue;
        
        Ball *other_ball = &state->balls[n];
        if (other_ball->position.x >= 2.0f * state->hb_len) continue;  // Skip if out of bounds
        
        vec2 dr = vec2_sub(r, other_ball->position);
        float rSep = length_vec2(dr);
        
        if (rSep < 2.0f * BALL_RADIUS) {  // Collision detection
            force = vec2_add(force, vec2_mul(dr, F_OVERLAP * (1.0f / rSep - 1.0f / (2.0f * BALL_RADIUS))));
        }
    }
    
    // Boundary forces
    vec2 dr = vec2_sub((vec2){state->hb_len, 1.75f * state->hb_len}, (vec2){fabsf(r.x), fabsf(r.y)});
    if (dr.x < 1.0f) {
        force.x -= F_OVERLAP * ((1.0f / dr.x - 1.0f) * dr.x) * ((r.x > 0.0f) ? 1.0f : -1.0f);
    }
    if (dr.y < 1.0f) {
        force.y -= F_OVERLAP * ((1.0f / dr.y - 1.0f) * dr.y) * ((r.y > 0.0f) ? 1.0f : -1.0f);
    }
    
    // Friction force
    force = vec2_sub(force, vec2_mul(v, FRIC));
    
    // Update velocity and position if in motion state
    if (state->run_state == 2.0f) {
        v = vec2_add(v, vec2_mul(force, state->dt));
        r = vec2_add(r, vec2_mul(v, state->dt));
        
        // Update rotation based on velocity
        float vm = length_vec2(v);
        if (vm > 1e-6f) {
            vec3 axis = normalize_vec3((vec3){v.y, 0.0f, -v.x});
            mat3 rot_mat = axis_angle_to_rotation_matrix(axis, vm * state->dt / 0.5f);
            mat3 current_rot = quat_to_rotation_matrix(ball->rotation_quat);
            mat3 new_rot = quat_to_rotation_matrix(rotation_matrix_to_quat((mat3){
                vec3_add(vec3_mul(current_rot.col0, rot_mat.col0.x), 
                        vec3_add(vec3_mul(current_rot.col1, rot_mat.col0.y), 
                                vec3_mul(current_rot.col2, rot_mat.col0.z))),
                vec3_add(vec3_mul(current_rot.col0, rot_mat.col1.x), 
                        vec3_add(vec3_mul(current_rot.col1, rot_mat.col1.y), 
                                vec3_mul(current_rot.col2, rot_mat.col1.z))),
                vec3_add(vec3_mul(current_rot.col0, rot_mat.col2.x), 
                        vec3_add(vec3_mul(current_rot.col1, rot_mat.col2.y), 
                                vec3_mul(current_rot.col2, rot_mat.col2.z)))
            }));
            ball->rotation_quat = rotation_matrix_to_quat(new_rot);
        }
    }
    
    // Check if ball is out of bounds (pocketed)
    vec2 abs_pos = (vec2){fabsf(r.x), fabsf(r.y)};
    if (distance_vec2(abs_pos, (vec2){state->hb_len, 1.75f * state->hb_len}) < 0.9f ||
        distance_vec2(abs_pos, (vec2){state->hb_len, 0.0f}) < 0.9f) {
        r.x = 100.0f * state->hb_len;  // Mark as out of bounds
    }
    
    // Update ball state
    ball->position = r;
    ball->velocity = v;
}

// Update the entire physics simulation
void update_physics(GameState *state) {
    state->t_cur += state->dt;  // Time increment
    
    // Handle game state transitions
    if (state->run_state == 0.0f) {
        // Waiting state
        if (state->n_step > 50.0f || state->n_step > 300.0f) {
            state->run_state = 1.0f;
            state->n_step = 0.0f;
        }
    } else if (state->run_state == 1.0f) {
        // Cue swing state
        if (state->n_step > 50.0f) {
            state->run_state = 2.0f;
        }
    }
    
    state->n_step++;
    
    if (state->run_state == 2.0f) {
        // Motion state - update physics for all balls
        for (int i = 0; i < NBALL; i++) {
            step_ball(state, i);
        }
        
        // Check if game should end
        if (state->n_step > 1800.0f) {
            state->run_state = 0.0f;
            state->n_step = 0.0f;
            state->n_play++;
        }
    }
    
    // Update cue angle when waiting
    if (state->run_state == 0.0f) {
        state->a_cue = PI * (0.5f + 0.09f * sinf(0.6f * 2.0f * PI * state->t_cur));
    }
    
    // Calculate cue distance during swing
    if (state->run_state == 1.0f) {
        state->d_cue = 3.0f * smoothstep(0.0f, 30.0f, state->n_step) * 
                      (1.0f - smoothstep(30.0f, 50.0f, state->n_step));
    } else {
        state->d_cue = 0.0f;
    }
    
    state->show_cue = (state->run_state != 2.0f || state->n_step < 150.0f);
}

// Convert HSV to RGB
Color hsv_to_rgb(float h, float s, float v) {
    if (s == 0.0f) {
        return (Color){
            (unsigned char)(v * 255), 
            (unsigned char)(v * 255), 
            (unsigned char)(v * 255)
        };
    }
    
    h = fmodf(h, 1.0f);
    if (h < 0) h += 1.0f;
    
    int hi = (int)(h * 6);
    float f = h * 6 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    
    switch (hi) {
        case 0: return (Color){(unsigned char)(v * 255), (unsigned char)(t * 255), (unsigned char)(p * 255)};
        case 1: return (Color){(unsigned char)(q * 255), (unsigned char)(v * 255), (unsigned char)(p * 255)};
        case 2: return (Color){(unsigned char)(p * 255), (unsigned char)(v * 255), (unsigned char)(t * 255)};
        case 3: return (Color){(unsigned char)(p * 255), (unsigned char)(q * 255), (unsigned char)(v * 255)};
        case 4: return (Color){(unsigned char)(t * 255), (unsigned char)(p * 255), (unsigned char)(v * 255)};
        default: return (Color){(unsigned char)(v * 255), (unsigned char)(p * 255), (unsigned char)(q * 255)};
    }
}
