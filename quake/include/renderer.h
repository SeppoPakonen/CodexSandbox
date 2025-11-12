#ifndef RENDERER_H
#define RENDERER_H

// Common renderer interface
typedef struct {
    int width;
    int height;
    float *framebuffer;  // RGB values stored as R, G, B, R, G, B...
} Renderer;

// Initialize renderer
Renderer* init_renderer(int width, int height);

// Clean up renderer
void cleanup_renderer(Renderer *renderer);

// Clear framebuffer with a color
void clear_framebuffer(Renderer *renderer, float r, float g, float b);

// Present the rendered frame
void present_frame(Renderer *renderer);

#endif // RENDERER_H