/*
 * Compatibility header for Allegro 5
 * This file contains wrapper definitions to make the transition from Allegro 4 to Allegro 5
 */

#ifndef ALLEGRO5_COMPAT_H
#define ALLEGRO5_COMPAT_H

#include "allegro5_wrapper.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

// Type definitions to match Allegro 5
typedef ALLEGRO_BITMAP BITMAP;
typedef ALLEGRO_COLOR PALETTE;
typedef ALLEGRO_COLOR RGB;
typedef ALLEGRO_DATAFILE DATAFILE;
typedef struct {
    void *dat;
    int type;
    long size;
    void *extra;
} DATAFILE_STRUCT;

// Define Allegro 5 equivalents for Allegro 4 functions
#define allegro_init() (al_init(), al_install_keyboard(), al_install_mouse(), al_install_joystick(), al_install_timer())
#define allegro_exit() al_uninstall_system()
#define install_timer() al_install_timer()
#define install_keyboard() al_install_keyboard()
#define install_mouse() al_install_mouse()
#define install_joystick(joy_type) al_install_joystick()
#define remove_timer() al_uninstall_timer()
#define set_gfx_mode(mode, w, h, v_w, v_h) (al_set_new_display_flags(0), al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST), (al_create_display(w, h) ? 0 : -1))
#define set_uformat(fmt)
#define set_window_title(title) al_set_window_title(al_get_current_display(), title)
#define set_display_switch_mode(mode) al_set_display_option(al_get_current_display(), ALLEGRO_DISPLAY_ADAPTER, 0)

// Timer functions
#define BPS_TO_TIMER(bps) (60.0f / bps)
#define SECS_TO_TIMER(secs) (secs * 1000.0f)
#define install_int(f, t) al_add_timer_count(al_get_timer_count(al_get_timer(f)), t)
#define install_int_ex(f, t) al_add_timer_count(al_get_timer_count(al_get_timer(f)), t)

// Bitmap functions
#define create_bitmap(w, h) al_create_bitmap(w, h)
#define destroy_bitmap(bmp) al_destroy_bitmap(bmp)
#define clear_bitmap(bmp) al_clear_to_color(al_map_rgb(0, 0, 0))
#define load_bitmap(filename, pal) al_load_bitmap(filename)
#define save_tga(filename, bmp, pal) al_save_bitmap(filename, bmp)
#define blit(source, dest, s_x, s_y, d_x, d_y, w, h) (al_set_target_bitmap(dest), al_draw_bitmap_region(source, s_x, s_y, w, h, d_x, d_y, 0))
#define stretch_blit(source, dest, s_x, s_y, s_w, s_h, d_x, d_y, d_w, d_h) (al_set_target_bitmap(dest), al_draw_scaled_bitmap(source, s_x, s_y, s_w, s_h, d_x, d_y, d_w, d_h, 0))
#define draw_sprite(bmp, sprite, x, y) (al_set_target_bitmap(bmp), al_draw_bitmap(sprite, x, y, 0))

// Color functions
#define makecol8(r, g, b) al_map_rgb(r, g, b)
#define makecol(r, g, b) al_map_rgb(r, g, b)

// Input functions
#define key scancode_table
extern char scancode_table[];

// Datafile functions (will need special handling)
#define load_datafile(filename) NULL
#define unload_datafile(dat) 
#define load_datafile_object(filename, object_name) NULL
#define unload_datafile_object(obj)

// Graphics primitives
#define circlefill(bmp, x, y, radius, color) al_draw_filled_circle(x, y, radius, color)
#define rectfill(bmp, x1, y1, x2, y2, color) al_draw_filled_rectangle(x1, y1, x2, y2, color)

// Constants that should not conflict
#ifndef SWITCH_BACKGROUND
#define SWITCH_BACKGROUND 0
#endif

// Palette functions
#define set_palette(pal)

// Password functions
#define packfile_password(pwd)

// Error handling
extern char allegro_error[256];
#define allegro_message(msg, ...) al_show_native_message_box(al_get_current_display(), "Error", "Error", msg, NULL, 0)

// Some common defines that need to be redefined for Allegro 5
#define END_OF_FUNCTION(f)
#define LOCK_VARIABLE(v)
#define LOCK_FUNCTION(f)

// GFX mode constants
#define GFX_TEXT 0
#define GFX_AUTODETECT_FULLSCREEN 1
#define GFX_AUTODETECT_WINDOWED 2
#define GFX_AUTODETECT 3

// Joystick types
#define JOY_TYPE_NONE 0
#define JOY_TYPE_4BUTTON 1
#define JOY_TYPE_AUTODETECT 2

// Text output functions
#define text_mode(mode)

#endif // ALLEGRO5_COMPAT_H