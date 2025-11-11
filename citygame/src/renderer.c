#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "renderer.h"
#include "game.h"

// Renderer state for testing purposes
static int screen_width = 0;
static int screen_height = 0;
static int render_mode = RENDERER_OLD_SCHOOL;

int init_renderer(int width, int height, int mode) {
    screen_width = width;
    screen_height = height;
    render_mode = mode;
    
    // For testing purposes, just print that initialization was successful
    printf("Renderer initialized: %dx%d, mode: %s\n", width, height, 
           mode == RENDERER_OLD_SCHOOL ? "oldschool" : "modern");
    
    return 0;
}

void render_frame() {
    if (render_mode == RENDERER_OLD_SCHOOL) {
        printf("Rendering with old school renderer\n");
        render_city_oldschool();
    } else {
        printf("Rendering with modern renderer\n");
        render_city_modern();
    }
}

void cleanup_renderer() {
    printf("Renderer cleaned up\n");
}

void set_viewport(int width, int height) {
    screen_width = width;
    screen_height = height;
}

void set_pixel(int x, int y, Vec3 color) {
    // For testing, just print the pixel info
    if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        printf("Pixel set at (%d, %d): R=%.2f G=%.2f B=%.2f\n", 
               x, y, color.x, color.y, color.z);
    }
}

Vec3 get_block_color(Block *block) {
    Vec3 color = {0.0f, 0.0f, 0.0f};
    
    if (block->th < 1) {
        // Water
        color.x = 0.0f / 255.0f;
        color.y = 112.0f / 255.0f;
        color.z = 144.0f / 255.0f;
    } else if (block->btype == 0 && block->ltype == 0) {
        // Grass
        color.x = 0.0f / 255.0f;
        color.y = 153.0f / 255.0f;
        color.z = 0.0f / 255.0f;
    } else if (block->btype == 0 && block->ltype > 0) {
        // Building
        color.x = 204.0f / 255.0f;
        color.y = 153.0f / 255.0f;
        color.z = 153.0f / 255.0f;
    } else if (block->btype == 1) {
        // Roundabout
        color.x = 102.0f / 255.0f;
        color.y = 102.0f / 255.0f;
        color.z = 102.0f / 255.0f;
    } else if (block->btype == 2) {
        // Semaphore
        if (block->semaphore == 0) {
            color.x = 255.0f / 255.0f;  // Red
            color.y = 0.0f / 255.0f;
            color.z = 0.0f / 255.0f;
        } else if (block->semaphore == 1) {
            color.x = 255.0f / 255.0f;  // Yellow
            color.y = 255.0f / 255.0f;
            color.z = 0.0f / 255.0f;
        } else {
            color.x = 0.0f / 255.0f;   // Green
            color.y = 255.0f / 255.0f;
            color.z = 0.0f / 255.0f;
        }
    } else {
        // Road
        color.x = 102.0f / 255.0f;
        color.y = 102.0f / 255.0f;
        color.z = 102.0f / 255.0f;
    }
    
    return color;
}

void render_block_oldschool(Block *block, int x, int y) {
    if (!block) return;
    
    // For testing, just print block info
    printf("Rendering block at (%d, %d): type=%d, ltype=%d, height=%d\n", 
           x, y, block->btype, block->ltype, block->th);
}

void render_road_oldschool(Block *block) {
    // For testing
    printf("Rendering road\n");
}

void render_building_oldschool(Block *block) {
    // For testing
    printf("Rendering building\n");
}

void render_city_oldschool() {
    printf("Rendering city with old school renderer\n");
    
    // Render all blocks
    for (int y = 0; y < BMAX_Y && y < 3; y++) {  // Limit for testing
        for (int x = 0; x < BMAX_X && x < 3; x++) {  // Limit for testing
            Block *block = get_block(x, y);
            if (block) {
                render_block_oldschool(block, x, y);
            }
        }
    }
    
    // Render vehicles for testing
    printf("Rendering vehicles\n");
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            for (int pos = 0; pos < 4 && pos < BLOCK_BUFFER_X * BLOCK_BUFFER_Y; pos++) {  // Limit for testing
                Cell *cell = get_cell(x, y, pos);
                if (cell && cell->road) {
                    for (int lane = 0; lane < LANES; lane++) {
                        if (cell->v[lane].vtype > 0) {
                            printf("Vehicle at (%d, %d, %d): type=%d, offset=%.2f\n", 
                                   x, y, pos, cell->v[lane].vtype, cell->v[lane].offset);
                        }
                    }
                }
            }
        }
    }
}

void render_city_modern() {
    printf("Rendering city with modern renderer (placeholder)\n");
    // In a real implementation, this would use OpenGL shaders
    render_city_oldschool();
}

void render_block_modern(Block *block, int x, int y) {
    // For testing
    printf("Rendering block with modern renderer\n");
}

void render_road_modern(Block *block) {
    // For testing
    printf("Rendering road with modern renderer\n");
}

void render_building_modern(Block *block) {
    // For testing
    printf("Rendering building with modern renderer\n");
}

// Dummy shader functions for compilation
unsigned int load_shaders() {
    printf("Loading shaders (dummy function for testing)\n");
    return 1; // Simulate successful shader loading
}

unsigned int compile_shader(const char* source, int type) {
    // For testing
    return 1;
}

unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader) {
    // For testing
    return 1;
}