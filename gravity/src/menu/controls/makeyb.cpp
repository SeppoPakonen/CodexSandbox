/*
 *   GRAVITY STRIKE -- menu/controls/makeyb.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "makeyb.h"
#include "allegttf.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../mpixels.h"
#include "../gsmd.h"
#include "mtypes.h"
#include <ctype.h>
#include <stdio.h>

extern mPixelRain *mpixels;

void mKeyboardOption::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  strcpy(text, "");
  dispkey = 0;
}

void mKeyboardOption::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
    char keytext[30];

    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }
    if (flags & MC_PUSHED) { c = color3; }

    FONT *f = (FONT*)globals->fontdat[FONT_ICE].dat;
    FONT *f2 = (FONT*)globals->fontdat[FONT_KEYPUNCH].dat;

    int tlen, theight = 1, tlen2 = 1, theight2 = 1;
    // normal text
    if (strlen(text) > 0)
    {
      tlen = text_length(f, text);
      theight = text_height(f);
    }

    // key text
    strcpy(keytext, "");
    // normal letter or number
    if (dispkey < 37)
    {
      keytext[0] = toupper(scancode_to_ascii(dispkey));
      keytext[1] = '\0';
    }
    // other keys except modifiers
    else if (dispkey >= 37 && dispkey < 94)
    {
      strncpy(keytext, language->get_string(TEXT_STRINGS(T_KEY_0_PAD+dispkey-KEY_0_PAD)), 20);
    }
    // key modifiers
    else if (dispkey >= 103 && dispkey < 1000)
    {
      strncpy(keytext, language->get_string(TEXT_STRINGS(T_KEY_LSHIFT+dispkey-KEY_MODIFIERS)), 20);
    }
    else if (dispkey >= 1000)
    {
      strncpy(keytext, language->get_string(TEXT_STRINGS(T_JOY0_UP+dispkey-1000)), 20);
    }

//      strncpy(keytext, language->get_string(TEXT_STRINGS(T_KEY_LSHIFT+108-103)), 20);
    
//    sprintf(keytext, "%d", dispkey);

    if (strlen(keytext) > 0)
    {
      tlen2 = text_length(f2, keytext);
      theight2 = text_height(f2);
    }

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

    if (strlen(text) > 0)
    {
      text_mode(-1);
      aatextout(v, f, text, (int)x+5, (int)y+h/2-theight/2, tc);
      aatextout(v, f2, keytext, (int)x+w-tlen2-5, (int)y+h/2-theight/2+5, tc);

      if (flags & MC_PUSHED)
      {
        mpixels->add(10,x+w - 5 - fxrand()%tlen2,
                        fxrand()%theight2 + y+2,
                        0, 0, 1-(fxrand() % 3), 1-(fxrand() % 3),
                        50, globals->tbl_green, 10, 1000, 10);
      }
    }

    clearlist->add(v, x, y, w, h);
  }
}


