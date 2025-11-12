#include "../include/game.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>

static GameState g_game_state;
SDL_Window* g_window = NULL;
SDL_Renderer* g_sdl_renderer = NULL;

void init_game(GameState* state) {
    // Initialize player
    state->player.position = (Vec3){0.0f, -72.0f, 0.0f};
    state->player.velocity = (Vec3){0.0f, 0.0f, 0.0f};
    state->player.rotation = PI;
    state->player.target_rotation = PI;
    state->player.rotation_time = 0.0f;
    state->player.state = 0;
    
    // Initialize game time
    state->time = 0.0f;
    state->delta_time = 1.0f/60.0f;  // Default to 60fps
    state->frame_count = 0.0f;
    
    // Initialize buffer A (simulating shader buffer)
    state->buffer_a[0] = (Vec4){state->player.target_rotation, 0.0f, 0.0f, 0.0f};  // target rotation
    state->buffer_a[1] = (Vec4){state->player.rotation, 0.0f, 0.0f, 0.0f};         // actual rotation
    state->buffer_a[2] = (Vec4){state->player.position.x, state->player.position.y, 
                               state->player.position.z, (float)state->player.state}; // player pos + state
    state->buffer_a[3] = (Vec4){state->player.velocity.x, state->player.velocity.y, 
                               state->player.velocity.z, 0.0f};                      // player velocity
    
    // Initialize key states
    for(int i = 0; i < 256; i++) {
        state->keys[i] = false;
    }
}

void update_game(GameState* state) {
    float old_time = state->time;
    state->time = (float)clock() / CLOCKS_PER_SEC;
    state->delta_time = state->time - old_time;
    state->frame_count++;
    
    // Handle input to update target rotation
    float rotation_change = 0.0f;
    if(state->keys[SDL_SCANCODE_D]) rotation_change += 1.0f;
    if(state->keys[SDL_SCANCODE_A]) rotation_change -= 1.0f;
    state->buffer_a[0].x += rotation_change * PI * 0.5f * state->delta_time;
    state->buffer_a[0].x = fmodf(state->buffer_a[0].x, 2.0f * PI);
    if(fabsf(rotation_change) > 0.001f)
        state->buffer_a[0].y = state->time;

    // Update actual rotation
    float target_rot = state->buffer_a[0].x;
    float current_rot = state->buffer_a[1].x;
    state->buffer_a[1].x = fmodf(current_rot, 2.0f * PI);
    
    // Calculate shortest distance to target rotation
    float difference = target_rot - current_rot;
    float difference2 = difference - 2.0f * PI;
    float difference3 = difference + 2.0f * PI;
    if (fabsf(difference) < fabsf(difference2)) {
        if (fabsf(difference) < fabsf(difference3))
            difference = difference;
        else
            difference = difference3;
    } else {
        difference = difference2;
    }

    // Rotate
    state->buffer_a[1].x += (fminf(state->time - state->buffer_a[0].y, state->time * 0.05f) * difference) * state->delta_time * 32.0f;

    // Snap to nearest cardinal direction
    if (fmodf(state->buffer_a[1].x, PI * 0.5f) < EPSILON || 
        fmodf(state->buffer_a[1].x, PI * 0.5f) > PI * 0.5f - EPSILON) {
        state->buffer_a[1].x = fmodf(roundf(state->buffer_a[1].x * 2.0f / PI) * 0.5f * PI, 2.0f * PI);
    }

    // Update player position and physics
    Vec3 player_pos = {state->buffer_a[2].x, state->buffer_a[2].y, state->buffer_a[2].z};
    int player_state = (int)state->buffer_a[2].w;
    Vec3 player_vel = {state->buffer_a[3].x, state->buffer_a[3].y, state->buffer_a[3].z};
    float rotation = state->buffer_a[1].x;

    // Check if player is not rotating
    if (fmodf(rotation, PI * 0.5f) < EPSILON && fmodf(rotation, PI * 0.5f) < PI * 0.5f - EPSILON) {
        float left_key = state->keys[SDL_SCANCODE_LEFT] ? 1.0f : 0.0f;
        float right_key = state->keys[SDL_SCANCODE_RIGHT] ? 1.0f : 0.0f;

        Vec2 trig = {cosf(rotation), sinf(rotation)};
        Vec3 new_pos = player_pos;

        // Apply movement
        new_pos.x -= trig.y * (left_key - right_key) * (state->delta_time * 80.0f);
        new_pos.z += trig.x * (left_key - right_key) * (state->delta_time * 80.0f);

        new_pos.y += player_vel.y * state->delta_time * 16.0f;

        // Simple collision detection
        Tile tile = world((Vec3){new_pos.x * 0.0625f, new_pos.y * 0.0625f, new_pos.z * 0.0625f});
        if(tile.ID == 0) {
            player_pos = new_pos;
        }

        // Ground check and physics
        Vec3 ground_check = {player_pos.x, player_pos.y - 9.0f, player_pos.z};
        tile = world((Vec3){ground_check.x * 0.0625f, ground_check.y * 0.0625f, ground_check.z * 0.0625f});

        if(tile.ID != 0) {
            // Player is grounded
            player_state = (player_state % 2 == 0) ? 2 : 3;
            player_pos.y = floorf(player_pos.y * 0.0625f) * 16.0f + 8.0f;
        } else {
            player_state = (player_state % 2 == 0) ? 0 : 1;
        }

        // Apply gravity
        player_vel.y -= (state->delta_time * 48.0f);

        // Jump
        if ((state->keys[SDL_SCANCODE_SPACE] || state->keys[SDL_SCANCODE_UP]) && (player_state % 4) > 1) {
            player_vel.y = 18.0f;
        }

        // Respawn if fallen too far
        if (player_pos.y < -256.0f) {
            player_pos = (Vec3){0.0f, -64.0f, 0.0f};
            player_state = (player_state % 4 < 3) ? 0 : 2;
        }

        // Update buffers
        state->buffer_a[2] = (Vec4){player_pos.x, player_pos.y, player_pos.z, (float)player_state};
        state->buffer_a[3] = (Vec4){player_vel.x, player_vel.y, player_vel.z, 0.0f};
    } else {
        // Player is rotating
        player_state = (player_state % 4 < 3) ? 0 : 2;
        state->buffer_a[2].w = (float)player_state;
    }
}

void render_frame(GameState* state) {
    // Choose between software and shader renderers
    bool use_software_renderer = true; // For now, default to software renderer
    
    if(use_software_renderer) {
        render_software(state);
    } else {
        render_shader(state);
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize OpenGL context for shader renderer (we'll support both)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window with OpenGL support for shader renderer
    g_window = SDL_CreateWindow("FEZ Game",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!g_window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context for shader rendering
    SDL_GLContext context = SDL_GL_CreateContext(g_window);
    if(!context) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    // Initialize OpenGL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize game state
    init_game(&g_game_state);

    // Main game loop
    bool quit = false;
    SDL_Event e;
    
    while(!quit) {
        // Handle events
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            } else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym < 256) {
                    g_game_state.keys[e.key.keysym.sym] = true;
                }
                
                // Toggle renderer with 'R' key
                if(e.key.keysym.sym == SDLK_r) {
                    printf("Renderer toggle not implemented yet\n");
                }
            } else if(e.type == SDL_KEYUP) {
                if(e.key.keysym.sym < 256) {
                    g_game_state.keys[e.key.keysym.sym] = false;
                }
            }
        }

        // Update game logic
        update_game(&g_game_state);

        // Render frame
        render_frame(&g_game_state);

        // Cap frame rate (simple)
        SDL_Delay(16);  // ~60 FPS
    }

    // Clean up
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(g_window);
    SDL_Quit();

    return 0;
}