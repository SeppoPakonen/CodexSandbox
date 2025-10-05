/*
 *   GRAVITY STRIKE -- gsmap.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegttf.h"
#include "gsmap.h"
#include "gserror.h"
#include "gsglob.h"
#include "gsplayer.h"
#include "gseffect.h"
#include "gsbase.h"
#include "gsosd.h"
#include "gshelper.h"
#include <math.h>

gsMap::gsMap(BITMAP *v, char *t, char *m)
{
  pathfinder = NULL;
  set(v, t, m);
}

gsMap::gsMap()
{
  pathfinder = NULL;
}

gsMap::~gsMap()
{
  if (osd) delete(osd);

  MapFreeMem();

  delete pathfinder;
}

void gsMap::set(BITMAP *v, char *t, char *m)
{
  scroll_x = scroll_y = 0;
  fscroll_x = fscroll_y = 0;
  map_x = map_y = 0;
  scroll_to_x = scroll_to_y = -1;
  redraw_needed = FALSE;

  map_width = map_height = 100;

  min_scroll_x = 0;
  max_scroll_x = map_width * 48;
  min_scroll_y = 0;
  max_scroll_y = map_height * 48;

  scr = v;

  if (strlen(t) > 0 && strlen(m) > 0)
  {
    errors->log(2, "loading map", t, m);
    load(t,m);
    second_map = FALSE;
  }
  else
    second_map = TRUE;

  osd = new gsOsd(this);

  /*
  BITMAP *mybit = create_bitmap(96, 96); 
  mybit = load_bitmap("./dat/paratest.tga", NULL);
  parallax = MapMakeParallaxBitmap(mybit, 0);
  */
  
//  update();
}

int gsMap::load(char *tileset, char *map)
{
  int err=0;

  // load tileset
  errors->log(2, "loading map", "MapLoad", get_filename(tileset));
  if ((err = MapLoad(tileset)) != 0)
  {
    char newtileset[150];
    // if error, load tileset from second location (dat dir)
    sprintf(newtileset, "%s%s", globals->data_dir,
                                get_filename(tileset));
/*
    if ((err = MapLoadVRAM(newtileset)) != 0)
    {
      errors->log(1, "map load", "can't use VRAM for map load, trying normal...", get_filename(tileset));
      if ((err = MapLoad(newtileset)) != 0)
      {
        errors->log(0, "map load", "ERROR: can't load tileset!", get_filename(tileset));
        errors->er_exit("ERROR: Can't load tileset!", 10);
      }
    }
*/
      if ((err = MapLoad(newtileset)) != 0)
      {
        errors->log(0, "map load", "ERROR: can't load tileset!", get_filename(tileset));
        errors->er_exit("ERROR: Can't load tileset!", 10);
      }

  }

  strncpy(style, get_filename(tileset), 30);
  errors->log(2, "Map", "loaded tileset");

  // load map
  errors->log(2, "loading map", "MapLoadMAR", map);
  if (MapLoadMAR(map, 0))
  {
    errors->log(0, "map load", "ERROR: can't load map!");
    errors->er_exit("ERROR: Can't load map!", 11);
  }
  errors->log(2, "Map", "loaded map");

  MapGenerateYLookup();
  MapInitAnims();

  // init pathfinding
  if (pathfinder) delete(pathfinder);
  pathfinder = new AStar();
  pathfinder->create_from_playmap(this);

  return 0;
}

void gsMap::update()
{
  int w,h,ph,pw;

  pw = globals->playscreen_width;
  ph = globals->playscreen_height;

  w = (map_x + pw + globals->plus_x > map_width*globals->tile_w) ?
      map_width*globals->tile_w - map_x :
      pw+globals->plus_x;

  h = (map_y + ph + globals->plus_y > map_height*globals->tile_w) ?
      map_height*globals->tile_h - map_y :
      ph+globals->plus_y;

  MapDraw8BG(scr, map_x, map_y, 0, 0, w, h);
  bases->kill_landscape(this);
  //effects->craters->reset(this);
  effects->craters->draw(this);
}


void gsMap::scroll_to(int x, int y, float s)
{
  scroll_to_x = x;
  scroll_to_y = y;

  if (s > 0)
  {
    float xab = abs(scroll_to_x - get_scroll_x());
    float yab = abs(scroll_to_y - get_scroll_y());
    float dist = sqrt( (xab*xab) + (yab*yab) );
    if (dist==0) dist=1;

    scroll_speed_x = float( s * (xab/dist) );
    scroll_speed_y = float( s * (yab/dist) );
  }
  else
  {
    scroll_speed_x = scroll_speed_y = 0;
  }

  if (scroll_to_x < 0) scroll_to_x = 0;
  if (scroll_to_y < 0) scroll_to_y = 0;

  if (scroll_to_x >= map_width * globals->tile_w  - globals->playscreen_width) scroll_to_x = map_width * globals->tile_w - globals->playscreen_width;
  if (scroll_to_y >= map_height * globals->tile_h  - globals->playscreen_height) scroll_to_y = map_height * globals->tile_h - globals->playscreen_height;

  scroll();
}

void gsMap::scroll_to(gsPlayer *pl)
{
  scroll_to_x = (int)pl->get_x() - globals->playscreen_width/2;
  scroll_to_y = (int)pl->get_y() - globals->playscreen_height/2;

  if (scroll_to_x < 0) scroll_to_x = 0;
  if (scroll_to_y < 0) scroll_to_y = 0;

  if (scroll_to_x > map_width * globals->tile_w - globals->playscreen_width) scroll_to_x = map_width * globals->tile_w - globals->playscreen_width;
  if (scroll_to_y > map_height * globals->tile_h - globals->playscreen_height) scroll_to_y = map_height * globals->tile_h - globals->playscreen_height;

  scroll_speed_x = scroll_speed_y = 0;

  scroll();
}

void gsMap::scroll_to(gsPlayer *pl, float s)
{
  scroll_to_x = (int)pl->get_x() - globals->playscreen_width/2;
  scroll_to_y = (int)pl->get_y() - globals->playscreen_height/2;

  if (s > 0)
  {
    float xab = abs(scroll_to_x - get_scroll_x());
    float yab = abs(scroll_to_y - get_scroll_y());
    float dist = sqrt( (xab*xab) + (yab*yab) );
    if (dist==0) dist=1;

    scroll_speed_x = float( xab / s );
    scroll_speed_y = float( yab / s );
  }
  else
  {
    scroll_speed_x = scroll_speed_y = 0;
  }

  if (scroll_to_x < 0) scroll_to_x = 0;
  if (scroll_to_y < 0) scroll_to_y = 0;

  if (scroll_to_x >= map_width * globals->tile_w  - globals->playscreen_width) scroll_to_x = map_width * globals->tile_w - globals->playscreen_width;
  if (scroll_to_y >= map_height * globals->tile_h  - globals->playscreen_height) scroll_to_y = map_height * globals->tile_h - globals->playscreen_height;

  scroll();
}


void gsMap::scroll()
{
  int w,h,px,py;
  float cx,cy,tmp;

  if (scroll_to_x >= 0 && scroll_to_y >= 0)
  {
    if (scroll_speed_x+scroll_speed_y > 0)
    {
      cx = fscroll_x + map_x;
      cy = fscroll_y + map_y;
  
      if (cx < scroll_to_x-scroll_speed_x)
        cx += scroll_speed_x;
      else if (cx < scroll_to_x)
        cx = scroll_to_x;
  
      if (cy < scroll_to_y-scroll_speed_y)
        cy += scroll_speed_y;
      else if (cy < scroll_to_y)
        cy = scroll_to_y;
  
      if (cx > scroll_to_x+scroll_speed_x)
        cx -= scroll_speed_x;
      else if (cx > scroll_to_x)
        cx = scroll_to_x;
  
      if (cy > scroll_to_y+scroll_speed_y)
        cy -= scroll_speed_y;
      else if (cy > scroll_to_y)
        cy = scroll_to_y;
    }
    else
    {
      cx = scroll_to_x;
      cy = scroll_to_y;
    }

    // get scroll_x/y and map_x/y out of cx/y
    tmp = map_x;
    map_x = ((int)(cx / globals->plus_x)) * globals->plus_x;
    if (map_x != tmp) request_redraw();
    fscroll_x = (int)cx % globals->plus_x;
    fscroll_x += cx - int(cx);

    tmp = map_y;
    map_y = ((int)(cy / globals->plus_y)) * globals->plus_y;
    if (map_y != tmp) request_redraw();
    fscroll_y = (int)cy % globals->plus_y;
    fscroll_y += cy - int(cy);

    // abort when target reached
    if (cx == scroll_to_x && cy == scroll_to_y)
    {
      scroll_to_x = scroll_to_y = -1;
      scroll_speed_x = 0;
      scroll_speed_y = 0;
    }
  }

  // map "hard" scrolling
  w = globals->playscreen_width;
  h = globals->playscreen_height;

  px = globals->plus_x;
  py = globals->plus_y;

  if (fscroll_x >= px)
  {
    map_x += px;
    request_redraw();
    fscroll_x = 0;
  }
  else if (fscroll_x < 0)
  {
    if (map_x >= px)
    {
      map_x -= px;
      if (map_x < 0) map_x = 0;
      request_redraw();
      fscroll_x = px;
    }
  }

  if (fscroll_y >= py)
  {
    map_y += py;
    request_redraw();
    fscroll_y = 0;
  }
  else if (fscroll_y < 0)
  {
    if (map_y >= py)
    {
      map_y -= py;
      if (map_y < 0) map_y = 0;
      request_redraw();
      fscroll_y = py;
    }
  }

  scroll_x = map_x + (int)fscroll_x;
  scroll_y = map_y + (int)fscroll_y;

  if (scroll_x > max_scroll_x) { scroll_x = max_scroll_x; fscroll_x = scroll_x - map_x; }
  if (scroll_y > max_scroll_y) { scroll_y = max_scroll_y; fscroll_y = scroll_y - map_y; }
  if (scroll_x < min_scroll_x) { scroll_x = min_scroll_x; fscroll_x = scroll_x - map_x; }
  if (scroll_y < min_scroll_y) { scroll_y = min_scroll_y; fscroll_y = scroll_y - map_y; }

  if (redraw_needed) redraw();

  // craters
//  if (globals->game_time % 5 == 0)
//    effects->craters->draw(this);
}

void gsMap::redraw()
{
  update();
  redraw_needed = FALSE;
}

int gsMap::is_on_screen(int x, int y, int w, int h)
{
  int ph = gpanel[0]->get_active() ? globals->playscreen_height : globals->playscreen_full_height;
  
  if (x + w > get_fscroll_x() + get_map_x() &&
      x < get_fscroll_x() + get_map_x() + globals->playscreen_width + w &&
      y + h > get_fscroll_y() + get_map_y() &&
      y < get_fscroll_y() + get_map_y() + ph + h)
  {
    return 1;
  }

  return 0;
}

int gsMap::is_on_map(int x, int y, int w, int h)
{
  int ph = gpanel[0]->get_active() ? globals->playscreen_height : globals->playscreen_full_height;

  if (x >= get_map_x() &&
      x + w <= get_map_x() + globals->plus_x + globals->playscreen_width &&
      y >= get_map_y() &&
      y + h <= get_map_y() + globals->plus_y + ph)
  {
    return 1;
  }

  return 0;
}

int gsMap::is_pixel(int x, int y)
{
  if (is_on_map(x, y, 1, 1))
    if (getpixel(scr, x-map_x, y-map_y)) return 1;
  return 0;
}

int gsMap::get_pixel(int x, int y)
{
  if (is_on_map(x, y, 1, 1))
    return getpixel(scr, x-map_x, y-map_y);
  return 0;
}

int gsMap::is_tile(int x, int y)
{
  BLKSTR *block = NULL;
  block = MapGetBlockInPixels(x, y);
  if (block)
    return block->tl;
  else
    return 0;
}

void gsMap::draw_text(int fnum, char *t, int col, int px, int py)
{
  FONT *f = (FONT*)globals->fontdat[fnum].dat;
  if (fnum == FONT_MINIFONT) strupr(t);

  aatextout(scr, f, t, px-map_x, py-map_y, col);
}

void gsMap::draw_rectangle(int col, int x1, int y1, int x2, int y2)
{
  rect(scr, x1-map_x, y1-map_y, x2-map_x, y2-map_y, col);
}

void gsMap::draw_filled_rectangle(int col, int x1, int y1, int x2, int y2)
{
  rectfill(scr, x1-map_x, y1-map_y, x2-map_x, y2-map_y, col);
}

void gsMap::draw_line(int col, int x1, int y1, int x2, int y2)
{
  line(scr, x1-map_x, y1-map_y, x2-map_x, y2-map_y, col);
}

void gsMap::draw_circle(int col, int px, int py, int r)
{
  circle(scr, px-map_x, py-map_y, r, col);
}

void gsMap::draw_filled_circle(int col, int px, int py, int r)
{
  circlefill(scr, px-map_x, py-map_y, r, col);
}

void gsMap::draw_ellipse(int col, int px, int py, int rx, int ry)
{
  ellipse(scr, px-map_x, py-map_y, rx, ry, col);
}

void gsMap::draw_filled_ellipse(int col, int px, int py, int rx, int ry)
{
  ellipsefill(scr, px-map_x, py-map_y, rx, ry, col);
}

void gsMap::draw_pixel(int col, int px, int py)
{
  putpixel(scr, px-map_x, py-map_y, col);
}

void gsMap::draw_sprite(int imgnum, int px, int py, int frame)
{
  Image *frames;
  
  frames = images->get_image(imgnum);
  if (frame >= frames->get_framecount()) frame = frames->get_framecount()-1;
  else if (frame < 0) frame = 0;
  
  draw_rle_sprite(scr, frames->get_rle_frame(frame), px-map_x, py-map_y);
}

void gsMap::fill_area(int col, int px, int py)
{
  floodfill(scr, px-map_x, py-map_y, col);
}


int gsMap::find_path(int startx, int starty, int targetx, int targety)
{
  if (!pathfinder) return 0;

  // check if targetx/targety is on a walkable tile
  BLKSTR *block = NULL;
  int tx = targetx/globals->tile_w;
  int ty = targety/globals->tile_h;
  if (tx < 0) tx = 0;
  if (ty < 0) ty = 0;
  if (tx > 99) tx = 99;
  if (ty > 99) ty = 99;
  int otx = tx;
  int oty = ty;
  
  bool unwalkable = false;

  block = MapGetBlock(tx, ty);
  if (block && block->tl) 
  { 
    tx -= 1; ty -= 1; // check top left
    block = MapGetBlock(tx, ty);
    if (block && block->tl)
    {
      tx += 1; // check top
      block = MapGetBlock(tx, ty);
      if (block && block->tl)
      {
        tx += 1; // check top right
        block = MapGetBlock(tx, ty);
        if (block && block->tl)
        {
          ty += 1; // check right
          block = MapGetBlock(tx, ty);
          if (block && block->tl)
          {
            ty += 1; // check bottom right
            block = MapGetBlock(tx, ty);
            if (block && block->tl)
            {
              tx -= 1; // check bottom
              block = MapGetBlock(tx, ty);
              if (block && block->tl)
              {
                tx -= 1; // check bottom left
                block = MapGetBlock(tx, ty);
                if (block && block->tl)
                {
                  ty -= 1; // check left
                  block = MapGetBlock(tx, ty);
                  if (block && block->tl)
                  {
                    unwalkable = true;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  targetx = tx*globals->tile_w + globals->tile_w/2;
  targety = ty*globals->tile_w + globals->tile_h/2;

  if (tx != otx || ty != oty) 
  {
    //errors->log(2, "adjusted target x (tx/otx)", tx, otx);
    //errors->log(2, "adjusted target y (ty/oty)", ty, oty);
  }

  if (unwalkable)
  {
    //errors->log(2, "target is unwalkable", targetx, targety);
  }


  int length = 0;

  if (!unwalkable)
  {
    pathfinder->InitializePathfinder();
    int ret = pathfinder->FindPath(startx, starty, targetx, targety);
    //  errors->log(2, "path finding", "return value", ret);
    if (ret == 1)
      length = pathfinder->get_path_length();
    else
      ;//errors->log(2, "target is unreachable", targetx, targety);
  }
 
  return length;
}
  
void gsMap::store_path(PathNode *pnodes)
{
  int length = pathfinder->get_path_length();
  
//  errors->log(2, "path finding", "length", length);
  if (length > 0)
  {
    for (int n=1; n <= length; n++)
    {
      pnodes[n-1].x = pathfinder->ReadPathX(n);
      pnodes[n-1].y = pathfinder->ReadPathY(n);
    }
  }

  pathfinder->EndPathfinder();
}
