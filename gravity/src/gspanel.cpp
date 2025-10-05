/*
 *   GRAVITY STRIKE -- gspanel.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsscript.h"
#include <stdio.h>
#ifdef ALLEGRO_DJGPP
  #include <dpmi.h>
#endif
#include "gspanel.h"
#include "gsglob.h"
#include "gserror.h"
#include "gsplayer.h"
#include "gseffect.h"
#include "gsmap.h"
#include "gsradar.h"
#include "gsbase.h"
#include "gshelper.h"
#include "gsfade.h"
#include "gslng.h"
#include "gsosd.h"


gsPanel::gsPanel(int mode, gsPlayer *pl, gsMap *m)
{
  osd = NULL;
  active = 1;
  init(mode);
  bind_to_player(pl);

  active_radar_noise = 0;
  radar_width  = 89-6;
  radar_height = 50-6;

  radar = new gsRadar(m, pl, radar_width, radar_height);
}

gsPanel::~gsPanel()
{
  if (panelbit) destroy_bitmap(panelbit);
  if (background) destroy_bitmap(background);
  if (radar) delete(radar);
  if (osd) delete(osd);
}

void gsPanel::blit_orig_paneldat()
{
  BITMAP *tmp = (BITMAP*)globals->paneldat[panel_half].dat;
  blit(tmp, panelbit, 0, 0, 0, 0, tmp->w, tmp->h);

  if (globals->network_game || globals->singleplayer || globals->minigame)
  {
    blit((BITMAP*)globals->paneldat[panel_div].dat, panelbit, 
         0, 0,
         globals->panel_half_width-2, 0, 
         5, globals->panel_half_height);
  
    tmp = (BITMAP*)globals->paneldat[panel_half_right].dat;
    blit(tmp, panelbit, 0, 0, globals->panel_half_width+2, 0, tmp->w, tmp->h);
  }

  // Fuel / Extra / Frag Count Descriptions
  char tmps[11];

  strncpy(tmps, language->get_string(T_GAME_PANEL_FUEL), 10);
  mytextout_center(panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                   tmps, 248, 269, 6, makecol8(100, 100, 255), 0);

  strncpy(tmps, language->get_string(T_GAME_PANEL_EXTRA), 10);
  mytextout_center(panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                   tmps, 270, 291, 6, makecol8(100, 100, 255), 0);

  if (globals->singleplayer || globals->minigame)
    strncpy(tmps, language->get_string(T_GAME_PANEL_SHIPS), 10);
  else
    strncpy(tmps, language->get_string(T_GAME_PANEL_FRAGS), 10);
  mytextout_center(panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                   tmps, 290, 317, 6, makecol8(100, 100, 255), 0);
}

void gsPanel::init(int mode)
{
  // full screen
  if (mode == 0)
  {
    create_panelbit(globals->panel_full_width, globals->panel_full_height);
    x = globals->playscreen_full_width/2 - globals->panel_full_width/2;
    y = globals->panel_full_y;
    w = globals->panel_full_width;
    h = globals->panel_full_height;
    blit_orig_paneldat();

    osd = new gsOsd(panelbit);
    osdbit = osd->add(1000,
                      globals->panel_half_width+5, 3,
                      globals->panel_half_width-4, globals->panel_half_height-5,
                      OSDBIT_PANEL);
    osdbit->set_active(1);
    osdbit->clear(getpixel(panelbit, globals->panel_half_width+4, 5));
//    osdbit->draw_rectangle(globals->col_yellow);
  }
  // splitscreen, left side
  else if (mode == 1)
  {
    create_panelbit(globals->panel_half_width, globals->panel_half_height);
    x = globals->playscreen_split_width/2 - globals->panel_half_width/2;
    y = globals->panel_full_y;
    w = globals->panel_half_width;
    h = globals->panel_half_height;
    blit_orig_paneldat();
    osdbit = NULL;
  }
  // splitscreen, right side
  else if (mode == 2)
  {
    create_panelbit(globals->panel_half_width, globals->panel_half_height);
    x = globals->playscreen_full_width/2 + globals->playscreen_split_width/2 - globals->panel_half_width/2;
    y = globals->panel_full_y;
    w = globals->panel_half_width;
    h = globals->panel_half_height;
    blit_orig_paneldat();
    osdbit = NULL;
  }

  first_anim = last_anim = NULL;
  debug_mode = 0;
  old_fuel = -1;
}

void gsPanel::create_panelbit(int w, int h)
{
  panelbit = create_bitmap(w,h);
  background = create_bitmap(w,h);

  clear_bitmap(panelbit);
  clear_bitmap(background);

  if (!panelbit && !background)
  {
    errors->log(0, "ERROR", "couldn't create bitmap for panel! (no memory?)");
  }

  panel_clear();
}

void gsPanel::bind_to_player(gsPlayer *pl)
{
  panel_player = pl;
  panel_player->set_panel(this);
}

void gsPanel::draw(BITMAP *v)
{
  blit(panelbit, v, 0, 0, x, y, panelbit->w, panelbit->h);

  draw_anims();
}

void gsPanel::panel_clear()
{
  if (debug_mode)
    clear_bitmap(panelbit);
}

void gsPanel::update_debug()
{
  if (debug_mode) debug();
}

void gsPanel::debug()
{
  textout_ex(panelbit, font, ftoa(tmpstr, "x", player[0]->get_x()), 10, 10, makecol8(255,255,255), -1);
  textout_ex(panelbit, font, ftoa(tmpstr, "y", player[0]->get_y()), 10, 20, makecol8(255,255,255), -1);
  textout_ex(panelbit, font, ftoa(tmpstr, "hcap", player[0]->get_handicap()), 10, 30, makecol8(255,255,255), -1);
//  textout_ex(panelbit, font, ftoa(tmpstr, "sbrel", player[0]->get_weapon_slot(panel_player->get_active_weapon())->reload), 100, 10, makecol8(255,255,255), -1);
  textout_ex(panelbit, font, ftoa(tmpstr, "map_x", playmap[0]->get_map_x()), 10, 40, makecol8(255,255,255), -1);
  textout_ex(panelbit, font, ftoa(tmpstr, "map_y", playmap[0]->get_map_y()), 100, 40, makecol8(255,255,255), -1);
//  #ifdef ALLEGRO_DJGPP
//    textout_ex(panelbit, font, ftoa(tmpstr, "mem", _go32_dpmi_remaining_virtual_memory()), 100, 20, makecol8(255,255,255), -1);
    textout_ex(panelbit, font, ftoa(tmpstr, "gccnt", script->get_gccount(glob_lua)), 100, 20, makecol8(255,255,255), -1);
//  #endif
  textout_ex(panelbit, font, ftoa(tmpstr, "fps", globals->fpst), 100, 30, makecol8(255,255,255), -1);
}

void gsPanel::update_shields()
{
  if (!debug_mode)
  {
    // clear shield panel
    draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_shield_empty].dat, 188, 5);
  
    // draw ship
    int dw = ((BITMAP *)globals->paneldat[panel_shield_empty].dat)->w;
    draw_sprite(panelbit, panel_player->get_ship(0), 188 + dw/2 - panel_player->get_owidth()/2, 19);

    // make ship a little bit more "integrated" by removing near black pixels
    if (!fade->get_fade_active())
    {
      int back_col = getpixel(panelbit, 4, 4);
  
      for (int a=0; a < 25; a++)
        for (int b=0; b < 25; b++)
        {
          int c = getpixel(panelbit, 188 + dw/2 - panel_player->get_width()/2 + a, 19+b);
          int cr = getr8(c); int cg = getg8(c); int cb = getb8(c);
          if (cr < 100 && cg < 100 && cb < 100) putpixel(panelbit, 188 + dw/2 - panel_player->get_width()/2 + a, 19+b, back_col);
        }
    }
      
    // draw shield status (max: 5)
    float shield     = panel_player->get_shield();
    float maxshield  = panel_player->get_max_shield();
    int shieldstatus = int( (shield / maxshield) * 5 );

    if (shieldstatus >= 1 && shieldstatus <= 5)
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_shield_1 + shieldstatus - 1].dat, 196, 9);
  
    // draw hull status (max: 3)
    float hull     = panel_player->get_hull();
    float maxhull  = panel_player->get_max_hull();
    int hullstatus = int( (hull / maxhull) * 3 );

    if (hullstatus >= 1 && hullstatus <= 3)
    {
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_hull_l_1 + hullstatus - 1].dat, 203, 20);
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_hull_r_1 + hullstatus - 1].dat, 223, 20);
    }
    
    // draw handicap status (max: 10)
    int hc = panel_player->get_handicap();
    if (hc)
    {
      for (int hcn=1; hcn <= hc; hcn++)
        hline(panelbit, 191 + hcn*4 + 1, 8, 191 + hcn*4 + 3, globals->col_orange);
    }

    // draw wall shield status (max: 5)
    float wshield  = panel_player->get_wall_shield();
    float maxwshield = panel_player->get_max_wall_shield();
    int wshieldstatus = int( (wshield / maxwshield) * 5 );

    if (wshieldstatus >= 1 && wshieldstatus <= 5 && !panel_player->get_dead())
    {
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_wshield_1 + wshieldstatus - 1].dat, 190, 6);
      draw_sprite_h_flip(panelbit, (BITMAP *)globals->paneldat[panel_wshield_1 + wshieldstatus - 1].dat, 190+dw-9, 6);
    }
    
    // draw hull & shield hitpoints as little numbers
    char hullcnt[10], shieldcnt[10];
    int scol = makecol8(120, 120, 220);
    int hcol = makecol8(150, 150, 150);
    if (panel_player->get_damage()) hcol = globals->col_orange;
    
    sprintf(shieldcnt, "%02d", (int)panel_player->get_shield());
    sprintf(hullcnt, "%02d", (int)panel_player->get_hull());
    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                shieldcnt, 197, 43, scol, -1);
    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                hullcnt, 197+30, 43, hcol, -1);
    

  } // if !debug_mode
}

void gsPanel::update_fuel()
{
  if (!debug_mode)
  {
    float fuel     = panel_player->get_fuel();
    float maxfuel  = panel_player->get_max_fuel();
    int fuelstatus = int( (fuel / maxfuel) * 14 );
  
//    if (fuelstatus != old_fuel)
    {
      old_fuel = fuelstatus;
  
      // clear fuel panel
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuelfrag_vert].dat, 251, 13);
    
      // draw fuel points (max: 15)
      for (int n=0; n < fuelstatus; n++)
        draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuel_line_vert].dat,
                    253, 40 - n*2 );
    }

    // draw fuel count
    char tmp[20];
    int a = int((fuel/maxfuel) * 100);
    if (a >= 100) a = 99;
    sprintf(tmp, "%02d", a);
    int mycol = makecol8(0, 255, 0);
    if (a < 10) mycol = makecol8(255, 100, 0);

    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                tmp, 254, 43, mycol, -1);

  } // if !debug_mode
}

void gsPanel::update_extra_energy()
{
  if (!debug_mode)
  {
    float extraen  = panel_player->get_extra_power();
    float maxextraen = panel_player->get_max_extra_power();
    int extrastatus = int( (extraen / maxextraen) * 14 );
  
    // clear extra panel
    draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuelfrag_vert].dat, 273, 13);
  
    // draw extra points (max: 14)
    for (int n=0; n < extrastatus; n++)
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuel_line_vert].dat,
                  275, 40 - n*2 );

    // draw extraen count
    char tmp[20];
    int a = int((extraen / maxextraen) * 100);
    if (a >= 100) a = 99;
    sprintf(tmp, "%02d", a);
    int mycol = makecol8(0, 255, 0);
    if (a < 10) mycol = makecol8(255, 100, 0);

    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                tmp, 276, 43, mycol, -1);

  } // if !debug_mode
}

void gsPanel::update_frags()
{
  if (!debug_mode)
  {
    draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuelfrag_vert].dat,
                295, 13);

    int a2;
    int a = a2 = panel_player->get_frags() - panel_player->get_suicides();
    if (a > 14) a2 = 14;
    if (a < -14) a2 = -14;
    if (a2 <= 14 && a2 >= -14)
    {
      if (a2 > 0)
      {
        for (int n=0; n < a2; n++)
        {
          draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_frag_line_vert].dat,
                      297, 40 - n*2);
        }
      }
      else if (a2 < 0)
      {
        for (int n=0; n > a2; n--)
        {
          draw_lit_sprite(panelbit, (BITMAP *)globals->paneldat[panel_frag_line_vert].dat,
                          297, 40 - abs(n)*2, globals->col_blue);
        }
      }
    }

    char tmp[20];
    sprintf(tmp, "%02d", abs(a));
    int mycol = makecol8(0, 255, 0);
    if (a < 0) mycol = makecol8(255, 100, 0);

    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                tmp, 298, 43, mycol, -1);

/*
    for (int n=0; n < (int)strlen(tmp); n++)
    {
      char tmp2[2];
      tmp2[0] = tmp[n]; tmp2[1] = '\0';
      int number = atoi(tmp2);
      draw_lit_sprite(panelbit, (BITMAP *)globals->paneldat[panel_num_small_0 + number].dat,
                  307-strlen(tmp)*5+n*5, 41, mycol);
    }
*/

  }
}

void gsPanel::update_lifes()
{
  if (!debug_mode && (globals->singleplayer || globals->minigame))
  {
    draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_fuelfrag_vert].dat,
                295, 13);

    int lines = panel_player->get_lifes();
    if (lines > 14) lines = 14;

    for (int n=0; n < lines; n++)
    {
      draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_frag_line_vert].dat,
                  297, 40 - n*2);
    }

    char tmp[20];
    sprintf(tmp, "%02d", panel_player->get_lifes());
    int mycol = makecol8(0, 255, 0);

    textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                tmp, 298, 43, mycol, -1);

  }
}

void gsPanel::update_weapons()
{
  if (!debug_mode)
  {
    // draw empty weapon display
    draw_sprite(panelbit, (BITMAP*)globals->paneldat[panel_weapon_empty].dat, 96, 11);

    // draw ship 90 degrees right rotated
    rotate_sprite(panelbit, panel_player->get_ship(0), 100, 19, itofix(64));

    // make ship a little bit more "integrated" by removing near black pixels
    if (!fade->get_fade_active())
    {
      int back_col = getpixel(panelbit, 4, 4);
  
      for (int a=0; a < 25; a++)
        for (int b=0; b < 25; b++)
        {
          int c = getpixel(panelbit, 98+a, 19+b);
          int cr = getr8(c); int cg = getg8(c); int cb = getb8(c);
          if (cr < 100 && cg < 100 && cb < 100) putpixel(panelbit, 98+a, 19+b, back_col);
        }
    }

    int slot_coords[6][2] = { {96,5}, {124,5}, {152,5}, {96,44}, {124,44}, {152,44} };
  
    // draw weapon slots + selection
    for (int n=0; n < 6; n++)
    {
      weaponslot *temp = panel_player->get_weapon_slot(n);
      BITMAP *tempbit = (BITMAP *)globals->paneldat[panel_weapon1_1].dat;
  
      switch (n)
      {
        case 0 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon1_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon1_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon1_2].dat;
                 break;
  
        case 1 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon2_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon2_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon2_2].dat;
                 break;
  
        case 2 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon3_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon3_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon3_2].dat;
                 break;
  
        case 3 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon4_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon4_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon4_2].dat;
                 break;
  
        case 4 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon5_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon5_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon5_2].dat;
                 break;
  
        case 5 : if (panel_player->get_active_weapon() == n)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon6_3].dat;
                 else if (temp->type == W_NONE || temp->bullets == 0 || temp->active == 0)
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon6_1].dat;
                 else
                   tempbit = (BITMAP *)globals->paneldat[panel_weapon6_2].dat;
                 break;
      }
  
      draw_sprite(panelbit, tempbit, slot_coords[n][0], slot_coords[n][1]);
    } // for weaponslots
  
    // draw pic of current weapon
    weaponslot *temp = panel_player->get_weapon_slot( panel_player->get_active_weapon() );
    BITMAP *tempbit;
  
    switch (temp->type)
    {
      case W_NONE     : tempbit = (BITMAP *)globals->paneldat[panel_pic_empty].dat; break;
      case W_SINGLE   : tempbit = (BITMAP *)globals->paneldat[panel_pic_single].dat; break;
      case W_DOUBLE   : tempbit = (BITMAP *)globals->paneldat[panel_pic_double].dat; break;
      case W_BOMB     : tempbit = (BITMAP *)globals->paneldat[panel_pic_bomb].dat; break;
      case W_BIGBOMB  : tempbit = (BITMAP *)globals->paneldat[panel_pic_bigbomb].dat; break;
      case W_ROCKET   : tempbit = (BITMAP *)globals->paneldat[panel_pic_rocket].dat; break;
      case W_MINE     : tempbit = (BITMAP *)globals->paneldat[panel_pic_mine1].dat; break;
      case W_FREEZER  : tempbit = (BITMAP *)globals->paneldat[panel_pic_freezer].dat; break;
      case W_SHRINKER : tempbit = (BITMAP *)globals->paneldat[panel_pic_shrinker].dat; break;
      case W_PROBE    : tempbit = (BITMAP *)globals->paneldat[panel_pic_probe].dat; break;
      case W_GAS      : tempbit = (BITMAP *)globals->paneldat[panel_pic_gas].dat; break;
      case W_LASER    : tempbit = (BITMAP *)globals->paneldat[panel_pic_laser].dat; break;
      case W_BUMPER   : tempbit = (BITMAP *)globals->paneldat[panel_pic_bumper].dat; break;
      case W_SHOTGUN  : tempbit = (BITMAP *)globals->paneldat[panel_pic_shotgun].dat; break;
      case W_ACID     : tempbit = (BITMAP *)globals->paneldat[panel_pic_acid].dat; break;
      default         : tempbit = (BITMAP *)globals->paneldat[panel_pic_empty].dat; break;
    } // switch type
  
    draw_sprite(panelbit, tempbit, 115, 18);

    update_bullets();

    if (temp->upgrade)
    {
      char upgradeline[30];
      sprintf(upgradeline, "UPGRADE %d", temp->upgrade);
      textout_ex( panelbit, (FONT*)globals->fontdat[FONT_MINIFONT].dat,
                  upgradeline, 122, 15, globals->col_white, -1);
    }
    
  } // if !debug_mode
}

void gsPanel::update_weapon_ready()
{
  
  if (!debug_mode)
  {
    weaponslot *temp = panel_player->get_weapon_slot(panel_player->get_active_weapon());
    if (temp->reload > 0)
    {
      int mreload = fast_ftol((temp->reload * (1+0.08*panel_player->get_handicap())));
      int maxh = fast_ftol( (14.0 * ((float)temp->cur_reload / (float)mreload)));
      vline(panelbit, 116, 19, 19+15, getpixel(panelbit, 4, 4));
      if (temp->cur_reload > 0) vline(panelbit, 116, 27-maxh/2, 27+maxh/2, globals->col_orange);
    }
  }    
}  

void gsPanel::update_bullets()
{
  if (!debug_mode)
  {
    weaponslot *temp = panel_player->get_weapon_slot(panel_player->get_active_weapon());
  
    // draw remaining bullets
    if (temp->bullets >= 0)
    {
      char tmp[20];
      sprintf(tmp, "%d", temp->bullets);
      rectfill(panelbit, 178-strlen(tmp)*5-5, 35, 178, 41, getpixel(panelbit, 181, 35));
      for (int n=0; n < (int)strlen(tmp); n++)
      {
        char tmp2[2];
        tmp2[0] = tmp[n]; tmp2[1] = '\0';
        int number = atoi(tmp2);
        draw_sprite(panelbit, (BITMAP *)globals->paneldat[panel_num_small_0 + number].dat,
                    178-strlen(tmp)*5+n*5, 35);
      }
    }

  }
}

void gsPanel::update_radar()
{
  if (!debug_mode)
  {
    if (panel_player->get_damage() != DAMAGE_RADAR && !panel_player->get_dead())
    {
      if (active_radar_noise) remove_anim_radar_noise();
      radar->update();
      blit(radar->get_radarbit(), panelbit, 0, 0, 6, 6, radar_width, radar_height);
    }
    else if (panel_player->get_damage() == DAMAGE_RADAR || panel_player->get_dead() == 2)
    {
  //    blit((BITMAP *)globals->paneldat[panel_radar_empty].dat, panelbit, 0, 0, 5, 5, radar_width+1, radar_height+1);
      if (!active_radar_noise) add_anim_radar_noise(panel_player->get_damage());
    }
  }
}

void gsPanel::update_all()
{
  update_shields();
  update_fuel();
  update_frags();
  update_lifes();
  update_extra_energy();
  update_weapons();
  update_radar();
  if (globals->network_game) update_netplayers();
}

void gsPanel::update_netplayers()
{
  if (!globals->network_game) return;

  struct coords {
    int x1, y1, x2, y2;
  };

  coords c[6];

  int n;
  for (n=1; n < 6; n++)
  {
    c[n].x1 = globals->panel_half_width + 12 + (n-1)*60;
    c[n].y1 = 8;
    c[n].x2 = globals->panel_half_width + 12 + (n-1)*60 + 55;
    c[n].y2 = globals->panel_half_height - 8;
    rect(panelbit, c[n].x1, c[n].y1, c[n].x2, c[n].y2, globals->col_blue);
    rectfill(panelbit, c[n].x1+1, c[n].y1+1, c[n].x2-1, c[n].y2-1, getpixel(panelbit, 181, 35));
  }

  int mycol = makecol8(100, 100, 255);
  FONT *f = (FONT*)globals->fontdat[FONT_MINIFONT].dat;

  for (n=1; n <= globals->players; n++)
  {
    // player name
    char plname[15];
    strncpy(plname, player[n-1]->get_name(), 14);

    int hmiddle = c[n].x1 + (c[n].x2 - c[n].x1)/2;
    int vmiddle = c[n].y1 + (c[n].y2 - c[n].y1)/2;

    int tw = text_length(f, plname);
    textout_ex( panelbit, f,
                plname, 
                hmiddle - tw/2, 
                c[n].y1 + 3, 
                mycol, -1);

    // ship imamge
    draw_sprite(panelbit, player[n-1]->get_ship(0), 
                hmiddle - player[n-1]->get_ship(0)->w/2,
                vmiddle - player[n-1]->get_ship(0)->h/2);

    update_netplayer_stats();
  }
}

void gsPanel::update_netplayer_stats()
{
  if (!globals->network_game) return;

  struct coords {
    int x1, y1, x2, y2;
  };

  coords c[6];

  int n;
  for (n=1; n < 6; n++)
  {
    c[n].x1 = globals->panel_half_width + 12 + (n-1)*60;
    c[n].y1 = 8;
    c[n].x2 = globals->panel_half_width + 12 + (n-1)*60 + 55;
    c[n].y2 = globals->panel_half_height - 8;
    rect(panelbit, c[n].x1, c[n].y1, c[n].x2, c[n].y2, globals->col_blue);
  }

  int fragcol = makecol8(0, 255, 0);
  int suicol = makecol8(255, 100, 0);

  FONT *f = (FONT*)globals->fontdat[FONT_MINIFONT].dat;

  for (n=1; n <= globals->players; n++)
  {
    // frags and suicides
    char myfrags[10], mysuicides[10];
    sprintf(myfrags, "%02d", player[n-1]->get_frags());
    sprintf(mysuicides, "%02d", player[n-1]->get_suicides());

    int frags_tw = text_length(f, myfrags);
    int suicides_tw = text_length(f, mysuicides);

    rectfill(panelbit, c[n].x1+1, c[n].y2-8, c[n].x2-1, c[n].y2-1, getpixel(panelbit, 181, 35));
    textout_ex( panelbit, f,
                myfrags,
                c[n].x1 + 10, 
                c[n].y2 - 8,
                fragcol, -1);

    textout_ex( panelbit, f,
                mysuicides,
                c[n].x2 - suicides_tw - 10,
                c[n].y2 - 8,
                suicol, -1);
  }
}

void gsPanel::set_radar_zoom(int n)
{
  radar->set_zoom(n);
}

int gsPanel::get_radar_zoom()
{
  return int(radar->get_zoom());
}


// ANIMATION stuff

PanelAnim::PanelAnim(gsPanel *pan, int ptype, int px, int py, int pmaxframe, int pmaxframet, int lp)
{
  ppanel = pan;
  type = ptype;
  x = px;
  y = py;
  maxframe = pmaxframe;
  maxframet = pmaxframet;
  curframe = 1;
  curframet = 0;
  loop = lp;
}

PanelAnim::~PanelAnim()
{
}

int PanelAnim::update()
{
  if (curframe >= maxframe)
  {
    if (!loop)
      return 1;
    else
    {
      curframe = 1;
      curframet = 0;
    }
  }
  else
  {
    curframet++;
    if (curframet >= maxframet)
    {
      curframet = 0;
      curframe++;
    }
  }

  return 0;
}

void PanelAnim::draw()
{
  BITMAP *bit = (BITMAP *)globals->paneldat[ type + curframe - 1 ].dat;

  draw_sprite(ppanel->get_panelbit(), bit,
              (int)x, (int)y);
}

void gsPanel::add_anim(int ptype, int px, int py, int maxf, int fdelay, int loop)
{
  PanelAnim *TempAnim;

  TempAnim = new PanelAnim(this, ptype, px, py, maxf, fdelay, loop);

  if (TempAnim != NULL)
  {
    if (first_anim==NULL)
    {
      last_anim = first_anim = TempAnim;
      first_anim->next = first_anim->prev = NULL;
      TempAnim->next = NULL; TempAnim->prev = NULL;
    }
    else
    {
      last_anim->next = TempAnim;
      TempAnim->prev = last_anim;
      last_anim = TempAnim;
      TempAnim->next = NULL;
    }
  } // TempAnim != NULL
}

void gsPanel::draw_anims()
{
  PanelAnim *temp = first_anim;

  while (temp != NULL)
  {
    temp->draw();
    temp = temp->next;
  }
}

void gsPanel::update_anims()
{
  PanelAnim *temp = first_anim, *next_anim;

  while (temp != NULL)
  {
    next_anim = temp->next;
    if (temp->update()) remove_anim(temp);
    temp = next_anim;
  } // while temp != NULL
}

void gsPanel::remove_anim(PanelAnim *temp)
{
  PanelAnim *next_anim, *prev_anim;

  if (temp != NULL)
  {
    prev_anim = temp->prev;
    next_anim = temp->next;

    if (prev_anim==NULL) first_anim = next_anim;
    else prev_anim->next = next_anim;

    if (next_anim==NULL) last_anim = prev_anim;
    else next_anim->prev = prev_anim;

    delete temp;
    temp = NULL;
  }
}

void gsPanel::add_anim_shield_explosion()
{
  add_anim(panel_shield_exp_1, 206, 17, 9, 6);
}

void gsPanel::add_anim_radar_noise(int only_radar)
{
  add_anim(panel_radar_noise_1, 6, 6, 3, 8, 1);
  if (!only_radar) add_anim(panel_radar_noise_1, 97, 6, 3, 8, 1);
  active_radar_noise = 1;
}

void gsPanel::remove_anim_radar_noise()
{
  PanelAnim *temp = first_anim, *next_anim;

  while (temp != NULL)
  {
    next_anim = temp->next;
    if (temp->loop == 1) { remove_anim(temp); }
    temp = next_anim;
  }

  active_radar_noise = 0;
}


