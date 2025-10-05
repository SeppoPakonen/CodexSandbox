/*
 *   GRAVITY STRIKE -- menu/mpixels.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MPIXELS_
#define _MPIXELS_

#include "../gspixel.h"


class mPixelRain : public gsPixelRain
{
  public:
    // redefining collision function
    void check_collisions(BITMAP *m);

};


#endif
