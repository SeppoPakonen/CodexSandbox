/*
    Gravity Strike -- main

    (c) 2001/2002 by Jens Hassler
*/

#include "gsmain.h"
#include "gsglob.h"

int gfxdriver = GFX_AUTODETECT;
int debuglevel = 0;
int noclip = 0;
int nocrcmessage = 0;

void parse_cmd_line(int argc, char *argv[])
{
  if (argc > 1)
  for (int n=1; n < argc; n++)
  {
    #ifdef ALLEGRO_WINDOWS
    if (!strcmp(argv[n], "/window"))
        gfxdriver = GFX_DIRECTX_WIN;
    #endif

    if (!strcmp(argv[n], "/debug"))
      debuglevel = 2;

    if (!strcmp(argv[n], "/noclip"))
      noclip = 1;
      
    if (!strcmp(argv[n], "/nocrcmessage"))
      nocrcmessage = 1;

    #ifdef ALLEGRO_LINUX
    if (!strcmp(argv[n], "/xdga2"))
      gfxdriver = GFX_XDGA2;
    else if (!strcmp(argv[n], "/xdga_win"))
      gfxdriver = GFX_XDGA;
    else if (!strcmp(argv[n], "/xdga"))
      gfxdriver = GFX_XDGA_FULLSCREEN;
    else if (!strcmp(argv[n], "/xwindows_win"))
      gfxdriver = GFX_XWINDOWS;
    else if (!strcmp(argv[n], "/xwindows"))
      gfxdriver = GFX_XWINDOWS_FULLSCREEN;

    #endif

  }
}

int main(int argc, char *argv[])
{
  parse_cmd_line(argc, argv);

  // main game object
  gsMain *gs_main = new gsMain();

  if (!gs_main->init())
    gs_main->play();

  delete(gs_main);

  return 0;
}

END_OF_MAIN();

