/*
 *   GRAVITY STRIKE -- gsosd.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsosd.h"
#include "allegro5_wrapper.h"
#include "gsmap.h"
#include "gserror.h"
#include "gsglob.h"
#include "gshelper.h"
#include "gssound.h"
#include <string.h>
#include <ctype.h>

OsdBitmap::OsdBitmap(gsMap *pscr, int psig, int px, int py, int pw, int ph, int pmode)
{
  sig = psig;
  x = px;
  y = py;
  w = pw;
  h = ph;
  mode = pmode;
  scr = pscr;
  to_bitmap = NULL;
  active = 1;
  visible = 1;
  remtime = -1;
  dont_save_back = 0;
  last_clear_color = 0;

  bitmap = create_bitmap(w, h);
  back = create_bitmap(w, h);
  ::clear(bitmap);
  ::clear(back);

  memset(&typewriter, 0, sizeof(typewriter_text));

  first_anim = last_anim = NULL;
}

OsdBitmap::OsdBitmap(BITMAP *pscr, int psig, int px, int py, int pw, int ph, int pmode)
{
  sig = psig;
  x = px;
  y = py;
  w = pw;
  h = ph;
  mode = pmode;
  scr = NULL;
  to_bitmap = pscr;
  active = 1;
  visible = 1;
  remtime = -1;
  dont_save_back = 1;
  last_clear_color = 0;

  bitmap = create_bitmap(w, h);
  back = create_bitmap(w, h);
  ::clear(bitmap);
  ::clear(back);

  memset(&typewriter, 0, sizeof(typewriter_text));

  first_anim = last_anim = NULL;
}

OsdBitmap::~OsdBitmap()
{
  if (bitmap) destroy_bitmap(bitmap);
  if (back) destroy_bitmap(back);
}

void OsdBitmap::draw()
{
  if (active && visible)
  switch (mode)
  {
    case OSDBIT_SCREEN :
         if (scr->is_on_screen(x+scr->get_scroll_x(),y+scr->get_scroll_y(),w,h))
         masked_blit(bitmap, scr->get_vscreen(),
                     0, 0,
                     (int)x + (int)scr->get_fscroll_x(),
                     (int)y + (int)scr->get_fscroll_y(),
                     w, h);
         break;

    case OSDBIT_MAP :
         if (scr->is_on_map(x,y,w,h))
         masked_blit(bitmap, scr->get_vscreen(),
                     0, 0,
                     (int)x - scr->get_map_x(),
                     (int)y - scr->get_map_y(),
                     w, h);
         break;
    case OSDBIT_PANEL :
         blit(bitmap, to_bitmap,
                     0, 0,
                     (int)x,
                     (int)y,
                     w, h);
         break;
  } // mode
}

void OsdBitmap::save_background()
{
  if (active && visible)
  switch (mode)
  {
    case OSDBIT_SCREEN :
         if (scr->is_on_screen(x+scr->get_scroll_x(),y+scr->get_scroll_y(),w,h))
         blit(scr->get_vscreen(), back,
              (int)x + (int)scr->get_fscroll_x(),
              (int)y + (int)scr->get_fscroll_y(),
              0, 0,
              w, h);
         break;

    case OSDBIT_MAP :
         if (scr->is_on_map(x,y,w,h))
         blit(scr->get_vscreen(), back,
              (int)x - scr->get_map_x(),
              (int)y - scr->get_map_y(),
              0, 0,
              w, h);
         break;
  } // mode
}

void OsdBitmap::restore_background()
{
  if (active && visible)
  switch (mode)
  {
    case OSDBIT_SCREEN :
         if (scr->is_on_screen(x+scr->get_scroll_x(),y+scr->get_scroll_y(),w,h))
         blit(back, scr->get_vscreen(),
              0, 0,
              (int)x + (int)scr->get_fscroll_x(),
              (int)y + (int)scr->get_fscroll_y(),
              w, h);
         break;

    case OSDBIT_MAP :
         if (scr->is_on_map(x,y,w,h))
         blit(back, scr->get_vscreen(),
              0, 0,
              (int)x - scr->get_map_x(),
              (int)y - scr->get_map_y(),
              w, h);
         break;
  } // mode
}

int OsdBitmap::update()
{
  if (typewriter.active && typewriter.remove_when_player_moves)
    if (player[0]->get_thrust() ||
        (player[0]->get_extra() == EXTRA_AFTERBURNER && player[0]->get_extra_action()) )
      { typewriter.active = 0; remtime = 1; }

  if (remtime != -1)
    if (--remtime == 0)
    {
      clear();
      set_active(0);
    }

  update_images();
  draw_images();

  update_typewriter_text();
  return 0;
}

void OsdBitmap::clear(int col)
{
  if (col == 0) col = last_clear_color;
  clear_to_color(bitmap, col);
  last_clear_color = col;
}

void OsdBitmap::draw_rectangle(int col, int x1, int y1, int x2, int y2)
{
  int mycol = (col == 0) ? globals->col_white : col;
  if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0)
    rect(bitmap, 0, 0, w-1, h-1, mycol);
  else
    rect(bitmap, x1, y1, x2, y2, mycol);
}

void OsdBitmap::draw_text(int fnum, char *t, int col, int px, int py, int mode)
{
  FONT *f = (FONT*)globals->fontdat[fnum].dat;
  int colnum = (col == 0) ? globals->col_white : col;

  typewriter.active = 0;

  if (fnum == FONT_MINIFONT) strupr(t);

  if (mode == 0)
    mytextout_left(bitmap, f, t, px, px+w, py, colnum, 0);
  else
    mytextout_center(bitmap, f, t, px, px+w, py, colnum, 0);
}

void OsdBitmap::draw_colored_text(int fnum, char *t, int col, int px, int py, int mode)
{
  FONT *f = (FONT*)globals->fontdat[fnum].dat;
  int colnum = (col == 0) ? globals->col_white : col;

  typewriter.active = 0;

  if (fnum == FONT_MINIFONT) strupr(t);

  int curcol = col;
  int len = strlen(t);
  int minus = 0;
  int curx = 0;

  for (int n=0; n < len; n++)
  {
    char mystr[2];
    mystr[0] = t[n];
    mystr[1] = '\0';
    int tl = text_length(f, mystr);
    if (t[n] == ' ') tl = 3;

    if (t[n] == '[' && n+2 <= len && t[n+2] == ']')
    {
      switch (t[n+1])
      {
        case 'W' : curcol = globals->col_white; break;
        case 'G' : curcol = globals->col_green; break;
        case 'Y' : curcol = globals->col_yellow; break;
        case 'B' : curcol = globals->col_black; break;
        case 'O' : curcol = globals->col_orange; break;
        case 'R' : curcol = globals->col_red; break;
        case '/' : curcol = col; break;
      }
      n += 2;
      minus += 3;
    }
    else
    {
      if (mode == 1) curcol = col;
      aatextout(bitmap, f, mystr, px+curx, py, curcol);
      curx += tl;
    }
  }
}

void OsdBitmap::draw_filled_rectangle(int col, int x1, int y1, int x2, int y2)
{
  rectfill(bitmap, x1, y1, x2, y2, col);
}

void OsdBitmap::draw_line(int col, int x1, int y1, int x2, int y2)
{
  line(bitmap, x1, y1, x2, y2, col);
}

void OsdBitmap::draw_circle(int col, int px, int py, int r)
{
  circle(bitmap, px, py, r, col);
}

void OsdBitmap::draw_filled_circle(int col, int px, int py, int r)
{
  circlefill(bitmap, px, py, r, col);
}

void OsdBitmap::draw_ellipse(int col, int px, int py, int rx, int ry)
{
  ellipse(bitmap, px, py, rx, ry, col);
}

void OsdBitmap::draw_filled_ellipse(int col, int px, int py, int rx, int ry)
{
  ellipsefill(bitmap, px, py, rx, ry, col);
}

void OsdBitmap::draw_pixel(int col, int px, int py)
{
  putpixel(bitmap, px, py, col);
}

void OsdBitmap::fill_area(int col, int px, int py)
{
  floodfill(bitmap, px, py, col);
}

void get_next_word(char *from, char *to, char div, int max)
{
  for (int n=0; n <= (signed)strlen(from); n++)
  {
    if (from[n] == div || from[n] == '\0')
    {
      if (n >= max) n = max;
      strncpy(to, from, n+1);
      to[n+1] = '\0';
      return;
    }
  }

  strcpy(to, "");
}

void get_text_without_tokens(char *from, char *to, int max)
{
  while (*from != '\0')
  {
    while (*from == '[')
    {
      do from++;
      while (*from != ']' && *from != '\0');
      from++;
    }
    *to = *from;
    to++;
    from++;
  }
  *to = '\0';
}

void OsdBitmap::update_typewriter_text()
{
  if (!typewriter.active) return;

  if (typewriter.pause) { typewriter.pause--; return; }

  if (typewriter.curnextcharwait < typewriter.speed)
  {
    typewriter.curnextcharwait++;
    return;
  }
  else typewriter.curnextcharwait = 0;

  int textlen = strlen(typewriter.text);
  if (typewriter.curpos >= textlen)
  {
    typewriter.active = 0;
    return;
  }

  set_timeout(120);

  // check for tokens
//    errors->log(2, "origtext is now", typewriter.origtext);
//    errors->log(2, "text pos is now", typewriter.text+typewriter.curpos);
  if (*typewriter.origtextptr == '[')
  {
    char mytoken[20] = "";
    get_next_word(typewriter.origtextptr, mytoken, ']', 20);
//    errors->log(2, "token found", mytoken);
    typewriter.origtextptr += strlen(mytoken);

    // colors
    if      (!strcmp(mytoken, "[c:yellow]")) { typewriter.attr.color = globals->col_yellow; }
    else if (!strcmp(mytoken, "[c:green]")) { typewriter.attr.color = globals->col_green; }
    else if (!strcmp(mytoken, "[c:white]")) { typewriter.attr.color = globals->col_white; }
    else if (!strcmp(mytoken, "[c:orange]")) { typewriter.attr.color = globals->col_orange; }
    else if (!strcmp(mytoken, "[c:black]")) { typewriter.attr.color = globals->col_black; }
    // fonts
    else if (!strcmp(mytoken, "[f:impact10]")) { typewriter.attr.font = FONT_IMPACT10; typewriter.curline_length = 0; }
    else if (!strcmp(mytoken, "[f:impact14]")) { typewriter.attr.font = FONT_IMPACT14; typewriter.curline_length = 0; }
    else if (!strcmp(mytoken, "[f:minifont]")) { typewriter.attr.font = FONT_MINIFONT; typewriter.curline_length = 0; }
    // special
    else if (!strcmp(mytoken, "[u]")) { typewriter.attr.underlined = TRUE; }
    else if (!strcmp(mytoken, "[/u]")) { typewriter.attr.underlined = FALSE; }
    else if (!strcmp(mytoken, "[center]")) { typewriter.attr.center = TRUE;  typewriter.curline_length = 0; }
    else if (!strcmp(mytoken, "[left]")) { typewriter.attr.center = FALSE;  typewriter.curline_length = 0; }
    else if (!strcmp(mytoken, "[br]")) { typewriter.curline_length = 0; }
    else if (!strcmp(mytoken, "[clr]")) // clear display
    {
      rectfill(bitmap, 1, typewriter.y,
                       get_width()-2, get_height()-typewriter.y-2,
                       last_clear_color);
    }
    else if (!strcmp(mytoken, "[ret]")) // go to beginning
    {
      typewriter.cur_x = 1;
      typewriter.cur_y = typewriter.y;
      typewriter.curline_length = 0;
    }
    else if (!strncmp(mytoken, "[w:", 3)) // pause
    {
      char mytime[10];
      memset(mytime, 0, sizeof(mytime));
      strncpy(mytime, mytoken+3, strlen(mytoken)-4);
      typewriter.pause = atoi(mytime);
      set_timeout(typewriter.pause+60);
      return;
    }
    else if (!strncmp(mytoken, "[s:", 3)) // change speed
    {
      char mytime[10];
      memset(mytime, 0, sizeof(mytime));
      strncpy(mytime, mytoken+3, strlen(mytoken)-4);
      typewriter.speed = atoi(mytime);
//      errors->log(2, "new typewriter speed", mytime, typewriter.speed);
      return;
    }
  }

  FONT *myfont = (FONT*)globals->fontdat[typewriter.attr.font].dat;
  char chrstr[2];
  chrstr[0] = typewriter.text[typewriter.curpos];
  chrstr[1] = '\0';
  int char_length = text_length(myfont, chrstr);

  // check for line breaks
  if (typewriter.curline_length == 0)
  {
    char thisline[200] = "";
    char nextword[50] = "";
    char *mytextpos = typewriter.text + typewriter.curpos;

    while (1)
    {
      get_next_word(mytextpos, nextword, ' ', 50);
      mytextpos += strlen(nextword);
//      errors->log(2, "nextword", nextword);

      if (strlen(nextword) == 0 || text_length(myfont, thisline) + text_length(myfont, nextword) > get_width())
        break;

      strcat(thisline, nextword);
    }

    typewriter.curline_length = strlen(thisline);
    if (typewriter.attr.center)
      typewriter.cur_x = (get_width()-4)/2 - text_length(myfont, thisline)/2 + 4;
    else
      typewriter.cur_x = 5;

    if (typewriter.curpos > 0) typewriter.cur_y += text_height(myfont);

    if (typewriter.cur_y > get_height()-text_height(myfont)-1)
    {
      typewriter.cur_y -= text_height(myfont);

      // kill upper line and "scroll down"
      blit(bitmap, bitmap,
           1, typewriter.y + text_height(myfont),
           1, typewriter.y,
           get_width(), get_height()-typewriter.y-text_height(myfont)-1);

      if (mode == OSDBIT_PANEL)
        rectfill(bitmap, 1, typewriter.cur_y,
                         get_width()-2, typewriter.cur_y+text_height(myfont),
                         last_clear_color);
    }
  }

  if (*typewriter.origtextptr != '[')
  {
    if (typewriter.attr.font == FONT_MINIFONT) typewriter.text[typewriter.curpos] = toupper(typewriter.text[typewriter.curpos]);

    aacharout(bitmap, myfont, typewriter.text[typewriter.curpos],
              typewriter.cur_x, typewriter.cur_y,
              typewriter.attr.color);

    if (typewriter.attr.underlined)
      hline(bitmap, typewriter.cur_x, typewriter.cur_y + text_height(myfont) - 1, typewriter.cur_x + char_length, typewriter.attr.color);

    char tmpstr[2]; tmpstr[0] = typewriter.text[typewriter.curpos]; tmpstr[1] = '\0';
    char_length = text_length(myfont, tmpstr);
    typewriter.cur_x += char_length;
    typewriter.curline_length--;
    typewriter.curpos++;
    typewriter.origtextptr++;

    if (mode != OSDBIT_PANEL) gsound->play_sound(SOUND_OSD_TICK);
  }

}

void OsdBitmap::draw_typewriter_text(char *t, int starty, int speed, int rect, int remove)
{
  if (!t || strlen(t) == 0) return;

  strncpy(typewriter.origtext, t, 4000);
  typewriter.origtextptr = typewriter.origtext;
  get_text_without_tokens(typewriter.origtext, typewriter.text, 4000);
//  errors->log(2, "mit tokens", typewriter.origtext);
//  errors->log(2, "ohne token", typewriter.text);
  typewriter.y = starty;
  typewriter.speed = speed;
  typewriter.curpos = 0;
  typewriter.pause = 0;
  typewriter.curnextcharwait = 0;
  typewriter.cur_y = starty;
  typewriter.curline_length = 0;
  typewriter.attr.color = globals->col_yellow;
  typewriter.attr.font = FONT_IMPACT10;
  typewriter.attr.underlined = FALSE;
  typewriter.attr.center = FALSE;

  typewriter.active = 1;
  typewriter.remove_when_player_moves = remove;

  set_timeout(60);
  if (mode != OSDBIT_PANEL) clear();
  if (rect) draw_rectangle();
}

gsOsd::gsOsd(gsMap *m)
{
  mymap = m;
  to_bitmap = NULL;
  first_osdbit = last_osdbit = NULL;
  count = 0;

  OsdBitmap *t = add(1000, globals->playscreen_width/2-125, globals->playscreen_height/2-125, 250, 100, OSDBIT_SCREEN);
  t->set_active(0);
}

gsOsd::gsOsd(BITMAP *m)
{
  mymap = NULL;
  to_bitmap = m;
  first_osdbit = last_osdbit = NULL;
  count = 0;
}

gsOsd::~gsOsd()
{
  OsdBitmap *temp, *next_osdbit;
  temp = first_osdbit;

  while (temp != NULL)
  {
    next_osdbit = temp->get_next();
    delete temp;
    temp = next_osdbit;
  }
}

OsdBitmap *gsOsd::add(int sig, int x, int y, int w, int h, int mode)
{
  OsdBitmap *temp_osdbit;

  if (mode == OSDBIT_PANEL)
    temp_osdbit = new OsdBitmap(to_bitmap, sig, x, y, w, h, mode);
  else
    temp_osdbit = new OsdBitmap(mymap, sig, x, y, w, h, mode);

  if (temp_osdbit != NULL)
  {
    if (first_osdbit==NULL)
    {
      last_osdbit = first_osdbit = temp_osdbit;
      first_osdbit->set_next(NULL); first_osdbit->set_prev(NULL);
      temp_osdbit->set_next(NULL); temp_osdbit->set_prev(NULL);
    }
    else
    {
      last_osdbit->set_next(temp_osdbit);
      temp_osdbit->set_prev(last_osdbit);
      last_osdbit = temp_osdbit;
      temp_osdbit->set_next(NULL);
    }
  } // temp_osdbit != NULL

  count++;

  return temp_osdbit;
}

OsdBitmap *gsOsd::add_label(int sig, int x, int y, int mode, int fontnum, char *t, int col, int rect)
{
  int mycol = (col==0) ? globals->col_white : col;
  int len = text_length((FONT*)globals->fontdat[fontnum].dat, t);
  int height = text_height((FONT*)globals->fontdat[fontnum].dat);
  OsdBitmap *myosd;

  if (rect == 0)
  {
    myosd = add(sig, x, y, len+1, height, mode);
    myosd->draw_text(fontnum, t, mycol);
  }
  else
  {
    myosd = add(sig, x, y, len+3, height+3, mode);
    myosd->draw_rectangle();
    myosd->draw_text(fontnum, t, mycol, 2, 2);
  }

  return myosd;
}

void gsOsd::remove(OsdBitmap *temp)
{
  OsdBitmap *next_osdbit, *prev_osdbit;

  if (temp != NULL)
  {
    prev_osdbit = temp->get_prev();
    next_osdbit = temp->get_next();

    if (prev_osdbit==NULL) first_osdbit = next_osdbit;
    else prev_osdbit->set_next(next_osdbit);

    if (next_osdbit==NULL) last_osdbit = prev_osdbit;
    else next_osdbit->set_prev(prev_osdbit);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsOsd::draw()
{
  OsdBitmap *temp = first_osdbit;

  while (temp != NULL)
  {
    temp->draw();
    temp = temp->get_next();
  }
}

void gsOsd::save_background()
{
  OsdBitmap *temp = first_osdbit;

  while (temp != NULL)
  {
    if (!temp->get_dont_save_back()) temp->save_background();
    temp = temp->get_next();
  }
}

void gsOsd::restore_background()
{
  OsdBitmap *temp = first_osdbit;

  while (temp != NULL)
  {
    if (!temp->get_dont_save_back()) temp->restore_background();
    temp = temp->get_next();
  }
}

void gsOsd::update()
{
  OsdBitmap *temp = first_osdbit;

  while (temp != NULL)
  {
    temp->update();
    temp = temp->get_next();
  }
}

OsdBitmap *gsOsd::get_osdbit(int sig)
{
  OsdBitmap *temp = first_osdbit;

  while (temp != NULL)
  {
    if (temp->get_sig() == sig) return temp;
    temp = temp->get_next();
  }

  return NULL;
}

// ANIMATION stuff
UserImageAnim *OsdBitmap::add_image(int inum, int px, int py, int pmaxframet, int loop)
{
  UserImageAnim *TempAnim;

  TempAnim = new UserImageAnim(this, inum, px, py, pmaxframet, loop);

  if (TempAnim != NULL)
  {
    if (first_anim==NULL)
    {
      last_anim = first_anim = TempAnim;
      first_anim->set_next(NULL); first_anim->set_prev(NULL);
      TempAnim->set_next(NULL); TempAnim->set_prev(NULL);
    }
    else
    {
      last_anim->set_next(TempAnim);
      TempAnim->set_prev(last_anim);
      last_anim = TempAnim;
      TempAnim->set_next(NULL);
    }

    return TempAnim;
  } // TempAnim != NULL

  return NULL;
}

void OsdBitmap::draw_images()
{
  UserImageAnim *temp = first_anim;

  while (temp != NULL)
  {
    temp->draw();
    temp = temp->get_next();
  }
}

void OsdBitmap::update_images()
{
  UserImageAnim *temp = first_anim, *next_anim;

  while (temp != NULL)
  {
    next_anim = temp->get_next();
    if (temp->update()) remove_image(temp);
    temp = next_anim;
  } // while temp != NULL
}

void OsdBitmap::remove_image(UserImageAnim *temp)
{
  UserImageAnim *next_anim, *prev_anim;

  if (temp != NULL)
  {
    prev_anim = temp->get_prev();
    next_anim = temp->get_next();

    if (prev_anim==NULL) first_anim = next_anim;
    else prev_anim->set_next(next_anim);

    if (next_anim==NULL) last_anim = prev_anim;
    else next_anim->set_prev(prev_anim);

    delete temp;
    temp = NULL;
  }
}

UserImageAnim::UserImageAnim(OsdBitmap *osdbit, int inum, int px, int py, int pmaxframet, int lp)
{
  pbitmap = osdbit;
  imgnum = inum;
  x = px;
  y = py;
  maxframet = pmaxframet;
  startframe = 0;
  curframe = 0;
  curframet = 0;
  loop = lp;

  frames = images->get_image(imgnum);
  maxframe = frames->get_framecount() - 1;
}

UserImageAnim::~UserImageAnim()
{
  rectfill(pbitmap->get_bitmap(), x, y,
           x + frames->get_rle_frame(0)->w,
           y + frames->get_rle_frame(0)->h,
           pbitmap->get_last_clear_color() );
  frames = NULL;
  startframe = curframe = curframet = maxframe = maxframet = 0;
}

void UserImageAnim::set_image(int n)
{
  frames = images->get_image(n);
  maxframe = frames->get_framecount() - 1;
}

int UserImageAnim::update()
{
  if (maxframe)
    {
    curframet++;
    if (curframet >= maxframet)
    {
      curframet = 0;
      curframe++;
      if (curframe > maxframe)
      {
        if (!loop)
          return 1;
        else
        {
          curframe = startframe;
          curframet = 0;
        }
      }
    }
  }
  return 0;
}

void UserImageAnim::draw()
{
  draw_rle_sprite(pbitmap->get_bitmap(), frames->get_rle_frame(curframe), x, y);
}

