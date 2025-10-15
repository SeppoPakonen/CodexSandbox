/*
 *   GRAVITY STRIKE -- menu/controls/mtextfld.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"
#include "mtextfld.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gssound.h"
#include "../gsmd.h"

void mTextField::create(int px, int py, int dx, int dy, int pw, int ph, int fpos)
{
  mControl::create(px, py, dx, dy);
  w = pw;
  h = ph;
  field_pos = fpos;
  maxchars = 0;
  strcpy(text, "");
  strcpy(caption, "");
  enter = false;
}

void mTextField::set_text(char *n)
{
  if (strlen(caption) == 0)
    strncpy(caption, n, 50);
  else
    strncpy(text, n, 150);
}

void mTextField::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = color2;
    int tc = globals->col_green;
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    int tlen = text_length(f, text);

    rect(v, (int)x + field_pos, (int)y, (int)x+w, (int)y+h, c);

    aatextout(v, f, caption, (int)x, (int)y+1, c);

    BITMAP *tmp = create_bitmap(tlen+2, h);
    clear_bitmap(tmp);

    // draw text
    aatextout(tmp, f, text, 0, 0, tc);

    // draw a (blinking) cursor when active
    if (flags & MC_ACTIVE)
    {
      vline(tmp, tlen+1, 1, h-3, color3);
    }

    //masked_blit(tmp, v, 0, 0, (int)x+field_pos+2, (int)y+1, w-field_pos-5, tmp->h);
    int sx = tmp->w - w + field_pos + 5;
    if (sx < 0) sx = 0;
    masked_blit(tmp, v, sx, 0, (int)x+field_pos+2, (int)y+1, w-field_pos-5, tmp->h);

    destroy_bitmap(tmp);

    clearlist->add(v, x, y, w, h);
  }
}

int mTextField::update()
{
  if (flags & MC_ACTIVE)
  {
    // process keyboard input
    int k = 0, kascii = 0;
  
    if (keypressed())
    {
      gsound->menu_keypressed();
      k = readkey();
      kascii = k & 0xff;
      k = k >> 8;
      if (k == 0) k = readkey();
    }
  
    if (k)
    {
      if (k == KEY_BACKSPACE)
      {
        if (strlen(text) > 0) text[strlen(text)-1] = '\0';
      }
      else if (k == KEY_ENTER && strlen(text) > 0)
      {
        enter = true;
      }
      else
      if ( (k >= KEY_A && k <= KEY_9) || k == KEY_SPACE ||
           k == KEY_STOP || k == KEY_COMMA || k == KEY_COLON ||
           k == KEY_MINUS || k == KEY_PLUS_PAD || k == KEY_EQUALS ||
           k == KEY_QUOTE || k == KEY_SLASH || k == KEY_ASTERISK ||
           k == KEY_OPENBRACE || k == KEY_CLOSEBRACE)
      {
        if (strlen(text) < (unsigned)maxchars-1)
        {
          int l = strlen(text);
          text[l] = kascii;
          text[l+1] = '\0';
        }
      }

      clear_keybuf();
    }
  } // if active

  return (mControl::update());
}


