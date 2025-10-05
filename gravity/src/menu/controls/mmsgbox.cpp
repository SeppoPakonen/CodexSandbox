/*
 *   GRAVITY STRIKE -- menu/controls/mmsgbox.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mmsgbox.h"
#include "allegttf.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"

mMessageBox::~mMessageBox()
{
  for (int n=0; n < 3; n++)
    if (msg_button[n]) delete(msg_button[n]);
}

void mMessageBox::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  strcpy(text, "");

  for (int n=0; n < 3; n++) msg_button[n] = new mSmallButton();

  msg_button[0]->create(px+3, py+h-18, px+3, py-18, pw/3-5, 14);
  msg_button[1]->create(px+pw/2-pw/6+2, py+h-18, px+pw/2-pw/6+2, py-18, pw/3-5, 14);
  msg_button[2]->create(px+pw-pw/3+2, py+h-18, px+pw-pw/3+2, py-18, pw/3-5, 14);

  msg_button[0]->set_text(language->get_string(T_MENU_YES));
  msg_button[1]->set_text(language->get_string(T_MENU_OK));
  msg_button[2]->set_text(language->get_string(T_MENU_NO));

  mode = 0;
}

void mMessageBox::set_int(int n)
{
  mode = n;

  switch(n)
  {
    case MSGBOX_OK :
         msg_button[0]->set_flag(MC_INACTIVE);
         msg_button[0]->set_flag(MC_INVISIBLE);
         msg_button[1]->del_flag(MC_INACTIVE);
         msg_button[1]->del_flag(MC_INVISIBLE);
         msg_button[2]->set_flag(MC_INACTIVE);
         msg_button[2]->set_flag(MC_INVISIBLE);
         break;
    case MSGBOX_YESNO :
         msg_button[0]->del_flag(MC_INACTIVE);
         msg_button[0]->del_flag(MC_INVISIBLE);
         msg_button[1]->set_flag(MC_INACTIVE);
         msg_button[1]->set_flag(MC_INVISIBLE);
         msg_button[2]->del_flag(MC_INACTIVE);
         msg_button[2]->del_flag(MC_INVISIBLE);
         break;
  }
}

void mMessageBox::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }

    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;

    int tlen, theight;
    if (strlen(text) > 0)
    {
      tlen = text_length(f, text);
      theight = text_height(f);
    }

    rectfill(v, (int)x+1, (int)y+1, (int)x+w-1, (int)y+h-1, globals->col_black);
    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, color2);

    if (strlen(text) > 0)
    {
      text_mode(-1);
      mytextout_center(v, f, text, (int)x+5, (int)x+w-10, (int)y+20, globals->col_yellow);
    }

    for (int n=0; n < 3; n++) msg_button[n]->draw(v);

    clearlist->add(v, x, y, w, h);
  }
}

int mMessageBox::update()
{
  for (int n=0; n < 3; n++) msg_button[n]->update();

  return (mControl::update());
}

void mMessageBox::check()
{
  for (int n=0; n < 3; n++) msg_button[n]->check();

  mControl::check();
}

void mMessageBox::set_dest_x(int n)
{
  mControl::set_dest_x(n);

  msg_button[0]->set_dest_x(n+3);
  msg_button[1]->set_dest_x(n+w/2-w/6+2);
  msg_button[2]->set_dest_x(n+w-w/3+2);
}

void mMessageBox::set_dest_y(int n)
{
  mControl::set_dest_y(n);

  msg_button[0]->set_dest_y(n+h-16);
  msg_button[1]->set_dest_y(n+h-16);
  msg_button[2]->set_dest_y(n+h-16);
}

