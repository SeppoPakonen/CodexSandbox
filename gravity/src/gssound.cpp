/*
 *   GRAVITY STRIKE -- gssound.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gserror.h"
#include "gsplayer.h"
#include "gsglob.h"
#include "gssound.h"
#include "gshelper.h"
#include "gsmap.h"

gsSound::gsSound()
{
  player_thrust = 0;
  status_play_music = 1;
  status_play_menu_music = 1;
  status_play_game_music = 0;
  samplecount = 0;
  music_volume = 0;
  
  memset(&replaced_sounds, 0, sizeof(replaced_sounds));
  mymusic = NULL;
  game_music = NULL;

  strcpy(menu_music_path, globals->data_dir);
  strcat(menu_music_path, "gs.mod");
  if (!exists(menu_music_path))
  {
    replace_extension(menu_music_path, menu_music_path, "xm", 40);
    if (!exists(menu_music_path))
    {
      replace_extension(menu_music_path, menu_music_path, "s3m", 40);
      if (!exists(menu_music_path))
        status_play_menu_music = 0;
    }
  }

  // Reserve voices - using Allegro 5 audio instead
  // reserve_voices(16, -1);
  // set_volume_per_voice(0);

  // Initialize Allegro 5 audio system
  if (!al_install_audio())
  {
    allegro_message("ERROR: couldn't initialize sound card!\nthe error was: %s\nstarting without sound...", "al_install_audio failed");
    errors->log(1, "ERROR initializing sound", "al_install_audio failed");
    status_play_sound = 0;
    status_play_music = 0;
  }
  else
  {
    char path[255];
    strcpy(path, globals->data_dir);
    strcat(path, globals->soundfile_name);
    packfile_password(globals->pwd);
    globals->sounddat = load_datafile(path);
    packfile_password(NULL);

    if (!globals->sounddat)
    {
      errors->log(1, "ERROR loading sound data", path);
      allegro_message("ERROR: couldn't load sound data (%s)!\nstarting without sound...", path);
      status_play_sound = 0;
    }
    else
    {
      errors->log(2, "init", "sound initialization successful");
      status_play_sound = 1;

      // load music
      // Allegro 5 automatically handles audio streaming allocation
      // The acodec addon should already be installed in allegro5_wrapper.cpp
      if (!al_is_audio_installed())
      {
        status_play_music = 0;
        errors->log(2, "init", "FAILED initializing audio system for music files");
      }

    }
  }
}

gsSound::~gsSound()
{
  // remove_sound();  // Not needed in Allegro 5
  if (globals->sounddat) unload_datafile(globals->sounddat); globals->sounddat = NULL;
}

void gsSound::play(int smp, int freq, int loop, int x, int y, int v)
{
  int maxdist = 800;
  int dist,curvol,pan,xab;

  if (status_play_sound)
  {
    if (x != 0 && y != 0)
    {
      if (globals->splitscreen)
      {
        int dist1 = int(distance(int(player[0]->get_x()),int(player[0]->get_y()),x,y));
        int dist2 = int(distance(int(player[1]->get_x()),int(player[1]->get_y()),x,y));

        dist = MIN(dist1,dist2);
        xab  = (dist == dist1) ? (int)player[0]->get_x() - x : (int)player[1]->get_x() - x;
      }
      else
      {
//        dist = int(distance(int(player[0]->get_x()),int(player[0]->get_y()),x,y));
        dist = int(distance(playmap[0]->get_scroll_x()+globals->playscreen_width/2, playmap[0]->get_scroll_y()+globals->playscreen_height/2, x, y));
        xab  = (playmap[0]->get_scroll_x()+globals->playscreen_width/2) - x;
      }

      // volume
      curvol = int((255.0 * (float)(1-((float)dist/(float)maxdist))) * (v/100.0));
      curvol = int((float)curvol * (float)(globals->soundvol/100.0));
    
#ifdef ALLEGRO_WINDOWS
      xab = -xab;
#endif

      // pan
      if (xab < 0)
      {
        pan = int(128.0 * (float)(1-((float)abs(xab)/(float)maxdist)));
      }
      else if (xab > 0)
      {
        pan = int((128.0 * (float)(1-((float)abs(xab)/(float)maxdist))) + 128);
      }
      else
      {
        pan = 128;
      }
    
      if (curvol > 255) curvol = 255;
    }
    else
    {
      dist = 0;
      curvol = int((float)v * (float)(globals->soundvol/100.0));
      pan = 128;
    }

    if (dist < maxdist)
    {
      if (smp < 1000)
        play_sample((SAMPLE *)globals->sounddat[smp].dat,curvol,pan,freq,loop);
      else
        play_sample(user_sample[smp-1000],curvol,pan,freq,loop);
    }
  }
}

void gsSound::load_music(char *f)
{
  char mypath[255];
  sprintf(mypath, "%s%s/%s", globals->level_dir,
                             globals->level_files[globals->selected_level].leveldir,
                             f);

  if (!exists(mypath))
  {
    game_music = NULL;
    status_play_game_music = 0;
    errors->log(1, "music file does not exist!", f);
    return;
  }

  game_music = al_load_audio_stream(mypath, 4, 2048);  // 4 samples in queue, 2048 bytes per sample
  if (game_music) {
    al_set_audio_stream_gain(game_music, 1.0);  // This will be adjusted by set_music_volume
    music_volume = 150;
    status_play_game_music = 1;
  } else {
    game_music = NULL;
    status_play_game_music = 0;
    errors->log(1, "FAILED to load music file!", mypath);
  }
}

void gsSound::load_menu_music()
{
  mymusic = al_load_audio_stream(menu_music_path, 4, 2048);  // 4 samples in queue, 2048 bytes per sample

  if (mymusic)
  {
    al_set_audio_stream_gain(mymusic, 1.0);  // This will be adjusted by set_music_volume
    errors->log(2, "menu init", "loading menu music done", get_filename(menu_music_path));
    music_volume = 150;
    status_play_menu_music = 1;
  }
  else
  {
    errors->log(1, "menu init", "loading menu music FAILED!", get_filename(menu_music_path));
    status_play_menu_music = 0;
  }
}

void gsSound::destroy_music()
{
  if (game_music)
  {
    al_set_audio_stream_playing(game_music, false);
    al_destroy_audio_stream(game_music);
    status_play_game_music = 0;
    game_music = NULL;
  }
}

void gsSound::destroy_menu_music()
{
  if (mymusic)
  {
    al_set_audio_stream_playing(mymusic, false);
    al_destroy_audio_stream(mymusic);
    mymusic = NULL;
    status_play_menu_music = 0;
  }
}

void gsSound::start_menu_music()
{
  if (status_play_sound && status_play_music && status_play_menu_music) {
    al_set_audio_stream_playing(mymusic, true);
  }
}

void gsSound::start_music()
{
  if (status_play_sound && status_play_music && status_play_game_music) {
    al_set_audio_stream_playing(game_music, true);
  }
}

void gsSound::stop_music()
{
  if (status_play_sound && status_play_music && status_play_game_music) {
    al_set_audio_stream_playing(game_music, false);
  }
}

void gsSound::set_music_volume(int v)
{
  if (status_play_sound && status_play_music && v >= 0 && v <= 150)
  {
    float gain = v / 150.0f;  // Convert to 0.0-1.0 range for Allegro 5
    if (mymusic) {
      al_set_audio_stream_gain(mymusic, gain);
    }
    if (game_music) {
      al_set_audio_stream_gain(game_music, gain);
    }
    music_volume = v;
  }
}


void gsSound::stop(int smp)
{
  if (status_play_sound)
    stop_sample((SAMPLE *)globals->sounddat[smp].dat);
}

void gsSound::menu_change()
{
  play(snd_menu_change, 1000, 0, 0, 0, 80);
}

void gsSound::menu_select()
{
  play(snd_menu_select, 1000, 0, 0, 0, 100);
}

void gsSound::menu_changebar()
{
  play(snd_menu_change, 1500, 0, 0, 0, 75);
}

void gsSound::menu_nono()
{
  play(snd_menu_change, 500, 0, 0, 0, 100);
}

void gsSound::menu_keypressed()
{
  play(snd_menu_change, 2000, 0, 0, 0, 100);
}

void gsSound::menu_shoot()
{
  play(snd_shot_single, 1000, 0, 0, 0, 32);
}

void gsSound::menu_dead()
{
  play(snd_exp_mid_1, 1000, 0, 0, 0, 32);
}

void gsSound::start_player_thrusters(long plsig)
{
  {
    int plnum = get_player_nr_by_sig(plsig);
    play(snd_thrusters, 1000, 1, player[plnum]->get_x(), player[plnum]->get_y(), 100);
    player_thrust += (plnum+1);
  }
}

void gsSound::stop_player_thrusters(long plsig)
{
  int plnum = get_player_nr_by_sig(plsig);

  player_thrust -= (plnum+1);
  if (player_thrust <= 0)
    stop(snd_thrusters);
}

void gsSound::extra_taken(int x, int y, int v, int f)
{
  play( snd_osd_on, 3000, 0, x, y, v);
}

void gsSound::cargo_taken(int x, int y, int v, int f)
{
  play( snd_cargoin, f, 0, x, y, v);
}

void gsSound::cargo_returned(int x, int y, int v, int f)
{
  play( snd_cargoout, f, 0, x, y, v);
}

void gsSound::base_jump(int x, int y, int v, int f)
{
  play( snd_land, f, 0, x, y, v);
}

void gsSound::wall_hit(int x, int y, int v, int f)
{
  play( snd_bonk, f, 0, x, y, v);
}

void gsSound::ship_hit(int x, int y, int v, int f)
{
  play( snd_bonk, f, 0, x, y, v);
}

void gsSound::shield_hit(int x, int y, int v, int f)
{
  play( snd_shield, f, 0, x, y, v);
}

void gsSound::hull_hit(int x, int y, int v, int f)
{
  play( snd_bonk, 1500, 0, x, y, v);
}

void gsSound::shoot_empty(int x, int y, int v, int f)
{
  play( snd_noweap, 500, 0, x, y, v);
}

void gsSound::shoot_single(int x, int y, int v, int f)
{
  play( snd_shot_single, f, 0, x, y, v);
}

void gsSound::shoot_double(int x, int y, int v, int f)
{
  play( snd_shot_double, f, 0, x, y, v);
}

void gsSound::shoot_bomb(int x, int y, int v, int f)
{
  play( snd_shot_single, f, 0, x, y, v);
}

void gsSound::shoot_bigbomb(int x, int y, int v, int f)
{
  play( snd_shot_single, f, 0, x, y, v);
}

void gsSound::shoot_rocket(int x, int y, int v, int f)
{
  play( snd_shot_rocket, f, 0, x, y, v);
}

void gsSound::shoot_freezer(int x, int y, int v, int f)
{
  play( snd_shot_freezer, 1000, 0, x, y, v);
}

void gsSound::shoot_shrinker(int x, int y, int v, int f)
{
  play( snd_shot_rocket, 600, 0, x, y, v);
}

void gsSound::shoot_probe(int x, int y, int v, int f)
{
  play( snd_shot_single, 2000, 0, x, y, v);
}

void gsSound::shoot_mine(int x, int y, int v, int f)
{
  play( snd_shot_single, f, 0, x, y, v);
}

void gsSound::shoot_gas(int x, int y, int v, int f)
{
  play( snd_shot_single, 2000, 0, x, y, v);
}

void gsSound::shoot_laser(int x, int y, int v, int f)
{
  play( snd_shot_laser, 1000, 0, x, y, v);
}

void gsSound::shoot_bumper(int x, int y, int v, int f)
{
  play( snd_shot_single, 2000, 0, x, y, v);
}

void gsSound::shoot_shotgun(int x, int y, int v, int f)
{
  play( snd_shot_shotgun, 1000, 0, x, y, v);
}

void gsSound::shoot_acid(int x, int y, int v, int f)
{
  play( snd_shot_single, f, 0, x, y, v);
}


void gsSound::hit_bumper(int x, int y, int v, int f)
{
  play( snd_hit_bumper, 1000, 0, x, y, v);
}

void gsSound::hit_freezer(int x, int y, int v, int f)
{
  play( snd_hit_freezer, 1000, 0, x, y, v);
}

void gsSound::hit_shrinker(int x, int y, int v, int f)
{
  play( snd_hit_shrinker, 1000, 0, x, y, v);
}

void gsSound::ship_damage(int x, int y, int v, int f)
{
  play( snd_shipdamage, 1000, 0, x, y, v);
}

void gsSound::expl_small1(int x, int y, int v, int f)
{
  play( snd_exp_sm_1, f, 0, x, y, v);
}

void gsSound::expl_small2(int x, int y, int v, int f)
{
  play( snd_exp_sm_2, f, 0, x, y, v);
}

void gsSound::expl_medium1(int x, int y, int v, int f)
{
  play( snd_exp_mid_1, f, 0, x, y, v);
}

void gsSound::expl_medium2(int x, int y, int v, int f)
{
  play( snd_exp_mid_2, f, 0, x, y, v);
}

void gsSound::expl_medium3(int x, int y, int v, int f)
{
  play( snd_exp_mid_3, f, 0, x, y, v);
}

void gsSound::expl_big1(int x, int y, int v, int f)
{
  play( snd_exp_big_1, f, 0, x, y, v);
}

void gsSound::expl_damage(int x, int y, int v, int f)
{
  play( snd_exp_mid_1, f, 0, x, y, v);
}

void gsSound::beam_ship(int x, int y, int v, int f)
{
  play( snd_beamship, f, 0, x, y, v);
}

void gsSound::beam_bullet(int x, int y, int v, int f)
{
  play( snd_beambull, f, 0, x, y, v);
}

void gsSound::do_switch(int x, int y, int v, int f)
{
  play( snd_switch, f, 0, x, y, v);
}

void gsSound::osd_tick(int x, int y, int v, int f)
{
  play( snd_click, f, 0, x, y, v);
}

void gsSound::bump_wall(int x, int y, int v, int f)
{
  play( snd_switch, 1500, 0, x, y, v);
}

void gsSound::play_sound(int sound, int x, int y, int v, int f)
{
  if (sound < 100 && replaced_sounds[sound] > 0)
    sound = replaced_sounds[sound];

  if (sound < 1000)
  {
    switch (sound)
    {
      case SOUND_MENU_CHANGE       : menu_change(); break;
      case SOUND_MENU_SELECT       : menu_select(); break;
      case SOUND_MENU_CHANGEBAR    : menu_changebar(); break;
      case SOUND_MENU_NONO         : menu_nono(); break;
      case SOUND_MENU_KEYPRESSED   : menu_keypressed(); break;
      case SOUND_MENU_SHOOT        : menu_shoot(); break;
      case SOUND_MENU_DEAD         : menu_dead(); break;
  
      case SOUND_OSD_TICK          : osd_tick(x,y,v,f); break;
      case SOUND_EXTRA_TAKEN       : extra_taken(x,y,v,f); break;
      case SOUND_CARGO_TAKEN       : cargo_taken(x,y,v,f); break;
      case SOUND_CARGO_RETURNED    : cargo_returned(x,y,v,f); break;
      case SOUND_BASE_JUMP         : base_jump(x,y,v,f); break;
      case SOUND_DO_SWITCH         : do_switch(x,y,v,f); break;
      case SOUND_BUMP_WALL         : bump_wall(x,y,v,f); break;
      case SOUND_SHIELD_HIT        : shield_hit(x,y,v,f); break;
      case SOUND_HULL_HIT          : hull_hit(x,y,v,f); break;
      case SOUND_WALL_HIT          : wall_hit(x,y,v,f); break;
      case SOUND_SHIP_HIT          : ship_hit(x,y,v,f); break;
      case SOUND_SHOOT_EMPTY       : shoot_empty(x,y,v,f); break;
      case SOUND_SHOOT_SINGLE      : shoot_single(x,y,v,f); break;
      case SOUND_SHOOT_DOUBLE      : shoot_double(x,y,v,f); break;
      case SOUND_SHOOT_BOMB        : shoot_bomb(x,y,v,f); break;
      case SOUND_SHOOT_BIGBOMB     : shoot_bigbomb(x,y,v,f); break;
      case SOUND_SHOOT_ROCKET      : shoot_rocket(x,y,v,f); break;
      case SOUND_SHOOT_FREEZER     : shoot_freezer(x,y,v,f); break;
      case SOUND_SHOOT_SHRINKER    : shoot_shrinker(x,y,v,f); break;
      case SOUND_SHOOT_PROBE       : shoot_probe(x,y,v,f); break;
      case SOUND_SHOOT_MINE        : shoot_mine(x,y,v,f); break;
      case SOUND_SHOOT_GAS         : shoot_gas(x,y,v,f); break;
      case SOUND_SHOOT_LASER       : shoot_laser(x,y,v,f); break;
      case SOUND_SHOOT_BUMPER      : shoot_bumper(x,y,v,f); break;
      case SOUND_SHOOT_SHOTGUN     : shoot_shotgun(x,y,v,f); break;
      case SOUND_SHOOT_ACID        : shoot_acid(x,y,v,f); break;
      case SOUND_HIT_BUMPER        : hit_bumper(x,y,v,f); break;
      case SOUND_HIT_FREEZER       : hit_freezer(x,y,v,f); break;
      case SOUND_HIT_SHRINKER      : hit_shrinker(x,y,v,f); break;
      case SOUND_SHIP_DAMAGE       : ship_damage(x,y,v,f); break;
      case SOUND_BEAM_SHIP         : beam_ship(x,y,v,f); break;
      case SOUND_BEAM_BULLET       : beam_bullet(x,y,v,f); break;
  
      case SOUND_EXPL_SMALL1       : expl_small1(x,y,v,f); break;
      case SOUND_EXPL_SMALL2       : expl_small2(x,y,v,f); break;
      case SOUND_EXPL_MEDIUM1      : expl_medium1(x,y,v,f); break;
      case SOUND_EXPL_MEDIUM2      : expl_medium2(x,y,v,f); break;
      case SOUND_EXPL_MEDIUM3      : expl_medium3(x,y,v,f); break;
      case SOUND_EXPL_BIG1         : expl_big1(x,y,v,f); break;
      case SOUND_EXPL_DAMAGE       : expl_damage(x,y,f); break;
    } // switch sound
  }
  else
  {
    if (sound >= 1000 && sound < 1000+samplecount)
      play(sound, f, 0, x, y, v);
    else
      menu_nono();
  }
}

int gsSound::load_sound(char *f)
{
  SAMPLE *temp;
  char mypath[255];
  sprintf(mypath, "%s%s/%s", globals->level_dir,
                             globals->level_files[globals->selected_level].leveldir,
                             f);

  temp = load_sample(mypath);

  if (temp)
  {
    errors->log(2, "loading user sound", mypath);
    user_sample[samplecount] = temp;
    samplecount++;
  }
  else
    errors->log(1, "loading user sound failed", mypath);


  return samplecount-1 + 1000;
}

void gsSound::destroy_user_samples()
{
  for (int n=0; n < samplecount; n++)
    destroy_sample(user_sample[n]);

  memset(&replaced_sounds, 0, sizeof(replaced_sounds));

  samplecount = 0;
}

void gsSound::replace_sound(int num, int repl)
{
  if (repl >= 1000)
  {
    replaced_sounds[num] = repl;
//    errors->log(2, "replacing sound", num, repl);
  }
}

