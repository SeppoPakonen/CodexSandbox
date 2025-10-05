/*
 *   GRAVITY STRIKE -- gseffect.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gseffect.h"
#include "gserror.h"
#include "gsmap.h"


gsEffect::gsEffect()
{
  init_pixelrain();
  init_explosions();
  init_craters();
  init_bullets();
}

gsEffect::~gsEffect()
{
  if (pixels)     delete(pixels);
  if (explosions) delete(explosions);
  if (craters)    delete(craters);
  if (bullets)    delete(bullets);
}

void gsEffect::init_pixelrain()
{
  errors->log(2, "effect", "init pixels");
  pixels = new gsPixelRain();
}

void gsEffect::init_explosions()
{
  errors->log(2, "effect", "init explosions");
  explosions = new gsExplosion(playmap[0], playmap[1]);
}

void gsEffect::init_craters()
{
  errors->log(2, "effect", "craters");
  craters = new gsCrater;
}

void gsEffect::init_bullets()
{
  errors->log(2, "effect", "init bullets");
  bullets = new gsBullet;
}

