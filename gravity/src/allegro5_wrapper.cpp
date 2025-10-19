/* 
 * Allegro 5 wrapper implementation for Gravity Strike
 * Provides Allegro 4 compatibility functions for Allegro 5
 */

#include "allegro5_wrapper.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <cctype>
#include <vector>

// Global palette array simulating Allegro 4 palette system
ALLEGRO_COLOR current_palette[256];
ALLEGRO_COLOR black_palette[256];
ALLEGRO_COLOR white_palette[256];
ALLEGRO_COLOR red_palette[256];

// Initialize the global palettes
void init_palettes() {
    for (int i = 0; i < 256; i++) {
        // Initialize black palette
        black_palette[i] = al_map_rgb(0, 0, 0);
        
        // Initialize white palette - using values appropriate for Allegro 5
        white_palette[i] = al_map_rgb(255, 255, 255);
        
        // Initialize red palette
        red_palette[i] = al_map_rgb(255, 0, 0);
        
        // Initialize current palette to default
        current_palette[i] = al_map_rgb(0, 0, 0);
    }
}

// Initialize palettes on startup
struct PaletteInit {
    PaletteInit() {
        init_palettes();
    }
} palette_init;

// Allegro 4 style palette functions for compatibility
void get_palette(PALETTE pal) {
    // In Allegro 5, we don't actually read from a palette since there isn't one
    // This is a stub function to provide compatibility
    (void)pal; // avoid unused parameter warning
}

void set_palette(const PALETTE pal) {
    // In Allegro 5, we don't actually set the palette since there isn't one
    // This is a stub function to provide compatibility
    (void)pal; // avoid unused parameter warning
}

void set_palette_range(const RGB *p, int from, int to, int vsync) {
    // In Allegro 5, we don't actually set the palette since there isn't one
    // This is a stub function to provide compatibility
    (void)p; (void)from; (void)to; (void)vsync; // avoid unused parameter warnings
}

void set_color(int index, RGB *color) {
    // In Allegro 5, we don't actually set individual palette colors
    // This is a stub function to provide compatibility
    (void)index; (void)color; // avoid unused parameter warnings
}

void get_color(int index, RGB *color) {
    // In Allegro 5, we don't actually get individual palette colors
    // This is a stub function to provide compatibility
    (void)index; (void)color; // avoid unused parameter warnings
}

// System initialization
int allegro_init() {
    if (!al_init()) {
        return -1;
    }
    
    al_install_keyboard();
    al_install_mouse();
    
    // Timer system is typically available by default in Allegro 5
    // No explicit installation needed like Allegro 4
    
    return 0;
}

void allegro_exit() {
    // Cleanup if needed
}

// Timer functions
int install_timer() {
    // Already handled by Allegro 5 initialization
    return 0;
}

void remove_timer() {
    // Not needed in Allegro 5
}

void install_int(void (*proc)(void), int speed) {
    // Set up timer callback for Allegro 5 - simplified implementation
    (void)proc; (void)speed; // avoid unused parameter warnings
}

void install_int_ex(void (*proc)(void), int speed) {
    // Set up timer callback for Allegro 5 - simplified implementation
    (void)proc; (void)speed; // avoid unused parameter warnings
}

// Input functions
int install_keyboard() {
    return al_install_keyboard() ? 0 : -1;
}

int install_mouse() {
    return al_install_mouse() ? 0 : -1;
}

int install_joystick(int type) {
    (void)type; // avoid unused parameter warning
    return al_install_joystick() ? 0 : -1;
}

// Graphics functions
int set_gfx_mode(int graphics_driver, int width, int height, int virtual_width, int virtual_height) {
    (void)virtual_width; (void)virtual_height; // Allegro 5 doesn't use virtual resolution the same way
    
    al_set_new_display_flags(graphics_driver);
    display = al_create_display(width, height);
    
    if (!display) {
        return -1;
    }
    
    // Create a screen bitmap for compatibility
    if (screen) {
        destroy_bitmap(screen);
    }
    screen = create_bitmap(width, height);
    
    return 0;
}

void set_window_title(const char *title) {
    al_set_window_title(display, title);
}

void set_display_switch_mode(int mode) {
    (void)mode; // Not used in Allegro 5
}

// Bitmap functions
BITMAP *create_bitmap(int width, int height) {
    ALLEGRO_BITMAP *al_bitmap = al_create_bitmap(width, height);
    if (!al_bitmap) {
        return nullptr;
    }
    
    BITMAP *bmp = new BITMAP;
    bmp->bitmap = al_bitmap;
    bmp->w = al_get_bitmap_width(al_bitmap);
    bmp->h = al_get_bitmap_height(al_bitmap);
    
    // Initialize the line member for Allegro 4 compatibility
    // This is a temporary workaround to satisfy the compiler
    // In a real implementation, this would need to be properly initialized
    bmp->line = nullptr;
    
    return bmp;
}

BITMAP *create_sub_bitmap(BITMAP *parent, int x, int y, int width, int height) {
    if (!parent || !parent->bitmap) {
        return nullptr;
    }
    
    ALLEGRO_BITMAP *sub_bitmap = al_create_sub_bitmap(parent->bitmap, x, y, width, height);
    if (!sub_bitmap) {
        return nullptr;
    }
    
    BITMAP *bmp = new BITMAP;
    bmp->bitmap = sub_bitmap;
    bmp->w = width;
    bmp->h = height;
    
    return bmp;
}

void destroy_bitmap(BITMAP *bitmap) {
    if (bitmap) {
        if (bitmap->bitmap) {
            al_destroy_bitmap(bitmap->bitmap);
        }
        delete bitmap;
    }
}

void clear_bitmap(BITMAP *bitmap) {
    if (bitmap && bitmap->bitmap) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bitmap->bitmap);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_set_target_bitmap(old_target);
    }
}

int save_tga(const char *filename, BITMAP *bitmap, PALETTE *palette) {
    (void)palette; // palette is ignored in Allegro 5
    
    if (!bitmap || !bitmap->bitmap) {
        return -1;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bitmap->bitmap);
    
    int result = al_save_bitmap(filename, bitmap->bitmap) ? 0 : -1;
    
    al_set_target_bitmap(old_target);
    return result;
}

void blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int d_x, int d_y, int w, int h) {
    if (!source || !source->bitmap || !dest || !dest->bitmap) {
        return;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(dest->bitmap);
    al_draw_bitmap_region(source->bitmap, s_x, s_y, w, h, d_x, d_y, 0);
    al_set_target_bitmap(old_target);
}

void stretch_blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int s_w, int s_h, int d_x, int d_y, int d_w, int d_h) {
    if (!source || !source->bitmap || !dest || !dest->bitmap) {
        return;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(dest->bitmap);
    al_draw_scaled_bitmap(source->bitmap, s_x, s_y, s_w, s_h, d_x, d_y, d_w, d_h, 0);
    al_set_target_bitmap(old_target);
}

void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) {
        return;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    al_draw_bitmap(sprite->bitmap, x, y, 0);
    al_set_target_bitmap(old_target);
}

void rotate_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) {
        return;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    float angle_f = (float)angle / 65536.0f; // Convert fixed point to float
    al_draw_rotated_bitmap(sprite->bitmap, 0, 0, x, y, angle_f, 0);
    al_set_target_bitmap(old_target);
}

void rotate_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle, fixed scale) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) {
        return;
    }
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    float angle_f = (float)angle / 65536.0f;
    float scale_f = (float)scale / 65536.0f;
    al_draw_scaled_rotated_bitmap(sprite->bitmap, 0, 0, x, y, scale_f, scale_f, angle_f, 0);
    al_set_target_bitmap(old_target);
}

// Helper functions for accessing bitmap properties
int get_bitmap_width(BITMAP *bmp) {
    if (bmp && bmp->bitmap) {
        return al_get_bitmap_width(bmp->bitmap);
    }
    return 0;
}

int get_bitmap_height(BITMAP *bmp) {
    if (bmp && bmp->bitmap) {
        return al_get_bitmap_height(bmp->bitmap);
    }
    return 0;
}

// Primitives
void circlefill(BITMAP *bmp, int x, int y, int radius, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Convert Allegro 4 color index to Allegro 5 color (simplified)
    al_draw_filled_circle(x, y, radius, al_map_rgb(255, 255, 255)); 
    al_set_target_bitmap(old_target);
}

void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Convert Allegro 4 color index to Allegro 5 color (simplified)
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255));
    al_set_target_bitmap(old_target);
}

void putpixel(BITMAP *bmp, int x, int y, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Convert Allegro 4 color index to Allegro 5 color (simplified)
    al_put_pixel(x, y, al_map_rgb(255, 255, 255));
    al_set_target_bitmap(old_target);
}

int getpixel(BITMAP *bmp, int x, int y) {
    if (!bmp || !bmp->bitmap) return 0;
    
    // Allegro 5 doesn't have a direct getpixel function in the same way
    // This is a simplified version
    (void)x; (void)y; 
    return 0;
}

// Additional primitive drawing functions
void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to RGB (simplified)
    ALLEGRO_COLOR al_color = al_map_rgb(255, 255, 255); // White for now
    
    al_draw_line(x1 + 0.5f, y1 + 0.5f, x2 + 0.5f, y2 + 0.5f, al_color, 1.0f);
    
    al_set_target_bitmap(old_target);
}

void circle(BITMAP *bmp, int x, int y, int radius, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to RGB (simplified)
    ALLEGRO_COLOR al_color = al_map_rgb(255, 255, 255); // White for now
    
    al_draw_circle(x, y, radius, al_color, 1.0);
    
    al_set_target_bitmap(old_target);
}

void ellipse(BITMAP *bmp, int x, int y, int rx, int ry, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to RGB (simplified)
    ALLEGRO_COLOR al_color = al_map_rgb(255, 255, 255); // White for now
    
    al_draw_ellipse(x, y, rx, ry, al_color, 1.0);
    
    al_set_target_bitmap(old_target);
}

void ellipsefill(BITMAP *bmp, int x, int y, int rx, int ry, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to RGB (simplified)
    ALLEGRO_COLOR al_color = al_map_rgb(255, 255, 255); // White for now
    al_draw_filled_ellipse(x, y, rx, ry, al_color);
    
    al_set_target_bitmap(old_target);
}

void floodfill(BITMAP *bmp, int x, int y, int color) {
    // In Allegro 5, there's no direct floodfill function
    // This is a no-op implementation for now
    (void)bmp; (void)x; (void)y; (void)color;
}

void do_circle(BITMAP *bmp, int x, int y, int radius, int color, void (*proc)(BITMAP *, int, int, int)) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // For now, just draw a circle using primitives
    al_draw_circle(x, y, radius, al_map_rgb(255, 255, 255), 1.0);
    al_set_target_bitmap(old_target);
    
    if (proc) {
        // Execute the callback for each point on the circle
        proc(bmp, x, y, color);
    }
}

// Text handling
int text_length(ALLEGRO_FONT *f, const char *str) {
    if (!f || !str) return 0;
    return al_get_text_width(f, str);
}

int text_height(ALLEGRO_FONT *f) {
    if (!f) return 0;
    return al_get_font_line_height(f);
}

void aatextout(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color) {
    if (!bmp || !bmp->bitmap || !f || !str) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to Allegro 5 color
    // For now, we'll just use white color, but in a real implementation you'd map color indices
    (void)color; // unused for now
    al_draw_text(f, al_map_rgb(255, 255, 255), x, y, 0, str);
    
    al_set_target_bitmap(old_target);
}

void text_mode(int mode) {
    (void)mode; // Not used in Allegro 5
}

// Datafile handling
DATAFILE *load_datafile(const char *filename) {
    (void)filename;
    // Not implemented - need a real implementation based on Allegro 5 data system
    return nullptr;
}

void unload_datafile(DATAFILE *datafile) {
    (void)datafile;
    // Not implemented
}

DATAFILE *load_datafile_object(const char *filename, const char *object_name) {
    (void)filename; (void)object_name;
    // Not implemented
    return nullptr;
}

void unload_datafile_object(DATAFILE *datafile) {
    (void)datafile;
    // Not implemented
}

// Input functions
int keypressed() {
    // In Allegro 5, we'd check the event queue
    // This is a simplified version for compatibility
    return 0;
}

int readkey() {
    // In Allegro 5, we'd check the event queue
    // This is a simplified version for compatibility
    return 0;
}

void clear_keybuf() {
    // In Allegro 5, we'd clear the event queue
    // This is a simplified version for compatibility
}

// Error messages
void allegro_message(const char *fmt, ...) {
    (void)fmt; // avoid unused parameter warning
    // Not implemented - would use printf or similar
}

// Audio functions
int play_sample(SAMPLE *sample, int vol, int pan, int freq, int loop) {
    (void)sample; (void)vol; (void)pan; (void)freq; (void)loop;
    // Not implemented yet
    return 0;
}

void stop_sample(SAMPLE *sample) {
    (void)sample; // avoid unused parameter warning
}

void destroy_sample(SAMPLE *sample) {
    if (sample && sample->sample) {
        al_destroy_sample(sample->sample);
    }
    delete sample;
}

SAMPLE *load_sample(const char *filename) {
    (void)filename;
    // Not implemented yet
    return nullptr;
}

// RLE Sprite functions
void draw_rle_sprite(BITMAP *bmp, RLE_SPRITE *sprite, int x, int y) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) return;
    // Draw the RLE sprite to the target bitmap
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    al_draw_bitmap(sprite->bitmap, x, y, 0);
    al_set_target_bitmap(old_target);
}

void stretch_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) return;
    // Draw the sprite with stretched dimensions
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    al_draw_scaled_bitmap(sprite->bitmap, 0, 0, sprite->w, sprite->h, 
                         x, y, w, h, 0);
    al_set_target_bitmap(old_target);
}

// Additional Allegro 4 functions needed for compatibility
const char* get_filename(const char* path) {
    // Return just the filename part of the path
    const char* last_slash = strrchr(path, '/');
    if (last_slash) {
        return last_slash + 1;
    }
    const char* last_backslash = strrchr(path, '\\');
    if (last_backslash) {
        return last_backslash + 1;
    }
    return path;
}

BITMAP* load_bitmap(const char* filename, RGB* pal) {
    (void)pal; // ignore palette for Allegro 5
    ALLEGRO_BITMAP *al_bitmap = al_load_bitmap(filename);
    if (!al_bitmap) {
        return nullptr;
    }
    
    BITMAP *bmp = new BITMAP;
    bmp->bitmap = al_bitmap;
    bmp->w = al_get_bitmap_width(al_bitmap);
    bmp->h = al_get_bitmap_height(al_bitmap);
    
    return bmp;
}

void destroy_rle_sprite(RLE_SPRITE* spr) {
    if (spr) {
        if (spr->bitmap) {
            al_destroy_bitmap(spr->bitmap);
        }
        delete spr;
    }
}

RLE_SPRITE* get_rle_sprite(BITMAP* bmp) {
    if (!bmp || !bmp->bitmap) {
        return nullptr;
    }
    
    // In Allegro 5, we'll just return a wrapper that contains the bitmap
    RLE_SPRITE* rle = new RLE_SPRITE;
    rle->bitmap = bmp->bitmap;
    rle->w = bmp->w;
    rle->h = bmp->h;
    
    return rle;
}

void clear(BITMAP* bmp) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_set_target_bitmap(old_target);
}

void clear_to_color(BITMAP* bmp, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color to RGB (simplified)
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    
    al_clear_to_color(al_map_rgb(r, g, b));
    al_set_target_bitmap(old_target);
}

void rect(BITMAP* bmp, int x1, int y1, int x2, int y2, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Use a simple white color for now - in a full implementation you'd need to map from the color index
    al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255), 1.0);
    al_set_target_bitmap(old_target);
}

void textout_centre(BITMAP* bmp, ALLEGRO_FONT* f, const char* str, int x, int y, int color) {
    if (!bmp || !bmp->bitmap || !str) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Use a simple white color for now - in a full implementation you'd need to map from the color index
    // In Allegro 5, we'd need to create or have an appropriate font
    (void)f; (void)color; // unused for now
    // For now, just skip text rendering until we have proper font handling
    // In a real implementation, you'd need to load or initialize a font
    (void)x; (void)y; (void)str;  // avoid unused parameter warnings
    al_set_target_bitmap(old_target);
}

void textout_centre_ex(BITMAP* bmp, ALLEGRO_FONT* f, const char* str, int x, int y, int color, int bg) {
    if (!bmp || !bmp->bitmap || !str) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    // Use a simple white color for now - in a full implementation you'd need to map from the color index
    // In Allegro 5, we'd need to create or have an appropriate font
    (void)f; (void)color; (void)bg; // unused for now
    // For now, just skip text rendering until we have proper font handling
    // In a real implementation, you'd need to load or initialize a font
    (void)x; (void)y; (void)str;  // avoid unused parameter warnings
    al_set_target_bitmap(old_target);
}

// Additional Allegro 4 compatibility functions
int stricmp(const char *s1, const char *s2) {
    return strcasecmp(s1, s2);
}

char* strupr(char *str) {
    char *p = str;
    while (*p) {
        *p = toupper(*p);
        p++;
    }
    return str;
}

void replace_extension(char *dest, const char *src, const char *ext, int size) {
    // Copy source to destination first
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
    
    // Find the last dot to replace extension
    char *dot = strrchr(dest, '.');
    if (dot) {
        // Replace extension
        snprintf(dot, size - (dot - dest), ".%s", ext);
    } else {
        // No extension, append
        int len = strlen(dest);
        if (len + strlen(ext) + 2 < size) {
            strcat(dest, ".");
            strcat(dest, ext);
        }
    }
}

// Character encoding (Allegro 4 compatibility)
void set_uformat(int format) {
    // In Allegro 5, UTF-8 is the default and only supported encoding
    // This function is a no-op for compatibility
    (void)format;
}


int exists(const char* filename) {
    if (!filename) return 0;
    
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Additional Allegro 4 functions for Gravity Strike compatibility
void hline(BITMAP *bmp, int x1, int y, int x2, int color) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color to RGB (simplified)
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    
    al_draw_line(x1, y, x2, y, al_map_rgb(r, g, b), 1.0);
    al_set_target_bitmap(old_target);
}

// Color component extraction functions (Allegro 4 compatibility)
int getr8(int color) {
    return (color >> 16) & 0xFF;
}

int getg8(int color) {
    return (color >> 8) & 0xFF;
}

int getb8(int color) {
    return color & 0xFF;
}

// Font variable for compatibility (stub)
ALLEGRO_FONT *font = nullptr;

// Text output functions (Allegro 4 compatibility)
void textout_ex(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color, int bg) {
    if (!bmp || !bmp->bitmap || !str) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color to RGB (simplified)
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    
    al_draw_text(f ? f : font, al_map_rgb(r, g, b), x, y, 0, str);
    al_set_target_bitmap(old_target);
    
    (void)bg; // unused in Allegro 5
}

// Sprite drawing functions (Allegro 4 compatibility)
void draw_sprite_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    al_draw_bitmap(sprite->bitmap, x, y, ALLEGRO_FLIP_HORIZONTAL);
    al_set_target_bitmap(old_target);
}

void draw_lit_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int color) {
    if (!bmp || !bmp->bitmap || !sprite || !sprite->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color to RGB (simplified)
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    
    al_draw_tinted_bitmap(sprite->bitmap, al_map_rgb(r, g, b), x, y, 0);
    al_set_target_bitmap(old_target);
}
ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
ALLEGRO_TIMER *timer = nullptr;
ALLEGRO_TIMER *fps_timer = nullptr;
ALLEGRO_TIMER *second_timer = nullptr;
bool redraw = false;
char allegro_error[256] = {0};

// Allegro 4 compatibility globals
BITMAP *screen = nullptr;  // To be set after display creation

// Input state
bool key[ALLEGRO_KEY_MAX] = {false};
int key_shifts = 0;
int mouse_x = 0;
int mouse_y = 0;
int mouse_b = 0;

// Global color mapping tables
RGB_MAP *rgb_map = nullptr;
COLOR_MAP *color_map = nullptr;

// Joystick (global)
JOYSTICK joy[2];

// Functions for color mapping (no-op implementations)
void create_rgb_table(RGB_MAP *table, RGB *pal, void (*callback)(int, int)) {
    (void)table; (void)pal; (void)callback; // avoid unused parameter warnings
    // No-op in Allegro 5: color mapping is handled differently
}

void create_trans_table(COLOR_MAP *table, RGB *pal, int r, int g, int b, void (*callback)(int, int)) {
    (void)table; (void)pal; (void)r; (void)g; (void)b; (void)callback; // avoid unused parameter warnings
    // No-op in Allegro 5: transparency is handled differently
}

void create_light_table(COLOR_MAP *table, RGB *pal, int r, int g, int b, void (*callback)(int, int)) {
    (void)table; (void)pal; (void)r; (void)g; (void)b; (void)callback; // avoid unused parameter warnings
    // No-op in Allegro 5: lighting is handled differently
}

// Antialiasing functions (simplified - no-op)
void antialias_init(RGB *pal) {
    (void)pal; // avoid unused parameter warning
    // No-op - text antialiasing is different in Allegro 5
}

void antialias_exit() {
    // No-op
}

// Bitmap locking functions (no-op in Allegro 5)
void acquire_bitmap(BITMAP *bmp) {
    (void)bmp; // avoid unused parameter warning
    // No-op in Allegro 5 - no manual bitmap locking required
}

void release_bitmap(BITMAP *bmp) {
    (void)bmp; // avoid unused parameter warning
    // No-op in Allegro 5 - no manual bitmap locking required
}

// Timer and input functions
void rest(unsigned int milliseconds) {
    al_rest(milliseconds / 1000.0);  // Convert milliseconds to seconds
}

int keyboard_needs_poll() {
    // Allegro 5 uses event queues, so polling is not needed in the same way
    return 0;
}

void poll_keyboard() {
    // In Allegro 5, keyboard events are handled through event queues
    // This is a simplified implementation 
}

void poll_joystick() {
    // In Allegro 5, joystick events are handled through event queues
    // This is a simplified implementation
}

// Additional Allegro 4 compatibility functions needed for Gravity Strike
void aatextout_right(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color) {
    if (!bmp || !bmp->bitmap || !f || !str) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color index to Allegro 5 color
    // For now, we'll just use white color, but in a real implementation you'd map color indices
    (void)color; // unused for now
    
    // Get text width to align right
    int text_width = al_get_text_width(f, str);
    al_draw_text(f, al_map_rgb(255, 255, 255), x - text_width, y, 0, str);
    
    al_set_target_bitmap(old_target);
}
// Line drawing functions (Allegro 4 compatibility)
void do_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color, void (*proc)(BITMAP *, int, int, int)) {
    if (!bmp || !bmp->bitmap) return;
    
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
    al_set_target_bitmap(bmp->bitmap);
    
    // Convert Allegro 4 color to RGB (simplified)
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    
    // Draw the line using Allegro 5
    al_draw_line(x1 + 0.5f, y1 + 0.5f, x2 + 0.5f, y2 + 0.5f, al_map_rgb(r, g, b), 1.0f);
    
    // Call the callback for each point on the line
    if (proc) {
        // For simplicity, just call it at the endpoints
        // A real implementation would iterate over all points on the line
        proc(bmp, x1, y1, color);
        proc(bmp, x2, y2, color);
    }
    
    al_set_target_bitmap(old_target);
}

// File utilities (Allegro 4 compatibility)
int file_size(const char *filename) {
    if (!filename) return -1;
    
    struct stat buffer;
    if (stat(filename, &buffer) == 0) {
        return buffer.st_size;
    }
    return -1;
}


// Mouse functions (Allegro 4 compatibility)
void position_mouse(int x, int y) {
    // In Allegro 5, we use al_set_mouse_xy
    // This is a simplified implementation for compatibility
    (void)x; (void)y; // Avoid unused parameter warnings
    // al_set_mouse_xy(display, x, y); // Would need access to display
}

int pack_fread(void *p, int size, PACKFILE *f) {
    if (!p || !f || !f->file) return 0;
    
    size_t result = al_fread(f->file, p, size);
    return result;
}

int pack_fwrite(const void *p, int size, PACKFILE *f) {
    if (!p || !f || !f->file) return 0;
    
    size_t result = al_fwrite(f->file, p, size);
    return result;
}

int pack_fclose(PACKFILE *f) {
    if (!f || !f->file) return -1;
    
    int result = al_fclose(f->file);
    delete f;
    return result;
}

// MappyAL global variables
int maperror = 0;                    /* Set to a MER_ error if something wrong happens */
short int mapwidth = 0, mapheight = 0, mapblockwidth = 0, mapblockheight = 0, mapdepth = 0;
short int mapblockstrsize = 0, mapnumblockstr = 0, mapnumblockgfx = 0;
short int mapaltdepth = 0, maptype = 0;
char *mappt = nullptr;                    /* Pointer to current map data */
char *maparraypt = nullptr;               /* Pointer to map data array */
char *mapcmappt = nullptr;                /* Pointer to colour map */
char *mapblockgfxpt = nullptr;            /* Pointer to block graphics data */
char *mapblockstrpt = nullptr;            /* Pointer to block structure data */
ANISTR *mapanimstrpt = nullptr;           /* Pointer to animation structure data */
ANISTR *mapanimstrendpt = nullptr;        /* Pointer to animation structure data */
short int **mapmappt = nullptr;           /* Pointer to map data matrix */
short int ***mapmaparraypt = nullptr;     /* Pointer to map data matrix array */
BITMAP **abmTiles = nullptr;              /* Array of bitmaps for tiles */
int mapblocksinvidmem = 0, mapblocksinsysmem = 0;
int mapblockgapx = 0, mapblockgapy = 0;
int mapblockstaggerx = 0, mapblockstaggery = 0;

// MappyAL function implementations
int MapLoad(char *mapname) {
    // Placeholder implementation
    (void)mapname;
    return 0;
}

int MapLoadMAR(char *mname, int marlyr) {
    // Placeholder implementation
    (void)mname; (void)marlyr;
    return 0;
}

void MapFreeMem(void) {
    // Placeholder implementation
}

int MapGenerateYLookup(void) {
    // Placeholder implementation
    return 0;
}

int MapInitAnims(void) {
    // Placeholder implementation
    return 0;
}

int MapRelocate(void) {
    // Placeholder implementation
    return 0;
}

int MapRelocate2(void) {
    // Placeholder implementation
    return 0;
}

int MapGetBlockID(int blid, int usernum) {
    // Placeholder implementation
    (void)blid; (void)usernum;
    return 0;
}

int MapGetBlockInPixels(int x, int y) {
    // Placeholder implementation
    (void)x; (void)y;
    return 0;
}

int MapGetBlock(int x, int y) {
    // Placeholder implementation
    (void)x; (void)y;
    return 0;
}

void MapSetBlockInPixels(int x, int y, int blid) {
    // Placeholder implementation
    (void)x; (void)y; (void)blid;
}

void MapSetBlock(int x, int y, int blid) {
    // Placeholder implementation
    (void)x; (void)y; (void)blid;
}

void MapChangeLayer(int laynum) {
    // Placeholder implementation
    (void)laynum;
}

void MapDrawBG(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph) {
    // Simple implementation for now
    (void)bmp; (void)mapxo; (void)mapyo; (void)mapx; (void)mapy; (void)mapw; (void)maph;
}

void MapDrawBGT(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph) {
    // Simple implementation for now
    (void)bmp; (void)mapxo; (void)mapyo; (void)mapx; (void)mapy; (void)mapw; (void)maph;
}

void MapDrawFG(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph, int maprw) {
    // Simple implementation for now
    (void)bmp; (void)mapxo; (void)mapyo; (void)mapx; (void)mapy; (void)mapw; (void)maph; (void)maprw;
}

void MapDrawRow(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph, int maprw, void (*cellcall)(int cx, int cy, int dx, int dy)) {
    // Simple implementation for now
    (void)bmp; (void)mapxo; (void)mapyo; (void)mapx; (void)mapy; (void)mapw; (void)maph; (void)maprw; (void)cellcall;
}

void MapCorrectColours(void) {
    // Placeholder implementation
}

void MapSetPal8(const PALETTE pal) {
    // Placeholder implementation
    (void)pal;
}

int MapGetXOffset(int blid, int dir) {
    // Placeholder implementation
    (void)blid; (void)dir;
    return 0;
}

int MapGetYOffset(int blid, int dir) {
    // Placeholder implementation
    (void)blid; (void)dir;
    return 0;
}

BITMAP *MapMakeParallaxBitmap(BITMAP *bmp, int flags) {
    // Placeholder implementation
    (void)bmp; (void)flags;
    return nullptr;
}

void MapDrawParallax(BITMAP *bmp, BITMAP *pbmp, int xo, int yo, int x, int y, int w, int h) {
    // Simple implementation for now
    (void)bmp; (void)pbmp; (void)xo; (void)yo; (void)x; (void)y; (void)w; (void)h;
}

// End of file



