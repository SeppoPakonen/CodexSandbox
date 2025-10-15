/*
 *   GRAVITY STRIKE -- gssound.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#ifndef _GSSOUND_
#define _GSSOUND_

#include "gssndat.h"
#include <allegro5/allegro_audio.h>



enum GS_SOUNDS {
  SOUND_MENU_CHANGE,
  SOUND_MENU_SELECT,
  SOUND_MENU_CHANGEBAR,
  SOUND_MENU_NONO,
  SOUND_MENU_KEYPRESSED,
  SOUND_MENU_SHOOT,
  SOUND_MENU_DEAD,
  SOUND_OSD_TICK,

  SOUND_EXTRA_TAKEN,
  SOUND_CARGO_TAKEN,
  SOUND_CARGO_RETURNED,
  SOUND_BASE_JUMP,
  SOUND_DO_SWITCH,
  SOUND_BUMP_WALL,
  SOUND_SHIELD_HIT,
  SOUND_HULL_HIT,
  SOUND_WALL_HIT,
  SOUND_SHIP_HIT,
  SOUND_SHOOT_EMPTY,
  SOUND_SHOOT_SINGLE,
  SOUND_SHOOT_DOUBLE,
  SOUND_SHOOT_BOMB,
  SOUND_SHOOT_BIGBOMB,
  SOUND_SHOOT_ROCKET,
  SOUND_SHOOT_FREEZER,
  SOUND_SHOOT_SHRINKER,
  SOUND_SHOOT_PROBE,
  SOUND_SHOOT_MINE,
  SOUND_SHOOT_GAS,
  SOUND_SHOOT_LASER,
  SOUND_SHOOT_BUMPER,
  SOUND_SHOOT_SHOTGUN,
  SOUND_SHOOT_ACID,
  SOUND_HIT_BUMPER,
  SOUND_HIT_FREEZER,
  SOUND_HIT_SHRINKER,
  SOUND_SHIP_DAMAGE,
  SOUND_BEAM_SHIP,
  SOUND_BEAM_BULLET,

  SOUND_EXPL_SMALL1,
  SOUND_EXPL_SMALL2,
  SOUND_EXPL_MEDIUM1,
  SOUND_EXPL_MEDIUM2,
  SOUND_EXPL_MEDIUM3,
  SOUND_EXPL_BIG1,
  SOUND_EXPL_DAMAGE
};

class gsSound
{
  private:
    int status_play_sound;                         // sound okay?
    int status_play_music;                         // music possible?
    int status_play_menu_music;                    // menu music available?
    int status_play_game_music;                    // game music available?
    int player_thrust;                             // thrusters of any player on?
    int music_volume;
    char menu_music_path[80];
    ALLEGRO_AUDIO_STREAM *mymusic;
    ALLEGRO_AUDIO_STREAM *game_music;
    SAMPLE *user_sample[100];                      // storage for user samples
    int replaced_sounds[100];                      // user-replaced standard sounds
    int samplecount;                               // how much user samples?

    void play(int smp, int freq, int loop, int x, int y, int v);
    void stop(int smp);

  public:
    // constr. / destrc.
    gsSound();
    ~gsSound();

    int get_play_music()                           { return status_play_music; }
    int get_play_sound()                           { return status_play_sound; }

    // functions

    void load_music(char *f);
    void destroy_music();
    void load_menu_music();
    void destroy_menu_music();
    void start_music();
    void start_menu_music();
    void stop_music();
    void set_music_volume(int);
    int  get_music_volume()                        { return music_volume; }

    void play_sound(int sound, int x=0, int y=0, int v=100, int freq=1000);
    int load_sound(char *f);
    void replace_sound(int num, int repl);
    void destroy_user_samples();

    void menu_change();
    void menu_select();
    void menu_changebar();
    void menu_nono();
    void menu_keypressed();
    void menu_shoot();
    void menu_dead();

    void start_player_thrusters(long pl);
    void stop_player_thrusters(long pl);

    void osd_tick(int x=0, int y=0, int v=100, int f=1000);
    void extra_taken(int x=0, int y=0, int v=100, int f=1000);
    void cargo_taken(int x=0, int y=0, int v=100, int f=1000);
    void cargo_returned(int x=0, int y=0, int v=100, int f=1000);
    void base_jump(int x=0, int y=0, int v=100, int f=1000);
    void do_switch(int x=0, int y=0, int v=100, int f=1000);
    void bump_wall(int x=0, int y=0, int v=100, int f=1000);
    void shield_hit(int x=0, int y=0, int v=100, int f=1000);
    void hull_hit(int x=0, int y=0, int v=100, int f=1000);
    void wall_hit(int x=0, int y=0, int v=100, int f=1000);
    void ship_hit(int x=0, int y=0, int v=100, int f=1000);
    void shoot_empty(int x=0, int y=0, int v=100, int f=1000);
    void shoot_single(int x=0, int y=0, int v=100, int f=1000);
    void shoot_double(int x=0, int y=0, int v=100, int f=1000);
    void shoot_bomb(int x=0, int y=0, int v=100, int f=1000);
    void shoot_bigbomb(int x=0, int y=0, int v=100, int f=1000);
    void shoot_rocket(int x=0, int y=0, int v=100, int f=1000);
    void shoot_freezer(int x=0, int y=0, int v=100, int f=1000);
    void shoot_shrinker(int x=0, int y=0, int v=100, int f=1000);
    void shoot_probe(int x=0, int y=0, int v=100, int f=1000);
    void shoot_mine(int x=0, int y=0, int v=100, int f=1000);
    void shoot_gas(int x=0, int y=0, int v=100, int f=1000);
    void shoot_laser(int x=0, int y=0, int v=100, int f=1000);
    void shoot_bumper(int x=0, int y=0, int v=100, int f=1000);
    void shoot_shotgun(int x=0, int y=0, int v=100, int f=1000);
    void shoot_acid(int x=0, int y=0, int v=100, int f=1000);
    void hit_bumper(int x=0, int y=0, int v=100, int f=1000);
    void hit_freezer(int x=0, int y=0, int v=100, int f=1000);
    void hit_shrinker(int x=0, int y=0, int v=100, int f=1000);
    void ship_damage(int x=0, int y=0, int v=100, int f=1000);
    void beam_ship(int x=0, int y=0, int v=100, int f=1000);
    void beam_bullet(int x=0, int y=0, int v=100, int f=1000);

    void expl_small1(int x=0, int y=0, int v=100, int f=1000);
    void expl_small2(int x=0, int y=0, int v=100, int f=1000);
    void expl_medium1(int x=0, int y=0, int v=100, int f=1000);
    void expl_medium2(int x=0, int y=0, int v=100, int f=1000);
    void expl_medium3(int x=0, int y=0, int v=100, int f=1000);
    void expl_big1(int x=0, int y=0, int v=100, int f=1000);
    void expl_damage(int x=0, int y=0, int v=100, int f=1000);    
};

extern gsSound *gsound;

#endif
