/*
 *   GRAVITY STRIKE -- gsradar.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSRADAR_
#define _GSRADAR_

class gsRadar
{
  private:
    class gsMap    *map;                           // bound to map ...
    class gsPlayer *pplayer;                       // bound to player ...

    BITMAP         *minimap;                       // bitmap with minimap
    BITMAP         *radarbit;                      // bitmap with radar image
    float          zoom;                           // zoom factor

    int            mini_width;                     // width of minimap
    int            mini_height;                    // height of minimap
    int            width;                          // width of radarbit
    int            height;                         // height of radarbit

    int            min_zoom;                       // minimum zoom
    int            max_zoom;                       // maximum zoom

  public:
    // Constr. / Destr.
    gsRadar(class gsMap *, class gsPlayer *, int rw, int rh);
    ~gsRadar();

    // get / put
    BITMAP         *get_radarbit()                 { return radarbit; }
    float          get_zoom()                      { return zoom; }
    void           set_zoom(float n);

    // functions
    void create_minimap();
    void update();
};

#endif
