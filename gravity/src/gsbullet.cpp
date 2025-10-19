/*
 *   GRAVITY STRIKE -- gsbullet.cpp
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */
 
/* TODO (#1#): User-Waffen (scriptgesteuert) */


#include "gsbullet.h"
#include "gshelper.h"
#include "gsplayer.h"
#include "gserror.h"
#include "gssound.h"
#include "gsscript.h"
#include "gseffect.h"
#include "gsmap.h"
#include "weapons/btypes.h"
#include "gsglob.h"

extern gsGlobals *globals;

Bullet::Bullet()
{
  user = 0;
  upgrade = 0;
  by_enemy = 0;
  by_player = 0;
  orig_ttl = ttl = 0;
  dont_continue = 0;
  invisible = 0;
  w = h = 0;
  kill = 0;
  hitp = maxhitp = 0;
  spawn = 0;
  just_coll = 0;
    
  hitp_hull = hitp_shield = 0;
  
  curframe = curframet = maxframet = 0;
  image = NULL;
  
  backbuf = NULL;
  use_backbuf = 0;

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

Bullet::~Bullet()
{
}

void Bullet::draw(BITMAP *m)
{
}

void Bullet::move(int n)
{
}

void Bullet::save_background(gsMap *m)
{
  if (backbuf && use_backbuf) {
    int backbuf_w = get_bitmap_width(backbuf);
    int backbuf_h = get_bitmap_height(backbuf);
    if (m->is_on_map(x, y, backbuf_w, backbuf_h))
      blit(m->get_vscreen(), backbuf, (int)x-m->get_map_x(), (int)y-m->get_map_y()-backbuf_h/2, 0, 0, backbuf_w, backbuf_h);
  }
}

void Bullet::restore_background(gsMap *m)
{
  if (backbuf && use_backbuf) {
    int backbuf_w = get_bitmap_width(backbuf);
    int backbuf_h = get_bitmap_height(backbuf);
    if (m->is_on_map(x, y, backbuf_w, backbuf_h))
      blit(backbuf, m->get_vscreen(), 0, 0, (int)x-m->get_map_x(), (int)y-m->get_map_y()-backbuf_h/2, backbuf_w, backbuf_h);
  }
}

void Bullet::move()
{
  // update framecount
  if (image && curframet++ == maxframet)
  {
    curframet = 0;
    if (curframe < image->get_framecount()-1) curframe++;
    else curframe = 0;
  }
}

void Bullet::reset_min_max()
{
  min_x = (int)get_x() - globals->playscreen_width * 2;
  max_x = (int)get_x() + globals->playscreen_width * 2;
  min_y = (int)get_y() - globals->playscreen_height * 3;
  max_y = (int)get_y() + globals->playscreen_height * 3;
}

void Bullet::hit(float n)
{
  if (maxhitp > 0)
  {
    hitp += n;
    if (hitp >= maxhitp)
    {
      remove();
    }
  }
}

gsBullet::gsBullet()
{
  first_bullet=NULL;
  last_bullet=NULL;
  count = 0;

  create_bullet_sprites();
 

  /* TEST!
  bulletLua = new Lua;
  script->init_script_commands(bulletLua);
  int err = bulletLua->dofile("./userweap/testwaffe.ini");
  if (err)
    errors->log(0, "ERROR executing global weapon script");
  */
}

gsBullet::~gsBullet()
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    delete temp;
    temp = next_bullet;
  } // while temp != NULL


  // destroy bullet sprites
  int nr, nr2;
  for (nr=0; nr < REAL_WEAPON_COUNT; nr++)
    for (nr2=0; nr2 < 8; nr2++)
      if (bullet_frames[nr][nr2]) delete bullet_frames[nr][nr2];
   
  // TEST!
//  script->kill_script_commands(bulletLua);
//  delete bulletLua;
}

Bullet *gsBullet::add(int t, class gsPlayer *p, int pupgrade, int pspawn)
{
  Bullet *TempBullet = NULL;
  Bullet *TempBullet2 = NULL;

  int apnr = get_player_nr_by_sig(p->get_sig());
  if (apnr == -1) apnr = 0;

  int puser = 0;

  switch(t)
  {
    case W_FAKE :
                       TempBullet = new bulletFake(p, puser, pupgrade);
                       break;
    case W_SINGLE :
                       TempBullet = new bulletNormal(p, 0, pupgrade);
                       gsound->play_sound(SOUND_SHOOT_SINGLE, p->get_x(), p->get_y());
                       break;
    case W_DOUBLE :
                       TempBullet = new bulletNormal(p, 1, pupgrade);
                       TempBullet2 = new bulletNormal(p, 2, pupgrade);
                       add_to_list(TempBullet2);
                       gsound->play_sound(SOUND_SHOOT_DOUBLE, p->get_x(), p->get_y());
                       break;
    case W_BOMB :
                       TempBullet = new bulletBomb(p,  puser, pupgrade, bullet_frames[W_BOMB][apnr]);
                       gsound->play_sound(SOUND_SHOOT_BOMB, p->get_x(), p->get_y());
                       break;
    case W_BIGBOMB :
                       TempBullet = new bulletBigBomb(p, puser, pupgrade, bullet_frames[W_BIGBOMB][apnr]);
                       gsound->play_sound(SOUND_SHOOT_BIGBOMB, p->get_x(), p->get_y());
                       break;
    case W_ROCKET :
                       TempBullet = new bulletRocket(p, puser, pupgrade, bullet_frames[W_ROCKET][apnr]);
                       gsound->play_sound(SOUND_SHOOT_ROCKET, p->get_x(), p->get_y());
                       break;
    case W_MINE :
                       TempBullet = new bulletMine(p, puser, pupgrade, bullet_frames[W_MINE][apnr]);
                       gsound->play_sound(SOUND_SHOOT_MINE, p->get_x(), p->get_y());
                       break;
    case W_FREEZER :
                       TempBullet = new bulletFreezer(p, puser, pupgrade);
                       gsound->play_sound(SOUND_SHOOT_FREEZER, p->get_x(), p->get_y());
                       break;
    case W_SHRINKER :
                       TempBullet = new bulletShrinker(p, puser, pupgrade);
                       gsound->play_sound(SOUND_SHOOT_SHRINKER, p->get_x(), p->get_y());
                       break;
    case W_PROBE :
                       TempBullet = new bulletProbe(p, puser, pupgrade, bullet_frames[W_PROBE][apnr]);
                       gsound->play_sound(SOUND_SHOOT_PROBE, p->get_x(), p->get_y());
                       break;
    case W_GAS :
                       TempBullet = new bulletGas(p, puser, pupgrade, bullet_frames[W_GAS][apnr]);
                       gsound->play_sound(SOUND_SHOOT_GAS, p->get_x(), p->get_y());
                       break;
    case W_LASER :
                       TempBullet = new bulletLaser(p, puser, pupgrade, bullet_frames[W_LASER][apnr]);
                       gsound->play_sound(SOUND_SHOOT_LASER, p->get_x(), p->get_y());
                       break;
    case W_BUMPER :
                       TempBullet = new bulletBumper(p, puser, pupgrade, bullet_frames[W_BUMPER][apnr]);
                       gsound->play_sound(SOUND_SHOOT_BUMPER, p->get_x(), p->get_y());
                       break;
    case W_SHOTGUN :
                       TempBullet = new bulletShotgun(p, 0, pupgrade, pspawn);
                       gsound->play_sound(SOUND_SHOOT_SHOTGUN, p->get_x(), p->get_y());
                       break;
    case W_ACID :
                       TempBullet = new bulletAcid(p,  puser, pupgrade, bullet_frames[W_ACID][apnr], pspawn);
                       gsound->play_sound(SOUND_SHOOT_BOMB, p->get_x(), p->get_y());
                       break;
    case W_USER :
                       TempBullet = new bulletUser(p, puser, pupgrade);
                       break;
  } // case bullet type

  TempBullet->set_type(t);

  add_to_list(TempBullet);

  return TempBullet;
}

// wrapper for scripting
Bullet *gsBullet::add(int t, long psig, int enr, int px, int py, float pxspd, float pyspd, float pstrength, float puser, int pupgrade, int net_dont_kill)
{
  return add(t, psig, enr, px, py, pxspd, pyspd, pstrength, pstrength, pstrength, puser, pupgrade, 0, 1);
}

Bullet *gsBullet::add(int t, long psig, int enr, int px, int py, float pxspd, float pyspd, float pstrength, float pshield, float phull, float puser, int pupgrade, int net_dont_kill, int pspawn)
{
  Bullet *TempBullet = NULL;

  int color = get_player_nr_by_sig(psig);
  if (color == -1) color = 0;

  switch(t)
  {
    case W_FAKE :
                       TempBullet = new bulletFake(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade);
                       break;
    case W_SINGLE :
                       TempBullet = new bulletNormal(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade);
                       break;
    case W_DOUBLE :
                       break;
    case W_BOMB :
                       TempBullet = new bulletBomb(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_BOMB][color]);
                       break;
    case W_BIGBOMB :
                       TempBullet = new bulletBigBomb(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_BIGBOMB][color]);
                       break;
    case W_ROCKET :
                       TempBullet = new bulletRocket(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_ROCKET][color]);
                       break;
    case W_MINE :
                       TempBullet = new bulletMine(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_MINE][color]);
                       break;

    case W_FREEZER :
                       TempBullet = new bulletFreezer(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade);
                       break;

    case W_SHRINKER :
                       TempBullet = new bulletShrinker(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade);
                       break;
    case W_PROBE :
                       TempBullet = new bulletProbe(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_PROBE][color]);
                       break;
    case W_GAS :
                       TempBullet = new bulletGas(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_GAS][color]);
                       break;
    case W_LASER :
                       TempBullet = new bulletLaser(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_LASER][color]);
                       break;
    case W_BUMPER :
                       TempBullet = new bulletBumper(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_BUMPER][color]);
                       break;
    case W_SHOTGUN :
                       TempBullet = new bulletShotgun(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, pspawn);
                       break;
    case W_ACID :
                       TempBullet = new bulletAcid(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade, bullet_frames[W_ACID][color], pspawn);
                       break;
    case W_USER :
                       TempBullet = new bulletUser(psig, enr, px, py, pxspd, pyspd, pstrength, pshield, phull, puser, pupgrade);
                       break;
    default : break;

  } // case bullet type

//  TempBullet->set_upgrade(upgrade);
  TempBullet->set_net_dont_kill(net_dont_kill);
  TempBullet->set_type(t);
//  TempBullet->set_user(puser);
  add_to_list(TempBullet);

  return TempBullet;
}

void gsBullet::add_to_list(Bullet *TempBullet)
{
  if (TempBullet != NULL)
  {
    if (first_bullet==NULL)
    {
      last_bullet = first_bullet = TempBullet;
      first_bullet->set_next(NULL); first_bullet->set_prev(NULL);
      TempBullet->set_next(NULL); TempBullet->set_prev(NULL);
    }
    else
    {
      last_bullet->set_next(TempBullet);
      TempBullet->set_prev(last_bullet);
      last_bullet = TempBullet;
      TempBullet->set_next(NULL);
    }
  } // TempBullet != NULL

  count++;
}

void gsBullet::remove(Bullet *temp)
{
  Bullet *next_bullet, *prev_bullet;

  if (temp != NULL)
  {
    prev_bullet = temp->get_prev();
    next_bullet = temp->get_next();

    if (prev_bullet==NULL) first_bullet = next_bullet;
    else prev_bullet->set_next(next_bullet);

    if (next_bullet==NULL) last_bullet = prev_bullet;
    else next_bullet->set_prev(prev_bullet);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsBullet::remove_for_script(Bullet *temp)
{
  temp->remove();
}

void gsBullet::move()
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->move();
    temp = next_bullet;
  } // while temp != NULL
}

void gsBullet::draw(gsMap *m)
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->draw(m);
    temp->dec_teleport_time();
    temp = next_bullet;
  } // while temp != NULL
}

void gsBullet::draw(BITMAP *m)
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->draw(m);
    temp = next_bullet;
  } // while temp != NULL
}

void gsBullet::save_background(gsMap *m)
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->save_background(m);
    temp = next_bullet;
  } // while temp != NULL
}

void gsBullet::restore_background(gsMap *m)
{
  Bullet *temp = first_bullet, *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->restore_background(m);
    temp = next_bullet;
  } // while temp != NULL
}

void gsBullet::check_collisions(gsMap *m1, gsMap *m2)
{
  Bullet *temp = first_bullet, *next_bullet;
  int hit;

  while (temp != NULL)
  {
    hit = 0;
    next_bullet = temp->get_next();

    if (temp->check_collisions()) hit = 1;
    else if (temp->check_map_collisions(m1)) hit = 1;
    else if (m2 && temp->check_map_collisions(m2)) hit = 1;
    else if ( (m1 && m2) &&
              (!m1->is_on_map(temp->get_x(), temp->get_y(), 2, 2)) &&
              (!m2->is_on_map(temp->get_x(), temp->get_y(), 2, 2)) )
            hit = 1;

    if (hit)
    {
      if ((globals->do_all_hooks || temp->get_do_hooks()))
        script->do_hook_bullet_destroyed(glob_lua, temp);
      if (!temp->get_dont_continue()) remove(temp);
    }
    else if (temp->get_remove())
    {
      remove(temp);
    }

    temp = next_bullet;
  } // while temp != NULL
}


void gsBullet::create_bullet_sprites()
{
  int nr, nr2;
  
  for (nr=0; nr < REAL_WEAPON_COUNT; nr++)
    for (nr2=0; nr2 < 8; nr2++)
      bullet_frames[nr][nr2] = NULL;

  // imaged weapons
  for (nr=0; nr < 8; nr++)
  {
    int imgnum = nr;
    if (imgnum > 2) imgnum = 0;

    bullet_frames[W_BOMB][nr] = new Image(1);
    bullet_frames[W_BOMB][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_bomb1+imgnum);

    bullet_frames[W_BIGBOMB][nr] = new Image(1);
    bullet_frames[W_BIGBOMB][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_bigbomb1+imgnum);

    bullet_frames[W_ROCKET][nr] = new Image(1);
    bullet_frames[W_ROCKET][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_rocket1+imgnum);

    bullet_frames[W_PROBE][nr] = new Image(1);
    bullet_frames[W_PROBE][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_probe1+imgnum);

    bullet_frames[W_MINE][nr] = new Image(1);
    bullet_frames[W_MINE][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_mine1_1+imgnum);

    bullet_frames[W_GAS][nr] = new Image(1);
    bullet_frames[W_GAS][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_mine3_1+imgnum);

//    sprintf(fname, "./userweap/laser%d.tga", imgnum+1);
    bullet_frames[W_LASER][nr] = new Image(1);
    bullet_frames[W_LASER][nr]->add_frames_from_imagefile("weapon", DATA_BITMAP, 0, 0, 1, globals->gamedat, weapon_laser1+imgnum);
//    bullet_frames[W_LASER][nr]->add_frames_from_imagefile(fname, DATA_BITMAP, 0, 0, 1);

    bullet_frames[W_BUMPER][nr] = new Image(1);
    bullet_frames[W_BUMPER][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_bumper1+imgnum);

    bullet_frames[W_ACID][nr] = new Image(1);
    bullet_frames[W_ACID][nr]->add_frames_from_imagefile("weapon", DATA_RLE, 0, 0, 1, globals->gamedat, weapon_acid1+imgnum);
  }

}
