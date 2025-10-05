/*
 *   GRAVITY STRIKE -- gseffect.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

/*
     Class gsEffect

     Wrapper Class for effect classes (explosions, pixels, craters, bullets)

*/

#include "allegro.h"

#ifndef _GSEFFECT_
#define _GSEFFECT_

#include "gspixel.h"
#include "gsexplo.h"
#include "gscrater.h"
#include "gsbullet.h"

class gsEffect
{
  public:
    gsPixelRain *pixels;
    gsExplosion *explosions;
    gsCrater    *craters;
    gsBullet    *bullets;

  public:
    // Funktionen
    gsEffect();
    ~gsEffect();

    void init_pixelrain();
    void init_explosions();
    void init_craters();
    void init_bullets();
};

extern gsEffect *effects;

#endif

