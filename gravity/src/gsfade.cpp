/*
 *   GRAVITY STRIKE -- gsfade.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsfade.h"
#include "gsglob.h"
#include "gserror.h"

gsFade::gsFade()
{
  fade_speed = fade_pos = fade_opos = 0;

  fade_count = fade_count_to = 0;
  fade_type = 0;
  fade_from_nr = 0;
  fade_to_nr = 255;

  for (int n=0; n < 256; n++)
  {
    // For Allegro 5 compatibility, using 0-255 range for RGB values
    white_palette[n] = al_map_rgb(255, 255, 255);
    red_palette[n] = al_map_rgb(255, 0, 0);
  }

  get_palette(cur_palette);
}

gsFade::~gsFade()
{
}

void gsFade::initiate(FADE_TYPES type, float speed, int count_to, int count_from)
{
  fade_type = type;
  fade_speed = speed;
  fade_pos = fade_opos = 0;
  fade_count = count_from;
  fade_count_to = count_to;
  fade_from_nr = 0; fade_to_nr = 255;

  fade_active = 1;
}

void gsFade::update()
{
  if (fade_count < fade_count_to)
  {
//    PALETTE tempp;
//    get_palette(tempp);
    switch (fade_type)
    {
      case FADE_TO_BLACK :
               fade_from_range((RGB*)cur_palette,(RGB*)black_palette,fade_from_nr,fade_to_nr,fade_speed);
               break;
      case FADE_TO_MAIN :
               fade_from_range((RGB*)cur_palette,(RGB*)globals->mainpal,fade_from_nr,fade_to_nr,fade_speed);
               break;
      case FADE_TO_WHITE :
               fade_from_range((RGB*)cur_palette,(RGB*)white_palette,fade_from_nr,fade_to_nr,fade_speed);
               break;
      case FADE_TO_RED :
               fade_from_range((RGB*)cur_palette,(RGB*)red_palette,fade_from_nr,fade_to_nr,fade_speed);
               break;
    } // switch fade_type

    fade_count++;
  }
  else
  {
    fade_active = 0;
    fade_count = fade_count_to = 0;
    fade_speed = 0;
    fade_from_nr = 0; fade_to_nr = 255;
    fade_pos = fade_opos = 0;
  }
}

void gsFade::fade_from_range(RGB *temp, const RGB *t, int from, int to, float speed)
{
  // In Allegro 5, we don't manipulate palettes directly. Instead, we use overlay effects.
  // This function now serves as a placeholder for fade calculations.
  // The actual fading effect would be implemented using alpha blending over the screen.
  (void)temp; (void)t; (void)from; (void)to; (void)speed; // avoid unused parameter warnings
  
  if (fade_pos > fade_opos) {
    fade_opos = fade_pos;
    // In Allegro 5, the actual fade effect would be implemented differently
    // using draw functions with appropriate alpha values
  }
  
  fade_pos += speed;
}

