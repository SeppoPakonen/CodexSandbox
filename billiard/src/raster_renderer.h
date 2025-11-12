#ifndef RASTER_RENDERER_H
#define RASTER_RENDERER_H

#include "billiard.h"

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600



// Pixel buffer
typedef struct {
    Color *pixels;
    int width;
    int height;
} Framebuffer;

// Initialize framebuffer
Framebuffer* init_framebuffer(int width, int height);

// Free framebuffer
void free_framebuffer(Framebuffer *fb);

// Clear framebuffer
void clear_framebuffer(Framebuffer *fb, Color color);

// Put pixel in framebuffer
void put_pixel(Framebuffer *fb, int x, int y, Color color);

// Draw a line using Bresenham's algorithm
void draw_line(Framebuffer *fb, int x0, int y0, int x1, int y1, Color color);

// Draw a circle
void draw_circle(Framebuffer *fb, int cx, int cy, int radius, Color color);

// Draw filled circle
void draw_filled_circle(Framebuffer *fb, int cx, int cy, int radius, Color color);

// Convert HSV to RGB
Color hsv_to_rgb(float h, float s, float v);

// Draw the billiard table
void draw_billiard_table(Framebuffer *fb, GameState *state);

// Draw balls on the table
void draw_balls(Framebuffer *fb, GameState *state);

// Draw the cue stick
void draw_cue(Framebuffer *fb, GameState *state);

// Render the scene using software rendering
void render_scene_software(Framebuffer *fb, GameState *state);

#endif // RASTER_RENDERER_H