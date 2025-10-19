/*
 * Allegro 5 wrapper for Gravity Strike
 * This file provides the necessary conversions from Allegro 4 to Allegro 5
 */

#ifndef ALLEGRO5_WRAPPER_H
#define ALLEGRO5_WRAPPER_H

#include <allegro5/allegro.h>
#include <lua.hpp>  // Include Lua header for lua_State
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

// Include mappyal.h for Map functions and types
#include "mappyal.h"

// Define common types
// For Allegro 5 compatibility, we use a structure that can provide the required interface
typedef struct {
    ALLEGRO_BITMAP *bitmap;
    int w, h;  // width and height
    
    // Add dummy line member for Allegro 4 compatibility
    // This is a temporary workaround to satisfy the compiler
    // In a real implementation, this would need to be properly initialized
    unsigned char **line;
} BITMAP;

// Map functions with C linkage for compatibility with mappyal.h
extern "C" {
} // extern "C"

typedef ALLEGRO_FONT FONT;

// RLE_SPRITE structure for Allegro 5 (simplified)
typedef struct {
    ALLEGRO_BITMAP *bitmap;
    int w, h;  // width and height
    // For Allegro 5 compatibility, we'll initialize these from the bitmap
} RLE_SPRITE;

// SAMPLE structure for Allegro 5 (simplified)
typedef struct {
    ALLEGRO_SAMPLE *sample;
    void *data;
    int priority;
    int param;
} SAMPLE;

// DATAFILE structure for Allegro 5 - corrected for Allegro 4 compatibility
// In Allegro 4, DATAFILE was the struct type itself, not a pointer
typedef struct {
    void *dat;
    int type;
    long size;
    void *extra;
} DATAFILE;

// Define DATAFILE_STRUCT as an alias to DATAFILE for compatibility
typedef DATAFILE DATAFILE_STRUCT;

// Define common macros and constants
#define DAT_BITMAP 1  // Example Allegro datafile type constants
#define DAT_RLE_SPRITE 2
#define DAT_SAMPLE 3

// MappyAL structures
extern ALLEGRO_TIMER *fps_timer;
extern ALLEGRO_TIMER *second_timer;
extern bool redraw;
extern char allegro_error[256];

// Allegro 4 compatibility globals
extern BITMAP *screen;

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
void position_mouse(int x, int y);
int install_joystick(int type);

// Graphics functions
int set_gfx_mode(int graphics_driver, int width, int height, int virtual_width, int virtual_height);
void set_uformat(int format);
void set_window_title(const char *title);
void set_display_switch_mode(int mode);

// Palette and color functions
typedef ALLEGRO_COLOR PALETTE[256];  // Allegro 4 used arrays of 256 colors
typedef ALLEGRO_COLOR RGB;

// Convert Allegro 4 color functions to Allegro 5
// In Allegro 5, colors are stored as ALLEGRO_COLOR objects, but many existing functions expect int indices
// For now, we'll use a simple mapping where the color index is used directly
#define makecol8(r, g, b) ((r << 16) | (g << 8) | b)  // Simple RGB to int mapping
#define makecol(r, g, b)  ((r << 16) | (g << 8) | b)  // Same as makecol8

// Palette functions (Allegro 4 compatibility)
void get_palette(PALETTE pal);
void set_palette(const PALETTE pal);
void set_palette_range(const RGB *p, int from, int to, int vsync);
void set_color(int index, RGB *color);
void get_color(int index, RGB *color);

// Predefined palettes
extern PALETTE black_palette;

// Helper functions for accessing bitmap properties
int get_bitmap_width(BITMAP *bmp);
int get_bitmap_height(BITMAP *bmp);

// Additional Allegro 4 compatibility functions
int stricmp(const char *s1, const char *s2);
char* strupr(char *str);
void replace_extension(char *dest, const char *src, const char *ext, int size);

// Bitmap functions
BITMAP *create_bitmap(int width, int height);
void destroy_bitmap(BITMAP *bitmap);
void clear_bitmap(BITMAP *bitmap);
int save_tga(const char *filename, BITMAP *bitmap, PALETTE *palette);

BITMAP *create_sub_bitmap(BITMAP *parent, int x, int y, int width, int height);
void blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int d_x, int d_y, int w, int h);
void stretch_blit(BITMAP *source, BITMAP *dest, int s_x, int s_y, int s_w, int s_h, int d_x, int d_y, int d_w, int d_h);
void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y);
void rotate_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle);
void rotate_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, fixed angle, fixed scale);

// Primitives
void circlefill(BITMAP *bmp, int x, int y, int radius, int color);
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void putpixel(BITMAP *bmp, int x, int y, int color);
int getpixel(BITMAP *bmp, int x, int y);
void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void circle(BITMAP *bmp, int x, int y, int radius, int color);
void ellipse(BITMAP *bmp, int x, int y, int rx, int ry, int color);
void floodfill(BITMAP *bmp, int x, int y, int color);
void ellipsefill(BITMAP *bmp, int x, int y, int rx, int ry, int color);
void do_circle(BITMAP *bmp, int x, int y, int radius, int color, void (*proc)(BITMAP *, int, int, int));

// Text handling
int text_length(ALLEGRO_FONT *f, const char *str);
int text_height(ALLEGRO_FONT *f);
void aatextout(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color);
void aatextout_right(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color);
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

// Boolean constants
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Additional Allegro 4 functions mapped to Allegro 5 equivalents
void draw_rle_sprite(BITMAP *bmp, RLE_SPRITE *sprite, int x, int y);
void stretch_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h);
void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y);

// For compatibility with direct member access, add getters


// Add missing Allegro 4 functions
#define ftofix(x) ((int)((x) * 65536.0))

// Audio functions
int play_sample(SAMPLE *sample, int vol, int pan, int freq, int loop);
void stop_sample(SAMPLE *sample);
void destroy_sample(SAMPLE *sample);
SAMPLE *load_sample(const char *filename);

// Additional Allegro 4 functions needed for compatibility
const char* get_filename(const char* path);
BITMAP* load_bitmap(const char* filename, RGB* pal);
void vline(BITMAP *bmp, int x, int y1, int y2, int color);
void masked_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);

// Additional Allegro 4 compatibility structures and functions
// File I/O structures (Allegro 4 compatibility)
typedef struct {
    ALLEGRO_FILE *file;
} PACKFILE;

typedef struct {
    ALLEGRO_COLOR table[256][256];  // Simplified RGB mapping table
} RGB_MAP;

typedef struct {
    ALLEGRO_COLOR table[256][256];  // Simplified color mapping table
} COLOR_MAP;

// Functions for color mapping
void create_rgb_table(RGB_MAP *table, RGB *pal, void (*callback)(int, int));
void create_trans_table(COLOR_MAP *table, RGB *pal, int r, int g, int b, void (*callback)(int, int));
void create_light_table(COLOR_MAP *table, RGB *pal, int r, int g, int b, void (*callback)(int, int));

// Global color mapping tables
extern RGB_MAP *rgb_map;
extern COLOR_MAP *color_map;

// Antialiasing functions (simplified)
void antialias_init(RGB *pal);
void antialias_exit();

// Bitmap locking functions (no-op in Allegro 5)
void acquire_bitmap(BITMAP *bmp);
void release_bitmap(BITMAP *bmp);

// Timer and input functions
void rest(unsigned int milliseconds);
int keyboard_needs_poll();
void poll_keyboard();
void poll_joystick();

// Include the implementation file (this is a common approach for template/inline functions)
//extern "C" {
//    #include "allegro5_wrapper.cpp"
//}

// Joystick types
#define JOY_TYPE_NONE 0
#define JOY_TYPE_4BUTTON 1
#define JOY_TYPE_AUTODETECT 2

// Character encoding (Allegro 4 compatibility)
#define U_ASCII 0
#define U_UTF8 1
void set_uformat(int format);
void packfile_password(const char* password);
int exists(const char* filename);

// Additional Allegro 4 functions needed for compatibility
void do_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color, void (*proc)(BITMAP *, int, int, int));
void hline(BITMAP *bmp, int x1, int y, int x2, int color);
int getr8(int color);
int getg8(int color);
int getb8(int color);
void textout_ex(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color, int bg);
void draw_sprite_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y);
void draw_lit_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int color);

// Font variable for compatibility (stub)
extern ALLEGRO_FONT *font;

// Common macros
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

// Additional Allegro 4 compatibility functions needed for Gravity Strike
void vline(BITMAP *bmp, int x, int y1, int y2, int color);
void masked_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void destroy_rle_sprite(RLE_SPRITE *spr);
RLE_SPRITE* get_rle_sprite(BITMAP *bmp);
void clear(BITMAP *bmp);
void clear_to_color(BITMAP *bmp, int color);
void rect(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void textout_centre(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color);
void textout_centre_ex(BITMAP *bmp, ALLEGRO_FONT *f, const char *str, int x, int y, int color, int bg);

// Input state
extern bool key[ALLEGRO_KEY_MAX];
extern int key_shifts;
extern int mouse_x, mouse_y;
extern int mouse_b;

// Key constants (Allegro 4 style)
#define KEY_A               ALLEGRO_KEY_A
#define KEY_B               ALLEGRO_KEY_B
#define KEY_C               ALLEGRO_KEY_C
#define KEY_D               ALLEGRO_KEY_D
#define KEY_E               ALLEGRO_KEY_E
#define KEY_F               ALLEGRO_KEY_F
#define KEY_G               ALLEGRO_KEY_G
#define KEY_H               ALLEGRO_KEY_H
#define KEY_I               ALLEGRO_KEY_I
#define KEY_J               ALLEGRO_KEY_J
#define KEY_K               ALLEGRO_KEY_K
#define KEY_L               ALLEGRO_KEY_L
#define KEY_M               ALLEGRO_KEY_M
#define KEY_N               ALLEGRO_KEY_N
#define KEY_O               ALLEGRO_KEY_O
#define KEY_P               ALLEGRO_KEY_P
#define KEY_Q               ALLEGRO_KEY_Q
#define KEY_R               ALLEGRO_KEY_R
#define KEY_S               ALLEGRO_KEY_S
#define KEY_T               ALLEGRO_KEY_T
#define KEY_U               ALLEGRO_KEY_U
#define KEY_V               ALLEGRO_KEY_V
#define KEY_W               ALLEGRO_KEY_W
#define KEY_X               ALLEGRO_KEY_X
#define KEY_Y               ALLEGRO_KEY_Y
#define KEY_Z               ALLEGRO_KEY_Z
#define KEY_0               ALLEGRO_KEY_0
#define KEY_1               ALLEGRO_KEY_1
#define KEY_2               ALLEGRO_KEY_2
#define KEY_3               ALLEGRO_KEY_3
#define KEY_4               ALLEGRO_KEY_4
#define KEY_5               ALLEGRO_KEY_5
#define KEY_6               ALLEGRO_KEY_6
#define KEY_7               ALLEGRO_KEY_7
#define KEY_8               ALLEGRO_KEY_8
#define KEY_9               ALLEGRO_KEY_9
#define KEY_0_PAD           ALLEGRO_KEY_PAD_0
#define KEY_1_PAD           ALLEGRO_KEY_PAD_1
#define KEY_2_PAD           ALLEGRO_KEY_PAD_2
#define KEY_3_PAD           ALLEGRO_KEY_PAD_3
#define KEY_4_PAD           ALLEGRO_KEY_PAD_4
#define KEY_5_PAD           ALLEGRO_KEY_PAD_5
#define KEY_6_PAD           ALLEGRO_KEY_PAD_6
#define KEY_7_PAD           ALLEGRO_KEY_PAD_7
#define KEY_8_PAD           ALLEGRO_KEY_PAD_8
#define KEY_9_PAD           ALLEGRO_KEY_PAD_9
#define KEY_F1              ALLEGRO_KEY_F1
#define KEY_F2              ALLEGRO_KEY_F2
#define KEY_F3              ALLEGRO_KEY_F3
#define KEY_F4              ALLEGRO_KEY_F4
#define KEY_F5              ALLEGRO_KEY_F5
#define KEY_F6              ALLEGRO_KEY_F6
#define KEY_F7              ALLEGRO_KEY_F7
#define KEY_F8              ALLEGRO_KEY_F8
#define KEY_F9              ALLEGRO_KEY_F9
#define KEY_F10             ALLEGRO_KEY_F10
#define KEY_F11             ALLEGRO_KEY_F11
#define KEY_F12             ALLEGRO_KEY_F12
#define KEY_ESC             ALLEGRO_KEY_ESCAPE
#define KEY_TILDE           ALLEGRO_KEY_TILDE
#define KEY_MINUS           ALLEGRO_KEY_MINUS
#define KEY_EQUALS          ALLEGRO_KEY_EQUALS
#define KEY_BACKSPACE       ALLEGRO_KEY_BACKSPACE
#define KEY_TAB             ALLEGRO_KEY_TAB
#define KEY_OPENBRACE       ALLEGRO_KEY_OPENBRACE
#define KEY_CLOSEBRACE      ALLEGRO_KEY_CLOSEBRACE
#define KEY_ENTER           ALLEGRO_KEY_ENTER
#define KEY_SEMICOLON       ALLEGRO_KEY_SEMICOLON
#define KEY_QUOTE           ALLEGRO_KEY_QUOTE
#define KEY_BACKSLASH       ALLEGRO_KEY_BACKSLASH
#define KEY_BACKSLASH2      ALLEGRO_KEY_BACKSLASH
#define KEY_COMMA           ALLEGRO_KEY_COMMA
#define KEY_STOP            ALLEGRO_KEY_FULLSTOP
#define KEY_SLASH           ALLEGRO_KEY_SLASH
#define KEY_SPACE           ALLEGRO_KEY_SPACE
#define KEY_INSERT          ALLEGRO_KEY_INSERT
#define KEY_DEL             ALLEGRO_KEY_DELETE
#define KEY_HOME            ALLEGRO_KEY_HOME
#define KEY_END             ALLEGRO_KEY_END
#define KEY_PGUP            ALLEGRO_KEY_PGUP
#define KEY_PGDN            ALLEGRO_KEY_PGDN
#define KEY_LEFT            ALLEGRO_KEY_LEFT
#define KEY_RIGHT           ALLEGRO_KEY_RIGHT
#define KEY_UP              ALLEGRO_KEY_UP
#define KEY_DOWN            ALLEGRO_KEY_DOWN
#define KEY_SLASH_PAD       ALLEGRO_KEY_PAD_SLASH
#define KEY_ASTERISK        ALLEGRO_KEY_PAD_ASTERISK
#define KEY_MINUS_PAD       ALLEGRO_KEY_PAD_MINUS
#define KEY_PLUS_PAD        ALLEGRO_KEY_PAD_PLUS
#define KEY_DEL_PAD         ALLEGRO_KEY_PAD_DELETE
#define KEY_ENTER_PAD       ALLEGRO_KEY_PAD_ENTER
#define KEY_PRTSCR          ALLEGRO_KEY_PRINTSCREEN
#define KEY_PAUSE           ALLEGRO_KEY_PAUSE
#define KEY_ABNT_C1         ALLEGRO_KEY_UNKNOWN
#define KEY_YEN             ALLEGRO_KEY_UNKNOWN
#define KEY_KANA            ALLEGRO_KEY_UNKNOWN
#define KEY_CONVERT         ALLEGRO_KEY_UNKNOWN
#define KEY_NOCONVERT       ALLEGRO_KEY_UNKNOWN
#define KEY_AT              ALLEGRO_KEY_UNKNOWN
#define KEY_CIRCUMFLEX      ALLEGRO_KEY_UNKNOWN
#define KEY_COLON           ALLEGRO_KEY_SEMICOLON
#define KEY_EQUALS_PAD      KEY_PLUS_PAD
#define KEY_BACKQUOTE       KEY_QUOTE
#define KEY_COMMAND         ALLEGRO_KEY_UNKNOWN
#define KEY_MODIFIERS       ALLEGRO_KEY_UNKNOWN
#define KEY_MAX             ALLEGRO_KEY_MAX
#define KEY_COLON2          ALLEGRO_KEY_UNKNOWN
#define KEY_KANJI           ALLEGRO_KEY_UNKNOWN
#define KEY_LSHIFT          ALLEGRO_KEY_LSHIFT
#define KEY_RSHIFT          ALLEGRO_KEY_RSHIFT
#define KEY_LCONTROL        ALLEGRO_KEY_LCTRL
#define KEY_RCONTROL        ALLEGRO_KEY_RCTRL
#define KEY_ALT             ALLEGRO_KEY_ALT
#define KEY_ALTGR           ALLEGRO_KEY_ALTGR
#define KEY_LWIN            ALLEGRO_KEY_LWIN
#define KEY_RWIN            ALLEGRO_KEY_RWIN
#define KEY_MENU            ALLEGRO_KEY_MENU
#define KEY_SCRLOCK         ALLEGRO_KEY_SCROLLLOCK
#define KEY_NUMLOCK         ALLEGRO_KEY_NUMLOCK
#define KEY_CAPSLOCK        ALLEGRO_KEY_CAPSLOCK

// Input functions
int keypressed();
int readkey();
void clear_keybuf();

// Joystick structure (matches expected Allegro 4 structure based on access pattern)
typedef struct {
    struct {
        struct {
            int d1, d2;  // digital axis values  
        } axis[2];  // x and y axis (axis[0] for x, axis[1] for y)
    } stick[1];  // Each joystick has 1 stick (array of 1 element)

    struct {
        int b;  // button pressed
    } button[6];  // Up to 6 buttons per joystick
} JOYSTICK;

extern JOYSTICK joy[2];

// Keyboard modifier constants (Allegro 4 style)
#define KB_SHIFT_FLAG       ALLEGRO_KEYMOD_SHIFT
#define KB_CTRL_FLAG        ALLEGRO_KEYMOD_CTRL
#define KB_ALT_FLAG         ALLEGRO_KEYMOD_ALT
#define KB_LWIN_FLAG        ALLEGRO_KEYMOD_LWIN
#define KB_RWIN_FLAG        ALLEGRO_KEYMOD_RWIN
#define KB_MENU_FLAG        ALLEGRO_KEYMOD_MENU
#define KB_SCROLOCK_FLAG    ALLEGRO_KEYMOD_SCROLLLOCK
#define KB_NUMLOCK_FLAG     ALLEGRO_KEYMOD_NUMLOCK
#define KB_CAPSLOCK_FLAG    ALLEGRO_KEYMOD_CAPSLOCK
// Flags that don't have direct Allegro 5 equivalents (set to 0)
#define KB_INALTSEQ_FLAG    0
#define KB_ACCENT1_FLAG     0
#define KB_ACCENT2_FLAG     0
#define KB_ACCENT3_FLAG     0
#define KB_ACCENT4_FLAG     0

// Error message
void allegro_message(const char *fmt, ...);

// File utility functions (Allegro 4 compatibility)
int file_size(const char *filename);

// File I/O functions (Allegro 4 compatibility)
PACKFILE *pack_fopen(const char *filename, const char *mode);
int pack_fread(void *p, int size, PACKFILE *f);
int pack_fwrite(const void *p, int size, PACKFILE *f);
int pack_fclose(PACKFILE *f);
void packfile_password(const char *password);

// Lua constants (Allegro 4 compatibility)
#define LUA_GLOBALSINDEX (-10002)  // Global table index in Lua 4/5

// Lua garbage collection functions (Allegro 4 compatibility)
int lua_getgcthreshold(lua_State *L);
int lua_getgccount(lua_State *L);
void lua_setgcthreshold(lua_State *L, int limit);


// Map functions with C linkage for compatibility with mappyal.h
extern "C" {
    int MapLoad(char *mapname);
    int MapLoadMAR(char *mname, int marlyr);
    void MapFreeMem(void);
    int MapGenerateYLookup(void);
    void MapInitAnims(void);
    int MapRelocate(void);
    int MapRelocate2(void);
    int MapGetBlockID(int blid, int usernum);
    BLKSTR * MapGetBlock(int x, int y);
    BLKSTR * MapGetBlockInPixels(int x, int y);
    void MapSetBlockInPixels(int x, int y, int blid);
    void MapSetBlock(int x, int y, int blid);
    int MapChangeLayer(int laynum);
    void MapDrawBG(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph);
    void MapDrawBGT(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph);
    void MapDrawFG(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph, int maprw);
    void MapDrawRow(BITMAP *bmp, int mapxo, int mapyo, int mapx, int mapy, int mapw, int maph, int maprw, void (*cellcall)(int cx, int cy, int dx, int dy));
    void MapCorrectColours(void);
    void MapSetPal8(void);
    int MapGetXOffset(int blid, int dir);
    int MapGetYOffset(int blid, int dir);
    BITMAP *MapMakeParallaxBitmap(BITMAP *bmp, int flags);
    void MapDrawParallax(BITMAP *bmp, BITMAP *pbmp, int xo, int yo, int x, int y, int w, int h);
} // extern "C"

#endif // ALLEGRO5_WRAPPER_H
