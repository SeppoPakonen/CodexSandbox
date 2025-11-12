#include "raster_renderer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Initialize framebuffer
Framebuffer* init_framebuffer(int width, int height) {
    Framebuffer *fb = (Framebuffer*)malloc(sizeof(Framebuffer));
    if (!fb) return NULL;
    
    fb->width = width;
    fb->height = height;
    fb->pixels = (Color*)malloc(width * height * sizeof(Color));
    if (!fb->pixels) {
        free(fb);
        return NULL;
    }
    
    clear_framebuffer(fb, (Color){0, 0, 0});  // Black background
    return fb;
}

// Free framebuffer
void free_framebuffer(Framebuffer *fb) {
    if (fb) {
        free(fb->pixels);
        free(fb);
    }
}

// Clear framebuffer
void clear_framebuffer(Framebuffer *fb, Color color) {
    if (!fb || !fb->pixels) return;
    
    for (int i = 0; i < fb->width * fb->height; i++) {
        fb->pixels[i] = color;
    }
}

// Put pixel in framebuffer
void put_pixel(Framebuffer *fb, int x, int y, Color color) {
    if (!fb || !fb->pixels) return;
    if (x < 0 || x >= fb->width || y < 0 || y >= fb->height) return;
    
    fb->pixels[y * fb->width + x] = color;
}

// Draw a line using Bresenham's algorithm
void draw_line(Framebuffer *fb, int x0, int y0, int x1, int y1, Color color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        put_pixel(fb, x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Draw a circle using midpoint circle algorithm
void draw_circle(Framebuffer *fb, int cx, int cy, int radius, Color color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (x <= y) {
        put_pixel(fb, cx + x, cy + y, color);
        put_pixel(fb, cx - x, cy + y, color);
        put_pixel(fb, cx + x, cy - y, color);
        put_pixel(fb, cx - x, cy - y, color);
        put_pixel(fb, cx + y, cy + x, color);
        put_pixel(fb, cx - y, cy + x, color);
        put_pixel(fb, cx + y, cy - x, color);
        put_pixel(fb, cx - y, cy - x, color);
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Draw filled circle
void draw_filled_circle(Framebuffer *fb, int cx, int cy, int radius, Color color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                put_pixel(fb, cx + x, cy + y, color);
            }
        }
    }
}



// Draw the billiard table
void draw_billiard_table(Framebuffer *fb, GameState *state) {
    // Draw table surface
    int table_width = fb->width * 0.8f;
    int table_height = fb->height * 0.6f;
    int table_x = (fb->width - table_width) / 2;
    int table_y = (fb->height - table_height) / 2;
    
    // Draw table background (green)
    for (int y = table_y; y < table_y + table_height; y++) {
        for (int x = table_x; x < table_x + table_width; x++) {
            put_pixel(fb, x, y, (Color){0, 100, 0});  // Green
        }
    }
    
    // Draw table border
    for (int x = table_x; x < table_x + table_width; x++) {
        put_pixel(fb, x, table_y, (Color){139, 69, 19});  // Brown
        put_pixel(fb, x, table_y + table_height - 1, (Color){139, 69, 19});
    }
    for (int y = table_y; y < table_y + table_height; y++) {
        put_pixel(fb, table_x, y, (Color){139, 69, 19});
        put_pixel(fb, table_x + table_width - 1, y, (Color){139, 69, 19});
    }
    
    // Draw corner pockets
    int pocket_radius = 10;
    // Top-left
    draw_filled_circle(fb, table_x, table_y, pocket_radius, (Color){0, 0, 0});
    // Top-right
    draw_filled_circle(fb, table_x + table_width - 1, table_y, pocket_radius, (Color){0, 0, 0});
    // Bottom-left
    draw_filled_circle(fb, table_x, table_y + table_height - 1, pocket_radius, (Color){0, 0, 0});
    // Bottom-right
    draw_filled_circle(fb, table_x + table_width - 1, table_y + table_height - 1, pocket_radius, (Color){0, 0, 0});
    // Middle sides
    draw_filled_circle(fb, table_x, table_y + table_height/2, pocket_radius, (Color){0, 0, 0});
    draw_filled_circle(fb, table_x + table_width - 1, table_y + table_height/2, pocket_radius, (Color){0, 0, 0});
}

// Draw balls on the table
void draw_balls(Framebuffer *fb, GameState *state) {
    int table_width = fb->width * 0.8f;
    int table_height = fb->height * 0.6f;
    int table_x = (fb->width - table_width) / 2;
    int table_y = (fb->height - table_height) / 2;
    
    for (int i = 0; i < NBALL; i++) {
        // Skip if ball is out of bounds
        if (state->balls[i].position.x >= 2.0f * state->hb_len) continue;
        
        // Map from game coordinates to screen coordinates
        // Game coordinates: x [-8, 8], y [-14, 14] (approx)
        int screen_x = table_x + (int)((state->balls[i].position.x + state->hb_len) * table_width / (2.0f * state->hb_len));
        int screen_y = table_y + (int)((state->balls[i].position.y + 1.75f * state->hb_len) * table_height / (2.0f * 1.75f * state->hb_len));
        
        screen_y = fb->height - screen_y;  // Flip Y coordinate
        
        // Get ball color based on ball index
        Color ball_color;
        if (i == 0) {  // Cue ball (white)
            ball_color = (Color){255, 255, 255};
        } else if (i == 1) {  // Black ball (8-ball)
            ball_color = (Color){0, 0, 0};
        } else {  // Colored balls
            float hue = fmodf((float)(i-1) / (float)NBALL, 1.0f);
            float sat = 1.0f - 0.3f * fmodf((float)(i-1), 3.0f);
            float val = 1.0f - 0.3f * fmodf((float)(i-1), 2.0f);
            ball_color = hsv_to_rgb(hue, sat, val);
        }
        
        // Draw the ball
        int radius = (int)(15.0f * 0.46f);  // Scale radius appropriately
        draw_filled_circle(fb, screen_x, screen_y, radius, ball_color);
        
        // Draw a stripe for all balls except cue ball
        if (i != 0) {
            Color stripe_color = (Color){255, 255, 255};  // White stripe
            for (int j = -radius/2; j <= radius/2; j++) {
                float angle = atan2f(j, radius/2);
                int stripe_x = screen_x + (int)(cosf(angle) * radius * 0.7f);
                int stripe_y = screen_y + j;
                if (j*j + (radius/2)*(radius/2) <= radius*radius) {
                    put_pixel(fb, stripe_x, stripe_y, stripe_color);
                }
            }
        }
    }
}

// Draw the cue stick
void draw_cue(Framebuffer *fb, GameState *state) {
    if (!state->show_cue) return;
    
    int table_width = fb->width * 0.8f;
    int table_height = fb->height * 0.6f;
    int table_x = (fb->width - table_width) / 2;
    int table_y = (fb->height - table_height) / 2;
    
    // Calculate cue position based on angle and distance
    float cue_angle = 0.5f * PI - state->a_cue;  // Adjust for our coordinate system
    float cue_distance = state->d_cue;
    
    // Base position (somewhat offset from the table)
    int base_x = table_x + table_width / 2;
    int base_y = table_y + table_height + 50;  // Below the table
    
    // Calculate end position based on angle and distance
    int end_x = base_x + (int)(cue_distance * 50.0f * cosf(cue_angle));
    int end_y = base_y - (int)(cue_distance * 50.0f * sinf(cue_angle));
    
    // Draw the cue stick as a line
    for (int i = 0; i < 3; i++) {
        draw_line(fb, base_x + i - 1, base_y, end_x + i - 1, end_y, (Color){245, 222, 179});  // Wood color
    }
    
    // Draw the tip (red)
    draw_filled_circle(fb, end_x, end_y, 3, (Color){255, 0, 0});
}

// Render the scene using software rendering
void render_scene_software(Framebuffer *fb, GameState *state) {
    // Clear the framebuffer
    clear_framebuffer(fb, (Color){10, 30, 10});  // Dark green background
    
    // Draw the billiard table
    draw_billiard_table(fb, state);
    
    // Draw the balls
    draw_balls(fb, state);
    
    // Draw the cue if visible
    draw_cue(fb, state);
}