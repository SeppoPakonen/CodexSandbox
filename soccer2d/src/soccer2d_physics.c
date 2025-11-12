#include "soccer2d.h"

// Initialize the game state with initial positions
void initialize_game_state(GameState* state) {
    // Initialize ball
    state->ball.x = 0.0f;
    state->ball.y = 0.0f;
    state->ball.z = 0.0f;
    state->ball.w = 0.0f;
    
    // Initialize players in default 4-4-2 formation
    // Red team (right side)
    state->players[0] = (vec4){.92f, 0.0f, 0.0f, 0.0f};      // Goalkeeper
    state->players[1] = (vec4){.6f, .36f, 0.0f, 0.0f};       // Defense
    state->players[2] = (vec4){.6f, .12f, 0.0f, 0.0f};       // Defense
    state->players[3] = (vec4){.6f, -.12f, 0.0f, 0.0f};      // Defense
    state->players[4] = (vec4){.6f, -.36f, 0.0f, 0.0f};      // Defense
    state->players[5] = (vec4){.3f, .36f, 0.0f, 0.0f};       // Midfield
    state->players[6] = (vec4){.3f, .12f, 0.0f, 0.0f};       // Midfield
    state->players[7] = (vec4){.3f, -.12f, 0.0f, 0.0f};      // Midfield
    state->players[8] = (vec4){.3f, -.36f, 0.0f, 0.0f};      // Midfield
    state->players[9] = (vec4){.05f, .18f, 0.0f, 0.0f};      // Attack
    state->players[10] = (vec4){.05f, -.18f, 0.0f, 0.0f};    // Attack
    
    // Blue team (left side)
    state->players[11] = (vec4){-.92f, 0.0f, 0.0f, 0.0f};    // Goalkeeper
    state->players[12] = (vec4){-.6f, .36f, 0.0f, 0.0f};     // Defense
    state->players[13] = (vec4){-.6f, .12f, 0.0f, 0.0f};     // Defense
    state->players[14] = (vec4){-.6f, -.12f, 0.0f, 0.0f};    // Defense
    state->players[15] = (vec4){-.6f, -.36f, 0.0f, 0.0f};    // Defense
    state->players[16] = (vec4){-.3f, -.32f, 0.0f, 0.0f};    // Midfield
    state->players[17] = (vec4){-.4f, 0.0f, 0.0f, 0.0f};     // Midfield
    state->players[18] = (vec4){-.3f, .32f, 0.0f, 0.0f};     // Midfield
    state->players[19] = (vec4){-.05f, .25f, 0.0f, 0.0f};    // Attack
    state->players[20] = (vec4){-.05f, 0.0f, 0.0f, 0.0f};    // Attack
    state->players[21] = (vec4){-.05f, -.25f, 0.0f, 0.0f};   // Attack
    
    // Initialize game state
    state->mode[0] = 1;  // Game mode: 1 = game in progress
    state->mode[1] = F_START;  // Frame count
    state->mode[2] = 0;  // Last player
    state->mode[3] = 0;  // Unused
    state->scores[0] = 0;  // Red team score
    state->scores[1] = 0;  // Blue team score
    state->last_kicker = 0;
    state->frame_count = 0;
    state->demo_mode = true;
    state->time = 0.0f;
    state->mouse_pos.x = 0.0f;
    state->mouse_pos.y = 0.0f;
    state->mouse_active = false;
    
    // Initialize all key states to 0
    for (int i = 0; i < 256; i++) {
        state->key_states[i] = 0;
    }
    
    // Initialize resolution
    state->resolution.x = 800.0f;
    state->resolution.y = 600.0f;
    
    // Initialize time delta
    state->dt = 1.0f/60.0f;  // 60 FPS
    
    // Allocate buffers
    state->buffer_a = (vec4*)calloc(BUFFER_SIZE, sizeof(vec4));
    state->buffer_b = (vec4*)calloc(BUFFER_SIZE, sizeof(vec4));
}

// Calculate signed distance to a box
float sd_box(const vec2 p, const vec2 b) {
    vec2 d = {fabsf(p.x) - b.x, fabsf(p.y) - b.y};
    return fminf(fmaxf(d.x, d.y), 0.0f) + sqrtf(fmaxf(0.0f, d.x*d.x + d.y*d.y));
}

// Calculate signed distance to a line segment
float sd_segment(const vec2 p, const vec2 a, const vec2 b) {
    vec2 pa = {p.x - a.x, p.y - a.y};
    vec2 ba = {b.x - a.x, b.y - a.y};
    float h = fminf(fmaxf((pa.x*ba.x + pa.y*ba.y) / (ba.x*ba.x + ba.y*ba.y), 0.0f), 1.0f);
    vec2 v = {pa.x - ba.x*h, pa.y - ba.y*h};
    return sqrtf(v.x*v.x + v.y*v.y);
}

// Calculate length of a 2D vector
float length_vec2(const vec2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}

// Normalize a 2D vector
vec2 normalize_vec2(const vec2 v) {
    float len = length_vec2(v);
    if (len > 0.0f) {
        return (vec2){v.x / len, v.y / len};
    }
    return (vec2){0.0f, 0.0f};
}

// Calculate dot product of two 2D vectors
float dot_vec2(const vec2 a, const vec2 b) {
    return a.x * b.x + a.y * b.y;
}

// Clamp a 2D vector between min and max values
vec2 clamp_vec2(const vec2 v, const vec2 min_val, const vec2 max_val) {
    return (vec2){
        fminf(fmaxf(v.x, min_val.x), max_val.x),
        fminf(fmaxf(v.y, min_val.y), max_val.y)
    };
}

// Return the sign of a number
float sign(float x) {
    return (x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f);
}

// Linear interpolation between two 2D vectors
vec2 mix_vec2(const vec2 a, const vec2 b, float t) {
    return (vec2){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

// Calculate the pitch boundary (field lines)
float pitch(const vec2 uv) {
    float d = 1e5f;
    vec2 abs_uv = {fabsf(uv.x), fabsf(uv.y)};
    d = fminf(d, fabsf(length_vec2(abs_uv) - 0.18f));
    d = fminf(d, fabsf(sd_box((vec2){abs_uv.x - PC_X + 0.34f, abs_uv.y}, (vec2){0.17f, 0.4f})));
    d = fminf(d, fabsf(sd_box((vec2){abs_uv.x - PC_X/2.0f, abs_uv.y}, (vec2){PC_X/2.0f, PC_Y})));
    d = fminf(d, fabsf(sd_box((vec2){abs_uv.x - (PC_X - 0.17f), abs_uv.y}, (vec2){0.17f, 0.4f})));
    d = fminf(d, fabsf(sd_box((vec2){abs_uv.x - (PC_X - 0.05f), abs_uv.y}, (vec2){0.05f, 0.15f})));
    d = fminf(d, length_vec2((vec2){abs_uv.x - (PC_X - 0.25f), abs_uv.y}));
    return d;
}

// Find the closest player to a position
int get_closest_player(const GameState* state, const vec2 pos, bool red_team) {
    int start_idx = red_team ? 0 : 11;
    int end_idx = red_team ? 10 : 21;
    int closest_idx = start_idx;
    float min_dist = 1e5f;
    
    for (int i = start_idx; i <= end_idx; i++) {
        vec2 player_pos = {state->players[i].x, state->players[i].y};
        float dist = sd_segment(player_pos, pos, pos);  // Actually just distance
        
        if (dist < min_dist) {
            min_dist = dist;
            closest_idx = i;
        }
    }
    
    return closest_idx;
}

// Calculate buffer B (closest players for each position)
void calculate_buffer_b(GameState* state) {
    // Implementation of buffer B calculation
    // This will find the closest player of each team to every position on the field
    // Simplified implementation - in a real implementation, we would calculate for each pixel
    for (int i = 0; i < BUFFER_SIZE; i++) {
        state->buffer_b[i] = (vec4){0.0f, 1e5f, 0.0f, 1e5f};  // Initialize with large distances
    }
    
    // For simplicity, just calculate for a few key positions
    vec2 field_center = {0.0f, 0.0f};
    vec2 ball_pos = {state->ball.x, state->ball.y};
    
    // Find closest red team player to field center
    int closest_red_center = get_closest_player(state, field_center, true);
    // Find closest blue team player to field center
    int closest_blue_center = get_closest_player(state, field_center, false);
    
    // Find closest red team player to ball
    int closest_red_ball = get_closest_player(state, ball_pos, true);
    // Find closest blue team player to ball
    int closest_blue_ball = get_closest_player(state, ball_pos, false);
    
    // Store in buffer B (simplified)
    state->buffer_b[0] = (vec4){
        .x = closest_red_center, 
        .y = sd_segment(field_center, 
                       (vec2){state->players[closest_red_center].x, state->players[closest_red_center].y}, 
                       field_center),
        .z = closest_blue_center,
        .w = sd_segment(field_center, 
                       (vec2){state->players[closest_blue_center].x, state->players[closest_blue_center].y}, 
                       field_center)
    };
    
    state->buffer_b[1] = (vec4){
        .x = closest_red_ball, 
        .y = sd_segment(ball_pos, 
                       (vec2){state->players[closest_red_ball].x, state->players[closest_red_ball].y}, 
                       ball_pos),
        .z = closest_blue_ball,
        .w = sd_segment(ball_pos, 
                       (vec2){state->players[closest_blue_ball].x, state->players[closest_blue_ball].y}, 
                       ball_pos)
    };
}