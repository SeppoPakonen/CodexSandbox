/*
 * Allegro 5 wrapper for Gravity Strike
 * This file provides the necessary conversions from Allegro 4 to Allegro 5
 */

#ifndef ALLEGRO5_WRAPPER_H
#define ALLEGRO5_WRAPPER_H

#include <allegro5/allegro.h>
#include <stdint.h>

// Fixed point math (simplified)
typedef int32_t fixed;
#define itofix(x) ((x) << 16)
#define fixtoi(x) ((x) >> 16)

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

// Define common types
typedef ALLEGRO_BITMAP BITMAP;
typedef struct {
    void *dat;
    int type;
    long size;
    void *extra;
} DATAFILE_STRUCT, *DATAFILE;

// Create global variables for Allegro 5
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_TIMER *fps_timer;
extern ALLEGRO_TIMER *second_timer;
extern bool redraw;
extern char allegro_error[256];

// System initialization
int allegro_init();
void allegro_exit();

// Timer functions
int install_timer();
void remove_timer();
void install_int(void (*proc)(void), int speed);
void install_int_ex(void (*proc)(void), int speed);

// Input functions
int install_keyboard();
int install_mouse();
int install_joystick(int type);

// Graphics functions
int set_gfx_mode(int graphics_driver, int width, int height, int virtual_width, int virtual_height);
void set_uformat(int format);
void set_window_title(const char *title);
void set_display_switch_mode(int mode);

// Palette and color functions
typedef ALLEGRO_COLOR PALETTE;
typedef ALLEGRO_COLOR RGB;
#define makecol8(r, g, b) al_map_rgb(r, g, b)
#define makecol(r, g, b) al_map_rgb(r, g, b)

// Bitmap functions
BITMAP *create_bitmap(int width, int height);
void destroy_bitmap(BITMAP *bitmap);
void clear_bitmap(BITMAP *bitmap);
int save_tga(const char *filename, BITMAP *bitmap, PALETTE *palette);

void blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int d_x, int d_y, int w, int h);
void stretch_blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int s_w, int s_h, int d_x, int d_y, int d_w, int d_h);
void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y);
void rotate_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle);
void rotate_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle, fixed scale);

// Primitives
void circlefill(BITMAP *bmp, int x, int y, int radius, int color);
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);

// Text handling
void text_mode(int mode);

// Constants
#define GFX_TEXT 0
#define GFX_AUTODETECT_WINDOWED ALLEGRO_WINDOWED
#define GFX_AUTODETECT_FULLSCREEN ALLEGRO_FULLSCREEN
#define GFX_AUTODETECT ALLEGRO_WINDOWED

#define SWITCH_BACKGROUND 0

#define BPS_TO_TIMER(bps) (1000.0f / bps)  // Convert bps to milliseconds
#define SECS_TO_TIMER(secs) (secs * 1000.0f)  // Convert seconds to milliseconds

// Datafile handling (needs to be implemented differently for Allegro 5)
DATAFILE *load_datafile(const char *filename);
void unload_datafile(DATAFILE *datafile);
DATAFILE *load_datafile_object(const char *filename, const char *object_name);
void unload_datafile_object(DATAFILE *datafile);

// Locking macros (not needed in Allegro 5)
#define LOCK_VARIABLE(v)
#define LOCK_FUNCTION(f)
#define END_OF_FUNCTION(f)

// Joystick types
#define JOY_TYPE_NONE 0
#define JOY_TYPE_4BUTTON 1
#define JOY_TYPE_AUTODETECT 2

// Input state
extern bool key[ALLEGRO_KEY_MAX];
extern int key_shifts;
extern int mouse_x, mouse_y;
extern int mouse_b;

// Error message
void allegro_message(const char *fmt, ...);

#endif // ALLEGRO5_WRAPPER_H