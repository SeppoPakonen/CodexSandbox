/*
 *   GRAVITY STRIKE -- gspanel.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSPANEL_
#define _GSPANEL_

class PanelAnim
{
  public:
    class gsPanel *ppanel;
    PanelAnim *prev, *next;

    int loop;                                      // identifier for looping animations (0 = no loop)
    int x;
    int y;
    int curframe;
    int maxframe;
    int curframet;
    int maxframet;
    int type;

    PanelAnim(class gsPanel *, int ptype, int px, int py, int pmaxframe, int pmaxframet, int loop=0);
    ~PanelAnim();

    int update();
    void draw();
};

class gsPanel
{
  private:
    // variables
    PanelAnim    *first_anim, *last_anim;
    BITMAP       *panelbit;                        // bitmap to draw panel onto
    BITMAP       *panelbit_right;                  // bitmap on right side (network)
    BITMAP       *background;                      // background for transp. panel
    class OsdBitmap *osdbit;                       // osd bitmap (single player, right side)
    class gsPlayer *panel_player;                  // bind panel to panel_player
    class gsRadar  *radar;                         // radar object
    int          x;                                // x start position
    int          y;                                // y start position
    int          w;                                // width of panel
    int          h;                                // height of panel
    int          debug_mode;                       // shows debug text
    int          old_fuel;                         // temp variable: old shield status
    int          active;                           // panel visible?

    int          radar_width;
    int          radar_height;

    char         tmpstr[50];                       // temporary string for debug output
    int          active_radar_noise;

    // functions
    void init(int);                                // initialise panel with mode
    void create_panelbit(int, int);                // create panelbit with width/height
    void debug();                                  // show debug information (text)

  public:
    // constructor, destructor
//    gsPanel(int, class gsMap *);  // mode 0 = full, 1 = half1, 2 = half2
    gsPanel(int, class gsPlayer *, class gsMap *);
    ~gsPanel();

    // variables
    class gsOsd  *osd;

    // get/put
    void set_debug(int n)                          { debug_mode = n; }  // turn debug mode on/off
    BITMAP *get_panelbit()                         { return panelbit; }
    void set_radar_zoom(int n);
    int  get_radar_zoom();
    int  get_debug()                               { return debug_mode; }
    int  get_active()                              { return active; }
    void set_active(int n)                         { active = n; }

    // functions
    void bind_to_player(class gsPlayer *);         // bind panel to player
    void update_debug();                           // updates panel with new data
    void draw(BITMAP *);                           // draw panel
    void draw_right(BITMAP *);                     // draw right half (networking mode)
    void blit_orig_paneldat();                     // draw panel background
    void save_background(class gsMap *);           // save back for trans panel
    void restore_background(class gsMap *);        // restore back for trans panel
    void panel_clear();                            // clear panel
    void update_all();                             // update all graphics on panel
    void update_shields();                         // updates shield status
    void update_weapons();                         // updates weapon status
    void update_weapon_ready();                    // updates weapon ready status
    void update_bullets();                         // updates bullet count
    void update_fuel();                            // updates fuel status
    void update_frags();                           // updates frag data
    void update_lifes();                           // updates lifes data
    void update_radar();                           // updates radar window
    void update_extra_energy();                    // updates extra energy status
    void update_netplayers();                      // updates network player panel
    void update_netplayer_stats();                 // updates only stats (not images)

    // animation functions
    void add_anim(int ptype, int px, int py, int maxf, int fdelay, int loop=0);
    void draw_anims();
    void update_anims();
    void remove_anim(PanelAnim *);

    void add_anim_shield_explosion();
    void add_anim_radar_noise(int only_radar=0);
    void remove_anim_radar_noise();
};

extern gsPanel *gpanel[];

#endif

