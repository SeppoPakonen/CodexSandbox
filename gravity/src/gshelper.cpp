/*
 *   GRAVITY STRIKE -- gshelper.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"
#include "allegro5_wrapper.h"
#ifdef ALLEGRO_WINDOWS
  #include "winalleg.h"
#endif
#include "gshelper.h"
#include "gsclear.h"
#include "gsplayer.h"
#include "gsmap.h"
#include "gsglob.h"
#include "gserror.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

// Function to check if a file exists
bool file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

#define ROUND(x) floor((x) + ((x) >= 0 ? 0.5 : -0.5))

extern float sinus[];
extern float cosinus[];

float get_time_to_collision(float x, float y, float dx, float dy, float r1, float x2, float y2, float dx2, float dy2, float r2)
{
  float a,b,c,radical,twoa,root1,root2;
  float temp1,temp2,temp3;

  temp1 = dx2 - dx;
  temp2 = dy2 - dy;
  a = temp1*temp1 + temp2*temp2;

  if (a == 0.0)
  {
    if (temp1 == 0.0 && temp2 == 0.0) a = 1;
    else return VERY_LONG_TIME;
  }

  temp1 = 2.0 * (x2 - x) * (dx2 - dx);
  temp2 = 2.0 * (y2 - y) * (dy2 - dy);
  b = temp1 + temp2;
  
  temp1 = x2 - x;
  temp2 = y2 - y;
  temp3 = r1 + r2;
  c = temp1*temp1 + temp2*temp2 - temp3*temp3;

  radical = b*b - (4.0*a*c);
  if (radical < 0.0) return VERY_LONG_TIME;

  radical = sqrt(radical);
  twoa = 2.0 * a;
  root1 = (-b + radical) / (twoa);
  root2 = (-b - radical) / (twoa);

  if (root1 > root2)
  {
    float temp;
    temp = root1;
    root1 = root2;
    root2 = temp;
  }

  if (root1 < 0 && root2 > 0) return VERY_LONG_TIME*2; // start overlapped
  if (root1 > 1.0 && root2 > 1.0) return VERY_LONG_TIME;

  if (root2 >= 0.0)
  {
    if (root1 >= 0.0) return root1;
    else return VERY_LONG_TIME;
  }
  else return VERY_LONG_TIME;
}

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void swap(float *a, float *b)
{
  float temp = *a;
  *a = *b;
  *b = temp;
}

int radius_player_collision(gsPlayer *pl1, gsPlayer *pl2)
{
  int r1 = MAX(pl1->get_width(), pl1->get_height()) / 2;
  int r2 = MAX(pl2->get_width(), pl2->get_height()) / 2;

  r1 += 2; r2 += 2;

  float ix1,iy1,ix2,iy2;
  float jx1,jy1,jx2,jy2;
  float cx1,cy1,cx2,cy2;
  float vx1,vy1,vx2,vy2;

  cx1 = pl1->get_x() + r1; cy1 = pl1->get_y() + r1;
  cx2 = pl2->get_x() + r2; cy2 = pl2->get_y() + r2;
  vx1 = pl1->get_xspd(); vy1 = pl1->get_yspd();
  vx2 = pl2->get_xspd(); vy2 = pl2->get_yspd();

  ix1 = cx1 - r1;
  iy1 = cy1 - r1;
  ix2 = cx1 + r1 + vx1;
  iy2 = cy1 + r1 + vy1;

  jx1 = cx2 - r2;
  jy1 = cy2 - r2;
  jx2 = cx2 + r2 + vx2;
  jy2 = cy2 + r2 + vy2;

  // sort/swap
  if (ix2 < ix1) swap(&ix1,&ix2);
  if (iy2 < iy1) swap(&iy1,&iy2);
  if (jx2 < jx1) swap(&jx1,&jx2);
  if (jy2 < jy1) swap(&jy1,&jy2);

//  rect((pl1->get_map())->get_vscreen(), ix1, iy1, ix2, iy2, globals->col_yellow);
//  rect((pl1->get_map())->get_vscreen(), ix1+1, iy1+1, ix2-1, iy2-1, 0);

  if (!((ix1 > jx2) || (ix2 < jx1) || (iy1 > jy2) || (iy2 < jy1)))
    return 1;

  return 0;
}

#include "gsmap.h"

int collision(float ax, float ay, int aw, int ah, float bx, float by, int bw, int bh, float axs, float ays, float bxs, float bys, 
              float *newx1, float *newy1, float *newx2, float *newy2, int usenew)
{
  float axopos = ax - axs;
  float ayopos = ay - ays;
  float bxopos = bx - bxs;
  float byopos = by - bys;

  int anx = fast_ftol(axopos < ax ? axopos : ax);
  int any = fast_ftol(ayopos < ay ? ayopos : ay);
  int bnx = fast_ftol(bxopos < bx ? bxopos : bx);
  int bny = fast_ftol(byopos < by ? byopos : by);

  int dist1 = fast_ftol(distance(axopos, ayopos, ax, ay));
  int dist2 = fast_ftol(distance(bxopos, byopos, bx, by));
 
  if (fabs(axs+ays+bxs+bys) > 0 &&
      (dist1 > 1 || dist2 > 1) && 
      (anx+fabs(axs)+aw > bnx && anx < bnx+fabs(bxs)+bw && any+fabs(ays)+ah > bny && any < bny+fabs(bys)+bh) ) // only modified collisions
  {
    float axplus; 
    float ayplus; 
    float bxplus; 
    float byplus;

    if (dist1 > 0)
    {
      axplus = axs / dist1;
      ayplus = ays / dist1;
    }
    else
    {
      dist1 = 1;
      axplus = 0;
      ayplus = 0;
    }  

    if (dist2 > 0)
    {
      bxplus = bxs / dist2;
      byplus = bys / dist2;
    }
    else
    {
      dist2 = 1;
      bxplus = 0;
      byplus = 0;
    }  
  
    int calcs = 0;
    for (int i=1; i <= dist1; i++)
      for (int j=1; j <= dist2; j++)
      {
        float max = axopos + i * axplus;
        float may = ayopos + i * ayplus;
        float mbx = bxopos + j * bxplus;
        float mby = byopos + j * byplus;
        calcs++;
       
        if (!(max > mbx+bw || mbx >  max+aw || may > mby+bh || mby > may+ah))
        {
//          putpixel(playmap[0]->get_vscreen(), mx1, my1, globals->col_yellow);
//          putpixel(playmap[0]->get_vscreen(), mx2, my2, globals->col_orange);

          if (newx1) *newx1 = max;
          if (newy1) *newy1 = may;
          if (newx2) *newx2 = mbx;
          if (newy2) *newy2 = mby;
          return 1;
        }
      }
//     errors->log(2, "calculations", dist1, dist2);

  }
  else
  {
    if (ax > bx+bw || bx > ax+aw || ay > by+bh || by > ay+ah)
      return 0;
    else
    {
      if (newx1) *newx1 = ax;
      if (newy1) *newy1 = ay;
      if (newx2) *newx2 = bx;
      if (newy2) *newy2 = by;
      return 1;
    }  
  }  

  return 0;
}

int collision(float ax, float ay, int aw, int ah, float bx, float by, int bw, int bh)
{
  if (ax > bx+bw || bx > ax+aw || ay > by+bh || by > ay+ah)
      return 0;
  else
      return 1;
}


float distance(float x1, float y1, float x2, float y2)
{
  //return float(sqrt(pow(x2-x1,2.0) + pow(y2-y1,2.0))); slow!
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

int strtrimr(char *pszStr)
{
      int   i, j;                               /* Local counters */
      j = i = strlen(pszStr) - 1; /* Calculate the length of the string */
      while (isspace(pszStr[i]) && (i >= 0))
            pszStr[ i-- ] = '\0';               /*- Replace blank with '\0' */

      return(j - i);                            /* Return no of replacements */
}


int strtriml(char *pszStr)
{
      int   i = 0, j;                                 /* Local counters */
      j = strlen(pszStr) - 1; /* Calculate the length of the string */
      while (isspace(pszStr[i]) && (i <= j))
            i++;                          /*- Count no of leading blanks */
      if (0 < i)                          /* IF leading blanks are found */
            strcpy(pszStr, &pszStr[i]);   /*- Shift string to the left */

      return(i);                          /* Return no of replacements */
}

int strtrim(char *pszStr)
{
      int   iBlank;
      iBlank  = strtrimr(pszStr);               /* Remove trailing blanks */
      iBlank += strtriml(pszStr);               /* Remove leading blanks */
      return(iBlank);
}

void make_screenshot()
{
  BITMAP *bmp;
  int dump = 0;
  char text[20];
  PALETTE pal;

  // packfile_password(NULL);  // Not available in Allegro 5

  do
  {
    dump++;
    sprintf(text, "gs%06d.tga", dump);
  } while (file_exists(text));  // Check if file exists using standard C++ function

  sprintf(text, "gs%06d.tga", dump);

  bmp = create_sub_bitmap(screen, 0, 0, screen->w, screen->h);
  save_tga(text,bmp,NULL);  // Use NULL for palette since Allegro 5 handles it differently
  destroy_bitmap(bmp);
}

char *ftoa(char *tmp, float n)
{
  sprintf(tmp,"%.2f",n);
  return tmp;
}

char *ftoa(char *tmp, char *t, float n)
{
  sprintf(tmp,"%s: %.2f", t, n);
  return tmp;
}

char *mitoa(char *tmp, int n)
{
  sprintf(tmp,"%d",n);
  return tmp;
}


int myrand(int M)
{
  double r;
  double x;
  int y;

  r = ( (double)rand() / (double)(RAND_MAX+1) ); 
  x = (r * M); 
  y = (int) x;
//errors->log(2, "myrand random number", y);
  return y;
}

int strcnt(char *s, char z)
{
  int cnt = 0;
  
  for (int n=0; n < strlen(s); n++)
    if (s[n] == z) cnt++;
  
  return cnt;
}

double V_ScalarProduct(myvector v1, myvector v2)
{
  return ((v1.x*v2.x)+(v1.y*v2.y));
}

myvector V_Normalize(myvector *v)
{
  double len=sqrt((v->x*v->x)+(v->y*v->y));
  if (len)
  {
    v->x/=len;
    v->y/=len;
  }
  return *v;
}

myvector V_DifferenceVector(myvector v1, myvector v2)
{
  myvector temp;
  temp.x=v2.x-v1.x;
  temp.y=v2.y-v1.y;
  return temp;
}

double V_GetAngle(int x1, int y1, int x2, int y2)
{
  double product;
  myvector temp,diff,v1,v2;
  v1.x = x1; v1.y = y1;
  v2.x = x2; v2.y = y2;
  temp.x=0;
  temp.y=1;
  diff=V_DifferenceVector(v1,v2);

  V_Normalize(&temp);
  V_Normalize(&diff);
  product = V_ScalarProduct(temp,diff);

  return acos(product);
}

void mytextout_center(BITMAP *v, FONT *f, char *s, int x1, int x2, int y, int col, int cl)
{
  int len = x2-x1;
  int tlen = text_length(f, s);
  int theight = text_height(f);

  if (tlen < len)
  {
    aatextout(v, f, s, x1+len/2-tlen/2, y, col);
    if (cl) clearlist->add(v, x1+len/2-tlen/2, y, tlen, theight);
  }
  else
  {
    char *line;
    int chars_per_line = len / text_length(f, "o");
    int slen = strlen(s);
//    int lines = (slen / chars_per_line) + 1;

    //for (int l=0; l < lines; l++)
    int l = 0;

    while (strlen(s) > 0)
    {
      slen = (int)strlen(s);

      if (slen < chars_per_line)
        chars_per_line = slen;

      line = new char[chars_per_line+1];
      strncpy(line, s, chars_per_line);
      line[chars_per_line] = '\0';

      if (slen > chars_per_line && strstr(line, " "))
        while (line[strlen(line)-1] != ' ')
          line[strlen(line)-1] = '\0';

      s += strlen(line);
      slen -= strlen(line);

      tlen = text_length(f, line);
      aatextout(v, f, line, x1+len/2-tlen/2, y+theight*l+l, col);
      if (cl) clearlist->add(v, x1+len/2-tlen/2, y+theight*l+l, tlen, theight);

      delete[](line);

      l++;
    }

  }
}

void mytextout_left(BITMAP *v, FONT *f, char *s, int x1, int x2, int y, int col, int cl)
{
  int len = x2-x1;
  int tlen = text_length(f, s);
  int theight = text_height(f);

  if (tlen < len)
  {
    aatextout(v, f, s, x1, y, col);
    if (cl) clearlist->add(v, x1, y, tlen, theight);
  }
  else
  {
    char *line;
    int chars_per_line = len / text_length(f, "o");
    int slen = strlen(s);
//    int lines = (slen / chars_per_line) + 1;

    //for (int l=0; l < lines; l++)
    int l = 0;

    while (strlen(s) > 0)
    {
      slen = (int)strlen(s);

      if (slen < chars_per_line)
        chars_per_line = slen;

      line = new char[chars_per_line+1];
      strncpy(line, s, chars_per_line);
      line[chars_per_line] = '\0';

      if (slen > chars_per_line && strstr(line, " "))
        while (line[strlen(line)-1] != ' ')
          line[strlen(line)-1] = '\0';

      s += strlen(line);
      slen -= strlen(line);

      tlen = text_length(f, line);
      aatextout(v, f, line, x1, y+theight*l+l, col);
      if (cl) clearlist->add(v, x1, y+theight*l+l, tlen, theight);

      delete[](line);

      l++;
    }

  }
}

void aacharout(BITMAP *v, FONT *f, char character, int xpos, int ypos, int color)
{
  char tmp[2];
  tmp[0] = character;
  tmp[1] = '\0';
  aatextout(v, f, tmp, xpos, ypos, color);
}


int filecopy(char *from, char *to)
{
  int c;
  FILE *in, *out;

  in  = fopen(from, "rb");
  out = fopen(to, "wb");

  if (in && out)
  {
    while ( (c=fgetc(in)) != EOF )
      fputc(c, out);

    fclose(in);
    fclose(out);
    return 0;
  }
  else
    return 1;
}

unsigned int byte2int(unsigned char *bvec)
{
  int i;
  int ival = 0;
  int tc[4];

  for (i = 0; i < 4; i++) tc[i] = bvec[i];
  for (i = 0; i < 4; i++) bvec[i] = tc[3-i];

  for ( i = 0; i < 4; i++ )
  {
    ival = ival << 8;
    ival = ival + *bvec;
    bvec = bvec + 1;
  }

  return ival;
}

void int2byte(unsigned int ival, unsigned char *bvec)
{
  int i;
  int tc[4];

  for ( i = 0; i < 4; i++ )
  {
    *bvec = ( ival >> (3-i)*8 );
    tc[3-i] = *bvec;
    bvec++;
  }

  bvec -= 4;

  for (i = 0; i < 4; i++) bvec[i] = tc[i];
}

char *strrepl(char *Str, size_t BufSiz, char *OldStr, char *NewStr)
{
  int OldLen, NewLen;
  char *p, *q;

  if(NULL == (p = strstr(Str, OldStr)))
    return Str;
  OldLen = strlen(OldStr);
  NewLen = strlen(NewStr);
  if ((strlen(Str) + NewLen - OldLen + 1) > BufSiz)
    return NULL;
  memmove(q = p+NewLen, p+OldLen, strlen(p+OldLen)+1);
  memcpy(p, NewStr, NewLen);

  return q;
}

void clear_key(int k)
{
  if (k == -1) clear_keyarray();
  else key[k] = 0;

  clear_keybuf();
}

void clear_keyarray()
{
  for (int n=0; n < 256; n++)
    key[n] = 0;
}

void text_to_clipboard(char *data)
{
#ifdef ALLEGRO_WINDOWS
  if (OpenClipboard(NULL))
  {
    EmptyClipboard();

    HGLOBAL clipboard_data;
    clipboard_data = GlobalAlloc(GMEM_DDESHARE, strlen(data)+1);
    
    char *ptr2data;
    ptr2data = (char*)GlobalLock(clipboard_data);
    
    strcpy(ptr2data, data);

    GlobalUnlock(clipboard_data);
    SetClipboardData(CF_TEXT, clipboard_data);

    CloseClipboard();
  }
#endif

#ifdef ALLEGRO_DOS
  SetClipboardData(cb_oemtext, data, strlen(data));
#endif
}

#ifdef ALLEGRO_DOS
char ClipboardAvailable()
{
  __dpmi_regs r;

  r.x.ax = 0x1700;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0x1700;
}

char CloseClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1708;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

char OpenClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1701;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

char EmptyClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1702;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

long int ClipboardCompact(int l)
{
  __dpmi_regs r;

  r.x.ax = 0x1709;
  r.x.si = l >> 16;
  r.x.cx = l;
  __dpmi_int(0x2f,&r);

  return r.x.ax + (r.x.dx << 16);
}

char SetClipboardData(char format, char *data, int l)
{
  __dpmi_regs r;

  OpenClipboard();

  if ( (data) && (l > 0) && (l < cb_bufsize) && (EmptyClipboard) &&
       (ClipboardCompact(l) >= l) )
  {
    dosmemput(data,l,__tb);

    r.x.ax = 0x1703;
    r.x.dx = format;
    r.x.bx = __tb & 0x0f;
    r.x.es = (__tb >> 4) & 0xffff;
    r.x.si = l >> 16;
    r.x.cx = l;
    __dpmi_int(0x2f,&r);

    return r.x.ax != 0;
  }

  return 0;
}
#endif

