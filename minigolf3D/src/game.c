#define _GNU_SOURCE
#include "game.h"
#include "physics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Game* init_game(RendererType renderer_type) {
    Game* game = (Game*)malloc(sizeof(Game));
    if (!game) return NULL;
    
    // Initialize renderer
    game->renderer = init_renderer(renderer_type, 800, 600, "Minigolf 3D");
    if (!game->renderer) {
        free(game);
        return NULL;
    }
    
    // Initialize game state
    game->state = GAME_STATE_IDLE;
    
    // Initialize ball
    game->ball.position.x = 0.0f;
    game->ball.position.y = BALL_RADIUS;  // Start above ground
    game->ball.position.z = 0.0f;
    game->ball.velocity.x = 0.0f;
    game->ball.velocity.y = 0.0f;
    game->ball.velocity.z = 0.0f;
    game->ball.radius = BALL_RADIUS;
    
    // Initialize hole
    game->hole.position.x = 0.0f;
    game->hole.position.y = 0.0f;  // At ground level
    game->hole.position.z = 5.0f;  // 5 units in front of start
    game->hole.radius = HOLE_RADIUS;
    
    // Initialize shot
    game->current_shot.position.x = 0.0f;
    game->current_shot.position.y = 0.0f;
    game->current_shot.position.z = 0.0f;
    game->current_shot.strength = 0.0f;
    game->current_shot.active = false;
    
    // Initialize camera
    game->camera_pos.x = 0.0f;
    game->camera_pos.y = 2.0f;
    game->camera_pos.z = -5.0f;
    game->camera_target.x = 0.0f;
    game->camera_target.y = 0.0f;
    game->camera_target.z = 0.0f;
    
    game->time = 0.0f;
    game->mouse_pressed = false;
    game->mouse_start_x = 0.0f;
    game->mouse_start_y = 0.0f;
    game->mouse_current_x = 0.0f;
    game->mouse_current_y = 0.0f;
    
    return game;
}

void cleanup_game(Game* game) {
    if (!game) return;
    
    cleanup_renderer(game->renderer);
    free(game);
}

bool is_game_running(Game* game) {
    if (!game || !game->renderer) return false;
    return !glfwWindowShouldClose(game->renderer->window);
}

void handle_input(Game* game) {
    if (!game || !game->renderer) return;
    
    // Handle quit
    if (glfwGetKey(game->renderer->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(game->renderer->window, true);
    }
    
    // Handle mouse input for aiming/shooting
    double xpos, ypos;
    glfwGetCursorPos(game->renderer->window, &xpos, &ypos);
    
    if (glfwGetMouseButton(game->renderer->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!game->mouse_pressed) {
            // Mouse just pressed
            game->mouse_pressed = true;
            game->mouse_start_x = (float)xpos;
            game->mouse_start_y = (float)ypos;
            game->current_shot.position = game->ball.position;
            game->state = GAME_STATE_AIMING;
        }
        
        game->mouse_current_x = (float)xpos;
        game->mouse_current_y = (float)ypos;
    } else {
        if (game->mouse_pressed) {
            // Mouse just released - fire the shot
            float dx = game->mouse_start_x - game->mouse_current_x;
            float dy = game->mouse_start_y - game->mouse_current_y;
            
            // Calculate shot strength and direction
            game->current_shot.strength = sqrt(dx * dx + dy * dy) / 100.0f;  // Scale appropriately
            if (game->current_shot.strength > 1.0f) game->current_shot.strength = 1.0f;
            
            // Calculate direction - based on camera orientation
            Vec3 forward = {
                game->camera_target.x - game->camera_pos.x,
                game->camera_target.y - game->camera_pos.y,
                game->camera_target.z - game->camera_pos.z
            };
            float len = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
            if (len > 0.0f) {
                forward.x /= len;
                forward.y /= len;
                forward.z /= len;
            }
            
            // Add some vertical component based on drag direction
            Vec3 shot_dir = {
                forward.x * game->current_shot.strength * 3.0f,
                -dy / 100.0f, // Vertical component from drag
                forward.z * game->current_shot.strength * 3.0f
            };
            
            game->ball.velocity = shot_dir;
            game->state = GAME_STATE_BALL_MOVING;
        }
        game->mouse_pressed = false;
    }
}

void update_game(Game* game, float delta_time) {
    if (!game) return;
    
    game->time += delta_time;
    
    // Handle input
    handle_input(game);
    
    // Update ball physics if moving
    if (game->state == GAME_STATE_BALL_MOVING) {
        update_ball_physics(&game->ball, delta_time);
        
        // Check if ball is in hole
        if (check_ball_collision_with_hole(&game->ball, &game->hole)) {
            game->state = GAME_STATE_BALL_IN_HOLE;
        }
        
        // If ball has very low velocity, stop it
        float vel_mag = sqrt(
            game->ball.velocity.x * game->ball.velocity.x +
            game->ball.velocity.y * game->ball.velocity.y +
            game->ball.velocity.z * game->ball.velocity.z
        );
        
        if (vel_mag < 0.01f && game->state != GAME_STATE_BALL_IN_HOLE) {
            game->ball.velocity.x = 0.0f;
            game->ball.velocity.y = 0.0f;
            game->ball.velocity.z = 0.0f;
            game->state = GAME_STATE_IDLE;
        }
    }
    
    // Update camera to follow ball
    game->camera_pos.x = game->ball.position.x;
    game->camera_pos.z = game->ball.position.z - 3.0f;  // Stay behind the ball
    // Keep some height
    if (game->camera_pos.y < 2.0f) game->camera_pos.y = 2.0f;
    
    game->camera_target = game->ball.position;
    game->camera_target.y += 0.5f;  // Look slightly above the ball
}

void render_game(Game* game) {
    if (!game || !game->renderer) return;
    
    start_frame(game->renderer);
    
    // Set up camera
    // Mat4 view_matrix = create_look_at_matrix(
    //     game->camera_pos,
    //     game->camera_target,
    //     (Vec3){0.0f, 1.0f, 0.0f}
    // );
    // 
    // Mat4 proj_matrix = create_perspective_matrix(
    //     45.0f * M_PI / 180.0f,  // FOV in radians
    //     (float)game->renderer->width / (float)game->renderer->height,  // Aspect ratio
    //     0.1f,  // Near plane
    //     100.0f  // Far plane
    // );
    
    // Draw ground
    for (int x = -10; x <= 10; x++) {
        for (int z = -10; z <= 10; z++) {
            Vec3 p0 = {(float)x, 0.0f, (float)z};
            Vec3 p1 = {(float)(x+1), 0.0f, (float)z};
            Vec3 p2 = {(float)x, 0.0f, (float)(z+1)};
            Vec3 p3 = {(float)(x+1), 0.0f, (float)(z+1)};
            
            // Alternate green shades for visual interest
            float green = 0.5f + 0.1f * sin(x * 0.5f) * cos(z * 0.5f);
            draw_triangle(game->renderer, p0, p1, p2, 0.0f, green, 0.0f);
            draw_triangle(game->renderer, p1, p3, p2, 0.0f, green, 0.0f);
        }
    }
    
    // Draw hole
    // Approximate hole as a circle of points
    for (int i = 0; i < 32; i++) {
        float angle = 2.0f * M_PI * i / 32.0f;
        float nx = game->hole.position.x + HOLE_RADIUS * cos(angle);
        float nz = game->hole.position.z + HOLE_RADIUS * sin(angle);
        Vec3 p1 = {nx, game->hole.position.y, nz};
        
        angle = 2.0f * M_PI * (i + 1) / 32.0f;
        float nx2 = game->hole.position.x + HOLE_RADIUS * cos(angle);
        float nz2 = game->hole.position.z + HOLE_RADIUS * sin(angle);
        Vec3 p2 = {nx2, game->hole.position.y, nz2};
        
        draw_line(game->renderer, 
                 (Vec3){game->hole.position.x, game->hole.position.y, game->hole.position.z}, 
                 p1, 0.2f, 0.2f, 0.2f);
        draw_line(game->renderer, p1, p2, 0.2f, 0.2f, 0.2f);
    }
    
    // Draw goal indicator above hole
    Vec3 flag_top = {game->hole.position.x, game->hole.position.y + 0.5f, game->hole.position.z};
    Vec3 flag_bottom = {game->hole.position.x, game->hole.position.y, game->hole.position.z};
    draw_line(game->renderer, flag_bottom, flag_top, 0.8f, 0.8f, 0.8f);
    
    // Draw red flag
    Vec3 flag_pos = {game->hole.position.x + 0.05f, game->hole.position.y + 0.4f, game->hole.position.z};
    draw_line(game->renderer, flag_top, flag_pos, 0.8f, 0.0f, 0.0f);
    
    // Draw ball
    draw_sphere(game->renderer, game->ball.position, game->ball.radius, 1.0f, 1.0f, 1.0f);
    
    // Draw aiming line if in aiming state
    if (game->state == GAME_STATE_AIMING && game->mouse_pressed) {
        float dx = game->mouse_start_x - game->mouse_current_x;
        float dy = game->mouse_start_y - game->mouse_current_y;
        float strength = sqrt(dx * dx + dy * dy) / 100.0f;
        if (strength > 1.0f) strength = 1.0f;
        
        Vec3 forward = {
            game->camera_target.x - game->camera_pos.x,
            game->camera_target.y - game->camera_pos.y,
            game->camera_target.z - game->camera_pos.z
        };
        float len = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
        if (len > 0.0f) {
            forward.x /= len;
            forward.y /= len;
            forward.z /= len;
        }
        
        Vec3 aim_end = {
            game->ball.position.x + forward.x * strength * 2.0f,
            game->ball.position.y + 0.1f,  // Slightly above the ball
            game->ball.position.z + forward.z * strength * 2.0f
        };
        
        draw_line(game->renderer, game->ball.position, aim_end, 1.0f, 1.0f, 0.0f);
    }
    
    end_frame(game->renderer);
}