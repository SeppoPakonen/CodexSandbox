/*
 *   GRAVITY STRIKE -- gshelper.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */


#ifndef _GSHELPER_
#define _GSHELPER_

#define VERY_LONG_TIME   9999.9

#include "network/rand.h"

inline int fast_ftol(const float a)
{
    static int b;
    
#if defined(_MSVC)
    __asm fld a
    __asm fistp b
#elif defined(__GNUG__)
    // use AT&T inline assembly style, document that
    // we use memory as output (=m) and input (m)
    __asm__ __volatile__ (
        "flds %1        \n\t"
        "fistpl %0      \n\t"
        : "=m" (b)
        : "m" (a));
#endif
    return b;
}

typedef struct myvector {
  double x;
  double y;
} myvector;

//important_globals saved_globals;
int collision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2, float xspd1, float yspd1, float xspd2, float yspd2, float *newx1, float *newy1, float *newx2, float *newy2, int usenew);
int collision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2);
int radius_player_collision(class gsPlayer *pl1, class gsPlayer *pl2);
float get_time_to_collision(float cx1, float cy1, float dx1, float dy1, float r1, float cx2, float cy2, float dx2, float dy2, float r2);
float distance(float x1, float y1, float x2, float y2);
void make_screenshot();
char *ftoa(char *tmp, float n);
char *ftoa(char *tmp, char *t, float n);
char *mitoa(char *tmp, int n);

double V_ScalarProduct(myvector v1, myvector v2);
myvector V_Normalize(myvector *v);
myvector V_DifferenceVector(myvector v1, myvector v2);
double V_GetAngle(int x1, int y1, int x2, int y2);

void aacharout(BITMAP *v, FONT *f, char c, int xpos, int ypos, int color);
void mytextout_center(BITMAP *v, FONT *f, char *s, int x1, int x2, int y, int col, int cl=1);
void mytextout_left(BITMAP *v, FONT *f, char *s, int x1, int x2, int y, int col, int cl=1);
int filecopy(char *, char *);

unsigned int byte2int(unsigned char *bvec);
void int2byte(unsigned int ival, unsigned char *bvec);

int strtrim(char *pszStr);
int strtriml(char *pszStr);
int strtrimr(char *pszStr);
char *strrepl(char *Str, size_t BufSiz, char *OldStr, char *NewStr);
int strcnt(char *s, char z);

void clear_key(int k=-1);
void clear_keyarray();

int myrand(int max);
inline int gsrand()
{
  //return randomMT();
  return rand();
}

inline void seed_gsrand(unsigned long seed)
{
  //seedMT(seed);
  srand(seed);
}

inline int fxrand()
{
  return rand();
}

char *decode_string(char *, int m=1);

void text_to_clipboard(char *);

#ifdef ALLEGRO_DOS
#define cb_text       1
#define cb_bitmap     2
#define cb_oemtext    7
#define cb_dsptext    0x81
#define cb_dspbitmap  0x82

#define cb_bufsize       _go32_info_block.size_of_transfer_buffer

char ClipboardAvailable();
char CloseClipboard();
char OpenClipboard();
char EmptyClipboard();
long int ClipboardCompact(int l);
char SetClipboardData(char format, char *data, int l);
#endif

#endif
