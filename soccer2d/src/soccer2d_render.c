#include "soccer2d.h"
#include <stdio.h>

#ifdef USE_SDL
#include <SDL2/SDL.h>
#elif defined(USE_GLFW) 
#include <GLFW/glfw3.h>
#endif

// Simple ASCII-based old school renderer
void render_old_school(const GameState* state) {
    printf("\033[2J"); // Clear screen using ANSI escape codes
    printf("\033[H");  // Move cursor to home position
    
    // Create a simple text-based representation of the field
    const int width = 60;
    const int height = 30;
    char field[height][width];
    
    // Initialize the field with empty spaces
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            field[y][x] = ' ';
        }
    }
    
    // Draw the field boundaries
    for (int x = 0; x < width; x++) {
        field[0][x] = '-';
        field[height-1][x] = '-';
    }
    for (int y = 0; y < height; y++) {
        field[y][0] = '|';
        field[y][width-1] = '|';
    }
    
    // Draw center line
    for (int y = 0; y < height; y++) {
        if (y != height/2) continue;
        field[y][width/2] = '|';
    }
    
    // Draw center circle (simplified)
    int center_x = width/2;
    int center_y = height/2;
    for (int y = center_y-2; y <= center_y+2; y++) {
        if (y >= 0 && y < height) {
            for (int x = center_x-2; x <= center_x+2; x++) {
                if (x >= 0 && x < width && (x-center_x)*(x-center_x) + (y-center_y)*(y-center_y) <= 4) {
                    field[y][x] = ':';
                }
            }
        }
    }
    
    // Draw goals
    int goal_height = 3;
    int goal_y_start = (height - goal_height) / 2;
    for (int y = goal_y_start; y < goal_y_start + goal_height; y++) {
        field[y][1] = '#';        // Left goal
        field[y][width-2] = '#';  // Right goal
    }
    
    // Convert game coordinates to field coordinates and place players/ball
    // Field coordinate system: x: [-0.95, 0.95] -> [2, width-3], y: [-0.52, 0.52] -> [2, height-3]
    int ball_x = (int)((state->ball.x + 0.95f) / (2.0f * 0.95f) * (width - 4)) + 2;
    int ball_y = (int)((-state->ball.y + 0.52f) / (2.0f * 0.52f) * (height - 4)) + 2;
    
    // Make sure ball is within bounds
    if (ball_x >= 0 && ball_x < width && ball_y >= 0 && ball_y < height) {
        field[ball_y][ball_x] = 'O';  // Ball
    }
    
    // Place players
    for (int i = 0; i < MAX_PLAYERS; i++) {
        int px = (int)((state->players[i].x + 0.95f) / (2.0f * 0.95f) * (width - 4)) + 2;
        int py = (int)((-state->players[i].y + 0.52f) / (2.0f * 0.52f) * (height - 4)) + 2;
        
        if (px >= 0 && px < width && py >= 0 && py < height) {
            if (i < 11) {
                field[py][px] = 'R';  // Red team
            } else {
                field[py][px] = 'B';  // Blue team
            }
        }
    }
    
    // Draw the field
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", field[y][x]);
        }
        printf("\n");
    }
    
    // Print game info
    printf("\nScore: Red %d - Blue %d\n", state->scores[0], state->scores[1]);
    printf("Mode: %s\n", 
           state->mode[0] == 0 ? "Goal Kick" : 
           state->mode[0] == 1 ? "Game" : 
           state->mode[0] == 2 ? "Throw-in/Corners" : "Unknown");
    printf("Controls: A=Shot, S=Pass, Mouse to direct player\n");
    printf("Press 'q' to quit\n");
}

// Placeholder for shader-based rendering
void render_with_shaders(const GameState* state) {
    // In a real implementation, this would use OpenGL to render with shaders
    #ifdef USE_OPENGL
    // OpenGL rendering code would go here
    // Initialize OpenGL context
    // Load shaders
    // Set uniforms
    // Draw game elements
    printf("OpenGL shader rendering would happen here\n");
    #else
    printf("Shader renderer not implemented yet (requires OpenGL)\n");
    #endif
    render_old_school(state);
}