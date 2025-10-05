/*
 *   GRAVITY STRIKE -- gsmap.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSWORLD_
#define _GSWORLD_

#include "mappyal.h"
#include "contrib/astar.h"

enum MAP_STYLE {
  MAP_GRASS,
  MAP_STONE
};

typedef struct PathNode {
  int x;
  int y;
} PathNode;

class gsMap
{
  private:
    // variables
    int   scroll_x;                                // "real" x-scrolling (0 = left)
    int   scroll_y;                                // "real" y-scrolling (0 = up)
    int   scroll_to_x;                             // scroll to x-coordinate
    int   scroll_to_y;                             // scroll to y-coordinate
    float scroll_speed_x;                          // scrolling speed, x direction
    float scroll_speed_y;                          // scrolling speed, x direction
    float fscroll_x;                               // internal fine x-scrolling
    float fscroll_y;                               // internal fine y-scrolling
    int   map_x;                                   // internal map starting x-coordinate
    int   map_y;                                   // internal map starting y-coordinate
    int   redraw_needed;                           // internal: must the map be redrawn?

    int   map_width;                               // width of map in tiles
    int   map_height;                              // height of map in tiles

    int   min_scroll_x, max_scroll_x;
    int   min_scroll_y, max_scroll_y;

    int   second_map;                              // right splitscreen?
    char  style[31];                               // style of map (fmp file)

    BITMAP *scr;                                   // virtual screen
    BITMAP *parallax;                              // parallax test bitmap

    AStar  *pathfinder;                            // A* algo for pathfinding

    // functions
    int load(char *tileset, char *map);
    void update();

  public:
    // public members
    class gsOsd *osd;                              // osd object

    // constructor, destructor
    gsMap();
    gsMap(BITMAP *v, char *t, char *m);
    ~gsMap();

    // get/put
    int get_scroll_x()                             { return scroll_x; }
    int get_scroll_y()                             { return scroll_y; }
    float get_scroll_speed_x()                     { return scroll_speed_x; }
    float get_scroll_speed_y()                     { return scroll_speed_y; }
    int get_scroll_to_x()                          { return scroll_to_x; }
    int get_scroll_to_y()                          { return scroll_to_y; }
    int get_map_x()                                { return map_x; }
    int get_map_y()                                { return map_y; }
    float get_fscroll_x()                          { return fscroll_x; }
    float get_fscroll_y()                          { return fscroll_y; }
    int is_redraw_needed()                         { return redraw_needed; }
    void request_redraw()                          { redraw_needed = TRUE; }
    BITMAP *get_vscreen()                          { return scr; }
    char *get_style()                              { return style; }

    void restrict_scroll_x(int min, int max)       { min_scroll_x = min; max_scroll_x = max; }
    void restrict_scroll_y(int min, int max)       { min_scroll_y = min; max_scroll_y = max; }

    // functions
    void set(BITMAP *v, char *t, char *m);
    void scroll_to(int x, int y, float s=0);
    void scroll_to(class gsPlayer *);
    void scroll_to(class gsPlayer *, float s);
    void scroll();
    void redraw();

    int  find_path(int startx, int starty, int targetx, int targety);
    void store_path(PathNode *pnodes);

    int  is_tile(int x, int y);
    int  is_pixel(int x, int y);
    int  is_on_screen(int x, int y, int w, int h);
    int  is_on_map(int x, int y, int w, int h);
    int  get_pixel(int x, int y);
    
    void draw_rectangle(int col, int x1, int y1, int x2, int y2);
    void draw_filled_rectangle(int col, int x1, int y1, int x2, int y2);
    void draw_pixel(int col, int px, int py);
    void draw_circle(int col, int px, int py, int r);
    void draw_filled_circle(int col, int px, int py, int r);
    void draw_ellipse(int col, int px, int py, int rx, int ry);
    void draw_filled_ellipse(int col, int px, int py, int rx, int ry);
    void draw_line(int col, int x1, int y1, int x2, int y2);
    void draw_text(int fnum, char *t, int col, int px, int py);
    void draw_sprite(int imgnum, int px, int py, int frame=0);
    void fill_area(int col, int px, int py);
};

extern gsMap *playmap[];

#endif

