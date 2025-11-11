#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep
#include <sys/time.h> // for struct timeval
#include "game.h"
#include "renderer.h"

// Frame rate control
#define FRAME_RATE 10  // Lower frame rate for testing
#define FRAME_DELAY (1000000 / FRAME_RATE)  // in microseconds

// Simple sleep function using select
void simple_sleep_microseconds(long usec) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = usec;
    select(0, NULL, NULL, NULL, &tv);
}

int main(int argc, char* argv[]) {
    printf("City Game starting...\n");
    
    // Initialize the game
    if (init_game() != 0) {
        printf("Failed to initialize game!\n");
        return -1;
    }
    
    printf("Game initialized successfully!\n");
    printf("Controls:\n");
    printf("  - Program runs for 10 seconds then exits\n");
    printf("  - Game will run automatically\n");
    
    // Main game loop - run for 10 seconds for testing
    int frames = 0;
    const int max_frames = FRAME_RATE * 10; // 10 seconds of frames
    
    while (frames < max_frames) {
        // Update game state
        update_game();
        
        // Render
        render_frame();
        
        // Simple delay using select
        simple_sleep_microseconds(FRAME_DELAY);
        
        frames++;
        
        // Add a simple exit condition for testing
        if (frames % (FRAME_RATE * 2) == 0) { // Print every 2 seconds
            printf("Running... Frame %d/%d\n", frames, max_frames);
        }
    }
    
    // Clean up
    cleanup_game();
    
    printf("Game exited successfully after %d frames!\n", frames);
    return 0;
}