#include "physics.h"
#include <math.h>
#include <string.h>

// Helper functions
float clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

float mix_val(float a, float b, float t) {
    return a + t * (b - a);
}

float sign(float x) {
    return (x >= 0.0f) ? 1.0f : -1.0f;
}

// Random function from GLSL
float rand_float(float x) {
    return fmodf(sinf(x) * 25314.67285f, 1.0f);
}

// 1D tent function
float tent1(float x) {
    return fmaxf(0.0f, (1.0f - fabsf(x)));
}

// 1D noise function
float noise1(float x) {
    float o = x;
    x = fmodf(x - 1.0f, 2.0f) - 1.0f;
    float y = sinf(3.1415927f * (sqrtf(5.0f) + 1.0f) * floorf((o - 1.0f) / 2.0f)); // pseudorandom hash
    float v = tent1(x);
    v *= sqrtf(0.5f); // fix sheared 'distance' measurements
    if (y < 0.0f) v = -v;
    return v;
}

// Fractional Brownian motion in 1D
float fbm1(float x) {
    float d = -1.0f;
    float s = 1.0f;
    
    for (int i = 0; i < LEVELS; i++) {
        float n = s * noise1(x);
        d = fmaxf(d, n);
        s *= 0.5f;
        x *= 2.0f;
    }
    return d;
}

// 1D layered noise
float lnoise1(float x, float y) {
    float n = fbm1(x);
    n = clamp(n, -0.01f, 0.19f);
    n *= DIFFICULTY_H * powf(2.0f, clamp(DIFFICULTY_E * x, 0.0f, 3.0f));
    n = fminf(n, 0.5f); // don't get too ridiculous
    const float slab = 1.0f; // for clipping max extent of hill
    float d0 = y - n;
    float d1 = sign(y) * fmaxf(0.0f, fabsf(y) - slab);
    return d0;
}

// Track signed distance function
float dtrack(float x, float y) {
    float pd = y;
    float nd = lnoise1(x * SCALE, (y * -SCALE + 0.2f)) / -SCALE;
    float d = fminf(nd, pd);
    return d;
}

// Track normal (x component)
float ntrack_x(float x, float y) {
    const float e = 0.002f;
    float dx = dtrack(x + e, y);
    float dy = dtrack(x, y + e);
    float dz = dtrack(x, y);
    return dx - dz;  // x component of gradient
}

// Track normal (y component) 
float ntrack_y(float x, float y) {
    const float e = 0.002f;
    float dx = dtrack(x + e, y);
    float dy = dtrack(x, y + e);
    float dz = dtrack(x, y);
    return dy - dz;  // y component of gradient
}

// Initialize bike particles in proper positions
void init_bike_particles(GameState* state) {
    // Initialize links (constraints between particles)
    state->links[0][0] = PID_BODY;         state->links[0][1] = PID_WHEEL_BACK;  // Body to back wheel
    state->links[1][0] = PID_BODY;         state->links[1][1] = PID_WHEEL_FRONT; // Body to front wheel  
    state->links[2][0] = PID_WHEEL_BACK;   state->links[2][1] = PID_WHEEL_FRONT; // Back wheel to front wheel
    
    // Initialize link lengths and spring constants
    state->link_lengths[0] = 0.13f; // Body to back wheel distance
    state->link_lengths[1] = 0.17f; // Body to front wheel distance
    state->link_lengths[2] = 0.20f; // Back wheel to front wheel distance
    
    // Set initial positions for particles (bike in upright position)
    // Body (main rider position)
    state->particles[PID_BODY][0] = 0.0f;   // x
    state->particles[PID_BODY][1] = 0.532f; // y (offset from original 0.232 + 0.3)
    state->particles[PID_BODY][2] = 0.5f;   // vx
    state->particles[PID_BODY][3] = 0.1f;   // vy
    
    // Back wheel
    state->particles[PID_WHEEL_BACK][0] = -0.1f; // x
    state->particles[PID_WHEEL_BACK][1] = 0.4f;  // y (offset from original 0.1 + 0.3) 
    state->particles[PID_WHEEL_BACK][2] = 0.5f;  // vx
    state->particles[PID_WHEEL_BACK][3] = 0.1f;  // vy
    
    // Front wheel
    state->particles[PID_WHEEL_FRONT][0] = 0.1f; // x
    state->particles[PID_WHEEL_FRONT][1] = 0.4f; // y (offset from original 0.1 + 0.3)
    state->particles[PID_WHEEL_FRONT][2] = 0.5f; // vx
    state->particles[PID_WHEEL_FRONT][3] = 0.1f; // vy
}

// Check if bike particles are in inverted orientation
bool is_inverted(float* p0, float* p1, float* p2) {
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float d = sqrtf(dx*dx + dy*dy);
    if (d > 0.0001f) {
        dx /= d;
        dy /= d;
    }
    
    // Perp dot product of (p2-p1) and (p0-p1)
    float pdot_val = dx * (p0[1] - p1[1]) - dy * (p0[0] - p1[0]);
    return pdot_val < 0.0f;
}

// Constraint function (spring physics)
void constraint(float* pa, float* pb, float* force, float* impulse, float rest_length, float kspring) {
    // pa and pb are arrays of 4 elements [x, y, vx, vy]
    float dx = pb[0] - pa[0];  // rel pos x
    float dy = pb[1] - pa[1];  // rel pos y
    float dvx = pb[2] - pa[2]; // rel vel x
    float dvy = pb[3] - pa[3]; // rel vel y
    
    float ld = sqrtf(dx*dx + dy*dy);
    if (ld < 1e-3f) ld = 1e-3f;
    
    float ex = dx / ld; // unit vector x component
    float ey = dy / ld; // unit vector y component
    
    float k = TENSION; // Using global tension constant as in original
    float spring_force = (ld - rest_length) * k;
    
    // Damping force
    float damping = SDAMP * (dvx*ex + dvy*ey) * k;
    
    force[0] = spring_force * ex + damping * ex;
    force[1] = spring_force * ey + damping * ey;
    
    impulse[0] = 0.0f;
    impulse[1] = 0.0f;
}

// Update the physics simulation
void update_physics(GameState* state) {
    float dt = state->time_delta;
    if (dt <= 0.0f || dt != dt) return; // Check for valid dt
    
    // Clamp dt to prevent instability
    dt = fminf(fmaxf(dt, 0.005f), 0.033f);
    
    // Store current positions for velocity calculation
    float old_positions[NPARTICLES][2];
    for (int i = 0; i < NPARTICLES; i++) {
        old_positions[i][0] = state->particles[i][0];
        old_positions[i][1] = state->particles[i][1];
    }
    
    // Get particle positions
    float* p0 = state->particles[PID_BODY];
    float* p1 = state->particles[PID_WHEEL_BACK];
    float* p2 = state->particles[PID_WHEEL_FRONT];
    
    // Check for inverted bike
    state->inverted = is_inverted(p0, p1, p2);
    
    // Determine control inputs
    bool accel = state->keys[KEY_SPACE];
    bool lean_back = state->keys[KEY_A] || state->keys[KEY_LEFT];
    bool lean_front = state->keys[KEY_D] || state->keys[KEY_RIGHT];
    bool reset = state->keys[KEY_ESC];
    bool inverted_control = state->inverted;
    
    // Apply control inputs to update particles
    for (int id = 0; id < NPARTICLES; id++) {
        float* c = state->particles[id];
        
        // Initialize forces
        float forces[2] = {0.0f, 0.0f};
        float impulses[2] = {0.0f, 0.0f};
        
        // Apply spring constraints
        for (int i = 0; i < 3; i++) {
            for (int x = 0; x < 2; x++) {
                if (id == state->links[i][x]) {
                    int j = state->links[i][x ^ 1];
                    float* pj = state->particles[j];
                    float f[2], v[2];
                    
                    constraint(c, pj, f, v, state->link_lengths[i], TENSION);
                    
                    forces[0] += f[0];
                    forces[1] += f[1];
                    impulses[0] += v[0];
                    impulses[1] += v[1];
                    break;
                }
            }
        }
        
        // Apply gravity
        forces[1] -= GRAVITY;
        
        // Update velocity
        c[2] += (forces[0] * dt + impulses[0]); // vx
        c[3] += (forces[1] * dt + impulses[1]); // vy
        
        // Store old velocity before collision
        float old_vx = c[2];
        float old_vy = c[3];
        
        // Update position
        c[0] += c[2] * dt; // x
        c[1] += c[3] * dt; // y
        
        // Check for track collision
        float htrack = dtrack(c[0], 0.0f);
        float dtr = dtrack(c[0], 0.0f);
        bool onground = (id == PID_WHEEL_BACK || id == PID_WHEEL_FRONT) && 
                        (c[1] <= WHEEL_RADIUS - dtr);
        
        if (onground) {
            // Calculate track normal
            float nx = ntrack_x(c[0], c[1]);
            float ny = ntrack_y(c[0], c[1]);
            float normal_len = sqrtf(nx*nx + ny*ny);
            if (normal_len > 0.0001f) {
                nx /= normal_len;
                ny /= normal_len;
            } else {
                nx = 0.0f; ny = 1.0f; // Default to up
            }
            
            // Ensure bike stays above ground
            c[1] = fmaxf(c[1], WHEEL_RADIUS - dtr);
            
            // Apply collision response (remove velocity component into the ground)
            float vdot = old_vx * nx + old_vy * ny;
            if (vdot <= 0.0f) {
                c[2] -= vdot * nx;
                c[3] -= vdot * ny;
            }
            
            // Apply friction and motor controls
            if (id == PID_WHEEL_BACK && accel) { // Back tire acceleration
                c[2] += MOTOR_POWER * dt;
            }
        }
        
        // Update velocity based on position change
        c[2] = (c[0] - old_positions[id][0]) / fmaxf(dt, 1e-6f);
        c[3] = (c[1] - old_positions[id][1]) / fmaxf(dt, 1e-6f);
        
        // Apply air control (leaning) to body
        if (id == PID_BODY) {
            float lean_force = 0.0f;
            if (lean_back) lean_force += 1.0f;
            if (lean_front) lean_force -= 1.0f;
            if (inverted_control) lean_force = -lean_force;
            
            // Calculate direction vector between wheels
            float dx = p1[0] - p2[0];
            float dy = p1[1] - p2[1];
            float len = sqrtf(dx*dx + dy*dy);
            if (len > 0.0001f) {
                dx /= len;
                dy /= len;
            }
            
            c[2] += LEAN_POWER * dt * lean_force * dx;
            c[3] += LEAN_POWER * dt * lean_force * dy;
        }
    }
    
    // Check if bike needs to be reset due to being inverted
    bool stopped = (p0[2]*p0[2] + p0[3]*p0[3]) < 3e-3f;
    if ((state->inverted || (stopped && state->inverted)) && reset) {
        init_bike_particles(state);
    }
    
    // Update score based on horizontal position of body
    state->score = (int)(fmaxf(0.0f, p0[0] / 10.0f));
}