#include "soccer2d.h"

// Physics update for the ball
void handle_ball_physics(GameState* state) {
    // Update ball position based on velocity
    state->ball.x += state->ball.z * 0.04f;  // z component is velocity X
    state->ball.y += state->ball.w * 0.04f;  // w component is velocity Y
    
    // Apply friction (slow down the ball)
    state->ball.z *= 0.96f;  // Reduce X velocity
    state->ball.w *= 0.96f;  // Reduce Y velocity
    
    // Boundary check - keep ball within field
    if (state->ball.x > PC_X) state->ball.x = PC_X;
    if (state->ball.x < -PC_X) state->ball.x = -PC_X;
    if (state->ball.y > PC_Y) state->ball.y = PC_Y;
    if (state->ball.y < -PC_Y) state->ball.y = -PC_Y;
    
    // Check for goal
    if (fabsf(state->ball.y) <= 0.07f) {
        if (state->mode[0] == 1) {  // Game in progress
            if (state->ball.x > PC_X) {
                // Blue team scores
                state->scores[1]++;
                // Reset for goal kick
                state->mode[0] = 0;  // Goal kick
                state->mode[1] = state->frame_count + F_START;  // Set next kick time
                state->ball.x = PC_X - 0.01f;
                state->ball.y = 0.0f;
                state->ball.z = 0.0f;
                state->ball.w = 0.0f;
            } else if (state->ball.x < -PC_X) {
                // Red team scores
                state->scores[0]++;
                // Reset for goal kick
                state->mode[0] = 0;  // Goal kick
                state->mode[1] = state->frame_count + F_START;  // Set next kick time
                state->ball.x = -PC_X + 0.01f;
                state->ball.y = 0.0f;
                state->ball.z = 0.0f;
                state->ball.w = 0.0f;
            }
        }
    }
    
    // Handle throw-ins and corners
    if ((state->ball.x > PC_X || state->ball.x < -PC_X || 
         state->ball.y > PC_Y || state->ball.y < -PC_Y) && 
        state->mode[0] != 0) {
        state->mode[0] = 2;  // Throw-in/corner
        state->mode[1] = state->frame_count;
        state->mode[2] = state->last_kicker;  // Store last kicker
        // Position ball at appropriate location based on where it went out
        if (state->ball.x > PC_X) {
            if (fabsf(state->ball.y) <= 0.07f) {
                // Corner for blue team
                state->ball.x = PC_X;
                state->ball.y = PC_Y * sign(state->ball.y);
            } else {
                // Throw-in for blue team
                state->ball.x = PC_X - 0.10f;
                state->ball.y = 0.15f * sign(state->ball.y);
            }
        } else if (state->ball.x < -PC_X) {
            if (fabsf(state->ball.y) <= 0.07f) {
                // Corner for red team
                state->ball.x = -PC_X;
                state->ball.y = PC_Y * sign(state->ball.y);
            } else {
                // Throw-in for red team
                state->ball.x = -PC_X + 0.10f;
                state->ball.y = 0.15f * sign(state->ball.y);
            }
        } else {
            // Lateral throw-in
            state->ball.x = state->ball.x;  // Keep X position
            state->ball.y = PC_Y * sign(state->ball.y);
        }
        state->ball.z = 0.0f;
        state->ball.w = 0.0f;
    }
    
    // Handle mouse click to kick the ball (for human player)
    if (state->mouse_active && state->mode[0] == 1) {  // Game mode and mouse active
        // Find closest player to the ball that belongs to the human team
        vec2 ball_pos = {state->ball.x, state->ball.y};
        int closest_player = get_closest_player(state, ball_pos, true);  // Human controls red team
        
        // Check if this player can kick the ball (is close enough)
        vec2 player_pos = {state->players[closest_player].x, state->players[closest_player].y};
        float dist_to_ball = length_vec2((vec2){player_pos.x - ball_pos.x, player_pos.y - ball_pos.y});
        
        if (dist_to_ball < 0.01f) {  // Close enough to kick
            // Calculate kick direction based on mouse position
            vec2 mouse_world = {
                (state->mouse_pos.x - state->resolution.x * 0.5f) / fminf(state->resolution.y / PC_Y / 2.0f, state->resolution.x / PC_X / 2.0f) * 1.05f,
                ((state->resolution.y - state->mouse_pos.y) - state->resolution.y * 0.5f) / fminf(state->resolution.y / PC_Y / 2.0f, state->resolution.x / PC_X / 2.0f) * 1.05f
            };
            
            vec2 kick_dir = normalize_vec2((vec2){mouse_world.x - state->ball.x, mouse_world.y - state->ball.y});
            float power = fminf(length_vec2((vec2){state->ball.x - mouse_world.x, state->ball.y - mouse_world.y}), 0.9f);
            
            // Check for special keys (A for shot, S for pass)
            if (state->key_states['A'] || state->key_states['a']) {
                power = 0.9f;  // Strong shot
            } else if (state->key_states['S'] || state->key_states['s']) {
                power = 0.5f;  // Gentle pass
            }
            
            state->ball.z = -kick_dir.x * power * 1.2f;
            state->ball.w = -kick_dir.y * power * 1.2f;
            state->last_kicker = closest_player;
        }
    }
    
    // Ball is stopped and we're in throw-in mode, after some frames
    if (state->mode[0] == 2 && state->frame_count > state->mode[1] && length_vec2((vec2){state->ball.z, state->ball.w}) < 0.001f) {
        state->mode[0] = 1;  // Return to game
    }
    
    // Goal kick countdown
    if (state->mode[0] == 0 && state->frame_count > state->mode[1]) {
        state->mode[0] = 1;  // Return to game
    }
}

// Physics update for players
void handle_player_physics(GameState* state) {
    vec2 ball_pos = {state->ball.x, state->ball.y};
    float move_speed = 0.0035f;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        vec2 target_pos;
        vec2 current_pos = {state->players[i].x, state->players[i].y};
        
        // Define target positions based on formation and game state
        if (state->mode[0] == 2) {  // Throw-in mode
            bool kickoff = (fabsf(ball_pos.x) < 0.05f && fabsf(ball_pos.y) < 0.05f);
            
            if (i > 11 && !kickoff) {  // Blue team
                target_pos.x = ball_pos.x * 0.5f + 0.22f + sign(ball_pos.x - 11.5f) * 0.03f;
                target_pos.y = ball_pos.y;
            } else if (i > 0 && i < 12 && !kickoff) {  // Red team
                target_pos.x = ball_pos.x * 0.5f - 0.22f + sign(ball_pos.x - 11.5f) * 0.03f;
                target_pos.y = ball_pos.y;
            } else {
                // Default positions or follow the ball
                target_pos.x = state->players[i].x;
                target_pos.y = state->players[i].y;
            }
            
            // If this player has the ball, move to it
            if ((int)state->buffer_b[1].x == i || (int)state->buffer_b[1].z == i) {
                target_pos = ball_pos;
            } else if (!kickoff) {
                target_pos = mix_vec2(target_pos, (vec2){-ball_pos.x, -ball_pos.y}, 
                                     fmaxf(fminf(0.2f, -0.2f), length_vec2((vec2){target_pos.x - ball_pos.x, target_pos.y - ball_pos.y})));
            }
        } else if (state->mode[0] == 1) {  // Normal game
            // Formation positions
            if (i == 0) target_pos = (vec2){0.92f, 0.0f};          // Red goalkeeper
            else if (i == 1) target_pos = (vec2){0.6f, 0.36f};     // Red defense
            else if (i == 2) target_pos = (vec2){0.6f, 0.12f};     // Red defense
            else if (i == 3) target_pos = (vec2){0.6f, -0.12f};    // Red defense
            else if (i == 4) target_pos = (vec2){0.6f, -0.36f};    // Red defense
            else if (i == 5) target_pos = (vec2){0.3f, 0.36f};     // Red midfield
            else if (i == 6) target_pos = (vec2){0.3f, 0.12f};     // Red midfield
            else if (i == 7) target_pos = (vec2){0.3f, -0.12f};    // Red midfield
            else if (i == 8) target_pos = (vec2){0.3f, -0.36f};    // Red midfield
            else if (i == 9) target_pos = (vec2){0.05f, 0.18f};    // Red attack
            else if (i == 10) target_pos = (vec2){0.05f, -0.18f};  // Red attack
            else if (i == 11) target_pos = (vec2){-0.92f, 0.0f};   // Blue goalkeeper
            else if (i == 12) target_pos = (vec2){-0.6f, 0.36f};   // Blue defense
            else if (i == 13) target_pos = (vec2){-0.6f, 0.12f};   // Blue defense
            else if (i == 14) target_pos = (vec2){-0.6f, -0.12f};  // Blue defense
            else if (i == 15) target_pos = (vec2){-0.6f, -0.36f};  // Blue defense
            else if (i == 16) target_pos = (vec2){-0.3f, -0.32f};  // Blue midfield
            else if (i == 17) target_pos = (vec2){-0.4f, 0.0f};    // Blue midfield
            else if (i == 18) target_pos = (vec2){-0.3f, 0.32f};   // Blue midfield
            else if (i == 19) target_pos = (vec2){-0.05f, 0.25f};  // Blue attack
            else if (i == 20) target_pos = (vec2){-0.05f, 0.0f};   // Blue attack
            else target_pos = (vec2){-0.05f, -0.25f};              // Blue attack
            
            // Apply game logic to target positions
            if (i > 11) {  // Blue team
                target_pos.x += ball_pos.x * 0.7f + 0.3f;
                target_pos.y += ball_pos.y * 0.3f;
            } else if (i > 0 && i < 12) {  // Red team
                target_pos.x += ball_pos.x * 0.7f - 0.3f;
                target_pos.y += ball_pos.y * 0.3f;
            }
            
            // Offside handling
            if (i > 0 && i < 12) target_pos.x = fmaxf(target_pos.x, state->offside_pos.y);  // Red team
            if (i > 12) target_pos.x = fminf(target_pos.x, state->offside_pos.x);           // Blue team
            
            // If player has the ball, move to it
            if ((int)state->buffer_b[1].x == i || (int)state->buffer_b[1].z == i) {
                target_pos = ball_pos;
            } else {
                // Ball is directed to player
                vec2 future_ball = {ball_pos.x + state->ball.z * 1.0f, 
                                   ball_pos.y + state->ball.w * 1.0f};
                future_ball.x = fmaxf(-PC_X, fminf(PC_X, future_ball.x));
                future_ball.y = fmaxf(-PC_Y, fminf(PC_Y, future_ball.y));
                
                int closest_future = get_closest_player(state, future_ball, i > 11);  // Same team as this player
                if (closest_future == i) {
                    target_pos = future_ball;
                } else {
                    // Press the player with the ball
                    target_pos = mix_vec2(target_pos, ball_pos, 
                                         fmaxf(fminf(0.5f, -0.2f), length_vec2((vec2){target_pos.x - ball_pos.x, target_pos.y - ball_pos.y})));
                }
            }
            
            // Random movements for some players
            if ((i >= 5 && i <= 9) || i >= 16) {
                int frame_offset = state->frame_count / 90;
                float rand_x = fabsf(sinf((frame_offset * 24 + i) * 0.1031f)) - 0.5f;
                float rand_y = fabsf(cosf((frame_offset * 24 + i) * 0.1030f)) - 0.5f;
                target_pos.x = fmaxf(-PC_X, fminf(PC_X, target_pos.x + rand_x * 0.4f - 0.2f));
                target_pos.y = fmaxf(-PC_Y, fminf(PC_Y, target_pos.y + rand_y * 0.4f - 0.2f));
            }
        }
        
        // Move player toward target
        vec2 dir = normalize_vec2((vec2){target_pos.x - current_pos.x, target_pos.y - current_pos.y});
        float distance = fminf(length_vec2((vec2){dir.x, dir.y}) * 50.0f, 1.0f);
        
        current_pos.x += dir.x * move_speed * distance;
        current_pos.y += dir.y * move_speed * distance;
        
        // Keep players within the field
        current_pos.x = fmaxf(-PC_X, fminf(PC_X, current_pos.x));
        current_pos.y = fmaxf(-PC_Y, fminf(PC_Y, current_pos.y));
        
        // Update player position
        state->players[i].x = current_pos.x;
        state->players[i].y = current_pos.y;
        
        // Update player direction (z component) and running distance (w component)
        float run_distance = length_vec2((vec2){dir.x * move_speed * distance, dir.y * move_speed * distance});
        if (run_distance < 0.001f) {
            // Calculate direction toward ball when not moving
            state->players[i].z = atan2f(-current_pos.y + ball_pos.y, -current_pos.x + ball_pos.x);
        } else {
            // Set direction based on movement
            state->players[i].z = atan2f(dir.y, dir.x);
        }
        state->players[i].w += run_distance;  // Accumulate running distance
    }
}

// Handle collisions between players
void handle_collisions(GameState* state) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        for (int j = i + 1; j < MAX_PLAYERS; j++) {
            vec2 pos_i = {state->players[i].x, state->players[i].y};
            vec2 pos_j = {state->players[j].x, state->players[j].y};
            
            float distance = length_vec2((vec2){pos_i.x - pos_j.x, pos_i.y - pos_j.y});
            
            if (distance < 0.02f) {  // Collision threshold
                // Simple collision response - push them apart
                vec2 diff = normalize_vec2((vec2){pos_i.x - pos_j.x, pos_i.y - pos_j.y});
                float push = 0.0035f;  // Push amount
                
                state->players[i].x += diff.x * push;
                state->players[i].y += diff.y * push;
                state->players[j].x -= diff.x * push;
                state->players[j].y -= diff.y * push;
                
                // Keep them within the field
                state->players[i].x = fmaxf(-PC_X, fminf(PC_X, state->players[i].x));
                state->players[i].y = fmaxf(-PC_Y, fminf(PC_Y, state->players[i].y));
                state->players[j].x = fmaxf(-PC_X, fminf(PC_X, state->players[j].x));
                state->players[j].y = fmaxf(-PC_Y, fminf(PC_Y, state->players[j].y));
            }
        }
    }
}

// Main physics update function
void update_physics(GameState* state) {
    handle_ball_physics(state);
    handle_player_physics(state);
    handle_collisions(state);
    calculate_buffer_b(state);  // Update buffer B with player positions
}

// Update game logic
void update_game_logic(GameState* state) {
    state->frame_count++;
    state->time += state->dt;
    
    // Update offside positions (simplified)
    state->offside_pos.x = -PC_X;  // Furthest blue player x position
    state->offside_pos.y = PC_X;   // Furthest red player x position
    
    for (int i = 1; i <= 10; i++) {
        if (state->players[i].x < state->offside_pos.y) {
            state->offside_pos.y = state->players[i].x;  // Red team offside line
        }
    }
    
    for (int i = 12; i <= 21; i++) {
        if (state->players[i].x > state->offside_pos.x) {
            state->offside_pos.x = state->players[i].x;  // Blue team offside line
        }
    }
}