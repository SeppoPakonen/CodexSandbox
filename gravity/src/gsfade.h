/*
 *   GRAVITY STRIKE -- gsfade.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSFADE_
#define _GSFADE_

enum FADE_TYPES {
  FADE_TO_MAIN,
  FADE_TO_BLACK,
  FADE_TO_WHITE,
  FADE_TO_RED
};

class gsFade
{
  private:
    PALETTE white_palette;
    PALETTE red_palette;
    PALETTE cur_palette;

    float fade_speed;
    float fade_pos;
    float fade_opos;

    int   fade_count;
    int   fade_count_to;
    int   fade_type;
    int   fade_from_nr;
    int   fade_to_nr;

    int   fade_active;
//    int   fade_out_active;

    // functions
    void  fade_from_range(RGB *temp, const RGB *t, int from, int to, float speed);

  public:
    // constr. / destr.
    gsFade();
    ~gsFade();

    // get/set
    int   get_fade_active()                        { return fade_active; }

    // functions
    void  update();
    void  initiate(FADE_TYPES type, float speed, int count_to, int count_from=0);
};

extern gsFade *fade;

#endif
