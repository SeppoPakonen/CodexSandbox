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
    white_palette[n].r = 63;
    white_palette[n].g = 63;
    white_palette[n].b = 63;

    red_palette[n].r = 63;
    red_palette[n].g = 0;
    red_palette[n].b = 0;
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
  int n;

  if (fade_pos > fade_opos)
  {
    for (n=from; n <= to; n++)
    {
      if ( temp[n].r < t[n].r) temp[n].r++;
      else if (temp[n].r > t[n].r) temp[n].r--;

      if (temp[n].g < t[n].g) temp[n].g++;
      else if (temp[n].g > t[n].g) temp[n].g--;

      if (temp[n].b < t[n].b) temp[n].b++;
      else if (temp[n].b > t[n].b) temp[n].b--;

//      set_color(n, &temp[n]);
    } // for n

    fade_opos = fade_pos;

    set_palette_range(temp, from, to, 0);
  }

  fade_pos += speed;
}

