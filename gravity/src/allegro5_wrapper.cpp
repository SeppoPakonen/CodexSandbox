/*
 * Allegro 5 wrapper implementation for Gravity Strike
 */

#include "allegro5_wrapper.h"
#include <allegro5/allegro_primitives.h>
#include <stdarg.h>
#include <string.h>
#include <cstdio>

// Global variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *fps_timer = NULL;
ALLEGRO_TIMER *second_timer = NULL;
bool redraw = true;
char allegro_error[256] = {0};
bool key[ALLEGRO_KEY_MAX] = {false};
int key_shifts = 0;
int mouse_x = 0, mouse_y = 0;
int mouse_b = 0;
ALLEGRO_KEYBOARD_STATE keyboard_state;
ALLEGRO_MOUSE_STATE mouse_state;

int allegro_init() {
    if (!al_init()) {
        strcpy(allegro_error, "Failed to initialize Allegro");
        return -1;
    }
    
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    
    if (!al_install_keyboard()) {
        strcpy(allegro_error, "Failed to initialize keyboard");
        return -1;
    }
    
    if (!al_install_mouse()) {
        strcpy(allegro_error, "Failed to initialize mouse");
        return -1;
    }
    
    if (!al_install_joystick()) {
        // Not critical, continue without joystick
    }
    
    // Timer is installed separately by install_timer() function
    return 0;
}

void allegro_exit() {
    if (display) {
        al_destroy_display(display);
        display = NULL;
    }
    if (event_queue) {
        al_destroy_event_queue(event_queue);
        event_queue = NULL;
    }
    if (timer) {
        al_destroy_timer(timer);
        timer = NULL;
    }
    if (fps_timer) {
        al_destroy_timer(fps_timer);
        fps_timer = NULL;
    }
    if (second_timer) {
        al_destroy_timer(second_timer);
        second_timer = NULL;
    }
    al_uninstall_system();
}

int install_timer() {
    // Timer installation is automatic in Allegro 5, just return success
    return 0;
}

void remove_timer() {
    if (timer) {
        al_destroy_timer(timer);
        timer = NULL;
    }
}

void install_int(void (*proc)(void), int speed) {
    if (timer) {
        al_destroy_timer(timer);
    }
    timer = al_create_timer(1.0 / speed);  // speed is Hz
    if (timer) {
        al_start_timer(timer);
        // In a real Allegro 4 to 5 migration, this would need event handling for the timer callback
    }
}

void install_int_ex(void (*proc)(void), int speed) {
    install_int(proc, speed);
}

int install_keyboard() {
    return al_install_keyboard() ? 0 : -1;
}

int install_mouse() {
    return al_install_mouse() ? 0 : -1;
}

int install_joystick(int type) {
    return al_install_joystick() ? 0 : -1;
}

int set_gfx_mode(int graphics_driver, int width, int height, int virtual_width, int virtual_height) {
    al_set_new_display_flags(graphics_driver);
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    
    display = al_create_display(width, height);
    if (!display) {
        strcpy(allegro_error, "Failed to create display");
        return -1;
    }
    
    // Set up event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        strcpy(allegro_error, "Failed to create event queue");
        return -1;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    // Set the display as the target
    al_set_target_bitmap(al_get_backbuffer(display));
    
    return 0;
}

void set_uformat(int format) {
    // Not needed in Allegro 5
}

void set_window_title(const char *title) {
    if (display) {
        al_set_window_title(display, title);
    }
}

void set_display_switch_mode(int mode) {
    // Not needed in Allegro 5
}

// Bitmap functions
BITMAP *create_bitmap(int width, int height) {
    return al_create_bitmap(width, height);
}

void destroy_bitmap(BITMAP *bitmap) {
    if (bitmap) {
        al_destroy_bitmap(bitmap);
    }
}

void clear_bitmap(BITMAP *bitmap) {
    if (bitmap) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bitmap);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_set_target_bitmap(old_target);
    }
}

int save_tga(const char *filename, BITMAP *bitmap, PALETTE *palette) {
    return al_save_bitmap(filename, bitmap) ? 0 : -1;
}

void blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int d_x, int d_y, int w, int h) {
    if (source && dest) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(dest);
        al_draw_bitmap_region(source, s_x, s_y, w, h, d_x, d_y, 0);
        al_set_target_bitmap(old_target);
    }
}

void stretch_blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int s_w, int s_h, int d_x, int d_y, int d_w, int d_h) {
    if (source && dest) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(dest);
        al_draw_scaled_bitmap(source, s_x, s_y, s_w, s_h, d_x, d_y, d_w, d_h, 0);
        al_set_target_bitmap(old_target);
    }
}

void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y) {
    if (bmp && sprite) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bmp);
        al_draw_bitmap(sprite, x, y, 0);
        al_set_target_bitmap(old_target);
    }
}

void rotate_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle) {
    if (bmp && sprite) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bmp);
        float angle_f = (float)angle / 256.0f; // Fixed point conversion
        al_draw_rotated_bitmap(sprite, 
                              al_get_bitmap_width(sprite)/2, 
                              al_get_bitmap_height(sprite)/2, 
                              x, y, angle_f, 0);
        al_set_target_bitmap(old_target);
    }
}

void rotate_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle, fixed scale) {
    if (bmp && sprite) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bmp);
        float angle_f = (float)angle / 256.0f; // Fixed point conversion
        float scale_f = (float)scale / 65536.0f; // Fixed point conversion
        al_draw_scaled_rotated_bitmap(sprite, 
                                     al_get_bitmap_width(sprite)/2, 
                                     al_get_bitmap_height(sprite)/2, 
                                     x, y, scale_f, scale_f, angle_f, 0);
        al_set_target_bitmap(old_target);
    }
}

// Primitives
void circlefill(BITMAP *bmp, int x, int y, int radius, int color) {
    if (bmp) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bmp);
        // Convert allegro 4 color to allegro 5 color - this is simplified
        al_draw_filled_circle(x, y, radius, al_map_rgb(255, 255, 255)); // Simplified color conversion
        al_set_target_bitmap(old_target);
    }
}

void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
    if (bmp) {
        ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
        al_set_target_bitmap(bmp);
        // Convert allegro 4 color to allegro 5 color - this is simplified
        al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255)); // Simplified color conversion
        al_set_target_bitmap(old_target);
    }
}

// Text handling
void text_mode(int mode) {
    // Not needed in Allegro 5
}

// Datafile handling (simplified version - needs actual implementation)
DATAFILE *load_datafile(const char *filename) {
    // Allegro 5 doesn't use datafiles directly, but for compatibility
    // we will return NULL since this needs to be handled differently
    return NULL;
}

void unload_datafile(DATAFILE *datafile) {
    // Not applicable in Allegro 5
}

DATAFILE *load_datafile_object(const char *filename, const char *object_name) {
    return NULL;
}

void unload_datafile_object(DATAFILE *datafile) {
    // Not applicable in Allegro 5
}

void allegro_message(const char *fmt, ...) {
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    if (display) {
        al_show_native_message_box(display, "Error", "Error", buffer, NULL, 0);
    } else {
        // Fallback to console if no display
        printf("Error: %s\n", buffer);
    }
}