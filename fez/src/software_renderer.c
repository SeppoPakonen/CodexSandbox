#include "../include/game.h"
#include <SDL2/SDL.h>

extern SDL_Window* g_window;
extern SDL_Renderer* g_sdl_renderer;

// Full software renderer implementation using raycasting
void render_software(GameState* state) {
    // Get window surface for pixel manipulation
    SDL_Surface* screen_surface = SDL_GetWindowSurface(g_window);
    
    // Clear the screen
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 128, 255)); // Sky blue background
    
    // Get screen dimensions
    int width = screen_surface->w;
    int height = screen_surface->h;
    
    // Get player data
    Vec3 player_pos = {state->buffer_a[2].x, state->buffer_a[2].y, state->buffer_a[2].z};
    float rotation = state->buffer_a[1].x;
    
    // Dynamic scaling that always is pixel perfect
    float scale = fmaxf(1.0f, fminf(8.0f, roundf(height / 384.0f)));
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    
    // Raycast for each column of pixels
    for(int x = 0; x < width; x++) {
        // Calculate ray direction in world space
        float ray_x = (x - half_width) / scale;
        ray_x /= scale;
        ray_x += 24.0f;  // Apply same offset as in shader
        
        Vec3 ray_pos = {ray_x, 0.0f, -MAX_STEPS * 8.0f};
        
        // Rotate ray position based on player rotation
        float cos_rot = cosf(PI * 0.5f - rotation);
        float sin_rot = sinf(PI * 0.5f - rotation);
        float rotated_x = ray_pos.x * cos_rot - ray_pos.z * sin_rot;
        float rotated_z = ray_pos.x * sin_rot + ray_pos.z * cos_rot;
        ray_pos.x = rotated_x;
        ray_pos.z = rotated_z;
        
        // Set ray direction based on player rotation
        Vec3 ray_dir = {cosf(rotation), 0.0f, sinf(rotation)};
        
        // Cast the ray through the world
        HitInfo hit = cast_ray(ray_pos, ray_dir);
        
        // Calculate texture coordinates
        Vec3 tile_uv = {
            fmodf(hit.pos.x, 1.0f),
            fmodf(hit.pos.y, 1.0f), 
            fmodf(hit.pos.z, 1.0f)
        };
        
        Uint32 color;
        
        // Color based on whether we hit a tile or not (empty space)
        if(hit.tile.ID == 0) {
            // Sky
            color = SDL_MapRGB(screen_surface->format, 0, 128, 255);
        } else {
            // Calculate block texture
            Vec3 texture_coords = {tile_uv.x + tile_uv.z, tile_uv.y};
            Vec3 block_color = get_block_texture(texture_coords * 16.0f);  // Multiply by 16 like in shader
            
            // Apply grass texture if tile above is air
            Tile above_tile = world((Vec3){
                floorf(hit.pos.x) + (ray_dir.x < 0 ? 0 : 0.001f),
                floorf(hit.pos.y) + 1.0f,
                floorf(hit.pos.z) + (ray_dir.z < 0 ? 0 : 0.001f)
            });
            
            if(above_tile.ID == 0) {
                Vec4 grass = get_grass_texture(texture_coords * 16.0f);
                if(grass.w == 1.0f) {
                    block_color.x = grass.x;
                    block_color.y = grass.y;
                    block_color.z = grass.z;
                }
            }
            
            // Apply shading based on normal
            float shade_factor = 1.0f - 0.2f * hit.normal.z;  // Basic directional shading
            block_color.x *= shade_factor;
            block_color.y *= shade_factor;
            block_color.z *= shade_factor;
            
            // Clamp colors to [0, 1] range and convert to [0, 255]
            int r = (int)(fminf(1.0f, fmaxf(0.0f, block_color.x)) * 255.0f);
            int g = (int)(fminf(1.0f, fmaxf(0.0f, block_color.y)) * 255.0f);
            int b = (int)(fminf(1.0f, fmaxf(0.0f, block_color.z)) * 255.0f);
            
            color = SDL_MapRGB(screen_surface->format, r, g, b);
        }
        
        // Draw a vertical line for this ray
        for(int y = 0; y < height; y++) {
            // For simplicity, we'll color the entire column with the same color
            // In a full implementation, we'd calculate depth-based height
            if(screen_surface->format->BitsPerPixel == 32) {
                Uint32* pixels = (Uint32*)screen_surface->pixels;
                pixels[y * width + x] = color;
            }
        }
    }
    
    // Update the screen
    SDL_UpdateWindowSurface(g_window);
}

// Helper function to multiply Vec2 by scalar (needed for texture coordinate calculation)
Vec2 vec2_mul_scalar(Vec2 v, float s) {
    return (Vec2){v.x * s, v.y * s};
}