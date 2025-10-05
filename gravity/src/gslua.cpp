/*
** Lua binding: gslua
** Generated automatically by tolua 5.0a on Sat Jun 11 19:19:39 2005.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua.h"

/* Exported function */
TOLUA_API int tolua_gslua_open (lua_State* tolua_S);

#include "gsbase.h"
#include "gsplayer.h"
#include "gsobject.h"
#include "gsglob.h"
#include "gspixel.h"
#include "gscontrl.h"
#include "gsexplo.h"
#include "gsbullet.h"
#include "gsmap.h"
#include "gseffect.h"
#include "gsscript.h"
#include "gsosd.h"
#include "gserror.h"
#include "gsnetw.h"
#include "gssound.h"
#include "gsenemy.h"
#include "gsframes.h"
#include "gspanel.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_UserStats (lua_State* tolua_S)
{
 UserStats* self = (UserStats*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_level_info (lua_State* tolua_S)
{
 level_info* self = (level_info*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"UserImageAnim");
 tolua_usertype(tolua_S,"gsMap");
 tolua_usertype(tolua_S,"OsdBitmap");
 tolua_usertype(tolua_S,"gsExplosion");
 tolua_usertype(tolua_S,"Enemy");
 tolua_usertype(tolua_S,"gsError");
 tolua_usertype(tolua_S,"gsChat");
 tolua_usertype(tolua_S,"PlayerID");
 tolua_usertype(tolua_S,"gsPixelRain");
 tolua_usertype(tolua_S,"gsWaypoint");
 tolua_usertype(tolua_S,"gsObject");
 tolua_usertype(tolua_S,"Explosion");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"weaponslot");
 tolua_usertype(tolua_S,"Frame");
 tolua_usertype(tolua_S,"Crater");
 tolua_usertype(tolua_S,"gsNetPlayer");
 tolua_usertype(tolua_S,"gsImage");
 tolua_usertype(tolua_S,"gsEffect");
 tolua_usertype(tolua_S,"Bullet");
 tolua_usertype(tolua_S,"gsControl");
 tolua_usertype(tolua_S,"Base");
 tolua_usertype(tolua_S,"gsBullet");
 tolua_usertype(tolua_S,"gsPanel");
 tolua_usertype(tolua_S,"Image");
 tolua_usertype(tolua_S,"gsNetwork");
 tolua_usertype(tolua_S,"RLE_SPRITE");
 tolua_usertype(tolua_S,"Waypoint");
 tolua_usertype(tolua_S,"gsCrater");
 tolua_usertype(tolua_S,"gsGlobals");
 tolua_usertype(tolua_S,"gsOsd");
 tolua_usertype(tolua_S,"gsSound");
 tolua_usertype(tolua_S,"gsEnemy");
 tolua_usertype(tolua_S,"gsBase");
 tolua_usertype(tolua_S,"UserStats");
 tolua_usertype(tolua_S,"gsPlayer");
 tolua_usertype(tolua_S,"statistics");
 tolua_usertype(tolua_S,"Pixel");
 tolua_usertype(tolua_S,"level_info");
}

/* get function: wp of class  Object */
static int tolua_get_Object_wp_ptr(lua_State* tolua_S)
{
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'wp'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->wp,"gsWaypoint");
 return 1;
}

/* set function: wp of class  Object */
static int tolua_set_Object_wp_ptr(lua_State* tolua_S)
{
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'wp'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsWaypoint",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wp = ((gsWaypoint*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_prev of class  Object */
static int tolua_gslua_Object_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Object */
static int tolua_gslua_Object_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_prev of class  Object */
static int tolua_gslua_Object_set_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  Object* n = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_prev'",NULL);
#endif
 {
  self->set_prev(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Object */
static int tolua_gslua_Object_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  Object* n = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_type of class  Object */
static int tolua_gslua_Object_set_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_type'",NULL);
#endif
 {
  self->set_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Object */
static int tolua_gslua_Object_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_nr of class  Object */
static int tolua_gslua_Object_get_nr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_nr'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_nr();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_nr'.",&tolua_err);
 return 0;
#endif
}

/* method: is_special of class  Object */
static int tolua_gslua_Object_is_special00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_special'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_special();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_special'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframe of class  Object */
static int tolua_gslua_Object_set_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframe'",NULL);
#endif
 {
  self->set_maxframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframe of class  Object */
static int tolua_gslua_Object_get_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframet of class  Object */
static int tolua_gslua_Object_set_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframet'",NULL);
#endif
 {
  self->set_maxframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframet of class  Object */
static int tolua_gslua_Object_get_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframe of class  Object */
static int tolua_gslua_Object_set_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframe'",NULL);
#endif
 {
  self->set_curframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframe of class  Object */
static int tolua_gslua_Object_get_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframet of class  Object */
static int tolua_gslua_Object_set_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframet'",NULL);
#endif
 {
  self->set_curframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframet of class  Object */
static int tolua_gslua_Object_get_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dispframe of class  Object */
static int tolua_gslua_Object_set_dispframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dispframe'",NULL);
#endif
 {
  self->set_dispframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dispframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_dispframe of class  Object */
static int tolua_gslua_Object_get_dispframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_dispframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_dispframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_dispframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_startframe of class  Object */
static int tolua_gslua_Object_set_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_startframe'",NULL);
#endif
 {
  self->set_startframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_startframe of class  Object */
static int tolua_gslua_Object_get_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_startframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_startframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  Object */
static int tolua_gslua_Object_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Object */
static int tolua_gslua_Object_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  Object */
static int tolua_gslua_Object_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  Object */
static int tolua_gslua_Object_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Object */
static int tolua_gslua_Object_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Object */
static int tolua_gslua_Object_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  Object */
static int tolua_gslua_Object_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  Object */
static int tolua_gslua_Object_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  Object */
static int tolua_gslua_Object_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  Object */
static int tolua_gslua_Object_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_x of class  Object */
static int tolua_gslua_Object_set_bound_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_x'",NULL);
#endif
 {
  self->set_bound_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_y of class  Object */
static int tolua_gslua_Object_set_bound_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_y'",NULL);
#endif
 {
  self->set_bound_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_w of class  Object */
static int tolua_gslua_Object_set_bound_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_w'",NULL);
#endif
 {
  self->set_bound_w(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_w'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_h of class  Object */
static int tolua_gslua_Object_set_bound_h00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_h'",NULL);
#endif
 {
  self->set_bound_h(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_h'.",&tolua_err);
 return 0;
#endif
}

/* method: add_hitpoints of class  Object */
static int tolua_gslua_Object_add_hitpoints00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int from = ((int)  tolua_tonumber(tolua_S,2,0));
  float n = ((float)  tolua_tonumber(tolua_S,3,0));
  int weap = ((int)  tolua_tonumber(tolua_S,4,0));
  int fx = ((int)  tolua_tonumber(tolua_S,5,0));
  int fy = ((int)  tolua_tonumber(tolua_S,6,0));
  float fxspd = ((float)  tolua_tonumber(tolua_S,7,0));
  float fyspd = ((float)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add_hitpoints'",NULL);
#endif
 {
  self->add_hitpoints(from,n,weap,fx,fy,fxspd,fyspd);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add_hitpoints'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hit of class  Object */
static int tolua_gslua_Object_get_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hit'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxhit of class  Object */
static int tolua_gslua_Object_get_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxhit'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxhit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hit of class  Object */
static int tolua_gslua_Object_set_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hit'",NULL);
#endif
 {
  self->set_hit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxhit of class  Object */
static int tolua_gslua_Object_set_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxhit'",NULL);
#endif
 {
  self->set_maxhit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby of class  Object */
static int tolua_gslua_Object_get_hitby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_hitby();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby_sx of class  Object */
static int tolua_gslua_Object_get_hitby_sx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby_sx'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hitby_sx();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby_sx'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby_sy of class  Object */
static int tolua_gslua_Object_get_hitby_sy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby_sy'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hitby_sy();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby_sy'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user of class  Object */
static int tolua_gslua_Object_get_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user of class  Object */
static int tolua_gslua_Object_set_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user'",NULL);
#endif
 {
  self->set_user(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user'.",&tolua_err);
 return 0;
#endif
}

/* method: get_active of class  Object */
static int tolua_gslua_Object_get_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_active'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_active();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_active'.",&tolua_err);
 return 0;
#endif
}

/* method: set_active of class  Object */
static int tolua_gslua_Object_set_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_active'",NULL);
#endif
 {
  self->set_active(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_active'.",&tolua_err);
 return 0;
#endif
}

/* method: get_teleport_time of class  Object */
static int tolua_gslua_Object_get_teleport_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_teleport_time'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_teleport_time();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_teleport_time'.",&tolua_err);
 return 0;
#endif
}

/* method: get_invisible of class  Object */
static int tolua_gslua_Object_get_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_invisible'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_invisible();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: set_invisible of class  Object */
static int tolua_gslua_Object_set_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_invisible'",NULL);
#endif
 {
  self->set_invisible(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dont_continue of class  Object */
static int tolua_gslua_Object_set_dont_continue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dont_continue'",NULL);
#endif
 {
  self->set_dont_continue(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dont_continue'.",&tolua_err);
 return 0;
#endif
}

/* method: get_do_hooks of class  Object */
static int tolua_gslua_Object_get_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_do_hooks'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_do_hooks();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_do_hooks of class  Object */
static int tolua_gslua_Object_set_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_do_hooks'",NULL);
#endif
 {
  self->set_do_hooks(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dont_update of class  Object */
static int tolua_gslua_Object_set_dont_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dont_update'",NULL);
#endif
 {
  self->set_dont_update(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dont_update'.",&tolua_err);
 return 0;
#endif
}

/* method: get_dont_update of class  Object */
static int tolua_gslua_Object_get_dont_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_dont_update'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_dont_update();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_dont_update'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  Object */
static int tolua_gslua_Object_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_remove of class  Object */
static int tolua_gslua_Object_get_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_remove'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_remove();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_restrict_to_map of class  Object */
static int tolua_gslua_Object_get_restrict_to_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_restrict_to_map'",NULL);
#endif
 {
  class gsMap* tolua_ret = (class gsMap*)  self->get_restrict_to_map();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"gsMap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_restrict_to_map'.",&tolua_err);
 return 0;
#endif
}

/* method: set_restrict_to_map of class  Object */
static int tolua_gslua_Object_set_restrict_to_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  class gsMap* tolua_var_1 = ((class gsMap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_restrict_to_map'",NULL);
#endif
 {
  self->set_restrict_to_map(tolua_var_1);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_restrict_to_map'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_int of class  Object */
static int tolua_gslua_Object_set_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int d = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_int'",NULL);
#endif
 {
  self->set_user_int(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_float of class  Object */
static int tolua_gslua_Object_set_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  float d = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_float'",NULL);
#endif
 {
  self->set_user_float(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_int of class  Object */
static int tolua_gslua_Object_get_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_int'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user_int(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_float of class  Object */
static int tolua_gslua_Object_get_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_float'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_user_float(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: script_redraw of class  Object */
static int tolua_gslua_Object_script_redraw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'script_redraw'",NULL);
#endif
 {
  self->script_redraw();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first_object of class  gsObject */
static int tolua_gslua_gsObject_get_first_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first_object'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_first_object();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first_object'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last_object of class  gsObject */
static int tolua_gslua_gsObject_get_last_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last_object'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_last_object();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last_object'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first_spobject of class  gsObject */
static int tolua_gslua_gsObject_get_first_spobject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first_spobject'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_first_spobject();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first_spobject'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last_spobject of class  gsObject */
static int tolua_gslua_gsObject_get_last_spobject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last_spobject'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_last_spobject();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last_spobject'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsObject */
static int tolua_gslua_gsObject_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsObject */
static int tolua_gslua_gsObject_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  int t = ((int)  tolua_tonumber(tolua_S,3,0));
  int x = ((int)  tolua_tonumber(tolua_S,4,0));
  int y = ((int)  tolua_tonumber(tolua_S,5,0));
  int user = ((int)  tolua_tonumber(tolua_S,6,0));
  int pf = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->add(num,t,x,y,user,pf);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: add_special of class  gsObject */
static int tolua_gslua_gsObject_add_special00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  int t = ((int)  tolua_tonumber(tolua_S,3,0));
  int x = ((int)  tolua_tonumber(tolua_S,4,0));
  int y = ((int)  tolua_tonumber(tolua_S,5,0));
  int user = ((int)  tolua_tonumber(tolua_S,6,0));
  int pf = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add_special'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->add_special(num,t,x,y,user,pf);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add_special'.",&tolua_err);
 return 0;
#endif
}

/* method: remove_for_script of class  gsObject */
static int tolua_gslua_gsObject_remove_for_script00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
  Object* tolua_var_2 = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove_for_script'",NULL);
#endif
 {
  self->remove_for_script(tolua_var_2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_object of class  gsObject */
static int tolua_gslua_gsObject_get_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_4 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_object'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_object(tolua_var_4);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_object'.",&tolua_err);
 return 0;
#endif
}

/* method: get_spobject of class  gsObject */
static int tolua_gslua_gsObject_get_spobject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsObject",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsObject* self = (gsObject*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_5 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_spobject'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_spobject(tolua_var_5);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_spobject'.",&tolua_err);
 return 0;
#endif
}

/* get function: objects */
static int tolua_get_objects_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)objects,"gsObject");
 return 1;
}

/* set function: objects */
static int tolua_set_objects_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsObject",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  objects = ((gsObject*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_x of class  Bullet */
static int tolua_gslua_Bullet_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Bullet */
static int tolua_gslua_Bullet_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  Bullet */
static int tolua_gslua_Bullet_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  Bullet */
static int tolua_gslua_Bullet_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  Bullet */
static int tolua_gslua_Bullet_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Bullet */
static int tolua_gslua_Bullet_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Bullet */
static int tolua_gslua_Bullet_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Bullet */
static int tolua_gslua_Bullet_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  Bullet */
static int tolua_gslua_Bullet_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  Bullet */
static int tolua_gslua_Bullet_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  Bullet */
static int tolua_gslua_Bullet_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  Bullet */
static int tolua_gslua_Bullet_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: set_width of class  Bullet */
static int tolua_gslua_Bullet_set_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_width'",NULL);
#endif
 {
  self->set_width(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_width'.",&tolua_err);
 return 0;
#endif
}

/* method: set_height of class  Bullet */
static int tolua_gslua_Bullet_set_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_height'",NULL);
#endif
 {
  self->set_height(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_min_x of class  Bullet */
static int tolua_gslua_Bullet_get_min_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_min_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_min_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_min_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_min_y of class  Bullet */
static int tolua_gslua_Bullet_get_min_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_min_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_min_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_min_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_x of class  Bullet */
static int tolua_gslua_Bullet_get_max_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_max_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_y of class  Bullet */
static int tolua_gslua_Bullet_get_max_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_max_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_min_x of class  Bullet */
static int tolua_gslua_Bullet_set_min_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_min_x'",NULL);
#endif
 {
  self->set_min_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_min_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_min_y of class  Bullet */
static int tolua_gslua_Bullet_set_min_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_min_y'",NULL);
#endif
 {
  self->set_min_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_min_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_x of class  Bullet */
static int tolua_gslua_Bullet_set_max_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_x'",NULL);
#endif
 {
  self->set_max_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_y of class  Bullet */
static int tolua_gslua_Bullet_set_max_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_y'",NULL);
#endif
 {
  self->set_max_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_by_player of class  Bullet */
static int tolua_gslua_Bullet_get_by_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_by_player'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_by_player();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_by_player'.",&tolua_err);
 return 0;
#endif
}

/* method: set_by_player of class  Bullet */
static int tolua_gslua_Bullet_set_by_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_by_player'",NULL);
#endif
 {
  self->set_by_player(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_by_player'.",&tolua_err);
 return 0;
#endif
}

/* method: get_by_enemy of class  Bullet */
static int tolua_gslua_Bullet_get_by_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_by_enemy'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_by_enemy();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_by_enemy'.",&tolua_err);
 return 0;
#endif
}

/* method: set_by_enemy of class  Bullet */
static int tolua_gslua_Bullet_set_by_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_by_enemy'",NULL);
#endif
 {
  self->set_by_enemy(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_by_enemy'.",&tolua_err);
 return 0;
#endif
}

/* method: get_strength of class  Bullet */
static int tolua_gslua_Bullet_get_strength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_strength'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_strength();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_strength'.",&tolua_err);
 return 0;
#endif
}

/* method: set_strength of class  Bullet */
static int tolua_gslua_Bullet_set_strength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_strength'",NULL);
#endif
 {
  self->set_strength(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_strength'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hull_hitp of class  Bullet */
static int tolua_gslua_Bullet_get_hull_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hull_hitp'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hull_hitp();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hull_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hull_hitp of class  Bullet */
static int tolua_gslua_Bullet_set_hull_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hull_hitp'",NULL);
#endif
 {
  self->set_hull_hitp(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hull_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shield_hitp of class  Bullet */
static int tolua_gslua_Bullet_get_shield_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shield_hitp'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_shield_hitp();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shield_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: set_shield_hitp of class  Bullet */
static int tolua_gslua_Bullet_set_shield_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_shield_hitp'",NULL);
#endif
 {
  self->set_shield_hitp(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_shield_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Bullet */
static int tolua_gslua_Bullet_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_ttl of class  Bullet */
static int tolua_gslua_Bullet_get_ttl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_ttl'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_ttl();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_ttl'.",&tolua_err);
 return 0;
#endif
}

/* method: set_ttl of class  Bullet */
static int tolua_gslua_Bullet_set_ttl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_ttl'",NULL);
#endif
 {
  self->set_ttl(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_ttl'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user of class  Bullet */
static int tolua_gslua_Bullet_get_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user of class  Bullet */
static int tolua_gslua_Bullet_set_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user'",NULL);
#endif
 {
  self->set_user(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user'.",&tolua_err);
 return 0;
#endif
}

/* method: get_invisible of class  Bullet */
static int tolua_gslua_Bullet_get_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_invisible'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_invisible();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: set_invisible of class  Bullet */
static int tolua_gslua_Bullet_set_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_invisible'",NULL);
#endif
 {
  self->set_invisible(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: get_upgrade of class  Bullet */
static int tolua_gslua_Bullet_get_upgrade00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_upgrade'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_upgrade();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_upgrade'.",&tolua_err);
 return 0;
#endif
}

/* method: set_upgrade of class  Bullet */
static int tolua_gslua_Bullet_set_upgrade00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_upgrade'",NULL);
#endif
 {
  self->set_upgrade(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_upgrade'.",&tolua_err);
 return 0;
#endif
}

/* method: get_target of class  Bullet */
static int tolua_gslua_Bullet_get_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_target'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_target();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_target'.",&tolua_err);
 return 0;
#endif
}

/* method: set_target of class  Bullet */
static int tolua_gslua_Bullet_set_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_target'",NULL);
#endif
 {
  self->set_target(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_target'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hit of class  Bullet */
static int tolua_gslua_Bullet_set_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hit'",NULL);
#endif
 {
  self->set_hit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxhit of class  Bullet */
static int tolua_gslua_Bullet_set_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxhit'",NULL);
#endif
 {
  self->set_maxhit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hit of class  Bullet */
static int tolua_gslua_Bullet_get_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hit'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxhit of class  Bullet */
static int tolua_gslua_Bullet_get_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxhit'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxhit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_dont_continue of class  Bullet */
static int tolua_gslua_Bullet_get_dont_continue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_dont_continue'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_dont_continue();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_dont_continue'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dont_continue of class  Bullet */
static int tolua_gslua_Bullet_set_dont_continue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dont_continue'",NULL);
#endif
 {
  self->set_dont_continue(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dont_continue'.",&tolua_err);
 return 0;
#endif
}

/* method: get_do_hooks of class  Bullet */
static int tolua_gslua_Bullet_get_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_do_hooks'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_do_hooks();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_do_hooks of class  Bullet */
static int tolua_gslua_Bullet_set_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_do_hooks'",NULL);
#endif
 {
  self->set_do_hooks(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  Bullet */
static int tolua_gslua_Bullet_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_remove of class  Bullet */
static int tolua_gslua_Bullet_get_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_remove'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_remove();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_remove'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_int of class  Bullet */
static int tolua_gslua_Bullet_set_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int d = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_int'",NULL);
#endif
 {
  self->set_user_int(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_float of class  Bullet */
static int tolua_gslua_Bullet_set_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  float d = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_float'",NULL);
#endif
 {
  self->set_user_float(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_int of class  Bullet */
static int tolua_gslua_Bullet_get_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_int'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user_int(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_float of class  Bullet */
static int tolua_gslua_Bullet_get_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Bullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Bullet* self = (Bullet*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_float'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_user_float(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsBullet */
static int tolua_gslua_gsBullet_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first of class  gsBullet */
static int tolua_gslua_gsBullet_get_first00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->get_first();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsBullet */
static int tolua_gslua_gsBullet_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
  int t = ((int)  tolua_tonumber(tolua_S,2,0));
  class gsPlayer* tolua_var_6 = ((class gsPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->add(t,tolua_var_6);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsBullet */
static int tolua_gslua_gsBullet_add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,11,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,12,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,13,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
  int t = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned long psig = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
  int enemy_nr = ((int)  tolua_tonumber(tolua_S,4,0));
  int px = ((int)  tolua_tonumber(tolua_S,5,0));
  int py = ((int)  tolua_tonumber(tolua_S,6,0));
  float pxspd = ((float)  tolua_tonumber(tolua_S,7,0));
  float pyspd = ((float)  tolua_tonumber(tolua_S,8,0));
  float pstrength = ((float)  tolua_tonumber(tolua_S,9,0));
  float puser = ((float)  tolua_tonumber(tolua_S,10,0));
  int pupgrade = ((int)  tolua_tonumber(tolua_S,11,0));
  int net_dont_kill = ((int)  tolua_tonumber(tolua_S,12,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->add(t,psig,enemy_nr,px,py,pxspd,pyspd,pstrength,puser,pupgrade,net_dont_kill);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
tolua_lerror:
 return tolua_gslua_gsBullet_add00(tolua_S);
}

/* method: add of class  gsBullet */
static int tolua_gslua_gsBullet_add02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,11,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,12,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,13,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,14,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,15,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
  int t = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned long psig = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
  int enemy_nr = ((int)  tolua_tonumber(tolua_S,4,0));
  int px = ((int)  tolua_tonumber(tolua_S,5,0));
  int py = ((int)  tolua_tonumber(tolua_S,6,0));
  float pxspd = ((float)  tolua_tonumber(tolua_S,7,0));
  float pyspd = ((float)  tolua_tonumber(tolua_S,8,0));
  float pstrength = ((float)  tolua_tonumber(tolua_S,9,0));
  int pshield = ((int)  tolua_tonumber(tolua_S,10,0));
  int phull = ((int)  tolua_tonumber(tolua_S,11,0));
  float puser = ((float)  tolua_tonumber(tolua_S,12,0));
  int pupgrade = ((int)  tolua_tonumber(tolua_S,13,0));
  int net_dont_kill = ((int)  tolua_tonumber(tolua_S,14,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Bullet* tolua_ret = (Bullet*)  self->add(t,psig,enemy_nr,px,py,pxspd,pyspd,pstrength,pshield,phull,puser,pupgrade,net_dont_kill);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bullet");
 }
 }
 return 1;
tolua_lerror:
 return tolua_gslua_gsBullet_add01(tolua_S);
}

/* method: remove_for_script of class  gsBullet */
static int tolua_gslua_gsBullet_remove_for_script00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBullet",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Bullet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBullet* self = (gsBullet*)  tolua_tousertype(tolua_S,1,0);
  Bullet* tolua_var_7 = ((Bullet*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove_for_script'",NULL);
#endif
 {
  self->remove_for_script(tolua_var_7);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* get function: type of class  weaponslot */
static int tolua_get_weaponslot_type(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}

/* set function: type of class  weaponslot */
static int tolua_set_weaponslot_type(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->type = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bullets of class  weaponslot */
static int tolua_get_weaponslot_bullets(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bullets);
 return 1;
}

/* set function: bullets of class  weaponslot */
static int tolua_set_weaponslot_bullets(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bullets = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: reload of class  weaponslot */
static int tolua_get_weaponslot_reload(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'reload'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->reload);
 return 1;
}

/* set function: reload of class  weaponslot */
static int tolua_set_weaponslot_reload(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'reload'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->reload = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: cur_reload of class  weaponslot */
static int tolua_get_weaponslot_cur_reload(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'cur_reload'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->cur_reload);
 return 1;
}

/* set function: cur_reload of class  weaponslot */
static int tolua_set_weaponslot_cur_reload(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'cur_reload'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cur_reload = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: upgrade of class  weaponslot */
static int tolua_get_weaponslot_upgrade(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'upgrade'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->upgrade);
 return 1;
}

/* set function: upgrade of class  weaponslot */
static int tolua_set_weaponslot_upgrade(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'upgrade'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->upgrade = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_upgrades of class  weaponslot */
static int tolua_get_weaponslot_max_upgrades(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_upgrades'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_upgrades);
 return 1;
}

/* set function: max_upgrades of class  weaponslot */
static int tolua_set_weaponslot_max_upgrades(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_upgrades'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_upgrades = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: active of class  weaponslot */
static int tolua_get_weaponslot_active(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'active'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->active);
 return 1;
}

/* set function: active of class  weaponslot */
static int tolua_set_weaponslot_active(lua_State* tolua_S)
{
  weaponslot* self = (weaponslot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'active'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->active = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: pname of class  statistics */
static int tolua_get_statistics_pname(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'pname'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->pname);
 return 1;
}

/* get function: frags of class  statistics */
static int tolua_get_statistics_frags(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'frags'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->frags);
 return 1;
}

/* set function: frags of class  statistics */
static int tolua_set_statistics_frags(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'frags'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->frags = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: suicides of class  statistics */
static int tolua_get_statistics_suicides(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'suicides'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->suicides);
 return 1;
}

/* set function: suicides of class  statistics */
static int tolua_set_statistics_suicides(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'suicides'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->suicides = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bullets_shot of class  statistics */
static int tolua_get_statistics_bullets_shot(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets_shot'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bullets_shot);
 return 1;
}

/* set function: bullets_shot of class  statistics */
static int tolua_set_statistics_bullets_shot(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets_shot'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bullets_shot = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bullets_hit of class  statistics */
static int tolua_get_statistics_bullets_hit(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets_hit'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bullets_hit);
 return 1;
}

/* set function: bullets_hit of class  statistics */
static int tolua_set_statistics_bullets_hit(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets_hit'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bullets_hit = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: extras_taken of class  statistics */
static int tolua_get_statistics_extras_taken(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'extras_taken'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->extras_taken);
 return 1;
}

/* set function: extras_taken of class  statistics */
static int tolua_set_statistics_extras_taken(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'extras_taken'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extras_taken = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_time_alive of class  statistics */
static int tolua_get_statistics_max_time_alive(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_time_alive'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_time_alive);
 return 1;
}

/* set function: max_time_alive of class  statistics */
static int tolua_set_statistics_max_time_alive(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_time_alive'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_time_alive = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: laptime of class  statistics */
static int tolua_get_gslua_statistics_laptime(lua_State* tolua_S)
{
 int tolua_index;
  statistics* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (statistics*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=10)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->laptime[tolua_index]);
 return 1;
}

/* set function: laptime of class  statistics */
static int tolua_set_gslua_statistics_laptime(lua_State* tolua_S)
{
 int tolua_index;
  statistics* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (statistics*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=10)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
  self->laptime[tolua_index] = ((int)  tolua_tonumber(tolua_S,3,0));
 return 0;
}

/* get function: bonus of class  statistics */
static int tolua_get_statistics_bonus(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bonus'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bonus);
 return 1;
}

/* set function: bonus of class  statistics */
static int tolua_set_statistics_bonus(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bonus'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bonus = ((long int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: enemy_bonus of class  statistics */
static int tolua_get_statistics_enemy_bonus(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'enemy_bonus'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->enemy_bonus);
 return 1;
}

/* set function: enemy_bonus of class  statistics */
static int tolua_set_statistics_enemy_bonus(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'enemy_bonus'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->enemy_bonus = ((long int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: mission_status of class  statistics */
static int tolua_get_statistics_mission_status(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'mission_status'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->mission_status);
 return 1;
}

/* set function: mission_status of class  statistics */
static int tolua_set_statistics_mission_status(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'mission_status'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mission_status = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: lostlifes of class  statistics */
static int tolua_get_statistics_lostlifes(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'lostlifes'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->lostlifes);
 return 1;
}

/* set function: lostlifes of class  statistics */
static int tolua_set_statistics_lostlifes(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'lostlifes'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lostlifes = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: used_time of class  statistics */
static int tolua_get_statistics_used_time(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'used_time'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->used_time);
 return 1;
}

/* set function: used_time of class  statistics */
static int tolua_set_statistics_used_time(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'used_time'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->used_time = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: plsig of class  statistics */
static int tolua_get_statistics_plsig(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plsig'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->plsig);
 return 1;
}

/* set function: plsig of class  statistics */
static int tolua_set_statistics_plsig(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plsig'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->plsig = ((long int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: set of class  statistics */
static int tolua_get_statistics_set(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'set'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->set);
 return 1;
}

/* set function: set of class  statistics */
static int tolua_set_statistics_set(lua_State* tolua_S)
{
  statistics* self = (statistics*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'set'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->set = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* get function: controls of class  gsPlayer */
static int tolua_get_gsPlayer_controls_ptr(lua_State* tolua_S)
{
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'controls'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->controls,"gsControl");
 return 1;
}

/* set function: controls of class  gsPlayer */
static int tolua_set_gsPlayer_controls_ptr(lua_State* tolua_S)
{
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'controls'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsControl",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->controls = ((class gsControl*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_x of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xacc of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_xacc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xacc'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xacc();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xacc'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yacc of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_yacc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yacc'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yacc();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yacc'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xacc of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_xacc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xacc'",NULL);
#endif
 {
  self->set_xacc(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xacc'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yacc of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_yacc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yacc'",NULL);
#endif
 {
  self->set_yacc(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yacc'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xacc2 of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_xacc200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xacc2'",NULL);
#endif
 {
  self->set_xacc2(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xacc2'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yacc2 of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_yacc200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yacc2'",NULL);
#endif
 {
  self->set_yacc2(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yacc2'.",&tolua_err);
 return 0;
#endif
}

/* method: set_thrust of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_thrust00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_thrust'",NULL);
#endif
 {
  self->set_thrust();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_thrust'.",&tolua_err);
 return 0;
#endif
}

/* method: get_thrust of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_thrust00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_thrust'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_thrust();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_thrust'.",&tolua_err);
 return 0;
#endif
}

/* method: show_thrust_frame of class  gsPlayer */
static int tolua_gslua_gsPlayer_show_thrust_frame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'show_thrust_frame'",NULL);
#endif
 {
  self->show_thrust_frame(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'show_thrust_frame'.",&tolua_err);
 return 0;
#endif
}

/* method: set_extra_action of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_extra_action00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_extra_action'",NULL);
#endif
 {
  self->set_extra_action();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_extra_action'.",&tolua_err);
 return 0;
#endif
}

/* method: get_extra_action of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_extra_action00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_extra_action'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_extra_action();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_extra_action'.",&tolua_err);
 return 0;
#endif
}

/* method: get_extra of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_extra00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_extra'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_extra();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_extra'.",&tolua_err);
 return 0;
#endif
}

/* method: set_extra of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_extra00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_extra'",NULL);
#endif
 {
  self->set_extra(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_extra'.",&tolua_err);
 return 0;
#endif
}

/* method: get_lifes of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_lifes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_lifes'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_lifes();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_lifes'.",&tolua_err);
 return 0;
#endif
}

/* method: set_lifes of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_lifes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_lifes'",NULL);
#endif
 {
  self->set_lifes(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_lifes'.",&tolua_err);
 return 0;
#endif
}

/* method: set_invulnerability of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_invulnerability00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_invulnerability'",NULL);
#endif
 {
  self->set_invulnerability(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_invulnerability'.",&tolua_err);
 return 0;
#endif
}

/* method: get_invulnerability of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_invulnerability00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_invulnerability'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_invulnerability();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_invulnerability'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_owidth of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_owidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_owidth'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_owidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_owidth'.",&tolua_err);
 return 0;
#endif
}

/* method: get_oheight of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_oheight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_oheight'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_oheight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_oheight'.",&tolua_err);
 return 0;
#endif
}

/* method: get_back_width of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_back_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_back_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_back_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_back_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_back_height of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_back_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_back_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_back_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_back_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shield_w of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shield_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shield_w'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_shield_w();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shield_w'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shield_h of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shield_h00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shield_h'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_shield_h();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shield_h'.",&tolua_err);
 return 0;
#endif
}

/* method: get_head of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_head00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_head'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_head();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_head'.",&tolua_err);
 return 0;
#endif
}

/* method: set_head of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_head00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_head'",NULL);
#endif
 {
  self->set_head(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_head'.",&tolua_err);
 return 0;
#endif
}

/* method: get_head_to of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_head_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_head_to'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_head_to();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_head_to'.",&tolua_err);
 return 0;
#endif
}

/* method: set_head_to of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_head_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_head_to'",NULL);
#endif
 {
  self->set_head_to(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_head_to'.",&tolua_err);
 return 0;
#endif
}

/* method: turn_to of class  gsPlayer */
static int tolua_gslua_gsPlayer_turn_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'turn_to'",NULL);
#endif
 {
  self->turn_to(x,y);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'turn_to'.",&tolua_err);
 return 0;
#endif
}

/* method: get_network of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_network00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_network'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_network();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_network'.",&tolua_err);
 return 0;
#endif
}

/* method: get_sig of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_sig'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->get_sig();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: set_sig of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  unsigned long a = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_sig'",NULL);
#endif
 {
  self->set_sig(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: get_frags of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_frags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_frags'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_frags();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_frags'.",&tolua_err);
 return 0;
#endif
}

/* method: set_frags of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_frags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_frags'",NULL);
#endif
 {
  self->set_frags(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_frags'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_frags of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_frags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_frags'",NULL);
#endif
 {
  self->inc_frags();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_frags'.",&tolua_err);
 return 0;
#endif
}

/* method: dec_frags of class  gsPlayer */
static int tolua_gslua_gsPlayer_dec_frags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'dec_frags'",NULL);
#endif
 {
  self->dec_frags();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'dec_frags'.",&tolua_err);
 return 0;
#endif
}

/* method: get_suicides of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_suicides00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_suicides'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_suicides();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_suicides'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_suicides of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_suicides00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_suicides'",NULL);
#endif
 {
  self->inc_suicides();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_suicides'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_bonus of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_bonus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_bonus'",NULL);
#endif
 {
  self->inc_bonus(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_bonus'.",&tolua_err);
 return 0;
#endif
}

/* method: dec_bonus of class  gsPlayer */
static int tolua_gslua_gsPlayer_dec_bonus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'dec_bonus'",NULL);
#endif
 {
  self->dec_bonus(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'dec_bonus'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_enemy_bonus of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_enemy_bonus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_enemy_bonus'",NULL);
#endif
 {
  self->inc_enemy_bonus(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_enemy_bonus'.",&tolua_err);
 return 0;
#endif
}

/* method: dec_enemy_bonus of class  gsPlayer */
static int tolua_gslua_gsPlayer_dec_enemy_bonus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'dec_enemy_bonus'",NULL);
#endif
 {
  self->dec_enemy_bonus(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'dec_enemy_bonus'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_bullets_shot of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_bullets_shot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_bullets_shot'",NULL);
#endif
 {
  self->inc_bullets_shot(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_bullets_shot'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullets_shot of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_bullets_shot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullets_shot'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_bullets_shot();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullets_shot'.",&tolua_err);
 return 0;
#endif
}

/* method: inc_bullets_hit of class  gsPlayer */
static int tolua_gslua_gsPlayer_inc_bullets_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'inc_bullets_hit'",NULL);
#endif
 {
  self->inc_bullets_hit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'inc_bullets_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullets_hit of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_bullets_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullets_hit'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_bullets_hit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullets_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_mission_status of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_mission_status00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_mission_status'",NULL);
#endif
 {
  self->set_mission_status(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_mission_status'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bonus of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_bonus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bonus'",NULL);
#endif
 {
  long int tolua_ret = (long int)  self->get_bonus();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bonus'.",&tolua_err);
 return 0;
#endif
}

/* method: get_statistics of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_statistics00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_statistics'",NULL);
#endif
 {
  statistics* tolua_ret = (statistics*)  self->get_statistics();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"statistics");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_statistics'.",&tolua_err);
 return 0;
#endif
}

/* method: set_statistics of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_statistics00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"statistics",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  statistics* n = ((statistics*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_statistics'",NULL);
#endif
 {
  self->set_statistics(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_statistics'.",&tolua_err);
 return 0;
#endif
}

/* method: get_homebase of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_homebase00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_homebase'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_homebase();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_homebase'.",&tolua_err);
 return 0;
#endif
}

/* method: set_homebase of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_homebase00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_8 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_homebase'",NULL);
#endif
 {
  self->set_homebase(tolua_var_8);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_homebase'.",&tolua_err);
 return 0;
#endif
}

/* method: get_home_x of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_home_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_home_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_home_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_home_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_home_y of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_home_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_home_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_home_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_home_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_home_x of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_home_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int a = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_home_x'",NULL);
#endif
 {
  self->set_home_x(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_home_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_home_y of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_home_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int a = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_home_y'",NULL);
#endif
 {
  self->set_home_y(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_home_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_name of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_name'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_name();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_name'.",&tolua_err);
 return 0;
#endif
}

/* method: set_name of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_name'",NULL);
#endif
 {
  self->set_name(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_name'.",&tolua_err);
 return 0;
#endif
}

/* method: get_ship_name of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_ship_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_ship_name'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_ship_name();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_ship_name'.",&tolua_err);
 return 0;
#endif
}

/* method: set_ship_name of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_ship_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_ship_name'",NULL);
#endif
 {
  self->set_ship_name(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_ship_name'.",&tolua_err);
 return 0;
#endif
}

/* method: get_ship_desc of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_ship_desc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_ship_desc'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_ship_desc();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_ship_desc'.",&tolua_err);
 return 0;
#endif
}

/* method: set_ship_desc of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_ship_desc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_ship_desc'",NULL);
#endif
 {
  self->set_ship_desc(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_ship_desc'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shipimage_filename of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shipimage_filename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shipimage_filename'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_shipimage_filename();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shipimage_filename'.",&tolua_err);
 return 0;
#endif
}

/* method: set_percent_value of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_percent_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int v = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_percent_value'",NULL);
#endif
 {
  self->set_percent_value(n,v);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_percent_value'.",&tolua_err);
 return 0;
#endif
}

/* method: get_percent_value of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_percent_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_percent_value'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_percent_value(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_percent_value'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dead of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_dead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dead'",NULL);
#endif
 {
  self->set_dead();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dead'.",&tolua_err);
 return 0;
#endif
}

/* method: get_dead of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_dead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_dead'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_dead();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_dead'.",&tolua_err);
 return 0;
#endif
}

/* method: set_freezed of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_freezed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_freezed'",NULL);
#endif
 {
  self->set_freezed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_freezed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_freezed of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_freezed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_freezed'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_freezed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_freezed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_shrinked of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_shrinked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_shrinked'",NULL);
#endif
 {
  self->set_shrinked(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_shrinked'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shrinked of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shrinked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shrinked'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_shrinked();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shrinked'.",&tolua_err);
 return 0;
#endif
}

/* method: set_delirium of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_delirium00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_delirium'",NULL);
#endif
 {
  self->set_delirium(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_delirium'.",&tolua_err);
 return 0;
#endif
}

/* method: get_delirium of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_delirium00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_delirium'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_delirium();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_delirium'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_hitby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_hitby();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby'.",&tolua_err);
 return 0;
#endif
}

/* method: get_killedby of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_killedby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_killedby'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_killedby();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_killedby'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hitby of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_hitby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hitby'",NULL);
#endif
 {
  self->set_hitby(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hitby'.",&tolua_err);
 return 0;
#endif
}

/* method: set_killedby of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_killedby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_killedby'",NULL);
#endif
 {
  self->set_killedby(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_killedby'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitbyweap of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_hitbyweap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitbyweap'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_hitbyweap();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitbyweap'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hitbyweap of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_hitbyweap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hitbyweap'",NULL);
#endif
 {
  self->set_hitbyweap(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hitbyweap'.",&tolua_err);
 return 0;
#endif
}

/* method: get_time_alive of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_time_alive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_time_alive'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_time_alive();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_time_alive'.",&tolua_err);
 return 0;
#endif
}

/* method: get_damage of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_damage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_damage'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_damage();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_damage'.",&tolua_err);
 return 0;
#endif
}

/* method: set_damage of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_damage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_damage'",NULL);
#endif
 {
  self->set_damage(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_damage'.",&tolua_err);
 return 0;
#endif
}

/* method: set_land of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_land00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_land'",NULL);
#endif
 {
  self->set_land(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_land'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_land00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_land();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land_dur of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_land_dur00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land_dur'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_land_dur();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land_dur'.",&tolua_err);
 return 0;
#endif
}

/* method: set_land_base of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_land_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_land_base'",NULL);
#endif
 {
  self->set_land_base(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_land_base'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land_base of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_land_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land_base'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_land_base();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land_base'.",&tolua_err);
 return 0;
#endif
}

/* method: get_weight of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weight'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_weight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weight'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weight of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weight'",NULL);
#endif
 {
  self->set_weight(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weight'.",&tolua_err);
 return 0;
#endif
}

/* method: get_weight_factor of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weight_factor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weight_factor'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_weight_factor();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weight_factor'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weight_factor of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weight_factor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weight_factor'",NULL);
#endif
 {
  self->set_weight_factor(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weight_factor'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hull of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_hull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hull'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hull();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hull'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shield'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_shield();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: get_fuel of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_fuel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_fuel'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_fuel();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_fuel'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hull of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_hull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hull'",NULL);
#endif
 {
  self->set_hull(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hull'.",&tolua_err);
 return 0;
#endif
}

/* method: set_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_shield'",NULL);
#endif
 {
  self->set_shield(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: set_fuel of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_fuel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_fuel'",NULL);
#endif
 {
  self->set_fuel(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_fuel'.",&tolua_err);
 return 0;
#endif
}

/* method: set_wall_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_wall_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_wall_shield'",NULL);
#endif
 {
  self->set_wall_shield(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_wall_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: get_wall_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_wall_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_wall_shield'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_wall_shield();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_wall_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: set_wall_shield_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_wall_shield_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_wall_shield_recharge'",NULL);
#endif
 {
  self->set_wall_shield_recharge(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_wall_shield_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: get_wall_shield_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_wall_shield_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_wall_shield_recharge'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_wall_shield_recharge(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_wall_shield_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_fuel of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_fuel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_fuel'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_max_fuel();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_fuel'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_hull of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_hull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_hull'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_max_hull(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_hull'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_shield'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_max_shield(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_max_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_shield'",NULL);
#endif
 {
  self->set_max_shield(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_hull of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_max_hull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_hull'",NULL);
#endif
 {
  self->set_max_hull(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_hull'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_wall_shield of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_wall_shield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_wall_shield'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_max_wall_shield();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_wall_shield'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hull_repair of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_hull_repair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hull_repair'",NULL);
#endif
 {
  self->set_hull_repair(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hull_repair'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hull_repair of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_hull_repair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hull_repair'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hull_repair(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hull_repair'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shield_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_shield_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shield_recharge'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_shield_recharge(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shield_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: set_shield_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_shield_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_shield_recharge'",NULL);
#endif
 {
  self->set_shield_recharge(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_shield_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: get_fuel_dec of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_fuel_dec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_fuel_dec'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_fuel_dec(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_fuel_dec'.",&tolua_err);
 return 0;
#endif
}

/* method: set_fuel_dec of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_fuel_dec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_fuel_dec'",NULL);
#endif
 {
  self->set_fuel_dec(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_fuel_dec'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_speed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_max_speed(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_max_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_speed'",NULL);
#endif
 {
  self->set_max_speed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_player_acc of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_player_acc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_player_acc'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_player_acc(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player_acc'.",&tolua_err);
 return 0;
#endif
}

/* method: set_player_acc of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_player_acc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_player_acc'",NULL);
#endif
 {
  self->set_player_acc(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_player_acc'.",&tolua_err);
 return 0;
#endif
}

/* method: get_turn_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_turn_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_turn_speed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_turn_speed(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_turn_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_turn_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_turn_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_turn_speed'",NULL);
#endif
 {
  self->set_turn_speed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_turn_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_ship_type of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_ship_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  predefined_ships n = ((predefined_ships) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_ship_type'",NULL);
#endif
 {
  self->set_ship_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_ship_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_ship_type of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_ship_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_ship_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_ship_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_ship_type'.",&tolua_err);
 return 0;
#endif
}

/* method: set_max_radar_zoom of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_max_radar_zoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_max_radar_zoom'",NULL);
#endif
 {
  self->set_max_radar_zoom(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_max_radar_zoom'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_radar_zoom of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_radar_zoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_radar_zoom'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_max_radar_zoom();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_radar_zoom'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weapon_storage of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weapon_storage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weapon_storage'",NULL);
#endif
 {
  self->set_weapon_storage(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weapon_storage'.",&tolua_err);
 return 0;
#endif
}

/* method: get_weapon_storage of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weapon_storage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weapon_storage'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_weapon_storage();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weapon_storage'.",&tolua_err);
 return 0;
#endif
}

/* method: get_locked_player of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_locked_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_locked_player'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_locked_player();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_locked_player'.",&tolua_err);
 return 0;
#endif
}

/* method: get_rocket_turn_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_rocket_turn_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_rocket_turn_speed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_rocket_turn_speed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_rocket_turn_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_rocket_turn_speed of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_rocket_turn_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_rocket_turn_speed'",NULL);
#endif
 {
  self->set_rocket_turn_speed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_rocket_turn_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_weapon_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weapon_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weapon_recharge'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_weapon_recharge();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weapon_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weapon_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weapon_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weapon_recharge'",NULL);
#endif
 {
  self->set_weapon_recharge(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weapon_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: get_extra_power of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_extra_power00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_extra_power'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_extra_power();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_extra_power'.",&tolua_err);
 return 0;
#endif
}

/* method: set_extra_power of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_extra_power00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_extra_power'",NULL);
#endif
 {
  self->set_extra_power(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_extra_power'.",&tolua_err);
 return 0;
#endif
}

/* method: get_extra_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_extra_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int h = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_extra_recharge'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_extra_recharge(h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_extra_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: set_extra_recharge of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_extra_recharge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_extra_recharge'",NULL);
#endif
 {
  self->set_extra_recharge(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_extra_recharge'.",&tolua_err);
 return 0;
#endif
}

/* method: get_max_extra_power of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_max_extra_power00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_max_extra_power'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_max_extra_power();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_max_extra_power'.",&tolua_err);
 return 0;
#endif
}

/* method: get_active_weapon of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_active_weapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_active_weapon'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_active_weapon();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_active_weapon'.",&tolua_err);
 return 0;
#endif
}

/* method: set_active_weapon of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_active_weapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_active_weapon'",NULL);
#endif
 {
  self->set_active_weapon(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_active_weapon'.",&tolua_err);
 return 0;
#endif
}

/* method: get_extra of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_extra01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_extra'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_extra();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_gslua_gsPlayer_get_extra00(tolua_S);
}

/* method: set_extra of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_extra01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_extra'",NULL);
#endif
 {
  self->set_extra(n);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsPlayer_set_extra00(tolua_S);
}

/* method: get_weapon_slot of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weapon_slot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weapon_slot'",NULL);
#endif
 {
  weaponslot* tolua_ret = (weaponslot*)  self->get_weapon_slot(n);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"weaponslot");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weapon_slot'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weapon_slot of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weapon_slot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"weaponslot",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  weaponslot* w = ((weaponslot*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weapon_slot'",NULL);
#endif
 {
  self->set_weapon_slot(n,w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weapon_slot'.",&tolua_err);
 return 0;
#endif
}

/* method: get_avail_weapon_slots of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_avail_weapon_slots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_avail_weapon_slots'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_avail_weapon_slots();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_avail_weapon_slots'.",&tolua_err);
 return 0;
#endif
}

/* method: set_avail_weapon_slots of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_avail_weapon_slots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_avail_weapon_slots'",NULL);
#endif
 {
  self->set_avail_weapon_slots(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_avail_weapon_slots'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weapon_bullets of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weapon_bullets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int slot = ((int)  tolua_tonumber(tolua_S,2,0));
  int bull = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weapon_bullets'",NULL);
#endif
 {
  self->set_weapon_bullets(slot,bull);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weapon_bullets'.",&tolua_err);
 return 0;
#endif
}

/* method: get_weapon_bullets of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_weapon_bullets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int slot = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_weapon_bullets'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_weapon_bullets(slot);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_weapon_bullets'.",&tolua_err);
 return 0;
#endif
}

/* method: activate_weapon_slot of class  gsPlayer */
static int tolua_gslua_gsPlayer_activate_weapon_slot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'activate_weapon_slot'",NULL);
#endif
 {
  self->activate_weapon_slot(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'activate_weapon_slot'.",&tolua_err);
 return 0;
#endif
}

/* method: deactivate_weapon_slot of class  gsPlayer */
static int tolua_gslua_gsPlayer_deactivate_weapon_slot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'deactivate_weapon_slot'",NULL);
#endif
 {
  self->deactivate_weapon_slot(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'deactivate_weapon_slot'.",&tolua_err);
 return 0;
#endif
}

/* method: set_ship_value of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_ship_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int what = ((int)  tolua_tonumber(tolua_S,2,0));
  int val = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_ship_value'",NULL);
#endif
 {
  self->set_ship_value(what,val);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_ship_value'.",&tolua_err);
 return 0;
#endif
}

/* method: get_ship_value of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_ship_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int what = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_ship_value'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_ship_value(what);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_ship_value'.",&tolua_err);
 return 0;
#endif
}

/* method: set_status_mode of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_status_mode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_status_mode'",NULL);
#endif
 {
  self->set_status_mode(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_status_mode'.",&tolua_err);
 return 0;
#endif
}

/* method: show_status_set_map of class  gsPlayer */
static int tolua_gslua_gsPlayer_show_status_set_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  class gsMap* m = ((class gsMap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'show_status_set_map'",NULL);
#endif
 {
  self->show_status_set_map(m);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'show_status_set_map'.",&tolua_err);
 return 0;
#endif
}

/* method: show_status of class  gsPlayer */
static int tolua_gslua_gsPlayer_show_status00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'show_status'",NULL);
#endif
 {
  self->show_status();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'show_status'.",&tolua_err);
 return 0;
#endif
}

/* method: hide_status of class  gsPlayer */
static int tolua_gslua_gsPlayer_hide_status00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'hide_status'",NULL);
#endif
 {
  self->hide_status();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'hide_status'.",&tolua_err);
 return 0;
#endif
}

/* method: get_handicap of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_handicap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_handicap'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_handicap();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_handicap'.",&tolua_err);
 return 0;
#endif
}

/* method: set_handicap of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_handicap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_handicap'",NULL);
#endif
 {
  self->set_handicap(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_handicap'.",&tolua_err);
 return 0;
#endif
}

/* method: get_recoil of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_recoil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_recoil'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_recoil();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_recoil'.",&tolua_err);
 return 0;
#endif
}

/* method: set_recoil of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_recoil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_recoil'",NULL);
#endif
 {
  self->set_recoil(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_recoil'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dont_continue of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_dont_continue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dont_continue'",NULL);
#endif
 {
  self->set_dont_continue(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dont_continue'.",&tolua_err);
 return 0;
#endif
}

/* method: get_do_hooks of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_do_hooks'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_do_hooks();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_do_hooks of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_do_hooks'",NULL);
#endif
 {
  self->set_do_hooks(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_int of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int d = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_int'",NULL);
#endif
 {
  self->set_user_int(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_float of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  float d = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_float'",NULL);
#endif
 {
  self->set_user_float(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_int of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_int'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user_int(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_float of class  gsPlayer */
static int tolua_gslua_gsPlayer_get_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_float'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_user_float(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: reinit of class  gsPlayer */
static int tolua_gslua_gsPlayer_reinit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'reinit'",NULL);
#endif
 {
  self->reinit();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'reinit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_standard_bullets of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_standard_bullets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_standard_bullets'",NULL);
#endif
 {
  self->set_standard_bullets();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_standard_bullets'.",&tolua_err);
 return 0;
#endif
}

/* method: return_home of class  gsPlayer */
static int tolua_gslua_gsPlayer_return_home00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'return_home'",NULL);
#endif
 {
  self->return_home();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'return_home'.",&tolua_err);
 return 0;
#endif
}

/* method: set_weapon of class  gsPlayer */
static int tolua_gslua_gsPlayer_set_weapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int m = ((int)  tolua_tonumber(tolua_S,3,0));
  int bullets = ((int)  tolua_tonumber(tolua_S,4,0));
  int recharge = ((int)  tolua_tonumber(tolua_S,5,-1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_weapon'",NULL);
#endif
 {
  self->set_weapon(n,m,bullets,recharge);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_weapon'.",&tolua_err);
 return 0;
#endif
}

/* method: shoot of class  gsPlayer */
static int tolua_gslua_gsPlayer_shoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'shoot'",NULL);
#endif
 {
  self->shoot();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'shoot'.",&tolua_err);
 return 0;
#endif
}

/* method: hit of class  gsPlayer */
static int tolua_gslua_gsPlayer_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  int frompl = ((int)  tolua_tonumber(tolua_S,2,0));
  int fromen = ((int)  tolua_tonumber(tolua_S,3,0));
  float hullhit = ((float)  tolua_tonumber(tolua_S,4,0));
  float shieldhit = ((float)  tolua_tonumber(tolua_S,5,0));
  int weapon = ((int)  tolua_tonumber(tolua_S,6,0));
  int xspd = ((int)  tolua_tonumber(tolua_S,7,0));
  int yspd = ((int)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'hit'",NULL);
#endif
 {
  self->hit(frompl,fromen,hullhit,shieldhit,weapon,xspd,yspd);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'hit'.",&tolua_err);
 return 0;
#endif
}

/* method: load_ship of class  gsPlayer */
static int tolua_gslua_gsPlayer_load_ship00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  char* n = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'load_ship'",NULL);
#endif
 {
  int tolua_ret = (int)  self->load_ship(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'load_ship'.",&tolua_err);
 return 0;
#endif
}

/* method: load_image of class  gsPlayer */
static int tolua_gslua_gsPlayer_load_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPlayer",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPlayer* self = (gsPlayer*)  tolua_tousertype(tolua_S,1,0);
  char* n = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'load_image'",NULL);
#endif
 {
  int tolua_ret = (int)  self->load_image(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'load_image'.",&tolua_err);
 return 0;
#endif
}

/* function: get_player_nr_by_sig */
static int tolua_gslua_get_player_nr_by_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  unsigned long tolua_var_9 = ((unsigned long)  tolua_tonumber(tolua_S,1,0));
 {
  int tolua_ret = (int)  get_player_nr_by_sig(tolua_var_9);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player_nr_by_sig'.",&tolua_err);
 return 0;
#endif
}

/* function: get_player_by_sig */
static int tolua_gslua_get_player_by_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  unsigned long tolua_var_10 = ((unsigned long)  tolua_tonumber(tolua_S,1,0));
 {
  gsPlayer* tolua_ret = (gsPlayer*)  get_player_by_sig(tolua_var_10);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"gsPlayer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player_by_sig'.",&tolua_err);
 return 0;
#endif
}

/* get function: player */
static int tolua_get_gslua_player(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)player[tolua_index],"gsPlayer");
 return 1;
}

/* set function: player */
static int tolua_set_gslua_player(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
  player[tolua_index] = ((gsPlayer*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* method: get_x of class  Base */
static int tolua_gslua_Base_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Base */
static int tolua_gslua_Base_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  Base */
static int tolua_gslua_Base_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  Base */
static int tolua_gslua_Base_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Base */
static int tolua_gslua_Base_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Base */
static int tolua_gslua_Base_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  Base */
static int tolua_gslua_Base_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  Base */
static int tolua_gslua_Base_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  Base */
static int tolua_gslua_Base_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  Base */
static int tolua_gslua_Base_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  Base */
static int tolua_gslua_Base_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Base */
static int tolua_gslua_Base_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_prev of class  Base */
static int tolua_gslua_Base_set_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  Base* n = ((Base*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_prev'",NULL);
#endif
 {
  self->set_prev(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Base */
static int tolua_gslua_Base_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  Base* n = ((Base*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land_x of class  Base */
static int tolua_gslua_Base_get_land_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_land_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land_y of class  Base */
static int tolua_gslua_Base_get_land_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_land_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_land_w of class  Base */
static int tolua_gslua_Base_get_land_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_land_w'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_land_w();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_land_w'.",&tolua_err);
 return 0;
#endif
}

/* method: set_land_x of class  Base */
static int tolua_gslua_Base_set_land_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_land_x'",NULL);
#endif
 {
  self->set_land_x(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_land_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_land_y of class  Base */
static int tolua_gslua_Base_set_land_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_land_y'",NULL);
#endif
 {
  self->set_land_y(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_land_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_land_w of class  Base */
static int tolua_gslua_Base_set_land_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  int w = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_land_w'",NULL);
#endif
 {
  self->set_land_w(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_land_w'.",&tolua_err);
 return 0;
#endif
}

/* method: set_type of class  Base */
static int tolua_gslua_Base_set_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  BASE_TYPE n = ((BASE_TYPE) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_type'",NULL);
#endif
 {
  self->set_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Base */
static int tolua_gslua_Base_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  BASE_TYPE tolua_ret = (BASE_TYPE)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: set_cargo_type of class  Base */
static int tolua_gslua_Base_set_cargo_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_cargo_type'",NULL);
#endif
 {
  self->set_cargo_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_cargo_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_cargo_type of class  Base */
static int tolua_gslua_Base_get_cargo_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_cargo_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_cargo_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_cargo_type'.",&tolua_err);
 return 0;
#endif
}

/* method: set_cargo_weight of class  Base */
static int tolua_gslua_Base_set_cargo_weight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_cargo_weight'",NULL);
#endif
 {
  self->set_cargo_weight(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_cargo_weight'.",&tolua_err);
 return 0;
#endif
}

/* method: get_cargo_weight of class  Base */
static int tolua_gslua_Base_get_cargo_weight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_cargo_weight'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_cargo_weight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_cargo_weight'.",&tolua_err);
 return 0;
#endif
}

/* method: get_cargo_object of class  Base */
static int tolua_gslua_Base_get_cargo_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_cargo_object'",NULL);
#endif
 {
  Object* tolua_ret = (Object*)  self->get_cargo_object();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_cargo_object'.",&tolua_err);
 return 0;
#endif
}

/* method: set_cargo_object of class  Base */
static int tolua_gslua_Base_set_cargo_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  Object* tolua_var_11 = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_cargo_object'",NULL);
#endif
 {
  self->set_cargo_object(tolua_var_11);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_cargo_object'.",&tolua_err);
 return 0;
#endif
}

/* method: set_cargo_name of class  Base */
static int tolua_gslua_Base_set_cargo_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  char* tolua_var_12 = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_cargo_name'",NULL);
#endif
 {
  self->set_cargo_name(tolua_var_12);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_cargo_name'.",&tolua_err);
 return 0;
#endif
}

/* method: get_cargo_name of class  Base */
static int tolua_gslua_Base_get_cargo_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_cargo_name'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_cargo_name();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_cargo_name'.",&tolua_err);
 return 0;
#endif
}

/* method: get_nr of class  Base */
static int tolua_gslua_Base_get_nr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_nr'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_nr();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_nr'.",&tolua_err);
 return 0;
#endif
}

/* method: get_player_on_base of class  Base */
static int tolua_gslua_Base_get_player_on_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_player_on_base'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_player_on_base();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player_on_base'.",&tolua_err);
 return 0;
#endif
}

/* method: set_player_on_base of class  Base */
static int tolua_gslua_Base_set_player_on_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_player_on_base'",NULL);
#endif
 {
  self->set_player_on_base(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_player_on_base'.",&tolua_err);
 return 0;
#endif
}

/* method: set_refuel_percent of class  Base */
static int tolua_gslua_Base_set_refuel_percent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int pl = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_refuel_percent'",NULL);
#endif
 {
  self->set_refuel_percent(n,pl);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_refuel_percent'.",&tolua_err);
 return 0;
#endif
}

/* method: get_refuel_percent of class  Base */
static int tolua_gslua_Base_get_refuel_percent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_refuel_percent'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_refuel_percent();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_refuel_percent'.",&tolua_err);
 return 0;
#endif
}

/* method: get_refuel_player of class  Base */
static int tolua_gslua_Base_get_refuel_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Base",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Base* self = (Base*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_refuel_player'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_refuel_player();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_refuel_player'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first_base of class  gsBase */
static int tolua_gslua_gsBase_get_first_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBase",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBase* self = (gsBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first_base'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->get_first_base();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first_base'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last_base of class  gsBase */
static int tolua_gslua_gsBase_get_last_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBase",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBase* self = (gsBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last_base'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->get_last_base();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last_base'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsBase */
static int tolua_gslua_gsBase_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBase",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBase* self = (gsBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsBase */
static int tolua_gslua_gsBase_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBase",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBase* self = (gsBase*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  int t = ((int)  tolua_tonumber(tolua_S,3,0));
  int x = ((int)  tolua_tonumber(tolua_S,4,0));
  int y = ((int)  tolua_tonumber(tolua_S,5,0));
  int user = ((int)  tolua_tonumber(tolua_S,6,0));
  int c = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->add(num,t,x,y,user,c);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: get_base of class  gsBase */
static int tolua_gslua_gsBase_get_base00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsBase",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsBase* self = (gsBase*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_13 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_base'",NULL);
#endif
 {
  Base* tolua_ret = (Base*)  self->get_base(tolua_var_13);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Base");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_base'.",&tolua_err);
 return 0;
#endif
}

/* get function: bases */
static int tolua_get_bases_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)bases,"gsBase");
 return 1;
}

/* set function: bases */
static int tolua_set_bases_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsBase",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  bases = ((gsBase*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: color of class  Pixel */
static int tolua_get_Pixel_color(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->color);
 return 1;
}

/* set function: color of class  Pixel */
static int tolua_set_Pixel_color(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->color = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: tcolor of class  Pixel */
static int tolua_get_Pixel_tcolor(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'tcolor'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tcolor);
 return 1;
}

/* set function: tcolor of class  Pixel */
static int tolua_set_Pixel_tcolor(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'tcolor'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tcolor = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: lifetime of class  Pixel */
static int tolua_get_Pixel_lifetime(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'lifetime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->lifetime);
 return 1;
}

/* set function: lifetime of class  Pixel */
static int tolua_set_Pixel_lifetime(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'lifetime'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lifetime = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_xab of class  Pixel */
static int tolua_get_Pixel_max_xab(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_xab'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_xab);
 return 1;
}

/* set function: max_xab of class  Pixel */
static int tolua_set_Pixel_max_xab(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_xab'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_xab = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_yab of class  Pixel */
static int tolua_get_Pixel_max_yab(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_yab'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_yab);
 return 1;
}

/* set function: max_yab of class  Pixel */
static int tolua_set_Pixel_max_yab(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_yab'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_yab = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: dspd of class  Pixel */
static int tolua_get_Pixel_dspd(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'dspd'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dspd);
 return 1;
}

/* set function: dspd of class  Pixel */
static int tolua_set_Pixel_dspd(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'dspd'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dspd = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bounce of class  Pixel */
static int tolua_get_Pixel_bounce(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bounce'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bounce);
 return 1;
}

/* set function: bounce of class  Pixel */
static int tolua_set_Pixel_bounce(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bounce'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bounce = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vdec of class  Pixel */
static int tolua_get_Pixel_vdec(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'vdec'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vdec);
 return 1;
}

/* set function: vdec of class  Pixel */
static int tolua_set_Pixel_vdec(lua_State* tolua_S)
{
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'vdec'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vdec = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: get_x of class  Pixel */
static int tolua_gslua_Pixel_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Pixel */
static int tolua_gslua_Pixel_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Pixel */
static int tolua_gslua_Pixel_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Pixel */
static int tolua_gslua_Pixel_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  Pixel */
static int tolua_gslua_Pixel_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  Pixel */
static int tolua_gslua_Pixel_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  Pixel */
static int tolua_gslua_Pixel_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  Pixel */
static int tolua_gslua_Pixel_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  Pixel */
static int tolua_gslua_Pixel_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Pixel* tolua_ret = (Pixel*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixel");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Pixel */
static int tolua_gslua_Pixel_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Pixel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Pixel* self = (Pixel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Pixel* tolua_ret = (Pixel*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixel");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first of class  gsPixelRain */
static int tolua_gslua_gsPixelRain_get_first00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPixelRain",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPixelRain* self = (gsPixelRain*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first'",NULL);
#endif
 {
  Pixel* tolua_ret = (Pixel*)  self->get_first();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixel");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last of class  gsPixelRain */
static int tolua_gslua_gsPixelRain_get_last00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPixelRain",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPixelRain* self = (gsPixelRain*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last'",NULL);
#endif
 {
  Pixel* tolua_ret = (Pixel*)  self->get_last();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixel");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsPixelRain */
static int tolua_gslua_gsPixelRain_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPixelRain",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPixelRain* self = (gsPixelRain*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsPixelRain */
static int tolua_gslua_gsPixelRain_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPixelRain",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,11,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,12,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,13,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,14,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,15,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,16,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,17,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPixelRain* self = (gsPixelRain*)  tolua_tousertype(tolua_S,1,0);
  int anz = ((int)  tolua_tonumber(tolua_S,2,0));
  int startx = ((int)  tolua_tonumber(tolua_S,3,0));
  int starty = ((int)  tolua_tonumber(tolua_S,4,0));
  int xradius = ((int)  tolua_tonumber(tolua_S,5,0));
  int yradius = ((int)  tolua_tonumber(tolua_S,6,0));
  float xab = ((float)  tolua_tonumber(tolua_S,7,0));
  float yab = ((float)  tolua_tonumber(tolua_S,8,0));
  int spd = ((int)  tolua_tonumber(tolua_S,9,0));
  int what = ((int)  tolua_tonumber(tolua_S,10,0));
  int maxtable = ((int)  tolua_tonumber(tolua_S,11,10));
  int lifetime = ((int)  tolua_tonumber(tolua_S,12,2000));
  int dspd = ((int)  tolua_tonumber(tolua_S,13,0));
  int max_xab = ((int)  tolua_tonumber(tolua_S,14,1500));
  int max_yab = ((int)  tolua_tonumber(tolua_S,15,1500));
  float vdec = ((float)  tolua_tonumber(tolua_S,16,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  self->add(anz,startx,starty,xradius,yradius,xab,yab,spd,what,maxtable,lifetime,dspd,max_xab,max_yab,vdec);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* get function: map_style of class  level_info */
static int tolua_get_level_info_map_style(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_style'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->map_style);
 return 1;
}

/* get function: onescreen_map of class  level_info */
static int tolua_get_level_info_onescreen_map(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'onescreen_map'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->onescreen_map);
 return 1;
}

/* set function: onescreen_map of class  level_info */
static int tolua_set_level_info_onescreen_map(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'onescreen_map'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->onescreen_map = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: map_start_x of class  level_info */
static int tolua_get_level_info_map_start_x(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_start_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->map_start_x);
 return 1;
}

/* set function: map_start_x of class  level_info */
static int tolua_set_level_info_map_start_x(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_start_x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->map_start_x = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: map_start_y of class  level_info */
static int tolua_get_level_info_map_start_y(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_start_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->map_start_y);
 return 1;
}

/* set function: map_start_y of class  level_info */
static int tolua_set_level_info_map_start_y(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_start_y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->map_start_y = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_extra_str of class  level_info */
static int tolua_get_level_info_activate_extra_str(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_extra_str'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->activate_extra_str);
 return 1;
}

/* get function: activate_extra_bar_str of class  level_info */
static int tolua_get_level_info_activate_extra_bar_str(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_extra_bar_str'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->activate_extra_bar_str);
 return 1;
}

/* get function: activate_ctf of class  level_info */
static int tolua_get_level_info_activate_ctf(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_ctf'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_ctf);
 return 1;
}

/* set function: activate_ctf of class  level_info */
static int tolua_set_level_info_activate_ctf(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_ctf'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_ctf = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_hh of class  level_info */
static int tolua_get_level_info_activate_hh(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_hh'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_hh);
 return 1;
}

/* set function: activate_hh of class  level_info */
static int tolua_set_level_info_activate_hh(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_hh'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_hh = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_global_script of class  level_info */
static int tolua_get_level_info_activate_global_script(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_global_script'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_global_script);
 return 1;
}

/* set function: activate_global_script of class  level_info */
static int tolua_set_level_info_activate_global_script(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_global_script'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_global_script = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_race of class  level_info */
static int tolua_get_level_info_activate_race(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_race'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_race);
 return 1;
}

/* set function: activate_race of class  level_info */
static int tolua_set_level_info_activate_race(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_race'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_race = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_minigame of class  level_info */
static int tolua_get_level_info_activate_minigame(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_minigame'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_minigame);
 return 1;
}

/* set function: activate_minigame of class  level_info */
static int tolua_set_level_info_activate_minigame(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_minigame'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_minigame = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: activate_singleplayer of class  level_info */
static int tolua_get_level_info_activate_singleplayer(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_singleplayer'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_singleplayer);
 return 1;
}

/* set function: activate_singleplayer of class  level_info */
static int tolua_set_level_info_activate_singleplayer(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_singleplayer'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activate_singleplayer = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: min_version of class  level_info */
static int tolua_get_level_info_min_version(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'min_version'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->min_version);
 return 1;
}

/* set function: min_version of class  level_info */
static int tolua_set_level_info_min_version(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'min_version'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->min_version = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_players of class  level_info */
static int tolua_get_level_info_max_players(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_players'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_players);
 return 1;
}

/* set function: max_players of class  level_info */
static int tolua_set_level_info_max_players(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_players'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_players = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  level_info */
static int tolua_get_level_info_name(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* get function: desc of class  level_info */
static int tolua_get_level_info_desc(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'desc'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* get function: missiondesc of class  level_info */
static int tolua_get_level_info_missiondesc(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'missiondesc'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->missiondesc);
 return 1;
}

/* get function: leveldir of class  level_info */
static int tolua_get_level_info_leveldir(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'leveldir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->leveldir);
 return 1;
}

/* get function: levelfile of class  level_info */
static int tolua_get_level_info_levelfile(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'levelfile'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->levelfile);
 return 1;
}

/* get function: mapfile of class  level_info */
static int tolua_get_level_info_mapfile(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'mapfile'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->mapfile);
 return 1;
}

/* get function: initscriptfile of class  level_info */
static int tolua_get_level_info_initscriptfile(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'initscriptfile'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->initscriptfile);
 return 1;
}

/* get function: scriptfile of class  level_info */
static int tolua_get_level_info_scriptfile(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'scriptfile'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->scriptfile);
 return 1;
}

/* get function: author of class  level_info */
static int tolua_get_level_info_author(lua_State* tolua_S)
{
  level_info* self = (level_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'author'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->author);
 return 1;
}

/* method: set_text of class  UserStats */
static int tolua_gslua_UserStats_set_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  char* n = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_text'",NULL);
#endif
 {
  self->set_text(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_text'.",&tolua_err);
 return 0;
#endif
}

/* method: set_textcolor of class  UserStats */
static int tolua_gslua_UserStats_set_textcolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_textcolor'",NULL);
#endif
 {
  self->set_textcolor(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_textcolor'.",&tolua_err);
 return 0;
#endif
}

/* method: set_value of class  UserStats */
static int tolua_gslua_UserStats_set_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int pl = ((int)  tolua_tonumber(tolua_S,2,0));
  char* n = ((char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_value'",NULL);
#endif
 {
  self->set_value(pl,n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_value'.",&tolua_err);
 return 0;
#endif
}

/* method: set_value of class  UserStats */
static int tolua_gslua_UserStats_set_value01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int pl = ((int)  tolua_tonumber(tolua_S,2,0));
  int n = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_value'",NULL);
#endif
 {
  self->set_value(pl,n);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_UserStats_set_value00(tolua_S);
}

/* method: set_valuecolor of class  UserStats */
static int tolua_gslua_UserStats_set_valuecolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int pl = ((int)  tolua_tonumber(tolua_S,2,0));
  int n = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_valuecolor'",NULL);
#endif
 {
  self->set_valuecolor(pl,n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_valuecolor'.",&tolua_err);
 return 0;
#endif
}

/* method: set_plusx of class  UserStats */
static int tolua_gslua_UserStats_set_plusx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_plusx'",NULL);
#endif
 {
  self->set_plusx(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_plusx'.",&tolua_err);
 return 0;
#endif
}

/* method: get_text of class  UserStats */
static int tolua_gslua_UserStats_get_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_text'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_text();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_text'.",&tolua_err);
 return 0;
#endif
}

/* method: get_textcolor of class  UserStats */
static int tolua_gslua_UserStats_get_textcolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_textcolor'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_textcolor();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_textcolor'.",&tolua_err);
 return 0;
#endif
}

/* method: get_value of class  UserStats */
static int tolua_gslua_UserStats_get_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int pl = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_value'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_value(pl);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_value'.",&tolua_err);
 return 0;
#endif
}

/* method: get_valuecolor of class  UserStats */
static int tolua_gslua_UserStats_get_valuecolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
  int pl = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_valuecolor'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_valuecolor(pl);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_valuecolor'.",&tolua_err);
 return 0;
#endif
}

/* method: get_plusx of class  UserStats */
static int tolua_gslua_UserStats_get_plusx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserStats",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserStats* self = (UserStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_plusx'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_plusx();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_plusx'.",&tolua_err);
 return 0;
#endif
}

/* get function: fps of class  gsGlobals */
static int tolua_get_gsGlobals_fps(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'fps'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fps);
 return 1;
}

/* get function: fpst of class  gsGlobals */
static int tolua_get_gsGlobals_fpst(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'fpst'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fpst);
 return 1;
}

/* get function: game_time of class  gsGlobals */
static int tolua_get_gsGlobals_game_time(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'game_time'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->game_time);
 return 1;
}

/* get function: overall_game_time of class  gsGlobals */
static int tolua_get_gsGlobals_overall_game_time(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'overall_game_time'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->overall_game_time);
 return 1;
}

/* get function: second_time of class  gsGlobals */
static int tolua_get_gsGlobals_second_time(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'second_time'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->second_time);
 return 1;
}

/* get function: splitscreen of class  gsGlobals */
static int tolua_get_gsGlobals_splitscreen(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'splitscreen'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->splitscreen);
 return 1;
}

/* get function: onescreen_map of class  gsGlobals */
static int tolua_get_gsGlobals_onescreen_map(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'onescreen_map'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->onescreen_map);
 return 1;
}

/* get function: players of class  gsGlobals */
static int tolua_get_gsGlobals_players(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'players'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->players);
 return 1;
}

/* get function: game_mode of class  gsGlobals */
static int tolua_get_gsGlobals_game_mode(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'game_mode'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->game_mode);
 return 1;
}

/* get function: singleplayer of class  gsGlobals */
static int tolua_get_gsGlobals_singleplayer(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'singleplayer'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->singleplayer);
 return 1;
}

/* get function: minigame of class  gsGlobals */
static int tolua_get_gsGlobals_minigame(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'minigame'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->minigame);
 return 1;
}

/* get function: network_game of class  gsGlobals */
static int tolua_get_gsGlobals_network_game(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'network_game'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->network_game);
 return 1;
}

/* get function: use_joystick of class  gsGlobals */
static int tolua_get_gsGlobals_use_joystick(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'use_joystick'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->use_joystick);
 return 1;
}

/* get function: joystick_installed of class  gsGlobals */
static int tolua_get_gsGlobals_joystick_installed(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'joystick_installed'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->joystick_installed);
 return 1;
}

/* get function: game_resolution_x of class  gsGlobals */
static int tolua_get_gsGlobals_game_resolution_x(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'game_resolution_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->game_resolution_x);
 return 1;
}

/* get function: game_resolution_y of class  gsGlobals */
static int tolua_get_gsGlobals_game_resolution_y(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'game_resolution_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->game_resolution_y);
 return 1;
}

/* get function: tile_w of class  gsGlobals */
static int tolua_get_gsGlobals_tile_w(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'tile_w'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tile_w);
 return 1;
}

/* get function: tile_h of class  gsGlobals */
static int tolua_get_gsGlobals_tile_h(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'tile_h'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tile_h);
 return 1;
}

/* get function: map_w of class  gsGlobals */
static int tolua_get_gsGlobals_map_w(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_w'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->map_w);
 return 1;
}

/* get function: map_h of class  gsGlobals */
static int tolua_get_gsGlobals_map_h(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_h'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->map_h);
 return 1;
}

/* get function: playscreen_width of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen_width(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen_width);
 return 1;
}

/* get function: playscreen_full_width of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen_full_width(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen_full_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen_full_width);
 return 1;
}

/* get function: playscreen_split_width of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen_split_width(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen_split_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen_split_width);
 return 1;
}

/* get function: playscreen_height of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen_height(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen_height'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen_height);
 return 1;
}

/* get function: playscreen_full_height of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen_full_height(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen_full_height'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen_full_height);
 return 1;
}

/* get function: playscreen1_sx of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen1_sx(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen1_sx'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen1_sx);
 return 1;
}

/* get function: playscreen2_sx of class  gsGlobals */
static int tolua_get_gsGlobals_playscreen2_sx(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'playscreen2_sx'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->playscreen2_sx);
 return 1;
}

/* get function: panel_full_width of class  gsGlobals */
static int tolua_get_gsGlobals_panel_full_width(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_full_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_full_width);
 return 1;
}

/* get function: panel_full_height of class  gsGlobals */
static int tolua_get_gsGlobals_panel_full_height(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_full_height'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_full_height);
 return 1;
}

/* get function: panel_half_width of class  gsGlobals */
static int tolua_get_gsGlobals_panel_half_width(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_half_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_half_width);
 return 1;
}

/* get function: panel_half_height of class  gsGlobals */
static int tolua_get_gsGlobals_panel_half_height(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_half_height'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_half_height);
 return 1;
}

/* get function: panel_full_x of class  gsGlobals */
static int tolua_get_gsGlobals_panel_full_x(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_full_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_full_x);
 return 1;
}

/* get function: panel_full_y of class  gsGlobals */
static int tolua_get_gsGlobals_panel_full_y(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'panel_full_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->panel_full_y);
 return 1;
}

/* get function: col_white of class  gsGlobals */
static int tolua_get_gsGlobals_col_white(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_white'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_white);
 return 1;
}

/* get function: col_grey of class  gsGlobals */
static int tolua_get_gsGlobals_col_grey(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_grey'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_grey);
 return 1;
}

/* get function: col_dgrey of class  gsGlobals */
static int tolua_get_gsGlobals_col_dgrey(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_dgrey'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_dgrey);
 return 1;
}

/* get function: col_yellow of class  gsGlobals */
static int tolua_get_gsGlobals_col_yellow(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_yellow'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_yellow);
 return 1;
}

/* get function: col_red of class  gsGlobals */
static int tolua_get_gsGlobals_col_red(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_red'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_red);
 return 1;
}

/* get function: col_bred of class  gsGlobals */
static int tolua_get_gsGlobals_col_bred(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_bred'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_bred);
 return 1;
}

/* get function: col_blue of class  gsGlobals */
static int tolua_get_gsGlobals_col_blue(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_blue'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_blue);
 return 1;
}

/* get function: col_lblue of class  gsGlobals */
static int tolua_get_gsGlobals_col_lblue(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_lblue'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_lblue);
 return 1;
}

/* get function: col_green of class  gsGlobals */
static int tolua_get_gsGlobals_col_green(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_green'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_green);
 return 1;
}

/* get function: col_black of class  gsGlobals */
static int tolua_get_gsGlobals_col_black(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_black'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_black);
 return 1;
}

/* get function: col_orange of class  gsGlobals */
static int tolua_get_gsGlobals_col_orange(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'col_orange'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->col_orange);
 return 1;
}

/* get function: tbl_blue of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_blue(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_blue[tolua_index]);
 return 1;
}

/* get function: tbl_white of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_white(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_white[tolua_index]);
 return 1;
}

/* get function: tbl_bluewhite of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_bluewhite(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_bluewhite[tolua_index]);
 return 1;
}

/* get function: tbl_green of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_green(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_green[tolua_index]);
 return 1;
}

/* get function: tbl_yellow of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_yellow(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_yellow[tolua_index]);
 return 1;
}

/* get function: tbl_red of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_red(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=21)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_red[tolua_index]);
 return 1;
}

/* get function: tbl_all of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_tbl_all(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=256)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->tbl_all[tolua_index]);
 return 1;
}

/* get function: slow_computer of class  gsGlobals */
static int tolua_get_gsGlobals_slow_computer(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'slow_computer'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->slow_computer);
 return 1;
}

/* get function: activate_extra of class  gsGlobals */
static int tolua_get_gsGlobals_activate_extra(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_extra'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_extra);
 return 1;
}

/* get function: activate_extra_bar of class  gsGlobals */
static int tolua_get_gsGlobals_activate_extra_bar(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'activate_extra_bar'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->activate_extra_bar);
 return 1;
}


/* get function: capture_the_flag of class  gsGlobals */
static int tolua_get_gsGlobals_capture_the_flag(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'capture_the_flag'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->capture_the_flag);
 return 1;
}

/* get function: hunter_hunted of class  gsGlobals */
static int tolua_get_gsGlobals_hunter_hunted(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'hunter_hunted'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->hunter_hunted);
 return 1;
}

/* get function: race of class  gsGlobals */
static int tolua_get_gsGlobals_race(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'race'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->race);
 return 1;
}

/* get function: color of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_color(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=10)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->color[tolua_index]);
 return 1;
}

/* get function: data_dir of class  gsGlobals */
static int tolua_get_gsGlobals_data_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'data_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->data_dir);
 return 1;
}

/* get function: ship_dir of class  gsGlobals */
static int tolua_get_gsGlobals_ship_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'ship_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->ship_dir);
 return 1;
}

/* get function: level_dir of class  gsGlobals */
static int tolua_get_gsGlobals_level_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'level_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->level_dir);
 return 1;
}

/* get function: map_dir of class  gsGlobals */
static int tolua_get_gsGlobals_map_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'map_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->map_dir);
 return 1;
}

/* get function: script_dir of class  gsGlobals */
static int tolua_get_gsGlobals_script_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'script_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->script_dir);
 return 1;
}

/* get function: hiscore_dir of class  gsGlobals */
static int tolua_get_gsGlobals_hiscore_dir(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'hiscore_dir'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->hiscore_dir);
 return 1;
}

/* get function: configfile_name of class  gsGlobals */
static int tolua_get_gsGlobals_configfile_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'configfile_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->configfile_name);
 return 1;
}

/* get function: datafile_name of class  gsGlobals */
static int tolua_get_gsGlobals_datafile_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'datafile_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->datafile_name);
 return 1;
}

/* get function: soundfile_name of class  gsGlobals */
static int tolua_get_gsGlobals_soundfile_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'soundfile_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->soundfile_name);
 return 1;
}

/* get function: shipdata_name of class  gsGlobals */
static int tolua_get_gsGlobals_shipdata_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shipdata_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->shipdata_name);
 return 1;
}

/* get function: fontdata_name of class  gsGlobals */
static int tolua_get_gsGlobals_fontdata_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'fontdata_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->fontdata_name);
 return 1;
}

/* get function: menu_datafile_name of class  gsGlobals */
static int tolua_get_gsGlobals_menu_datafile_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'menu_datafile_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->menu_datafile_name);
 return 1;
}

/* get function: language_file_name of class  gsGlobals */
static int tolua_get_gsGlobals_language_file_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'language_file_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->language_file_name);
 return 1;
}

/* get function: keyboard_file_name of class  gsGlobals */
static int tolua_get_gsGlobals_keyboard_file_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'keyboard_file_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->keyboard_file_name);
 return 1;
}

/* get function: global_ini of class  gsGlobals */
static int tolua_get_gsGlobals_global_ini(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'global_ini'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->global_ini);
 return 1;
}

/* get function: global_sc of class  gsGlobals */
static int tolua_get_gsGlobals_global_sc(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'global_sc'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->global_sc);
 return 1;
}

/* get function: hiscore_file_name of class  gsGlobals */
static int tolua_get_gsGlobals_hiscore_file_name(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'hiscore_file_name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->hiscore_file_name);
 return 1;
}

/* get function: def_ship of class  gsGlobals */
static int tolua_get_gsGlobals_def_ship(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'def_ship'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->def_ship);
 return 1;
}

/* get function: def_level of class  gsGlobals */
static int tolua_get_gsGlobals_def_level(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'def_level'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->def_level);
 return 1;
}

/* get function: level_files of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_level_files(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=MAX_LEVELS)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->level_files[tolua_index],"const level_info");
 return 1;
}

/* get function: selected_level of class  gsGlobals */
static int tolua_get_gsGlobals_selected_level(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'selected_level'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->selected_level);
 return 1;
}

/* get function: last_level of class  gsGlobals */
static int tolua_get_gsGlobals_last_level(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'last_level'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->last_level);
 return 1;
}

/* get function: last_splevel of class  gsGlobals */
static int tolua_get_gsGlobals_last_splevel(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'last_splevel'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->last_splevel);
 return 1;
}

/* get function: max_levels of class  gsGlobals */
static int tolua_get_gsGlobals_max_levels(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_levels'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_levels);
 return 1;
}

/* method: make_color of class  gsGlobals */
static int tolua_gslua_gsGlobals_make_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsGlobals",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'make_color'",NULL);
#endif
 {
  int tolua_ret = (int)  self->make_color(r,g,b);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'make_color'.",&tolua_err);
 return 0;
#endif
}

/* method: get_level_num_from_filename of class  gsGlobals */
static int tolua_gslua_gsGlobals_get_level_num_from_filename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsGlobals",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
  char* n = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_level_num_from_filename'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_level_num_from_filename(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_level_num_from_filename'.",&tolua_err);
 return 0;
#endif
}

/* method: set_highscore of class  gsGlobals */
static int tolua_gslua_gsGlobals_set_highscore00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsGlobals",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
  char* n = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_highscore'",NULL);
#endif
 {
  self->set_highscore(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_highscore'.",&tolua_err);
 return 0;
#endif
}

/* method: set_highscore of class  gsGlobals */
static int tolua_gslua_gsGlobals_set_highscore01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsGlobals",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_highscore'",NULL);
#endif
 {
  self->set_highscore(n);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsGlobals_set_highscore00(tolua_S);
}

/* method: create_level_done_file of class  gsGlobals */
static int tolua_gslua_gsGlobals_create_level_done_file00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsGlobals",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'create_level_done_file'",NULL);
#endif
 {
  self->create_level_done_file();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'create_level_done_file'.",&tolua_err);
 return 0;
#endif
}

/* get function: gravity_percent of class  gsGlobals */
static int tolua_get_gsGlobals_gravity_percent(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'gravity_percent'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->gravity_percent);
 return 1;
}

/* set function: gravity_percent of class  gsGlobals */
static int tolua_set_gsGlobals_gravity_percent(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'gravity_percent'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gravity_percent = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: level_gravity of class  gsGlobals */
static int tolua_get_gsGlobals_level_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'level_gravity'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->level_gravity);
 return 1;
}

/* set function: level_gravity of class  gsGlobals */
static int tolua_set_gsGlobals_level_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'level_gravity'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->level_gravity = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: particle_gravity of class  gsGlobals */
static int tolua_get_gsGlobals_particle_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'particle_gravity'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->particle_gravity);
 return 1;
}

/* set function: particle_gravity of class  gsGlobals */
static int tolua_set_gsGlobals_particle_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'particle_gravity'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->particle_gravity = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bomb_gravity of class  gsGlobals */
static int tolua_get_gsGlobals_bomb_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bomb_gravity'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bomb_gravity);
 return 1;
}

/* set function: bomb_gravity of class  gsGlobals */
static int tolua_set_gsGlobals_bomb_gravity(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bomb_gravity'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bomb_gravity = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: exit_level of class  gsGlobals */
static int tolua_get_gsGlobals_exit_level(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'exit_level'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->exit_level);
 return 1;
}

/* set function: exit_level of class  gsGlobals */
static int tolua_set_gsGlobals_exit_level(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'exit_level'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->exit_level = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_craters of class  gsGlobals */
static int tolua_get_gsGlobals_max_craters(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_craters'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_craters);
 return 1;
}

/* set function: max_craters of class  gsGlobals */
static int tolua_set_gsGlobals_max_craters(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_craters'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_craters = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: language of class  gsGlobals */
static int tolua_get_gsGlobals_language(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'language'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->language);
 return 1;
}

/* set function: language of class  gsGlobals */
static int tolua_set_gsGlobals_language(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'language'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->language = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: plus_x of class  gsGlobals */
static int tolua_get_gsGlobals_plus_x(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plus_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->plus_x);
 return 1;
}

/* set function: plus_x of class  gsGlobals */
static int tolua_set_gsGlobals_plus_x(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plus_x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->plus_x = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: plus_y of class  gsGlobals */
static int tolua_get_gsGlobals_plus_y(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plus_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->plus_y);
 return 1;
}

/* set function: plus_y of class  gsGlobals */
static int tolua_set_gsGlobals_plus_y(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'plus_y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->plus_y = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: pixamount of class  gsGlobals */
static int tolua_get_gsGlobals_pixamount(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'pixamount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->pixamount);
 return 1;
}

/* set function: pixamount of class  gsGlobals */
static int tolua_set_gsGlobals_pixamount(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'pixamount'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pixamount = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: soundvol of class  gsGlobals */
static int tolua_get_gsGlobals_soundvol(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'soundvol'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->soundvol);
 return 1;
}

/* set function: soundvol of class  gsGlobals */
static int tolua_set_gsGlobals_soundvol(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'soundvol'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->soundvol = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: race_laps of class  gsGlobals */
static int tolua_get_gsGlobals_race_laps(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'race_laps'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->race_laps);
 return 1;
}

/* set function: race_laps of class  gsGlobals */
static int tolua_set_gsGlobals_race_laps(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'race_laps'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->race_laps = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: shoot_speed of class  gsGlobals */
static int tolua_get_gsGlobals_shoot_speed(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shoot_speed'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->shoot_speed);
 return 1;
}

/* set function: shoot_speed of class  gsGlobals */
static int tolua_set_gsGlobals_shoot_speed(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shoot_speed'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->shoot_speed = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: shoot_speed_bomb of class  gsGlobals */
static int tolua_get_gsGlobals_shoot_speed_bomb(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shoot_speed_bomb'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->shoot_speed_bomb);
 return 1;
}

/* set function: shoot_speed_bomb of class  gsGlobals */
static int tolua_set_gsGlobals_shoot_speed_bomb(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shoot_speed_bomb'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->shoot_speed_bomb = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: player_respawn of class  gsGlobals */
static int tolua_get_gsGlobals_player_respawn(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player_respawn'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->player_respawn);
 return 1;
}

/* set function: player_respawn of class  gsGlobals */
static int tolua_set_gsGlobals_player_respawn(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player_respawn'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->player_respawn = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: player_indestructible of class  gsGlobals */
static int tolua_get_gsGlobals_player_indestructible(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player_indestructible'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->player_indestructible);
 return 1;
}

/* set function: player_indestructible of class  gsGlobals */
static int tolua_set_gsGlobals_player_indestructible(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player_indestructible'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->player_indestructible = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: extra_amount of class  gsGlobals */
static int tolua_get_gsGlobals_extra_amount(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'extra_amount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->extra_amount);
 return 1;
}

/* set function: extra_amount of class  gsGlobals */
static int tolua_set_gsGlobals_extra_amount(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'extra_amount'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extra_amount = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_frags of class  gsGlobals */
static int tolua_get_gsGlobals_max_frags(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_frags'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_frags);
 return 1;
}

/* set function: max_frags of class  gsGlobals */
static int tolua_set_gsGlobals_max_frags(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_frags'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_frags = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: max_time of class  gsGlobals */
static int tolua_get_gsGlobals_max_time(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_time'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->max_time);
 return 1;
}

/* set function: max_time of class  gsGlobals */
static int tolua_set_gsGlobals_max_time(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'max_time'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->max_time = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: do_all_hooks of class  gsGlobals */
static int tolua_get_gsGlobals_do_all_hooks(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'do_all_hooks'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->do_all_hooks);
 return 1;
}

/* set function: do_all_hooks of class  gsGlobals */
static int tolua_set_gsGlobals_do_all_hooks(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'do_all_hooks'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->do_all_hooks = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: computer_player of class  gsGlobals */
static int tolua_get_gsGlobals_computer_player(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'computer_player'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->computer_player);
 return 1;
}

/* set function: computer_player of class  gsGlobals */
static int tolua_set_gsGlobals_computer_player(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'computer_player'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->computer_player = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: computer_player_strength of class  gsGlobals */
static int tolua_get_gsGlobals_computer_player_strength(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'computer_player_strength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->computer_player_strength);
 return 1;
}

/* set function: computer_player_strength of class  gsGlobals */
static int tolua_set_gsGlobals_computer_player_strength(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'computer_player_strength'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->computer_player_strength = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: use_script_computer_player of class  gsGlobals */
static int tolua_get_gsGlobals_use_script_computer_player(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'use_script_computer_player'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->use_script_computer_player);
 return 1;
}

/* set function: use_script_computer_player of class  gsGlobals */
static int tolua_set_gsGlobals_use_script_computer_player(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'use_script_computer_player'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->use_script_computer_player = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: use_user_stats of class  gsGlobals */
static int tolua_get_gsGlobals_use_user_stats(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'use_user_stats'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->use_user_stats);
 return 1;
}

/* set function: use_user_stats of class  gsGlobals */
static int tolua_set_gsGlobals_use_user_stats(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'use_user_stats'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->use_user_stats = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: user_stats of class  gsGlobals */
static int tolua_get_gslua_gsGlobals_user_stats(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=10)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->user_stats[tolua_index],"UserStats");
 return 1;
}

/* set function: user_stats of class  gsGlobals */
static int tolua_set_gslua_gsGlobals_user_stats(lua_State* tolua_S)
{
 int tolua_index;
  gsGlobals* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (gsGlobals*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=10)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
  self->user_stats[tolua_index] = *((UserStats*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* get function: score_per_second of class  gsGlobals */
static int tolua_get_gsGlobals_score_per_second(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'score_per_second'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->score_per_second);
 return 1;
}

/* set function: score_per_second of class  gsGlobals */
static int tolua_set_gsGlobals_score_per_second(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'score_per_second'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->score_per_second = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: handicap of class  gsGlobals */
static int tolua_get_gsGlobals_handicap(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'handicap'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->handicap);
 return 1;
}

/* set function: handicap of class  gsGlobals */
static int tolua_set_gsGlobals_handicap(lua_State* tolua_S)
{
  gsGlobals* self = (gsGlobals*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'handicap'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->handicap = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: globals */
static int tolua_get_globals_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)globals,"gsGlobals");
 return 1;
}

/* set function: globals */
static int tolua_set_globals_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsGlobals",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  globals = ((gsGlobals*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: set_key of class  gsControl */
static int tolua_gslua_gsControl_set_key00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
  player_controls k = ((player_controls) (int)  tolua_tonumber(tolua_S,2,0));
  int n = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_key'",NULL);
#endif
 {
  self->set_key(k,n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_key'.",&tolua_err);
 return 0;
#endif
}

/* method: get_key of class  gsControl */
static int tolua_gslua_gsControl_get_key00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
  player_controls k = ((player_controls) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_key'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_key(k);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_key'.",&tolua_err);
 return 0;
#endif
}

/* method: update of class  gsControl */
static int tolua_gslua_gsControl_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update'",NULL);
#endif
 {
  self->update();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}

/* method: get_pressed_key of class  gsControl */
static int tolua_gslua_gsControl_get_pressed_key00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_pressed_key'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_pressed_key();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_pressed_key'.",&tolua_err);
 return 0;
#endif
}

/* method: enable of class  gsControl */
static int tolua_gslua_gsControl_enable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'enable'",NULL);
#endif
 {
  self->enable();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'enable'.",&tolua_err);
 return 0;
#endif
}

/* method: disable of class  gsControl */
static int tolua_gslua_gsControl_disable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'disable'",NULL);
#endif
 {
  self->disable();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'disable'.",&tolua_err);
 return 0;
#endif
}

/* method: get_disabled of class  gsControl */
static int tolua_gslua_gsControl_get_disabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_disabled'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_disabled();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_disabled'.",&tolua_err);
 return 0;
#endif
}

/* method: left of class  gsControl */
static int tolua_gslua_gsControl_left00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'left'",NULL);
#endif
 {
  int tolua_ret = (int)  self->left();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'left'.",&tolua_err);
 return 0;
#endif
}

/* method: right of class  gsControl */
static int tolua_gslua_gsControl_right00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'right'",NULL);
#endif
 {
  int tolua_ret = (int)  self->right();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'right'.",&tolua_err);
 return 0;
#endif
}

/* method: thrust of class  gsControl */
static int tolua_gslua_gsControl_thrust00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'thrust'",NULL);
#endif
 {
  int tolua_ret = (int)  self->thrust();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'thrust'.",&tolua_err);
 return 0;
#endif
}

/* method: shoot of class  gsControl */
static int tolua_gslua_gsControl_shoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'shoot'",NULL);
#endif
 {
  int tolua_ret = (int)  self->shoot();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'shoot'.",&tolua_err);
 return 0;
#endif
}

/* method: special of class  gsControl */
static int tolua_gslua_gsControl_special00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'special'",NULL);
#endif
 {
  int tolua_ret = (int)  self->special();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'special'.",&tolua_err);
 return 0;
#endif
}

/* method: extra_action of class  gsControl */
static int tolua_gslua_gsControl_extra_action00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'extra_action'",NULL);
#endif
 {
  int tolua_ret = (int)  self->extra_action();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'extra_action'.",&tolua_err);
 return 0;
#endif
}

/* method: nw of class  gsControl */
static int tolua_gslua_gsControl_nw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'nw'",NULL);
#endif
 {
  int tolua_ret = (int)  self->nw();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'nw'.",&tolua_err);
 return 0;
#endif
}

/* method: pw of class  gsControl */
static int tolua_gslua_gsControl_pw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'pw'",NULL);
#endif
 {
  int tolua_ret = (int)  self->pw();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'pw'.",&tolua_err);
 return 0;
#endif
}

/* method: w1 of class  gsControl */
static int tolua_gslua_gsControl_w100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w1'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w1();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w1'.",&tolua_err);
 return 0;
#endif
}

/* method: w2 of class  gsControl */
static int tolua_gslua_gsControl_w200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w2'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w2();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w2'.",&tolua_err);
 return 0;
#endif
}

/* method: w3 of class  gsControl */
static int tolua_gslua_gsControl_w300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w3'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w3();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w3'.",&tolua_err);
 return 0;
#endif
}

/* method: w4 of class  gsControl */
static int tolua_gslua_gsControl_w400(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w4'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w4();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w4'.",&tolua_err);
 return 0;
#endif
}

/* method: w5 of class  gsControl */
static int tolua_gslua_gsControl_w500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w5'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w5();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w5'.",&tolua_err);
 return 0;
#endif
}

/* method: w6 of class  gsControl */
static int tolua_gslua_gsControl_w600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsControl",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsControl* self = (gsControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'w6'",NULL);
#endif
 {
  int tolua_ret = (int)  self->w6();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'w6'.",&tolua_err);
 return 0;
#endif
}

/* get function: osd of class  gsMap */
static int tolua_get_gsMap_osd_ptr(lua_State* tolua_S)
{
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'osd'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->osd,"gsOsd");
 return 1;
}

/* set function: osd of class  gsMap */
static int tolua_set_gsMap_osd_ptr(lua_State* tolua_S)
{
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'osd'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsOsd",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->osd = ((class gsOsd*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_scroll_x of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_scroll_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_scroll_y of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_scroll_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_scroll_to_x of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_to_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_to_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_scroll_to_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_to_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_scroll_to_y of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_to_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_to_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_scroll_to_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_to_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_map_x of class  gsMap */
static int tolua_gslua_gsMap_get_map_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_map_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_map_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_map_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_map_y of class  gsMap */
static int tolua_gslua_gsMap_get_map_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_map_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_map_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_map_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_scroll_speed_x of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_speed_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_speed_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_scroll_speed_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_speed_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_scroll_speed_y of class  gsMap */
static int tolua_gslua_gsMap_get_scroll_speed_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_scroll_speed_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_scroll_speed_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_scroll_speed_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_fscroll_x of class  gsMap */
static int tolua_gslua_gsMap_get_fscroll_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_fscroll_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_fscroll_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_fscroll_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_fscroll_y of class  gsMap */
static int tolua_gslua_gsMap_get_fscroll_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_fscroll_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_fscroll_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_fscroll_y'.",&tolua_err);
 return 0;
#endif
}

/* method: is_redraw_needed of class  gsMap */
static int tolua_gslua_gsMap_is_redraw_needed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_redraw_needed'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_redraw_needed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_redraw_needed'.",&tolua_err);
 return 0;
#endif
}

/* method: request_redraw of class  gsMap */
static int tolua_gslua_gsMap_request_redraw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'request_redraw'",NULL);
#endif
 {
  self->request_redraw();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'request_redraw'.",&tolua_err);
 return 0;
#endif
}

/* method: get_style of class  gsMap */
static int tolua_gslua_gsMap_get_style00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_style'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_style();
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_style'.",&tolua_err);
 return 0;
#endif
}

/* method: restrict_scroll_x of class  gsMap */
static int tolua_gslua_gsMap_restrict_scroll_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int min = ((int)  tolua_tonumber(tolua_S,2,0));
  int max = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'restrict_scroll_x'",NULL);
#endif
 {
  self->restrict_scroll_x(min,max);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'restrict_scroll_x'.",&tolua_err);
 return 0;
#endif
}

/* method: restrict_scroll_y of class  gsMap */
static int tolua_gslua_gsMap_restrict_scroll_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int min = ((int)  tolua_tonumber(tolua_S,2,0));
  int max = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'restrict_scroll_y'",NULL);
#endif
 {
  self->restrict_scroll_y(min,max);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'restrict_scroll_y'.",&tolua_err);
 return 0;
#endif
}

/* method: scroll_to of class  gsMap */
static int tolua_gslua_gsMap_scroll_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  float s = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'scroll_to'",NULL);
#endif
 {
  self->scroll_to(x,y,s);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'scroll_to'.",&tolua_err);
 return 0;
#endif
}

/* method: scroll_to of class  gsMap */
static int tolua_gslua_gsMap_scroll_to01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"gsPlayer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  class gsPlayer* tolua_var_14 = ((class gsPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'scroll_to'",NULL);
#endif
 {
  self->scroll_to(tolua_var_14);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsMap_scroll_to00(tolua_S);
}

/* method: scroll_to of class  gsMap */
static int tolua_gslua_gsMap_scroll_to02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"gsPlayer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  class gsPlayer* tolua_var_15 = ((class gsPlayer*)  tolua_tousertype(tolua_S,2,0));
  float s = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'scroll_to'",NULL);
#endif
 {
  self->scroll_to(tolua_var_15,s);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsMap_scroll_to01(tolua_S);
}

/* method: scroll of class  gsMap */
static int tolua_gslua_gsMap_scroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'scroll'",NULL);
#endif
 {
  self->scroll();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'scroll'.",&tolua_err);
 return 0;
#endif
}

/* method: redraw of class  gsMap */
static int tolua_gslua_gsMap_redraw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'redraw'",NULL);
#endif
 {
  self->redraw();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'redraw'.",&tolua_err);
 return 0;
#endif
}

/* method: is_pixel of class  gsMap */
static int tolua_gslua_gsMap_is_pixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_pixel'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_pixel(x,y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_pixel'.",&tolua_err);
 return 0;
#endif
}

/* method: get_pixel of class  gsMap */
static int tolua_gslua_gsMap_get_pixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_pixel'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_pixel(x,y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_pixel'.",&tolua_err);
 return 0;
#endif
}

/* method: is_tile of class  gsMap */
static int tolua_gslua_gsMap_is_tile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_tile'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_tile(x,y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_tile'.",&tolua_err);
 return 0;
#endif
}

/* method: is_on_screen of class  gsMap */
static int tolua_gslua_gsMap_is_on_screen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int w = ((int)  tolua_tonumber(tolua_S,4,0));
  int h = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_on_screen'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_on_screen(x,y,w,h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_on_screen'.",&tolua_err);
 return 0;
#endif
}

/* method: is_on_map of class  gsMap */
static int tolua_gslua_gsMap_is_on_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int w = ((int)  tolua_tonumber(tolua_S,4,0));
  int h = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_on_map'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_on_map(x,y,w,h);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_on_map'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_rectangle of class  gsMap */
static int tolua_gslua_gsMap_draw_rectangle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_rectangle'",NULL);
#endif
 {
  self->draw_rectangle(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_rectangle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_rectangle of class  gsMap */
static int tolua_gslua_gsMap_draw_filled_rectangle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_rectangle'",NULL);
#endif
 {
  self->draw_filled_rectangle(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_rectangle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_pixel of class  gsMap */
static int tolua_gslua_gsMap_draw_pixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_pixel'",NULL);
#endif
 {
  self->draw_pixel(col,px,py);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_pixel'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_circle of class  gsMap */
static int tolua_gslua_gsMap_draw_circle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int r = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_circle'",NULL);
#endif
 {
  self->draw_circle(col,px,py,r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_circle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_circle of class  gsMap */
static int tolua_gslua_gsMap_draw_filled_circle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int r = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_circle'",NULL);
#endif
 {
  self->draw_filled_circle(col,px,py,r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_circle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_ellipse of class  gsMap */
static int tolua_gslua_gsMap_draw_ellipse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int rx = ((int)  tolua_tonumber(tolua_S,5,0));
  int ry = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_ellipse'",NULL);
#endif
 {
  self->draw_ellipse(col,px,py,rx,ry);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_ellipse'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_ellipse of class  gsMap */
static int tolua_gslua_gsMap_draw_filled_ellipse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int rx = ((int)  tolua_tonumber(tolua_S,5,0));
  int ry = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_ellipse'",NULL);
#endif
 {
  self->draw_filled_ellipse(col,px,py,rx,ry);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_ellipse'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_line of class  gsMap */
static int tolua_gslua_gsMap_draw_line00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_line'",NULL);
#endif
 {
  self->draw_line(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_line'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_text of class  gsMap */
static int tolua_gslua_gsMap_draw_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int fnum = ((int)  tolua_tonumber(tolua_S,2,0));
  char* t = ((char*)  tolua_tostring(tolua_S,3,0));
  int col = ((int)  tolua_tonumber(tolua_S,4,0));
  int px = ((int)  tolua_tonumber(tolua_S,5,0));
  int py = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_text'",NULL);
#endif
 {
  self->draw_text(fnum,t,col,px,py);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_text'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_sprite of class  gsMap */
static int tolua_gslua_gsMap_draw_sprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int imgnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int frame = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_sprite'",NULL);
#endif
 {
  self->draw_sprite(imgnum,px,py,frame);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_sprite'.",&tolua_err);
 return 0;
#endif
}

/* method: fill_area of class  gsMap */
static int tolua_gslua_gsMap_fill_area00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsMap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsMap* self = (gsMap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'fill_area'",NULL);
#endif
 {
  self->fill_area(col,px,py);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'fill_area'.",&tolua_err);
 return 0;
#endif
}

/* get function: playmap */
static int tolua_get_gslua_playmap(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=2)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)playmap[tolua_index],"gsMap");
 return 1;
}

/* set function: playmap */
static int tolua_set_gslua_playmap(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=2)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
  playmap[tolua_index] = ((gsMap*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* method: get_prev of class  Explosion */
static int tolua_gslua_Explosion_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Explosion* tolua_ret = (Explosion*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Explosion");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Explosion */
static int tolua_gslua_Explosion_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Explosion* tolua_ret = (Explosion*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Explosion");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  Explosion */
static int tolua_gslua_Explosion_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Explosion */
static int tolua_gslua_Explosion_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  Explosion */
static int tolua_gslua_Explosion_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  Explosion */
static int tolua_gslua_Explosion_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Explosion */
static int tolua_gslua_Explosion_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_starttime of class  Explosion */
static int tolua_gslua_Explosion_get_starttime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_starttime'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_starttime();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_starttime'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframe of class  Explosion */
static int tolua_gslua_Explosion_get_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframe of class  Explosion */
static int tolua_gslua_Explosion_get_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframet of class  Explosion */
static int tolua_gslua_Explosion_get_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_player_sig of class  Explosion */
static int tolua_gslua_Explosion_get_player_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Explosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Explosion* self = (Explosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_player_sig'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->get_player_sig();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first of class  gsExplosion */
static int tolua_gslua_gsExplosion_get_first00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsExplosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsExplosion* self = (gsExplosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first'",NULL);
#endif
 {
  Explosion* tolua_ret = (Explosion*)  self->get_first();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Explosion");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last of class  gsExplosion */
static int tolua_gslua_gsExplosion_get_last00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsExplosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsExplosion* self = (gsExplosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last'",NULL);
#endif
 {
  Explosion* tolua_ret = (Explosion*)  self->get_last();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Explosion");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsExplosion */
static int tolua_gslua_gsExplosion_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsExplosion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsExplosion* self = (gsExplosion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsExplosion */
static int tolua_gslua_gsExplosion_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsExplosion",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsExplosion* self = (gsExplosion*)  tolua_tousertype(tolua_S,1,0);
  int what = ((int)  tolua_tonumber(tolua_S,2,0));
  int speed = ((int)  tolua_tonumber(tolua_S,3,0));
  int pstart = ((int)  tolua_tonumber(tolua_S,4,0));
  int px = ((int)  tolua_tonumber(tolua_S,5,0));
  int py = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Explosion* tolua_ret = (Explosion*)  self->add(what,speed,pstart,px,py);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Explosion");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  Crater */
static int tolua_gslua_Crater_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Crater */
static int tolua_gslua_Crater_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Crater */
static int tolua_gslua_Crater_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Crater */
static int tolua_gslua_Crater_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_size of class  Crater */
static int tolua_gslua_Crater_get_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_size'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_size();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_size'.",&tolua_err);
 return 0;
#endif
}

/* method: set_size of class  Crater */
static int tolua_gslua_Crater_set_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_size'",NULL);
#endif
 {
  self->set_size(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_size'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  Crater */
static int tolua_gslua_Crater_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Crater* tolua_ret = (Crater*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Crater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: set_prev of class  Crater */
static int tolua_gslua_Crater_set_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
  Crater* n = ((Crater*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_prev'",NULL);
#endif
 {
  self->set_prev(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Crater */
static int tolua_gslua_Crater_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Crater* tolua_ret = (Crater*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Crater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Crater */
static int tolua_gslua_Crater_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Crater",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Crater* self = (Crater*)  tolua_tousertype(tolua_S,1,0);
  Crater* n = ((Crater*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first of class  gsCrater */
static int tolua_gslua_gsCrater_get_first00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsCrater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsCrater* self = (gsCrater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first'",NULL);
#endif
 {
  Crater* tolua_ret = (Crater*)  self->get_first();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Crater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last of class  gsCrater */
static int tolua_gslua_gsCrater_get_last00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsCrater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsCrater* self = (gsCrater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last'",NULL);
#endif
 {
  Crater* tolua_ret = (Crater*)  self->get_last();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Crater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsCrater */
static int tolua_gslua_gsCrater_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsCrater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsCrater* self = (gsCrater*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsCrater */
static int tolua_gslua_gsCrater_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsCrater",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsCrater* self = (gsCrater*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int size = ((int)  tolua_tonumber(tolua_S,4,0));
  int instantdraw = ((int)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  self->add(x,y,size,instantdraw);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  gsCrater */
static int tolua_gslua_gsCrater_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsCrater",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Crater",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsCrater* self = (gsCrater*)  tolua_tousertype(tolua_S,1,0);
  Crater* tolua_var_16 = ((Crater*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(tolua_var_16);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* get function: pixels of class  gsEffect */
static int tolua_get_gsEffect_pixels_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'pixels'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pixels,"gsPixelRain");
 return 1;
}

/* set function: pixels of class  gsEffect */
static int tolua_set_gsEffect_pixels_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'pixels'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsPixelRain",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pixels = ((gsPixelRain*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: bullets of class  gsEffect */
static int tolua_get_gsEffect_bullets_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->bullets,"gsBullet");
 return 1;
}

/* set function: bullets of class  gsEffect */
static int tolua_set_gsEffect_bullets_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'bullets'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsBullet",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bullets = ((gsBullet*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: explosions of class  gsEffect */
static int tolua_get_gsEffect_explosions_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'explosions'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->explosions,"gsExplosion");
 return 1;
}

/* set function: explosions of class  gsEffect */
static int tolua_set_gsEffect_explosions_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'explosions'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsExplosion",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->explosions = ((gsExplosion*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: craters of class  gsEffect */
static int tolua_get_gsEffect_craters_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'craters'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->craters,"gsCrater");
 return 1;
}

/* set function: craters of class  gsEffect */
static int tolua_set_gsEffect_craters_ptr(lua_State* tolua_S)
{
  gsEffect* self = (gsEffect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'craters'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsCrater",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->craters = ((gsCrater*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: effects */
static int tolua_get_effects_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)effects,"gsEffect");
 return 1;
}

/* set function: effects */
static int tolua_set_effects_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsEffect",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  effects = ((gsEffect*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_prev of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  UserImageAnim* tolua_ret = (UserImageAnim*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"UserImageAnim");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  UserImageAnim* tolua_ret = (UserImageAnim*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"UserImageAnim");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_loop of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_loop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_loop'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_loop();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_loop'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframet of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframet of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframe'",NULL);
#endif
 {
  self->set_curframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframe'",NULL);
#endif
 {
  self->set_maxframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframet of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframet'",NULL);
#endif
 {
  self->set_curframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframet of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframet'",NULL);
#endif
 {
  self->set_maxframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_startframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_startframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_startframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_startframe of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_startframe'",NULL);
#endif
 {
  self->set_startframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_image of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_get_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_image'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_image();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_image'.",&tolua_err);
 return 0;
#endif
}

/* method: set_image of class  UserImageAnim */
static int tolua_gslua_UserImageAnim_set_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  UserImageAnim* self = (UserImageAnim*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_image'",NULL);
#endif
 {
  self->set_image(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_image'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: get_mode of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_mode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_mode'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_mode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_mode'.",&tolua_err);
 return 0;
#endif
}

/* method: get_sig of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_sig'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_sig();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: get_active of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_active'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_active();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_active'.",&tolua_err);
 return 0;
#endif
}

/* method: get_timeout of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_timeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_timeout'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_timeout();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_timeout'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last_clear_color of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_last_clear_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last_clear_color'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_last_clear_color();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last_clear_color'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_prev01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
tolua_lerror:
 return tolua_gslua_OsdBitmap_get_prev00(tolua_S);
}

/* method: get_next of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_next01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
tolua_lerror:
 return tolua_gslua_OsdBitmap_get_next00(tolua_S);
}

/* method: set_mode of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_mode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_mode'",NULL);
#endif
 {
  self->set_mode(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_mode'.",&tolua_err);
 return 0;
#endif
}

/* method: set_active of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_active'",NULL);
#endif
 {
  self->set_active(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_active'.",&tolua_err);
 return 0;
#endif
}

/* method: set_timeout of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_timeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_timeout'",NULL);
#endif
 {
  self->set_timeout(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_timeout'.",&tolua_err);
 return 0;
#endif
}

/* method: set_visible of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_set_visible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_visible'",NULL);
#endif
 {
  self->set_visible(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_visible'.",&tolua_err);
 return 0;
#endif
}

/* method: get_visible of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_get_visible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_visible'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_visible();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_visible'.",&tolua_err);
 return 0;
#endif
}

/* method: clear of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
 {
  self->clear(col);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_pixel of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_pixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_pixel'",NULL);
#endif
 {
  self->draw_pixel(col,x,y);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_pixel'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_rectangle of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_rectangle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_rectangle'",NULL);
#endif
 {
  self->draw_rectangle(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_rectangle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_rectangle of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_filled_rectangle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_rectangle'",NULL);
#endif
 {
  self->draw_filled_rectangle(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_rectangle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_line of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_line00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int y1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int x2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int y2 = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_line'",NULL);
#endif
 {
  self->draw_line(col,x1,y1,x2,y2);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_line'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_circle of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_circle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int r = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_circle'",NULL);
#endif
 {
  self->draw_circle(col,x,y,r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_circle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_circle of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_filled_circle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int r = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_circle'",NULL);
#endif
 {
  self->draw_filled_circle(col,x,y,r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_circle'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_ellipse of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_ellipse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int rx = ((int)  tolua_tonumber(tolua_S,5,0));
  int ry = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_ellipse'",NULL);
#endif
 {
  self->draw_ellipse(col,x,y,rx,ry);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_ellipse'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_filled_ellipse of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_filled_ellipse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int rx = ((int)  tolua_tonumber(tolua_S,5,0));
  int ry = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_filled_ellipse'",NULL);
#endif
 {
  self->draw_filled_ellipse(col,x,y,rx,ry);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_filled_ellipse'.",&tolua_err);
 return 0;
#endif
}

/* method: fill_area of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_fill_area00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int col = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'fill_area'",NULL);
#endif
 {
  self->fill_area(col,x,y);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'fill_area'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_text of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int f = ((int)  tolua_tonumber(tolua_S,2,0));
  char* t = ((char*)  tolua_tostring(tolua_S,3,0));
  int col = ((int)  tolua_tonumber(tolua_S,4,0));
  int x = ((int)  tolua_tonumber(tolua_S,5,0));
  int y = ((int)  tolua_tonumber(tolua_S,6,0));
  int mode = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_text'",NULL);
#endif
 {
  self->draw_text(f,t,col,x,y,mode);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_text'.",&tolua_err);
 return 0;
#endif
}

/* method: draw_typewriter_text of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_draw_typewriter_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  char* t = ((char*)  tolua_tostring(tolua_S,2,0));
  int starty = ((int)  tolua_tonumber(tolua_S,3,10));
  int speed = ((int)  tolua_tonumber(tolua_S,4,3));
  int rect = ((int)  tolua_tonumber(tolua_S,5,1));
  int remove = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw_typewriter_text'",NULL);
#endif
 {
  self->draw_typewriter_text(t,starty,speed,rect,remove);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw_typewriter_text'.",&tolua_err);
 return 0;
#endif
}

/* method: add_image of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_add_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  int px = ((int)  tolua_tonumber(tolua_S,3,0));
  int py = ((int)  tolua_tonumber(tolua_S,4,0));
  int maxframet = ((int)  tolua_tonumber(tolua_S,5,5));
  int loop = ((int)  tolua_tonumber(tolua_S,6,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add_image'",NULL);
#endif
 {
  UserImageAnim* tolua_ret = (UserImageAnim*)  self->add_image(num,px,py,maxframet,loop);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"UserImageAnim");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add_image'.",&tolua_err);
 return 0;
#endif
}

/* method: remove_image of class  OsdBitmap */
static int tolua_gslua_OsdBitmap_remove_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OsdBitmap",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"UserImageAnim",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  OsdBitmap* self = (OsdBitmap*)  tolua_tousertype(tolua_S,1,0);
  UserImageAnim* tolua_var_17 = ((UserImageAnim*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove_image'",NULL);
#endif
 {
  self->remove_image(tolua_var_17);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove_image'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first_osdbit of class  gsOsd */
static int tolua_gslua_gsOsd_get_first_osdbit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first_osdbit'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_first_osdbit();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first_osdbit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last_osdbit of class  gsOsd */
static int tolua_gslua_gsOsd_get_last_osdbit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last_osdbit'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_last_osdbit();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last_osdbit'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsOsd */
static int tolua_gslua_gsOsd_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
  int sig = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int w = ((int)  tolua_tonumber(tolua_S,5,0));
  int h = ((int)  tolua_tonumber(tolua_S,6,0));
  int mode = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->add(sig,x,y,w,h,mode);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: add_label of class  gsOsd */
static int tolua_gslua_gsOsd_add_label00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
 !tolua_isstring(tolua_S,7,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,9,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
  int sig = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int mode = ((int)  tolua_tonumber(tolua_S,5,0));
  int fnum = ((int)  tolua_tonumber(tolua_S,6,0));
  char* t = ((char*)  tolua_tostring(tolua_S,7,0));
  int col = ((int)  tolua_tonumber(tolua_S,8,0));
  int rect = ((int)  tolua_tonumber(tolua_S,9,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add_label'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->add_label(sig,x,y,mode,fnum,t,col,rect);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add_label'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  gsOsd */
static int tolua_gslua_gsOsd_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"OsdBitmap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
  OsdBitmap* tolua_var_18 = ((OsdBitmap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(tolua_var_18);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_osdbit of class  gsOsd */
static int tolua_gslua_gsOsd_get_osdbit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsOsd",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsOsd* self = (gsOsd*)  tolua_tousertype(tolua_S,1,0);
  int sig = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_osdbit'",NULL);
#endif
 {
  OsdBitmap* tolua_ret = (OsdBitmap*)  self->get_osdbit(sig);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OsdBitmap");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_osdbit'.",&tolua_err);
 return 0;
#endif
}

/* method: log of class  gsError */
static int tolua_gslua_gsError_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsError",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isstring(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsError* self = (gsError*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_19 = ((int)  tolua_tonumber(tolua_S,2,0));
  char* tolua_var_20 = ((char*)  tolua_tostring(tolua_S,3,0));
  char* tolua_var_21 = ((char*)  tolua_tostring(tolua_S,4,0));
  long tolua_var_22 = ((long)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
 {
  self->log(tolua_var_19,tolua_var_20,tolua_var_21,tolua_var_22);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'log'.",&tolua_err);
 return 0;
#endif
}

/* method: log of class  gsError */
static int tolua_gslua_gsError_log01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsError",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isstring(tolua_S,4,0,&tolua_err) ||
 !tolua_isstring(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsError* self = (gsError*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_23 = ((int)  tolua_tonumber(tolua_S,2,0));
  char* tolua_var_24 = ((char*)  tolua_tostring(tolua_S,3,0));
  char* tolua_var_25 = ((char*)  tolua_tostring(tolua_S,4,0));
  char* tolua_var_26 = ((char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
 {
  self->log(tolua_var_23,tolua_var_24,tolua_var_25,tolua_var_26);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsError_log00(tolua_S);
}

/* method: log of class  gsError */
static int tolua_gslua_gsError_log02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsError",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isstring(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsError* self = (gsError*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_27 = ((int)  tolua_tonumber(tolua_S,2,0));
  char* tolua_var_28 = ((char*)  tolua_tostring(tolua_S,3,0));
  char* tolua_var_29 = ((char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
 {
  self->log(tolua_var_27,tolua_var_28,tolua_var_29);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsError_log01(tolua_S);
}

/* method: log of class  gsError */
static int tolua_gslua_gsError_log03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsError",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsError* self = (gsError*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_30 = ((int)  tolua_tonumber(tolua_S,2,0));
  char* tolua_var_31 = ((char*)  tolua_tostring(tolua_S,3,0));
  float tolua_var_32 = ((float)  tolua_tonumber(tolua_S,4,0));
  float tolua_var_33 = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
 {
  self->log(tolua_var_30,tolua_var_31,tolua_var_32,tolua_var_33);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsError_log02(tolua_S);
}

/* method: log of class  gsError */
static int tolua_gslua_gsError_log04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsError",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsError* self = (gsError*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_34 = ((int)  tolua_tonumber(tolua_S,2,0));
  char* tolua_var_35 = ((char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
 {
  self->log(tolua_var_34,tolua_var_35);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsError_log03(tolua_S);
}

/* get function: errors */
static int tolua_get_errors_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)errors,"gsError");
 return 1;
}

/* set function: errors */
static int tolua_set_errors_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsError",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  errors = ((gsError*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: player of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_player_ptr(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->player,"gsPlayer");
 return 1;
}

/* set function: player of class  gsNetPlayer */
static int tolua_set_gsNetPlayer_player_ptr(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'player'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsPlayer",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->player = ((gsPlayer*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: network of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_network(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'network'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->network);
 return 1;
}

/* set function: network of class  gsNetPlayer */
static int tolua_set_gsNetPlayer_network(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'network'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->network = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* get function: name of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_name(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* get function: ship_file of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_ship_file(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'ship_file'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->ship_file);
 return 1;
}

/* get function: shipimg_file of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_shipimg_file(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'shipimg_file'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->shipimg_file);
 return 1;
}

/* get function: net_id of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_net_id(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'net_id'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->net_id,"PlayerID");
 return 1;
}

/* set function: net_id of class  gsNetPlayer */
static int tolua_set_gsNetPlayer_net_id(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'net_id'",NULL);
 if (!tolua_isusertype(tolua_S,2,"PlayerID",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->net_id = *((PlayerID*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: ready of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_ready(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'ready'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->ready);
 return 1;
}

/* set function: ready of class  gsNetPlayer */
static int tolua_set_gsNetPlayer_ready(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'ready'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ready = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* get function: in_game of class  gsNetPlayer */
static int tolua_get_gsNetPlayer_in_game(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'in_game'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->in_game);
 return 1;
}

/* set function: in_game of class  gsNetPlayer */
static int tolua_set_gsNetPlayer_in_game(lua_State* tolua_S)
{
  gsNetPlayer* self = (gsNetPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'in_game'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->in_game = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* method: add of class  gsChat */
static int tolua_gslua_gsChat_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
  char* msg = ((char*)  tolua_tostring(tolua_S,2,0));
  char* player = ((char*)  tolua_tostring(tolua_S,3,0));
  int color = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  self->add(msg,player,color);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: get_message of class  gsChat */
static int tolua_gslua_gsChat_get_message00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_message'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_message(num);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_message'.",&tolua_err);
 return 0;
#endif
}

/* method: get_player of class  gsChat */
static int tolua_gslua_gsChat_get_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_player'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_player(num);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_player'.",&tolua_err);
 return 0;
#endif
}

/* method: get_message_with_playername of class  gsChat */
static int tolua_gslua_gsChat_get_message_with_playername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  char* buf = ((char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_message_with_playername'",NULL);
#endif
 {
  char* tolua_ret = (char*)  self->get_message_with_playername(num,buf);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_message_with_playername'.",&tolua_err);
 return 0;
#endif
}

/* method: get_color of class  gsChat */
static int tolua_gslua_gsChat_get_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_color'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_color(num);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_color'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsChat */
static int tolua_gslua_gsChat_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: clear of class  gsChat */
static int tolua_gslua_gsChat_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsChat",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsChat* self = (gsChat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
 {
  self->clear();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}

/* get function: chat of class  gsNetwork */
static int tolua_get_gsNetwork_chat(lua_State* tolua_S)
{
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'chat'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->chat,"gsChat");
 return 1;
}

/* set function: chat of class  gsNetwork */
static int tolua_set_gsNetwork_chat(lua_State* tolua_S)
{
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'chat'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsChat",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->chat = *((gsChat*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: is_server of class  gsNetwork */
static int tolua_gslua_gsNetwork_is_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsNetwork",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_server'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_server();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_server'.",&tolua_err);
 return 0;
#endif
}

/* method: is_client of class  gsNetwork */
static int tolua_gslua_gsNetwork_is_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsNetwork",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'is_client'",NULL);
#endif
 {
  int tolua_ret = (int)  self->is_client();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'is_client'.",&tolua_err);
 return 0;
#endif
}

/* method: get_netplayer of class  gsNetwork */
static int tolua_gslua_gsNetwork_get_netplayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsNetwork",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_netplayer'",NULL);
#endif
 {
  gsNetPlayer* tolua_ret = (gsNetPlayer*)  self->get_netplayer(n);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"gsNetPlayer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_netplayer'.",&tolua_err);
 return 0;
#endif
}

/* method: get_netplayer_by_id of class  gsNetwork */
static int tolua_gslua_gsNetwork_get_netplayer_by_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsNetwork",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"PlayerID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
  PlayerID id = *((PlayerID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_netplayer_by_id'",NULL);
#endif
 {
  gsNetPlayer* tolua_ret = (gsNetPlayer*)  self->get_netplayer_by_id(id);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"gsNetPlayer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_netplayer_by_id'.",&tolua_err);
 return 0;
#endif
}

/* method: get_netplayers of class  gsNetwork */
static int tolua_gslua_gsNetwork_get_netplayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsNetwork",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsNetwork* self = (gsNetwork*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_netplayers'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_netplayers();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_netplayers'.",&tolua_err);
 return 0;
#endif
}


/* get function: net */
static int tolua_get_net_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)net,"gsNetwork");
 return 1;
}

/* set function: net */
static int tolua_set_net_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsNetwork",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  net = ((gsNetwork*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_play_music of class  gsSound */
static int tolua_gslua_gsSound_get_play_music00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_play_music'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_play_music();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_play_music'.",&tolua_err);
 return 0;
#endif
}

/* method: get_play_sound of class  gsSound */
static int tolua_gslua_gsSound_get_play_sound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_play_sound'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_play_sound();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_play_sound'.",&tolua_err);
 return 0;
#endif
}

/* method: play_sound of class  gsSound */
static int tolua_gslua_gsSound_play_sound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
  int sound = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int v = ((int)  tolua_tonumber(tolua_S,5,100));
  int f = ((int)  tolua_tonumber(tolua_S,6,1000));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'play_sound'",NULL);
#endif
 {
  self->play_sound(sound,x,y,v,f);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'play_sound'.",&tolua_err);
 return 0;
#endif
}

/* method: load_sound of class  gsSound */
static int tolua_gslua_gsSound_load_sound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
  char* f = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'load_sound'",NULL);
#endif
 {
  int tolua_ret = (int)  self->load_sound(f);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'load_sound'.",&tolua_err);
 return 0;
#endif
}

/* method: replace_sound of class  gsSound */
static int tolua_gslua_gsSound_replace_sound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
  int what = ((int)  tolua_tonumber(tolua_S,2,0));
  int with = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'replace_sound'",NULL);
#endif
 {
  self->replace_sound(what,with);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'replace_sound'.",&tolua_err);
 return 0;
#endif
}

/* method: load_music of class  gsSound */
static int tolua_gslua_gsSound_load_music00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
  char* f = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'load_music'",NULL);
#endif
 {
  self->load_music(f);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'load_music'.",&tolua_err);
 return 0;
#endif
}

/* method: start_music of class  gsSound */
static int tolua_gslua_gsSound_start_music00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'start_music'",NULL);
#endif
 {
  self->start_music();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'start_music'.",&tolua_err);
 return 0;
#endif
}

/* method: stop_music of class  gsSound */
static int tolua_gslua_gsSound_stop_music00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'stop_music'",NULL);
#endif
 {
  self->stop_music();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'stop_music'.",&tolua_err);
 return 0;
#endif
}

/* method: destroy_music of class  gsSound */
static int tolua_gslua_gsSound_destroy_music00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'destroy_music'",NULL);
#endif
 {
  self->destroy_music();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'destroy_music'.",&tolua_err);
 return 0;
#endif
}

/* method: set_music_volume of class  gsSound */
static int tolua_gslua_gsSound_set_music_volume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_music_volume'",NULL);
#endif
 {
  self->set_music_volume(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_music_volume'.",&tolua_err);
 return 0;
#endif
}

/* method: get_music_volume of class  gsSound */
static int tolua_gslua_gsSound_get_music_volume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsSound",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsSound* self = (gsSound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_music_volume'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_music_volume();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_music_volume'.",&tolua_err);
 return 0;
#endif
}

/* get function: gsound */
static int tolua_get_gsound_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)gsound,"gsSound");
 return 1;
}

/* set function: gsound */
static int tolua_set_gsound_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsSound",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  gsound = ((gsSound*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: new of class  Waypoint */
static int tolua_gslua_Waypoint_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int speed = ((int)  tolua_tonumber(tolua_S,4,1));
  int pause = ((int)  tolua_tonumber(tolua_S,5,0));
 {
  Waypoint* tolua_ret = (Waypoint*)  new Waypoint(x,y,speed,pause);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Waypoint");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Waypoint */
static int tolua_gslua_Waypoint_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  Waypoint* n = ((Waypoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Waypoint */
static int tolua_gslua_Waypoint_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Waypoint* tolua_ret = (Waypoint*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Waypoint");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  Waypoint */
static int tolua_gslua_Waypoint_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Waypoint */
static int tolua_gslua_Waypoint_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_speed of class  Waypoint */
static int tolua_gslua_Waypoint_get_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_speed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_speed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_pause of class  Waypoint */
static int tolua_gslua_Waypoint_get_pause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_pause'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_pause();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_pause'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curpause of class  Waypoint */
static int tolua_gslua_Waypoint_get_curpause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curpause'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curpause();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curpause'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Waypoint */
static int tolua_gslua_Waypoint_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Waypoint */
static int tolua_gslua_Waypoint_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_speed of class  Waypoint */
static int tolua_gslua_Waypoint_set_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_speed'",NULL);
#endif
 {
  self->set_speed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_pause of class  Waypoint */
static int tolua_gslua_Waypoint_set_pause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_pause'",NULL);
#endif
 {
  self->set_pause(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_pause'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curpause of class  Waypoint */
static int tolua_gslua_Waypoint_set_curpause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Waypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Waypoint* self = (Waypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curpause'",NULL);
#endif
 {
  self->set_curpause(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curpause'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: get_mode of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_get_mode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_mode'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_mode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_mode'.",&tolua_err);
 return 0;
#endif
}

/* method: set_mode of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_set_mode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_mode'",NULL);
#endif
 {
  self->set_mode(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_mode'.",&tolua_err);
 return 0;
#endif
}

/* method: get_current of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_get_current00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_current'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_current();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_current'.",&tolua_err);
 return 0;
#endif
}

/* method: set_current of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_set_current00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_current'",NULL);
#endif
 {
  self->set_current(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_current'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  float spd = ((float)  tolua_tonumber(tolua_S,4,1));
  int pause = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Waypoint* tolua_ret = (Waypoint*)  self->add(x,y,spd,pause);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Waypoint");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Waypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
  Waypoint* tolua_var_36 = ((Waypoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(tolua_var_36);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: remove_all of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_remove_all00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove_all'",NULL);
#endif
 {
  self->remove_all();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove_all'.",&tolua_err);
 return 0;
#endif
}

/* method: get of class  gsWaypoint */
static int tolua_gslua_gsWaypoint_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsWaypoint",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsWaypoint* self = (gsWaypoint*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
 {
  Waypoint* tolua_ret = (Waypoint*)  self->get(n);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Waypoint");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}

/* get function: wp of class  Enemy */
static int tolua_get_Enemy_wp_ptr(lua_State* tolua_S)
{
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'wp'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->wp,"gsWaypoint");
 return 1;
}

/* set function: wp of class  Enemy */
static int tolua_set_Enemy_wp_ptr(lua_State* tolua_S)
{
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'wp'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsWaypoint",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wp = ((gsWaypoint*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_prev of class  Enemy */
static int tolua_gslua_Enemy_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Enemy */
static int tolua_gslua_Enemy_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: get_connected of class  Enemy */
static int tolua_gslua_Enemy_get_connected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_connected'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->get_connected();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_connected'.",&tolua_err);
 return 0;
#endif
}

/* method: set_connected of class  Enemy */
static int tolua_gslua_Enemy_set_connected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  Enemy* tolua_var_37 = ((Enemy*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_connected'",NULL);
#endif
 {
  self->set_connected(tolua_var_37);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_connected'.",&tolua_err);
 return 0;
#endif
}

/* method: set_prev of class  Enemy */
static int tolua_gslua_Enemy_set_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  Enemy* n = ((Enemy*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_prev'",NULL);
#endif
 {
  self->set_prev(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Enemy */
static int tolua_gslua_Enemy_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  Enemy* n = ((Enemy*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_type of class  Enemy */
static int tolua_gslua_Enemy_set_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_type'",NULL);
#endif
 {
  self->set_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Enemy */
static int tolua_gslua_Enemy_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_nr of class  Enemy */
static int tolua_gslua_Enemy_get_nr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_nr'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_nr();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_nr'.",&tolua_err);
 return 0;
#endif
}

/* method: set_nr of class  Enemy */
static int tolua_gslua_Enemy_set_nr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_nr'",NULL);
#endif
 {
  self->set_nr(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_nr'.",&tolua_err);
 return 0;
#endif
}

/* method: get_sig of class  Enemy */
static int tolua_gslua_Enemy_get_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_sig'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->get_sig();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: set_sig of class  Enemy */
static int tolua_gslua_Enemy_set_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  unsigned long n = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_sig'",NULL);
#endif
 {
  self->set_sig(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframe of class  Enemy */
static int tolua_gslua_Enemy_set_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframe'",NULL);
#endif
 {
  self->set_maxframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframe of class  Enemy */
static int tolua_gslua_Enemy_get_maxframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxframet of class  Enemy */
static int tolua_gslua_Enemy_set_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxframet'",NULL);
#endif
 {
  self->set_maxframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxframet of class  Enemy */
static int tolua_gslua_Enemy_get_maxframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dispframe of class  Enemy */
static int tolua_gslua_Enemy_set_dispframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dispframe'",NULL);
#endif
 {
  self->set_dispframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dispframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_dispframe of class  Enemy */
static int tolua_gslua_Enemy_get_dispframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_dispframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_dispframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_dispframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframe of class  Enemy */
static int tolua_gslua_Enemy_set_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframe'",NULL);
#endif
 {
  self->set_curframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframe of class  Enemy */
static int tolua_gslua_Enemy_get_curframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframe'.",&tolua_err);
 return 0;
#endif
}

/* method: set_curframet of class  Enemy */
static int tolua_gslua_Enemy_set_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_curframet'",NULL);
#endif
 {
  self->set_curframet(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: get_curframet of class  Enemy */
static int tolua_gslua_Enemy_get_curframet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_curframet'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_curframet();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_curframet'.",&tolua_err);
 return 0;
#endif
}

/* method: set_startframe of class  Enemy */
static int tolua_gslua_Enemy_set_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_startframe'",NULL);
#endif
 {
  self->set_startframe(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_startframe of class  Enemy */
static int tolua_gslua_Enemy_get_startframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_startframe'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_startframe();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_startframe'.",&tolua_err);
 return 0;
#endif
}

/* method: get_x of class  Enemy */
static int tolua_gslua_Enemy_get_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_y of class  Enemy */
static int tolua_gslua_Enemy_get_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_x of class  Enemy */
static int tolua_gslua_Enemy_set_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_x'",NULL);
#endif
 {
  self->set_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_y of class  Enemy */
static int tolua_gslua_Enemy_set_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_y'",NULL);
#endif
 {
  self->set_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_width of class  Enemy */
static int tolua_gslua_Enemy_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_width();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}

/* method: get_height of class  Enemy */
static int tolua_gslua_Enemy_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_x of class  Enemy */
static int tolua_gslua_Enemy_set_bound_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_x'",NULL);
#endif
 {
  self->set_bound_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_y of class  Enemy */
static int tolua_gslua_Enemy_set_bound_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_y'",NULL);
#endif
 {
  self->set_bound_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_w of class  Enemy */
static int tolua_gslua_Enemy_set_bound_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_w'",NULL);
#endif
 {
  self->set_bound_w(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_w'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bound_h of class  Enemy */
static int tolua_gslua_Enemy_set_bound_h00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bound_h'",NULL);
#endif
 {
  self->set_bound_h(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bound_h'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xradius of class  Enemy */
static int tolua_gslua_Enemy_get_xradius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xradius'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_xradius();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xradius'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xradius of class  Enemy */
static int tolua_gslua_Enemy_set_xradius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xradius'",NULL);
#endif
 {
  self->set_xradius(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xradius'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yradius of class  Enemy */
static int tolua_gslua_Enemy_get_yradius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yradius'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_yradius();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yradius'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yradius of class  Enemy */
static int tolua_gslua_Enemy_set_yradius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yradius'",NULL);
#endif
 {
  self->set_yradius(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yradius'.",&tolua_err);
 return 0;
#endif
}

/* method: get_xspd of class  Enemy */
static int tolua_gslua_Enemy_get_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_xspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_xspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_yspd of class  Enemy */
static int tolua_gslua_Enemy_get_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_yspd'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_yspd();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_xspd of class  Enemy */
static int tolua_gslua_Enemy_set_xspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_xspd'",NULL);
#endif
 {
  self->set_xspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_xspd'.",&tolua_err);
 return 0;
#endif
}

/* method: set_yspd of class  Enemy */
static int tolua_gslua_Enemy_set_yspd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_yspd'",NULL);
#endif
 {
  self->set_yspd(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_yspd'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hit of class  Enemy */
static int tolua_gslua_Enemy_get_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hit'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxhit of class  Enemy */
static int tolua_gslua_Enemy_get_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxhit'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_maxhit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_hit of class  Enemy */
static int tolua_gslua_Enemy_set_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_hit'",NULL);
#endif
 {
  self->set_hit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_hit'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxhit of class  Enemy */
static int tolua_gslua_Enemy_set_maxhit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxhit'",NULL);
#endif
 {
  self->set_maxhit(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxhit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_onehit of class  Enemy */
static int tolua_gslua_Enemy_get_onehit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_onehit'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_onehit();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_onehit'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby of class  Enemy */
static int tolua_gslua_Enemy_get_hitby00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_hitby();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby_sx of class  Enemy */
static int tolua_gslua_Enemy_get_hitby_sx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby_sx'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hitby_sx();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby_sx'.",&tolua_err);
 return 0;
#endif
}

/* method: get_hitby_sy of class  Enemy */
static int tolua_gslua_Enemy_get_hitby_sy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_hitby_sy'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_hitby_sy();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_hitby_sy'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user of class  Enemy */
static int tolua_gslua_Enemy_get_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user of class  Enemy */
static int tolua_gslua_Enemy_set_user00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user'",NULL);
#endif
 {
  self->set_user(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_speed of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_speed'",NULL);
#endif
 {
  self->set_bullet_speed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_speed of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_speed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_speed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_speed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_speed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_speed_x of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_speed_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_speed_x'",NULL);
#endif
 {
  self->set_bullet_speed_x(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_speed_x'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_speed_x of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_speed_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_speed_x'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_speed_x();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_speed_x'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_speed_y of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_speed_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_speed_y'",NULL);
#endif
 {
  self->set_bullet_speed_y(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_speed_y'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_speed_y of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_speed_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_speed_y'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_speed_y();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_speed_y'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_freq of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_freq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_freq'",NULL);
#endif
 {
  self->set_bullet_freq(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_freq'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_freq of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_freq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_freq'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_freq();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_freq'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_strength of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_strength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_strength'",NULL);
#endif
 {
  self->set_bullet_strength(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_strength'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_strength of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_strength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_strength'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_strength();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_strength'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_hull_hitp of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_hull_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_hull_hitp'",NULL);
#endif
 {
  self->set_bullet_hull_hitp(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_hull_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_hull_hitp of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_hull_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_hull_hitp'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_hull_hitp();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_hull_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_shield_hitp of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_shield_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_shield_hitp'",NULL);
#endif
 {
  self->set_bullet_shield_hitp(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_shield_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_shield_hitp of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_shield_hitp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_shield_hitp'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_bullet_shield_hitp();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_shield_hitp'.",&tolua_err);
 return 0;
#endif
}

/* method: set_bullet_type of class  Enemy */
static int tolua_gslua_Enemy_set_bullet_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_bullet_type'",NULL);
#endif
 {
  self->set_bullet_type(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_bullet_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_bullet_type of class  Enemy */
static int tolua_gslua_Enemy_get_bullet_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_bullet_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_bullet_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_bullet_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_active of class  Enemy */
static int tolua_gslua_Enemy_get_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_active'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_active();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_active'.",&tolua_err);
 return 0;
#endif
}

/* method: set_active of class  Enemy */
static int tolua_gslua_Enemy_set_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_active'",NULL);
#endif
 {
  self->set_active(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_active'.",&tolua_err);
 return 0;
#endif
}

/* method: get_teleport_time of class  Enemy */
static int tolua_gslua_Enemy_get_teleport_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_teleport_time'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_teleport_time();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_teleport_time'.",&tolua_err);
 return 0;
#endif
}

/* method: set_teleport_time of class  Enemy */
static int tolua_gslua_Enemy_set_teleport_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_teleport_time'",NULL);
#endif
 {
  self->set_teleport_time(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_teleport_time'.",&tolua_err);
 return 0;
#endif
}

/* method: set_invisible of class  Enemy */
static int tolua_gslua_Enemy_set_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_invisible'",NULL);
#endif
 {
  self->set_invisible(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: get_invisible of class  Enemy */
static int tolua_gslua_Enemy_get_invisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_invisible'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_invisible();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_invisible'.",&tolua_err);
 return 0;
#endif
}

/* method: get_freezed of class  Enemy */
static int tolua_gslua_Enemy_get_freezed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_freezed'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_freezed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_freezed'.",&tolua_err);
 return 0;
#endif
}

/* method: get_shrinked of class  Enemy */
static int tolua_gslua_Enemy_get_shrinked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_shrinked'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_shrinked();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_shrinked'.",&tolua_err);
 return 0;
#endif
}

/* method: set_freezed of class  Enemy */
static int tolua_gslua_Enemy_set_freezed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_freezed'",NULL);
#endif
 {
  self->set_freezed(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_freezed'.",&tolua_err);
 return 0;
#endif
}

/* method: set_shrinked of class  Enemy */
static int tolua_gslua_Enemy_set_shrinked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_shrinked'",NULL);
#endif
 {
  self->set_shrinked(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_shrinked'.",&tolua_err);
 return 0;
#endif
}

/* method: set_maxshrink of class  Enemy */
static int tolua_gslua_Enemy_set_maxshrink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  float n = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_maxshrink'",NULL);
#endif
 {
  self->set_maxshrink(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_maxshrink'.",&tolua_err);
 return 0;
#endif
}

/* method: get_maxshrink of class  Enemy */
static int tolua_gslua_Enemy_get_maxshrink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_maxshrink'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_maxshrink();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_maxshrink'.",&tolua_err);
 return 0;
#endif
}

/* method: set_dont_continue of class  Enemy */
static int tolua_gslua_Enemy_set_dont_continue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_dont_continue'",NULL);
#endif
 {
  self->set_dont_continue(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_dont_continue'.",&tolua_err);
 return 0;
#endif
}

/* method: get_do_hooks of class  Enemy */
static int tolua_gslua_Enemy_get_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_do_hooks'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_do_hooks();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: set_do_hooks of class  Enemy */
static int tolua_gslua_Enemy_set_do_hooks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_do_hooks'",NULL);
#endif
 {
  self->set_do_hooks(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_do_hooks'.",&tolua_err);
 return 0;
#endif
}

/* method: get_current_sprite of class  Enemy */
static int tolua_gslua_Enemy_get_current_sprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_current_sprite'",NULL);
#endif
 {
  RLE_SPRITE* tolua_ret = (RLE_SPRITE*)  self->get_current_sprite();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RLE_SPRITE");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_current_sprite'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  Enemy */
static int tolua_gslua_Enemy_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: get_remove of class  Enemy */
static int tolua_gslua_Enemy_get_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_remove'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_remove();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_remove'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_int of class  Enemy */
static int tolua_gslua_Enemy_set_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  int d = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_int'",NULL);
#endif
 {
  self->set_user_int(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: set_user_float of class  Enemy */
static int tolua_gslua_Enemy_set_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
  float d = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_user_float'",NULL);
#endif
 {
  self->set_user_float(n,d);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_int of class  Enemy */
static int tolua_gslua_Enemy_get_user_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_int'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_user_int(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_int'.",&tolua_err);
 return 0;
#endif
}

/* method: get_user_float of class  Enemy */
static int tolua_gslua_Enemy_get_user_float00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_user_float'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_user_float(n);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_user_float'.",&tolua_err);
 return 0;
#endif
}

/* method: find_path of class  Enemy */
static int tolua_gslua_Enemy_find_path00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int tx = ((int)  tolua_tonumber(tolua_S,2,0));
  int ty = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'find_path'",NULL);
#endif
 {
  self->find_path(tx,ty);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'find_path'.",&tolua_err);
 return 0;
#endif
}

/* method: hit of class  Enemy */
static int tolua_gslua_Enemy_hit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  int from = ((int)  tolua_tonumber(tolua_S,2,0));
  float n = ((float)  tolua_tonumber(tolua_S,3,0));
  int x = ((int)  tolua_tonumber(tolua_S,4,0));
  int y = ((int)  tolua_tonumber(tolua_S,5,0));
  float fxspd = ((float)  tolua_tonumber(tolua_S,6,0));
  float fyspd = ((float)  tolua_tonumber(tolua_S,7,0));
  int weapon = ((int)  tolua_tonumber(tolua_S,8,-1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'hit'",NULL);
#endif
 {
  self->hit(from,n,x,y,fxspd,fyspd,weapon);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'hit'.",&tolua_err);
 return 0;
#endif
}

/* method: shoot of class  Enemy */
static int tolua_gslua_Enemy_shoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'shoot'",NULL);
#endif
 {
  self->shoot();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'shoot'.",&tolua_err);
 return 0;
#endif
}

/* method: move of class  Enemy */
static int tolua_gslua_Enemy_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'move'",NULL);
#endif
 {
  self->move();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}

/* method: update of class  Enemy */
static int tolua_gslua_Enemy_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update'",NULL);
#endif
 {
  self->update();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}

/* method: draw of class  Enemy */
static int tolua_gslua_Enemy_draw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"gsMap",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
  class gsMap* tolua_var_38 = ((class gsMap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'draw'",NULL);
#endif
 {
  self->draw(tolua_var_38);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'draw'.",&tolua_err);
 return 0;
#endif
}

/* method: dead of class  Enemy */
static int tolua_gslua_Enemy_dead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Enemy* self = (Enemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'dead'",NULL);
#endif
 {
  self->dead();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'dead'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsEnemy */
static int tolua_gslua_gsEnemy_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsEnemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsEnemy* self = (gsEnemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first_enemy of class  gsEnemy */
static int tolua_gslua_gsEnemy_get_first_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsEnemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsEnemy* self = (gsEnemy*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first_enemy'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->get_first_enemy();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first_enemy'.",&tolua_err);
 return 0;
#endif
}

/* method: get_enemy of class  gsEnemy */
static int tolua_gslua_gsEnemy_get_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsEnemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsEnemy* self = (gsEnemy*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_enemy'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->get_enemy(n);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_enemy'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsEnemy */
static int tolua_gslua_gsEnemy_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsEnemy",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,9,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,10,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,11,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,12,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,13,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsEnemy* self = (gsEnemy*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  int t = ((int)  tolua_tonumber(tolua_S,3,0));
  int px = ((int)  tolua_tonumber(tolua_S,4,0));
  int py = ((int)  tolua_tonumber(tolua_S,5,0));
  unsigned long psig = ((unsigned long)  tolua_tonumber(tolua_S,6,-1));
  float user1 = ((float)  tolua_tonumber(tolua_S,7,0));
  float user2 = ((float)  tolua_tonumber(tolua_S,8,0));
  float user3 = ((float)  tolua_tonumber(tolua_S,9,0));
  float user4 = ((float)  tolua_tonumber(tolua_S,10,0));
  float user5 = ((float)  tolua_tonumber(tolua_S,11,0));
  float user6 = ((float)  tolua_tonumber(tolua_S,12,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Enemy* tolua_ret = (Enemy*)  self->add(num,t,px,py,psig,user1,user2,user3,user4,user5,user6);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Enemy");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: remove_for_script of class  gsEnemy */
static int tolua_gslua_gsEnemy_remove_for_script00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsEnemy",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Enemy",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsEnemy* self = (gsEnemy*)  tolua_tousertype(tolua_S,1,0);
  Enemy* tolua_var_39 = ((Enemy*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove_for_script'",NULL);
#endif
 {
  self->remove_for_script(tolua_var_39);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* get function: enemies */
static int tolua_get_enemies_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)enemies,"gsEnemy");
 return 1;
}

/* set function: enemies */
static int tolua_set_enemies_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsEnemy",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  enemies = ((gsEnemy*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: get_kill of class  Frame */
static int tolua_gslua_Frame_get_kill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_kill'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_kill();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_kill'.",&tolua_err);
 return 0;
#endif
}

/* method: get_w of class  Frame */
static int tolua_gslua_Frame_get_w00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_w'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_w();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_w'.",&tolua_err);
 return 0;
#endif
}

/* method: get_h of class  Frame */
static int tolua_gslua_Frame_get_h00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_h'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_h();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_h'.",&tolua_err);
 return 0;
#endif
}

/* method: get_disptime of class  Frame */
static int tolua_gslua_Frame_get_disptime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_disptime'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_disptime();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_disptime'.",&tolua_err);
 return 0;
#endif
}

/* method: set_disptime of class  Frame */
static int tolua_gslua_Frame_set_disptime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_disptime'",NULL);
#endif
 {
  self->set_disptime(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_disptime'.",&tolua_err);
 return 0;
#endif
}

/* method: get_type of class  Frame */
static int tolua_gslua_Frame_get_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_type'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_type();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_type'.",&tolua_err);
 return 0;
#endif
}

/* method: get_data of class  Frame */
static int tolua_gslua_Frame_get_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_data'",NULL);
#endif
 {
  void* tolua_ret = (void*)  self->get_data();
 tolua_pushuserdata(tolua_S,(void*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_data'.",&tolua_err);
 return 0;
#endif
}

/* method: get_framecount of class  Image */
static int tolua_gslua_Image_get_framecount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_framecount'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_framecount();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_framecount'.",&tolua_err);
 return 0;
#endif
}

/* method: get_prev of class  Image */
static int tolua_gslua_Image_get_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_prev'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->get_prev();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_next of class  Image */
static int tolua_gslua_Image_get_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_next'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->get_next();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_next of class  Image */
static int tolua_gslua_Image_set_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
  Image* n = ((Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_next'",NULL);
#endif
 {
  self->set_next(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_next'.",&tolua_err);
 return 0;
#endif
}

/* method: set_prev of class  Image */
static int tolua_gslua_Image_set_prev00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
  Image* n = ((Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_prev'",NULL);
#endif
 {
  self->set_prev(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_prev'.",&tolua_err);
 return 0;
#endif
}

/* method: get_sig of class  Image */
static int tolua_gslua_Image_get_sig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_sig'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->get_sig();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_sig'.",&tolua_err);
 return 0;
#endif
}

/* method: get_rle_frame of class  Image */
static int tolua_gslua_Image_get_rle_frame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Image",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Image* self = (Image*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_rle_frame'",NULL);
#endif
 {
  RLE_SPRITE* tolua_ret = (RLE_SPRITE*)  self->get_rle_frame(num);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RLE_SPRITE");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_rle_frame'.",&tolua_err);
 return 0;
#endif
}

/* method: get_first of class  gsImage */
static int tolua_gslua_gsImage_get_first00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_first'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->get_first();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_first'.",&tolua_err);
 return 0;
#endif
}

/* method: get_last of class  gsImage */
static int tolua_gslua_gsImage_get_last00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_last'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->get_last();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_last'.",&tolua_err);
 return 0;
#endif
}

/* method: get_count of class  gsImage */
static int tolua_gslua_gsImage_get_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_count'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_count();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_count'.",&tolua_err);
 return 0;
#endif
}

/* method: add of class  gsImage */
static int tolua_gslua_gsImage_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->add();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}

/* method: load of class  gsImage */
static int tolua_gslua_gsImage_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
  char* tolua_var_40 = ((char*)  tolua_tostring(tolua_S,2,0));
  int rotate = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
 {
  int tolua_ret = (int)  self->load(tolua_var_40,rotate);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  gsImage */
static int tolua_gslua_gsImage_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
  Image* tolua_var_41 = ((Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(tolua_var_41);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  gsImage */
static int tolua_gslua_gsImage_remove01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_42 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(tolua_var_42);
 }
 }
 return 0;
tolua_lerror:
 return tolua_gslua_gsImage_remove00(tolua_S);
}

/* method: get_image of class  gsImage */
static int tolua_gslua_gsImage_get_image00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsImage",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsImage* self = (gsImage*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_image'",NULL);
#endif
 {
  Image* tolua_ret = (Image*)  self->get_image(num);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_image'.",&tolua_err);
 return 0;
#endif
}

/* get function: images */
static int tolua_get_images_ptr(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)images,"gsImage");
 return 1;
}

/* set function: images */
static int tolua_set_images_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"gsImage",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  images = ((gsImage*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: osd of class  gsPanel */
static int tolua_get_gsPanel_osd_ptr(lua_State* tolua_S)
{
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'osd'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->osd,"gsOsd");
 return 1;
}

/* set function: osd of class  gsPanel */
static int tolua_set_gsPanel_osd_ptr(lua_State* tolua_S)
{
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in accessing variable 'osd'",NULL);
 if (!tolua_isusertype(tolua_S,2,"gsOsd",0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->osd = ((class gsOsd*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: set_debug of class  gsPanel */
static int tolua_gslua_gsPanel_set_debug00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_debug'",NULL);
#endif
 {
  self->set_debug(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_debug'.",&tolua_err);
 return 0;
#endif
}

/* method: get_debug of class  gsPanel */
static int tolua_gslua_gsPanel_get_debug00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_debug'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_debug();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_debug'.",&tolua_err);
 return 0;
#endif
}

/* method: set_active of class  gsPanel */
static int tolua_gslua_gsPanel_set_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_active'",NULL);
#endif
 {
  self->set_active(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_active'.",&tolua_err);
 return 0;
#endif
}

/* method: get_active of class  gsPanel */
static int tolua_gslua_gsPanel_get_active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_active'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_active();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_active'.",&tolua_err);
 return 0;
#endif
}

/* method: set_radar_zoom of class  gsPanel */
static int tolua_gslua_gsPanel_set_radar_zoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
  int n = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'set_radar_zoom'",NULL);
#endif
 {
  self->set_radar_zoom(n);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'set_radar_zoom'.",&tolua_err);
 return 0;
#endif
}

/* method: get_radar_zoom of class  gsPanel */
static int tolua_gslua_gsPanel_get_radar_zoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'get_radar_zoom'",NULL);
#endif
 {
  int tolua_ret = (int)  self->get_radar_zoom();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'get_radar_zoom'.",&tolua_err);
 return 0;
#endif
}

/* method: update_all of class  gsPanel */
static int tolua_gslua_gsPanel_update_all00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_all'",NULL);
#endif
 {
  self->update_all();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_all'.",&tolua_err);
 return 0;
#endif
}

/* method: update_lifes of class  gsPanel */
static int tolua_gslua_gsPanel_update_lifes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_lifes'",NULL);
#endif
 {
  self->update_lifes();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_lifes'.",&tolua_err);
 return 0;
#endif
}

/* method: update_shields of class  gsPanel */
static int tolua_gslua_gsPanel_update_shields00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_shields'",NULL);
#endif
 {
  self->update_shields();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_shields'.",&tolua_err);
 return 0;
#endif
}

/* method: update_weapons of class  gsPanel */
static int tolua_gslua_gsPanel_update_weapons00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_weapons'",NULL);
#endif
 {
  self->update_weapons();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_weapons'.",&tolua_err);
 return 0;
#endif
}

/* method: update_bullets of class  gsPanel */
static int tolua_gslua_gsPanel_update_bullets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_bullets'",NULL);
#endif
 {
  self->update_bullets();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_bullets'.",&tolua_err);
 return 0;
#endif
}

/* method: update_fuel of class  gsPanel */
static int tolua_gslua_gsPanel_update_fuel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_fuel'",NULL);
#endif
 {
  self->update_fuel();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_fuel'.",&tolua_err);
 return 0;
#endif
}

/* method: update_frags of class  gsPanel */
static int tolua_gslua_gsPanel_update_frags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_frags'",NULL);
#endif
 {
  self->update_frags();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_frags'.",&tolua_err);
 return 0;
#endif
}

/* method: update_radar of class  gsPanel */
static int tolua_gslua_gsPanel_update_radar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_radar'",NULL);
#endif
 {
  self->update_radar();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_radar'.",&tolua_err);
 return 0;
#endif
}

/* method: update_extra_energy of class  gsPanel */
static int tolua_gslua_gsPanel_update_extra_energy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"gsPanel",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  gsPanel* self = (gsPanel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_myerror(tolua_S,"invalid 'self' in function 'update_extra_energy'",NULL);
#endif
 {
  self->update_extra_energy();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_myerror(tolua_S,"#ferror in function 'update_extra_energy'.",&tolua_err);
 return 0;
#endif
}

/* get function: gpanel */
static int tolua_get_gslua_gpanel(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=2)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)gpanel[tolua_index],"gsPanel");
 return 1;
}

/* set function: gpanel */
static int tolua_set_gslua_gpanel(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_myerror(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0)-1;
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=2)
 tolua_myerror(tolua_S,"array indexing out of range.",NULL);
#endif
  gpanel[tolua_index] = ((gsPanel*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* Open function */
TOLUA_API int tolua_gslua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"KB_SHIFT_FLAG",KB_SHIFT_FLAG);
 tolua_constant(tolua_S,"KB_CTRL_FLAG",KB_CTRL_FLAG);
 tolua_constant(tolua_S,"KB_ALT_FLAG",KB_ALT_FLAG);
 tolua_constant(tolua_S,"KB_LWIN_FLAG",KB_LWIN_FLAG);
 tolua_constant(tolua_S,"KB_RWIN_FLAG",KB_RWIN_FLAG);
 tolua_constant(tolua_S,"KB_MENU_FLAG",KB_MENU_FLAG);
 tolua_constant(tolua_S,"KB_SCROLOCK_FLAG",KB_SCROLOCK_FLAG);
 tolua_constant(tolua_S,"KB_NUMLOCK_FLAG",KB_NUMLOCK_FLAG);
 tolua_constant(tolua_S,"KB_CAPSLOCK_FLAG",KB_CAPSLOCK_FLAG);
 tolua_constant(tolua_S,"KB_INALTSEQ_FLAG",KB_INALTSEQ_FLAG);
 tolua_constant(tolua_S,"KB_ACCENT1_FLAG",KB_ACCENT1_FLAG);
 tolua_constant(tolua_S,"KB_ACCENT2_FLAG",KB_ACCENT2_FLAG);
 tolua_constant(tolua_S,"KB_ACCENT3_FLAG",KB_ACCENT3_FLAG);
 tolua_constant(tolua_S,"KB_ACCENT4_FLAG",KB_ACCENT4_FLAG);
 tolua_constant(tolua_S,"KEY_A",KEY_A);
 tolua_constant(tolua_S,"KEY_B",KEY_B);
 tolua_constant(tolua_S,"KEY_C",KEY_C);
 tolua_constant(tolua_S,"KEY_D",KEY_D);
 tolua_constant(tolua_S,"KEY_E",KEY_E);
 tolua_constant(tolua_S,"KEY_F",KEY_F);
 tolua_constant(tolua_S,"KEY_G",KEY_G);
 tolua_constant(tolua_S,"KEY_H",KEY_H);
 tolua_constant(tolua_S,"KEY_I",KEY_I);
 tolua_constant(tolua_S,"KEY_J",KEY_J);
 tolua_constant(tolua_S,"KEY_K",KEY_K);
 tolua_constant(tolua_S,"KEY_L",KEY_L);
 tolua_constant(tolua_S,"KEY_M",KEY_M);
 tolua_constant(tolua_S,"KEY_N",KEY_N);
 tolua_constant(tolua_S,"KEY_O",KEY_O);
 tolua_constant(tolua_S,"KEY_P",KEY_P);
 tolua_constant(tolua_S,"KEY_Q",KEY_Q);
 tolua_constant(tolua_S,"KEY_R",KEY_R);
 tolua_constant(tolua_S,"KEY_S",KEY_S);
 tolua_constant(tolua_S,"KEY_T",KEY_T);
 tolua_constant(tolua_S,"KEY_U",KEY_U);
 tolua_constant(tolua_S,"KEY_V",KEY_V);
 tolua_constant(tolua_S,"KEY_W",KEY_W);
 tolua_constant(tolua_S,"KEY_X",KEY_X);
 tolua_constant(tolua_S,"KEY_Y",KEY_Y);
 tolua_constant(tolua_S,"KEY_Z",KEY_Z);
 tolua_constant(tolua_S,"KEY_0",KEY_0);
 tolua_constant(tolua_S,"KEY_1",KEY_1);
 tolua_constant(tolua_S,"KEY_2",KEY_2);
 tolua_constant(tolua_S,"KEY_3",KEY_3);
 tolua_constant(tolua_S,"KEY_4",KEY_4);
 tolua_constant(tolua_S,"KEY_5",KEY_5);
 tolua_constant(tolua_S,"KEY_6",KEY_6);
 tolua_constant(tolua_S,"KEY_7",KEY_7);
 tolua_constant(tolua_S,"KEY_8",KEY_8);
 tolua_constant(tolua_S,"KEY_9",KEY_9);
 tolua_constant(tolua_S,"KEY_0_PAD",KEY_0_PAD);
 tolua_constant(tolua_S,"KEY_1_PAD",KEY_1_PAD);
 tolua_constant(tolua_S,"KEY_2_PAD",KEY_2_PAD);
 tolua_constant(tolua_S,"KEY_3_PAD",KEY_3_PAD);
 tolua_constant(tolua_S,"KEY_4_PAD",KEY_4_PAD);
 tolua_constant(tolua_S,"KEY_5_PAD",KEY_5_PAD);
 tolua_constant(tolua_S,"KEY_6_PAD",KEY_6_PAD);
 tolua_constant(tolua_S,"KEY_7_PAD",KEY_7_PAD);
 tolua_constant(tolua_S,"KEY_8_PAD",KEY_8_PAD);
 tolua_constant(tolua_S,"KEY_9_PAD",KEY_9_PAD);
 tolua_constant(tolua_S,"KEY_F1",KEY_F1);
 tolua_constant(tolua_S,"KEY_F2",KEY_F2);
 tolua_constant(tolua_S,"KEY_F3",KEY_F3);
 tolua_constant(tolua_S,"KEY_F4",KEY_F4);
 tolua_constant(tolua_S,"KEY_F5",KEY_F5);
 tolua_constant(tolua_S,"KEY_F6",KEY_F6);
 tolua_constant(tolua_S,"KEY_F7",KEY_F7);
 tolua_constant(tolua_S,"KEY_F8",KEY_F8);
 tolua_constant(tolua_S,"KEY_F9",KEY_F9);
 tolua_constant(tolua_S,"KEY_F10",KEY_F10);
 tolua_constant(tolua_S,"KEY_F11",KEY_F11);
 tolua_constant(tolua_S,"KEY_F12",KEY_F12);
 tolua_constant(tolua_S,"KEY_ESC",KEY_ESC);
 tolua_constant(tolua_S,"KEY_TILDE",KEY_TILDE);
 tolua_constant(tolua_S,"KEY_MINUS",KEY_MINUS);
 tolua_constant(tolua_S,"KEY_EQUALS",KEY_EQUALS);
 tolua_constant(tolua_S,"KEY_BACKSPACE",KEY_BACKSPACE);
 tolua_constant(tolua_S,"KEY_TAB",KEY_TAB);
 tolua_constant(tolua_S,"KEY_OPENBRACE",KEY_OPENBRACE);
 tolua_constant(tolua_S,"KEY_CLOSEBRACE",KEY_CLOSEBRACE);
 tolua_constant(tolua_S,"KEY_ENTER",KEY_ENTER);
 tolua_constant(tolua_S,"KEY_COLON",KEY_COLON);
 tolua_constant(tolua_S,"KEY_QUOTE",KEY_QUOTE);
 tolua_constant(tolua_S,"KEY_BACKSLASH",KEY_BACKSLASH);
 tolua_constant(tolua_S,"KEY_BACKSLASH2",KEY_BACKSLASH2);
 tolua_constant(tolua_S,"KEY_COMMA",KEY_COMMA);
 tolua_constant(tolua_S,"KEY_STOP",KEY_STOP);
 tolua_constant(tolua_S,"KEY_SLASH",KEY_SLASH);
 tolua_constant(tolua_S,"KEY_SPACE",KEY_SPACE);
 tolua_constant(tolua_S,"KEY_INSERT",KEY_INSERT);
 tolua_constant(tolua_S,"KEY_DEL",KEY_DEL);
 tolua_constant(tolua_S,"KEY_HOME",KEY_HOME);
 tolua_constant(tolua_S,"KEY_END",KEY_END);
 tolua_constant(tolua_S,"KEY_PGUP",KEY_PGUP);
 tolua_constant(tolua_S,"KEY_PGDN",KEY_PGDN);
 tolua_constant(tolua_S,"KEY_LEFT",KEY_LEFT);
 tolua_constant(tolua_S,"KEY_RIGHT",KEY_RIGHT);
 tolua_constant(tolua_S,"KEY_UP",KEY_UP);
 tolua_constant(tolua_S,"KEY_DOWN",KEY_DOWN);
 tolua_constant(tolua_S,"KEY_SLASH_PAD",KEY_SLASH_PAD);
 tolua_constant(tolua_S,"KEY_ASTERISK",KEY_ASTERISK);
 tolua_constant(tolua_S,"KEY_MINUS_PAD",KEY_MINUS_PAD);
 tolua_constant(tolua_S,"KEY_PLUS_PAD",KEY_PLUS_PAD);
 tolua_constant(tolua_S,"KEY_DEL_PAD",KEY_DEL_PAD);
 tolua_constant(tolua_S,"KEY_ENTER_PAD",KEY_ENTER_PAD);
 tolua_constant(tolua_S,"KEY_PRTSCR",KEY_PRTSCR);
 tolua_constant(tolua_S,"KEY_PAUSE",KEY_PAUSE);
 tolua_constant(tolua_S,"KEY_ABNT_C1",KEY_ABNT_C1);
 tolua_constant(tolua_S,"KEY_YEN",KEY_YEN);
 tolua_constant(tolua_S,"KEY_KANA",KEY_KANA);
 tolua_constant(tolua_S,"KEY_CONVERT",KEY_CONVERT);
 tolua_constant(tolua_S,"KEY_NOCONVERT",KEY_NOCONVERT);
 tolua_constant(tolua_S,"KEY_AT",KEY_AT);
 tolua_constant(tolua_S,"KEY_CIRCUMFLEX",KEY_CIRCUMFLEX);
 tolua_constant(tolua_S,"KEY_COLON2",KEY_COLON2);
 tolua_constant(tolua_S,"KEY_KANJI",KEY_KANJI);
 tolua_constant(tolua_S,"KEY_EQUALS_PAD",KEY_EQUALS_PAD);
 tolua_constant(tolua_S,"KEY_BACKQUOTE",KEY_BACKQUOTE);
 tolua_constant(tolua_S,"KEY_SEMICOLON",KEY_SEMICOLON);
 tolua_constant(tolua_S,"KEY_COMMAND",KEY_COMMAND);
 tolua_constant(tolua_S,"KEY_MODIFIERS",KEY_MODIFIERS);
 tolua_constant(tolua_S,"KEY_LSHIFT",KEY_LSHIFT);
 tolua_constant(tolua_S,"KEY_RSHIFT",KEY_RSHIFT);
 tolua_constant(tolua_S,"KEY_LCONTROL",KEY_LCONTROL);
 tolua_constant(tolua_S,"KEY_RCONTROL",KEY_RCONTROL);
 tolua_constant(tolua_S,"KEY_ALT",KEY_ALT);
 tolua_constant(tolua_S,"KEY_ALTGR",KEY_ALTGR);
 tolua_constant(tolua_S,"KEY_LWIN",KEY_LWIN);
 tolua_constant(tolua_S,"KEY_RWIN",KEY_RWIN);
 tolua_constant(tolua_S,"KEY_MENU",KEY_MENU);
 tolua_constant(tolua_S,"KEY_SCRLOCK",KEY_SCRLOCK);
 tolua_constant(tolua_S,"KEY_NUMLOCK",KEY_NUMLOCK);
 tolua_constant(tolua_S,"KEY_CAPSLOCK",KEY_CAPSLOCK);
 tolua_constant(tolua_S,"KEY_MAX",KEY_MAX);
 tolua_constant(tolua_S,"OBJ_PILE",OBJ_PILE);
 tolua_constant(tolua_S,"OBJ_TUNNEL1",OBJ_TUNNEL1);
 tolua_constant(tolua_S,"OBJ_BUILDING1",OBJ_BUILDING1);
 tolua_constant(tolua_S,"OBJ_BUILDING2",OBJ_BUILDING2);
 tolua_constant(tolua_S,"OBJ_BUILDING3",OBJ_BUILDING3);
 tolua_constant(tolua_S,"OBJ_BUILDING4",OBJ_BUILDING4);
 tolua_constant(tolua_S,"OBJ_BUILDING4_2",OBJ_BUILDING4_2);
 tolua_constant(tolua_S,"OBJ_BUILDING_GRAV",OBJ_BUILDING_GRAV);
 tolua_constant(tolua_S,"OBJ_POOL1",OBJ_POOL1);
 tolua_constant(tolua_S,"OBJ_POOL2",OBJ_POOL2);
 tolua_constant(tolua_S,"OBJ_PPILE1",OBJ_PPILE1);
 tolua_constant(tolua_S,"OBJ_PPILE2",OBJ_PPILE2);
 tolua_constant(tolua_S,"OBJ_MAN1",OBJ_MAN1);
 tolua_constant(tolua_S,"OBJ_DOORH",OBJ_DOORH);
 tolua_constant(tolua_S,"OBJ_DOORV",OBJ_DOORV);
 tolua_constant(tolua_S,"OBJ_DOORH2",OBJ_DOORH2);
 tolua_constant(tolua_S,"OBJ_DOORV2",OBJ_DOORV2);
 tolua_constant(tolua_S,"OBJ_SWITCHR",OBJ_SWITCHR);
 tolua_constant(tolua_S,"OBJ_SWITCHL",OBJ_SWITCHL);
 tolua_constant(tolua_S,"OBJ_SWITCHU",OBJ_SWITCHU);
 tolua_constant(tolua_S,"OBJ_SWITCHD",OBJ_SWITCHD);
 tolua_constant(tolua_S,"OBJ_RACEPOLEL",OBJ_RACEPOLEL);
 tolua_constant(tolua_S,"OBJ_RACEPOLER",OBJ_RACEPOLER);
 tolua_constant(tolua_S,"OBJ_RACEPOLEU",OBJ_RACEPOLEU);
 tolua_constant(tolua_S,"OBJ_RACEPOLED",OBJ_RACEPOLED);
 tolua_constant(tolua_S,"OBJ_TREE",OBJ_TREE);
 tolua_constant(tolua_S,"OBJ_TREE2",OBJ_TREE2);
 tolua_constant(tolua_S,"OBJ_STONE_B1",OBJ_STONE_B1);
 tolua_constant(tolua_S,"OBJ_STONE_S1",OBJ_STONE_S1);
 tolua_constant(tolua_S,"OBJ_STONE_S2",OBJ_STONE_S2);
 tolua_constant(tolua_S,"OBJ_BLOCK_1",OBJ_BLOCK_1);
 tolua_constant(tolua_S,"OBJ_BLOCK_2",OBJ_BLOCK_2);
 tolua_constant(tolua_S,"SPOBJ_BUMPL",SPOBJ_BUMPL);
 tolua_constant(tolua_S,"SPOBJ_BUMPR",SPOBJ_BUMPR);
 tolua_constant(tolua_S,"SPOBJ_BUMPU",SPOBJ_BUMPU);
 tolua_constant(tolua_S,"SPOBJ_BUMPD",SPOBJ_BUMPD);
 tolua_constant(tolua_S,"SPOBJ_EXTRA",SPOBJ_EXTRA);
 tolua_constant(tolua_S,"SPOBJ_TELEPORT1",SPOBJ_TELEPORT1);
 tolua_constant(tolua_S,"SPOBJ_TELEPORT2",SPOBJ_TELEPORT2);
 tolua_constant(tolua_S,"SPOBJ_GRAVTRAP",SPOBJ_GRAVTRAP);
 tolua_constant(tolua_S,"SPOBJ_FLAG",SPOBJ_FLAG);
 tolua_constant(tolua_S,"SPOBJ_BACKTILE1",SPOBJ_BACKTILE1);
 tolua_constant(tolua_S,"SPOBJ_GWALL_L",SPOBJ_GWALL_L);
 tolua_constant(tolua_S,"SPOBJ_GWALL_R",SPOBJ_GWALL_R);
 tolua_constant(tolua_S,"SPOBJ_GWALL_U",SPOBJ_GWALL_U);
 tolua_constant(tolua_S,"SPOBJ_GWALL_D",SPOBJ_GWALL_D);
 tolua_cclass(tolua_S,"Object","Object","",NULL);
 tolua_beginmodule(tolua_S,"Object");
 tolua_variable(tolua_S,"wp",tolua_get_Object_wp_ptr,tolua_set_Object_wp_ptr);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Object_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Object_get_next00);
 tolua_function(tolua_S,"set_prev",tolua_gslua_Object_set_prev00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Object_set_next00);
 tolua_function(tolua_S,"set_type",tolua_gslua_Object_set_type00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Object_get_type00);
 tolua_function(tolua_S,"get_nr",tolua_gslua_Object_get_nr00);
 tolua_function(tolua_S,"is_special",tolua_gslua_Object_is_special00);
 tolua_function(tolua_S,"set_maxframe",tolua_gslua_Object_set_maxframe00);
 tolua_function(tolua_S,"get_maxframe",tolua_gslua_Object_get_maxframe00);
 tolua_function(tolua_S,"set_maxframet",tolua_gslua_Object_set_maxframet00);
 tolua_function(tolua_S,"get_maxframet",tolua_gslua_Object_get_maxframet00);
 tolua_function(tolua_S,"set_curframe",tolua_gslua_Object_set_curframe00);
 tolua_function(tolua_S,"get_curframe",tolua_gslua_Object_get_curframe00);
 tolua_function(tolua_S,"set_curframet",tolua_gslua_Object_set_curframet00);
 tolua_function(tolua_S,"get_curframet",tolua_gslua_Object_get_curframet00);
 tolua_function(tolua_S,"set_dispframe",tolua_gslua_Object_set_dispframe00);
 tolua_function(tolua_S,"get_dispframe",tolua_gslua_Object_get_dispframe00);
 tolua_function(tolua_S,"set_startframe",tolua_gslua_Object_set_startframe00);
 tolua_function(tolua_S,"get_startframe",tolua_gslua_Object_get_startframe00);
 tolua_function(tolua_S,"get_x",tolua_gslua_Object_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Object_get_y00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_Object_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_Object_get_yspd00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Object_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Object_set_y00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_Object_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_Object_set_yspd00);
 tolua_function(tolua_S,"get_width",tolua_gslua_Object_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_Object_get_height00);
 tolua_function(tolua_S,"set_bound_x",tolua_gslua_Object_set_bound_x00);
 tolua_function(tolua_S,"set_bound_y",tolua_gslua_Object_set_bound_y00);
 tolua_function(tolua_S,"set_bound_w",tolua_gslua_Object_set_bound_w00);
 tolua_function(tolua_S,"set_bound_h",tolua_gslua_Object_set_bound_h00);
 tolua_function(tolua_S,"add_hitpoints",tolua_gslua_Object_add_hitpoints00);
 tolua_function(tolua_S,"get_hit",tolua_gslua_Object_get_hit00);
 tolua_function(tolua_S,"get_maxhit",tolua_gslua_Object_get_maxhit00);
 tolua_function(tolua_S,"set_hit",tolua_gslua_Object_set_hit00);
 tolua_function(tolua_S,"set_maxhit",tolua_gslua_Object_set_maxhit00);
 tolua_function(tolua_S,"get_hitby",tolua_gslua_Object_get_hitby00);
 tolua_function(tolua_S,"get_hitby_sx",tolua_gslua_Object_get_hitby_sx00);
 tolua_function(tolua_S,"get_hitby_sy",tolua_gslua_Object_get_hitby_sy00);
 tolua_function(tolua_S,"get_user",tolua_gslua_Object_get_user00);
 tolua_function(tolua_S,"set_user",tolua_gslua_Object_set_user00);
 tolua_function(tolua_S,"get_active",tolua_gslua_Object_get_active00);
 tolua_function(tolua_S,"set_active",tolua_gslua_Object_set_active00);
 tolua_function(tolua_S,"get_teleport_time",tolua_gslua_Object_get_teleport_time00);
 tolua_function(tolua_S,"get_invisible",tolua_gslua_Object_get_invisible00);
 tolua_function(tolua_S,"set_invisible",tolua_gslua_Object_set_invisible00);
 tolua_function(tolua_S,"set_dont_continue",tolua_gslua_Object_set_dont_continue00);
 tolua_function(tolua_S,"get_do_hooks",tolua_gslua_Object_get_do_hooks00);
 tolua_function(tolua_S,"set_do_hooks",tolua_gslua_Object_set_do_hooks00);
 tolua_function(tolua_S,"set_dont_update",tolua_gslua_Object_set_dont_update00);
 tolua_function(tolua_S,"get_dont_update",tolua_gslua_Object_get_dont_update00);
 tolua_function(tolua_S,"remove",tolua_gslua_Object_remove00);
 tolua_function(tolua_S,"get_remove",tolua_gslua_Object_get_remove00);
 tolua_function(tolua_S,"get_restrict_to_map",tolua_gslua_Object_get_restrict_to_map00);
 tolua_function(tolua_S,"set_restrict_to_map",tolua_gslua_Object_set_restrict_to_map00);
 tolua_function(tolua_S,"set_user_int",tolua_gslua_Object_set_user_int00);
 tolua_function(tolua_S,"set_user_float",tolua_gslua_Object_set_user_float00);
 tolua_function(tolua_S,"get_user_int",tolua_gslua_Object_get_user_int00);
 tolua_function(tolua_S,"get_user_float",tolua_gslua_Object_get_user_float00);
 tolua_function(tolua_S,"draw",tolua_gslua_Object_script_redraw00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsObject","gsObject","",NULL);
 tolua_beginmodule(tolua_S,"gsObject");
 tolua_function(tolua_S,"get_first_object",tolua_gslua_gsObject_get_first_object00);
 tolua_function(tolua_S,"get_last_object",tolua_gslua_gsObject_get_last_object00);
 tolua_function(tolua_S,"get_first_spobject",tolua_gslua_gsObject_get_first_spobject00);
 tolua_function(tolua_S,"get_last_spobject",tolua_gslua_gsObject_get_last_spobject00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsObject_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsObject_add00);
 tolua_function(tolua_S,"add_special",tolua_gslua_gsObject_add_special00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsObject_remove_for_script00);
 tolua_function(tolua_S,"remove_special",tolua_gslua_gsObject_remove_for_script00);
 tolua_function(tolua_S,"get_object",tolua_gslua_gsObject_get_object00);
 tolua_function(tolua_S,"get_spobject",tolua_gslua_gsObject_get_spobject00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"objects",tolua_get_objects_ptr,tolua_set_objects_ptr);
 tolua_constant(tolua_S,"W_SINGLE",W_SINGLE);
 tolua_constant(tolua_S,"W_DOUBLE",W_DOUBLE);
 tolua_constant(tolua_S,"W_BOMB",W_BOMB);
 tolua_constant(tolua_S,"W_BIGBOMB",W_BIGBOMB);
 tolua_constant(tolua_S,"W_ROCKET",W_ROCKET);
 tolua_constant(tolua_S,"W_MINE",W_MINE);
 tolua_constant(tolua_S,"W_FREEZER",W_FREEZER);
 tolua_constant(tolua_S,"W_SHRINKER",W_SHRINKER);
 tolua_constant(tolua_S,"W_PROBE",W_PROBE);
 tolua_constant(tolua_S,"W_GAS",W_GAS);
 tolua_constant(tolua_S,"W_LASER",W_LASER);
 tolua_constant(tolua_S,"W_BUMPER",W_BUMPER);
 tolua_constant(tolua_S,"W_SHOTGUN",W_SHOTGUN);
 tolua_constant(tolua_S,"W_ACID",W_ACID);
 tolua_constant(tolua_S,"W_USER",W_USER);
 tolua_constant(tolua_S,"W_FAKE",W_FAKE);
 tolua_constant(tolua_S,"W_NONE",W_NONE);
 tolua_cclass(tolua_S,"Bullet","Bullet","",NULL);
 tolua_beginmodule(tolua_S,"Bullet");
 tolua_function(tolua_S,"get_x",tolua_gslua_Bullet_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Bullet_get_y00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_Bullet_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_Bullet_get_yspd00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Bullet_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Bullet_get_next00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Bullet_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Bullet_set_y00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_Bullet_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_Bullet_set_yspd00);
 tolua_function(tolua_S,"get_width",tolua_gslua_Bullet_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_Bullet_get_height00);
 tolua_function(tolua_S,"set_width",tolua_gslua_Bullet_set_width00);
 tolua_function(tolua_S,"set_height",tolua_gslua_Bullet_set_height00);
 tolua_function(tolua_S,"get_min_x",tolua_gslua_Bullet_get_min_x00);
 tolua_function(tolua_S,"get_min_y",tolua_gslua_Bullet_get_min_y00);
 tolua_function(tolua_S,"get_max_x",tolua_gslua_Bullet_get_max_x00);
 tolua_function(tolua_S,"get_max_y",tolua_gslua_Bullet_get_max_y00);
 tolua_function(tolua_S,"set_min_x",tolua_gslua_Bullet_set_min_x00);
 tolua_function(tolua_S,"set_min_y",tolua_gslua_Bullet_set_min_y00);
 tolua_function(tolua_S,"set_max_x",tolua_gslua_Bullet_set_max_x00);
 tolua_function(tolua_S,"set_max_y",tolua_gslua_Bullet_set_max_y00);
 tolua_function(tolua_S,"get_by_player",tolua_gslua_Bullet_get_by_player00);
 tolua_function(tolua_S,"set_by_player",tolua_gslua_Bullet_set_by_player00);
 tolua_function(tolua_S,"get_by_enemy",tolua_gslua_Bullet_get_by_enemy00);
 tolua_function(tolua_S,"set_by_enemy",tolua_gslua_Bullet_set_by_enemy00);
 tolua_function(tolua_S,"get_strength",tolua_gslua_Bullet_get_strength00);
 tolua_function(tolua_S,"set_strength",tolua_gslua_Bullet_set_strength00);
 tolua_function(tolua_S,"get_hull_hitp",tolua_gslua_Bullet_get_hull_hitp00);
 tolua_function(tolua_S,"set_hull_hitp",tolua_gslua_Bullet_set_hull_hitp00);
 tolua_function(tolua_S,"get_shield_hitp",tolua_gslua_Bullet_get_shield_hitp00);
 tolua_function(tolua_S,"set_shield_hitp",tolua_gslua_Bullet_set_shield_hitp00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Bullet_get_type00);
 tolua_function(tolua_S,"get_ttl",tolua_gslua_Bullet_get_ttl00);
 tolua_function(tolua_S,"set_ttl",tolua_gslua_Bullet_set_ttl00);
 tolua_function(tolua_S,"get_user",tolua_gslua_Bullet_get_user00);
 tolua_function(tolua_S,"set_user",tolua_gslua_Bullet_set_user00);
 tolua_function(tolua_S,"get_invisible",tolua_gslua_Bullet_get_invisible00);
 tolua_function(tolua_S,"set_invisible",tolua_gslua_Bullet_set_invisible00);
 tolua_function(tolua_S,"get_upgrade",tolua_gslua_Bullet_get_upgrade00);
 tolua_function(tolua_S,"set_upgrade",tolua_gslua_Bullet_set_upgrade00);
 tolua_function(tolua_S,"get_target",tolua_gslua_Bullet_get_target00);
 tolua_function(tolua_S,"set_target",tolua_gslua_Bullet_set_target00);
 tolua_function(tolua_S,"set_hit",tolua_gslua_Bullet_set_hit00);
 tolua_function(tolua_S,"set_maxhit",tolua_gslua_Bullet_set_maxhit00);
 tolua_function(tolua_S,"get_hit",tolua_gslua_Bullet_get_hit00);
 tolua_function(tolua_S,"get_maxhit",tolua_gslua_Bullet_get_maxhit00);
 tolua_function(tolua_S,"get_dont_continue",tolua_gslua_Bullet_get_dont_continue00);
 tolua_function(tolua_S,"set_dont_continue",tolua_gslua_Bullet_set_dont_continue00);
 tolua_function(tolua_S,"get_do_hooks",tolua_gslua_Bullet_get_do_hooks00);
 tolua_function(tolua_S,"set_do_hooks",tolua_gslua_Bullet_set_do_hooks00);
 tolua_function(tolua_S,"remove",tolua_gslua_Bullet_remove00);
 tolua_function(tolua_S,"get_remove",tolua_gslua_Bullet_get_remove00);
 tolua_function(tolua_S,"set_user_int",tolua_gslua_Bullet_set_user_int00);
 tolua_function(tolua_S,"set_user_float",tolua_gslua_Bullet_set_user_float00);
 tolua_function(tolua_S,"get_user_int",tolua_gslua_Bullet_get_user_int00);
 tolua_function(tolua_S,"get_user_float",tolua_gslua_Bullet_get_user_float00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsBullet","gsBullet","",NULL);
 tolua_beginmodule(tolua_S,"gsBullet");
 tolua_function(tolua_S,"get_count",tolua_gslua_gsBullet_get_count00);
 tolua_function(tolua_S,"get_first",tolua_gslua_gsBullet_get_first00);
 tolua_function(tolua_S,"add",tolua_gslua_gsBullet_add00);
 tolua_function(tolua_S,"add",tolua_gslua_gsBullet_add01);
 tolua_function(tolua_S,"add",tolua_gslua_gsBullet_add02);
 tolua_function(tolua_S,"remove",tolua_gslua_gsBullet_remove_for_script00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"SHIP_PIRANHA",SHIP_PIRANHA);
 tolua_constant(tolua_S,"SHIP_RAZOR",SHIP_RAZOR);
 tolua_constant(tolua_S,"SHIP_BOMBER",SHIP_BOMBER);
 tolua_constant(tolua_S,"SHIP_CUSTOM",SHIP_CUSTOM);
 tolua_constant(tolua_S,"PV_HULL",PV_HULL);
 tolua_constant(tolua_S,"PV_SHIELD",PV_SHIELD);
 tolua_constant(tolua_S,"PV_HULL_RE",PV_HULL_RE);
 tolua_constant(tolua_S,"PV_SHIELD_RE",PV_SHIELD_RE);
 tolua_constant(tolua_S,"PV_WALLSHIELD",PV_WALLSHIELD);
 tolua_constant(tolua_S,"PV_WEAPON_RE",PV_WEAPON_RE);
 tolua_constant(tolua_S,"PV_RADAR_ZOOM",PV_RADAR_ZOOM);
 tolua_constant(tolua_S,"PV_WEAPON_SLOTS",PV_WEAPON_SLOTS);
 tolua_constant(tolua_S,"PV_TURN_SPEED",PV_TURN_SPEED);
 tolua_constant(tolua_S,"PV_MAX_SPEED",PV_MAX_SPEED);
 tolua_constant(tolua_S,"PV_ACCELERATION",PV_ACCELERATION);
 tolua_constant(tolua_S,"PV_FUEL_CONS",PV_FUEL_CONS);
 tolua_constant(tolua_S,"PV_WEAPON_STORAGE",PV_WEAPON_STORAGE);
 tolua_constant(tolua_S,"EXTRA_NONE",EXTRA_NONE);
 tolua_constant(tolua_S,"EXTRA_AFTERBURNER",EXTRA_AFTERBURNER);
 tolua_constant(tolua_S,"EXTRA_CLOAK",EXTRA_CLOAK);
 tolua_constant(tolua_S,"EXTRA_DEFLECTOR",EXTRA_DEFLECTOR);
 tolua_constant(tolua_S,"DAMAGE_NONE",DAMAGE_NONE);
 tolua_constant(tolua_S,"DAMAGE_THRUSTER",DAMAGE_THRUSTER);
 tolua_constant(tolua_S,"DAMAGE_STEERING",DAMAGE_STEERING);
 tolua_constant(tolua_S,"DAMAGE_FUEL",DAMAGE_FUEL);
 tolua_constant(tolua_S,"DAMAGE_WEAPONS",DAMAGE_WEAPONS);
 tolua_constant(tolua_S,"DAMAGE_SHIELD",DAMAGE_SHIELD);
 tolua_constant(tolua_S,"DAMAGE_LANDING",DAMAGE_LANDING);
 tolua_constant(tolua_S,"DAMAGE_RADAR",DAMAGE_RADAR);
 tolua_constant(tolua_S,"DAMAGE_MAXDAMAGE",DAMAGE_MAXDAMAGE);
 tolua_cclass(tolua_S,"weaponslot","weaponslot","",NULL);
 tolua_beginmodule(tolua_S,"weaponslot");
 tolua_variable(tolua_S,"type",tolua_get_weaponslot_type,tolua_set_weaponslot_type);
 tolua_variable(tolua_S,"bullets",tolua_get_weaponslot_bullets,tolua_set_weaponslot_bullets);
 tolua_variable(tolua_S,"reload",tolua_get_weaponslot_reload,tolua_set_weaponslot_reload);
 tolua_variable(tolua_S,"cur_reload",tolua_get_weaponslot_cur_reload,tolua_set_weaponslot_cur_reload);
 tolua_variable(tolua_S,"upgrade",tolua_get_weaponslot_upgrade,tolua_set_weaponslot_upgrade);
 tolua_variable(tolua_S,"max_upgrades",tolua_get_weaponslot_max_upgrades,tolua_set_weaponslot_max_upgrades);
 tolua_variable(tolua_S,"active",tolua_get_weaponslot_active,tolua_set_weaponslot_active);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"statistics","statistics","",NULL);
 tolua_beginmodule(tolua_S,"statistics");
 tolua_variable(tolua_S,"pname",tolua_get_statistics_pname,NULL);
 tolua_variable(tolua_S,"frags",tolua_get_statistics_frags,tolua_set_statistics_frags);
 tolua_variable(tolua_S,"suicides",tolua_get_statistics_suicides,tolua_set_statistics_suicides);
 tolua_variable(tolua_S,"bullets_shot",tolua_get_statistics_bullets_shot,tolua_set_statistics_bullets_shot);
 tolua_variable(tolua_S,"bullets_hit",tolua_get_statistics_bullets_hit,tolua_set_statistics_bullets_hit);
 tolua_variable(tolua_S,"extras_taken",tolua_get_statistics_extras_taken,tolua_set_statistics_extras_taken);
 tolua_variable(tolua_S,"max_time_alive",tolua_get_statistics_max_time_alive,tolua_set_statistics_max_time_alive);
 tolua_array(tolua_S,"laptime",tolua_get_gslua_statistics_laptime,tolua_set_gslua_statistics_laptime);
 tolua_variable(tolua_S,"bonus",tolua_get_statistics_bonus,tolua_set_statistics_bonus);
 tolua_variable(tolua_S,"enemy_bonus",tolua_get_statistics_enemy_bonus,tolua_set_statistics_enemy_bonus);
 tolua_variable(tolua_S,"mission_status",tolua_get_statistics_mission_status,tolua_set_statistics_mission_status);
 tolua_variable(tolua_S,"lostlifes",tolua_get_statistics_lostlifes,tolua_set_statistics_lostlifes);
 tolua_variable(tolua_S,"used_time",tolua_get_statistics_used_time,tolua_set_statistics_used_time);
 tolua_variable(tolua_S,"plsig",tolua_get_statistics_plsig,tolua_set_statistics_plsig);
 tolua_variable(tolua_S,"set",tolua_get_statistics_set,tolua_set_statistics_set);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsPlayer","gsPlayer","",NULL);
 tolua_beginmodule(tolua_S,"gsPlayer");
 tolua_variable(tolua_S,"controls",tolua_get_gsPlayer_controls_ptr,tolua_set_gsPlayer_controls_ptr);
 tolua_function(tolua_S,"get_x",tolua_gslua_gsPlayer_get_x00);
 tolua_function(tolua_S,"set_x",tolua_gslua_gsPlayer_set_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_gsPlayer_get_y00);
 tolua_function(tolua_S,"set_y",tolua_gslua_gsPlayer_set_y00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_gsPlayer_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_gsPlayer_get_yspd00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_gsPlayer_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_gsPlayer_set_yspd00);
 tolua_function(tolua_S,"get_xacc",tolua_gslua_gsPlayer_get_xacc00);
 tolua_function(tolua_S,"get_yacc",tolua_gslua_gsPlayer_get_yacc00);
 tolua_function(tolua_S,"set_xacc",tolua_gslua_gsPlayer_set_xacc00);
 tolua_function(tolua_S,"set_yacc",tolua_gslua_gsPlayer_set_yacc00);
 tolua_function(tolua_S,"set_xacc2",tolua_gslua_gsPlayer_set_xacc200);
 tolua_function(tolua_S,"set_yacc2",tolua_gslua_gsPlayer_set_yacc200);
 tolua_function(tolua_S,"set_thrust",tolua_gslua_gsPlayer_set_thrust00);
 tolua_function(tolua_S,"get_thrust",tolua_gslua_gsPlayer_get_thrust00);
 tolua_function(tolua_S,"show_thrust_frame",tolua_gslua_gsPlayer_show_thrust_frame00);
 tolua_function(tolua_S,"set_extra_action",tolua_gslua_gsPlayer_set_extra_action00);
 tolua_function(tolua_S,"get_extra_action",tolua_gslua_gsPlayer_get_extra_action00);
 tolua_function(tolua_S,"get_extra",tolua_gslua_gsPlayer_get_extra00);
 tolua_function(tolua_S,"set_extra",tolua_gslua_gsPlayer_set_extra00);
 tolua_function(tolua_S,"get_lifes",tolua_gslua_gsPlayer_get_lifes00);
 tolua_function(tolua_S,"set_lifes",tolua_gslua_gsPlayer_set_lifes00);
 tolua_function(tolua_S,"set_invulnerability",tolua_gslua_gsPlayer_set_invulnerability00);
 tolua_function(tolua_S,"get_invulnerability",tolua_gslua_gsPlayer_get_invulnerability00);
 tolua_function(tolua_S,"get_width",tolua_gslua_gsPlayer_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_gsPlayer_get_height00);
 tolua_function(tolua_S,"get_owidth",tolua_gslua_gsPlayer_get_owidth00);
 tolua_function(tolua_S,"get_oheight",tolua_gslua_gsPlayer_get_oheight00);
 tolua_function(tolua_S,"get_back_width",tolua_gslua_gsPlayer_get_back_width00);
 tolua_function(tolua_S,"get_back_height",tolua_gslua_gsPlayer_get_back_height00);
 tolua_function(tolua_S,"get_shield_w",tolua_gslua_gsPlayer_get_shield_w00);
 tolua_function(tolua_S,"get_shield_h",tolua_gslua_gsPlayer_get_shield_h00);
 tolua_function(tolua_S,"get_head",tolua_gslua_gsPlayer_get_head00);
 tolua_function(tolua_S,"set_head",tolua_gslua_gsPlayer_set_head00);
 tolua_function(tolua_S,"get_head_to",tolua_gslua_gsPlayer_get_head_to00);
 tolua_function(tolua_S,"set_head_to",tolua_gslua_gsPlayer_set_head_to00);
 tolua_function(tolua_S,"turn_to",tolua_gslua_gsPlayer_turn_to00);
 tolua_function(tolua_S,"get_network",tolua_gslua_gsPlayer_get_network00);
 tolua_function(tolua_S,"get_sig",tolua_gslua_gsPlayer_get_sig00);
 tolua_function(tolua_S,"set_sig",tolua_gslua_gsPlayer_set_sig00);
 tolua_function(tolua_S,"get_frags",tolua_gslua_gsPlayer_get_frags00);
 tolua_function(tolua_S,"set_frags",tolua_gslua_gsPlayer_set_frags00);
 tolua_function(tolua_S,"inc_frags",tolua_gslua_gsPlayer_inc_frags00);
 tolua_function(tolua_S,"dec_frags",tolua_gslua_gsPlayer_dec_frags00);
 tolua_function(tolua_S,"get_suicides",tolua_gslua_gsPlayer_get_suicides00);
 tolua_function(tolua_S,"inc_suicides",tolua_gslua_gsPlayer_inc_suicides00);
 tolua_function(tolua_S,"inc_bonus",tolua_gslua_gsPlayer_inc_bonus00);
 tolua_function(tolua_S,"dec_bonus",tolua_gslua_gsPlayer_dec_bonus00);
 tolua_function(tolua_S,"inc_enemy_bonus",tolua_gslua_gsPlayer_inc_enemy_bonus00);
 tolua_function(tolua_S,"dec_enemy_bonus",tolua_gslua_gsPlayer_dec_enemy_bonus00);
 tolua_function(tolua_S,"inc_bullets_shot",tolua_gslua_gsPlayer_inc_bullets_shot00);
 tolua_function(tolua_S,"get_bullets_shot",tolua_gslua_gsPlayer_get_bullets_shot00);
 tolua_function(tolua_S,"inc_bullets_hit",tolua_gslua_gsPlayer_inc_bullets_hit00);
 tolua_function(tolua_S,"get_bullets_hit",tolua_gslua_gsPlayer_get_bullets_hit00);
 tolua_function(tolua_S,"set_mission_status",tolua_gslua_gsPlayer_set_mission_status00);
 tolua_function(tolua_S,"get_bonus",tolua_gslua_gsPlayer_get_bonus00);
 tolua_function(tolua_S,"get_statistics",tolua_gslua_gsPlayer_get_statistics00);
 tolua_function(tolua_S,"set_statistics",tolua_gslua_gsPlayer_set_statistics00);
 tolua_function(tolua_S,"get_homebase",tolua_gslua_gsPlayer_get_homebase00);
 tolua_function(tolua_S,"set_homebase",tolua_gslua_gsPlayer_set_homebase00);
 tolua_function(tolua_S,"get_home_x",tolua_gslua_gsPlayer_get_home_x00);
 tolua_function(tolua_S,"get_home_y",tolua_gslua_gsPlayer_get_home_y00);
 tolua_function(tolua_S,"set_home_x",tolua_gslua_gsPlayer_set_home_x00);
 tolua_function(tolua_S,"set_home_y",tolua_gslua_gsPlayer_set_home_y00);
 tolua_function(tolua_S,"get_name",tolua_gslua_gsPlayer_get_name00);
 tolua_function(tolua_S,"set_name",tolua_gslua_gsPlayer_set_name00);
 tolua_function(tolua_S,"get_ship_name",tolua_gslua_gsPlayer_get_ship_name00);
 tolua_function(tolua_S,"set_ship_name",tolua_gslua_gsPlayer_set_ship_name00);
 tolua_function(tolua_S,"get_ship_desc",tolua_gslua_gsPlayer_get_ship_desc00);
 tolua_function(tolua_S,"set_ship_desc",tolua_gslua_gsPlayer_set_ship_desc00);
 tolua_function(tolua_S,"get_shipimage_filename",tolua_gslua_gsPlayer_get_shipimage_filename00);
 tolua_function(tolua_S,"set_percent_value",tolua_gslua_gsPlayer_set_percent_value00);
 tolua_function(tolua_S,"get_percent_value",tolua_gslua_gsPlayer_get_percent_value00);
 tolua_function(tolua_S,"set_dead",tolua_gslua_gsPlayer_set_dead00);
 tolua_function(tolua_S,"get_dead",tolua_gslua_gsPlayer_get_dead00);
 tolua_function(tolua_S,"set_freezed",tolua_gslua_gsPlayer_set_freezed00);
 tolua_function(tolua_S,"get_freezed",tolua_gslua_gsPlayer_get_freezed00);
 tolua_function(tolua_S,"set_shrinked",tolua_gslua_gsPlayer_set_shrinked00);
 tolua_function(tolua_S,"get_shrinked",tolua_gslua_gsPlayer_get_shrinked00);
 tolua_function(tolua_S,"set_delirium",tolua_gslua_gsPlayer_set_delirium00);
 tolua_function(tolua_S,"get_delirium",tolua_gslua_gsPlayer_get_delirium00);
 tolua_function(tolua_S,"get_hitby",tolua_gslua_gsPlayer_get_hitby00);
 tolua_function(tolua_S,"get_killedby",tolua_gslua_gsPlayer_get_killedby00);
 tolua_function(tolua_S,"set_hitby",tolua_gslua_gsPlayer_set_hitby00);
 tolua_function(tolua_S,"set_killedby",tolua_gslua_gsPlayer_set_killedby00);
 tolua_function(tolua_S,"get_hitbyweap",tolua_gslua_gsPlayer_get_hitbyweap00);
 tolua_function(tolua_S,"set_hitbyweap",tolua_gslua_gsPlayer_set_hitbyweap00);
 tolua_function(tolua_S,"get_time_alive",tolua_gslua_gsPlayer_get_time_alive00);
 tolua_function(tolua_S,"get_damage",tolua_gslua_gsPlayer_get_damage00);
 tolua_function(tolua_S,"set_damage",tolua_gslua_gsPlayer_set_damage00);
 tolua_function(tolua_S,"set_land",tolua_gslua_gsPlayer_set_land00);
 tolua_function(tolua_S,"get_land",tolua_gslua_gsPlayer_get_land00);
 tolua_function(tolua_S,"get_land_dur",tolua_gslua_gsPlayer_get_land_dur00);
 tolua_function(tolua_S,"set_land_base",tolua_gslua_gsPlayer_set_land_base00);
 tolua_function(tolua_S,"get_land_base",tolua_gslua_gsPlayer_get_land_base00);
 tolua_function(tolua_S,"get_weight",tolua_gslua_gsPlayer_get_weight00);
 tolua_function(tolua_S,"set_weight",tolua_gslua_gsPlayer_set_weight00);
 tolua_function(tolua_S,"get_weight_factor",tolua_gslua_gsPlayer_get_weight_factor00);
 tolua_function(tolua_S,"set_weight_factor",tolua_gslua_gsPlayer_set_weight_factor00);
 tolua_function(tolua_S,"get_hull",tolua_gslua_gsPlayer_get_hull00);
 tolua_function(tolua_S,"get_shield",tolua_gslua_gsPlayer_get_shield00);
 tolua_function(tolua_S,"get_fuel",tolua_gslua_gsPlayer_get_fuel00);
 tolua_function(tolua_S,"set_hull",tolua_gslua_gsPlayer_set_hull00);
 tolua_function(tolua_S,"set_shield",tolua_gslua_gsPlayer_set_shield00);
 tolua_function(tolua_S,"set_fuel",tolua_gslua_gsPlayer_set_fuel00);
 tolua_function(tolua_S,"set_wall_shield",tolua_gslua_gsPlayer_set_wall_shield00);
 tolua_function(tolua_S,"get_wall_shield",tolua_gslua_gsPlayer_get_wall_shield00);
 tolua_function(tolua_S,"set_wall_shield_recharge",tolua_gslua_gsPlayer_set_wall_shield_recharge00);
 tolua_function(tolua_S,"get_wall_shield_recharge",tolua_gslua_gsPlayer_get_wall_shield_recharge00);
 tolua_function(tolua_S,"get_max_fuel",tolua_gslua_gsPlayer_get_max_fuel00);
 tolua_function(tolua_S,"get_max_hull",tolua_gslua_gsPlayer_get_max_hull00);
 tolua_function(tolua_S,"get_max_shield",tolua_gslua_gsPlayer_get_max_shield00);
 tolua_function(tolua_S,"set_max_shield",tolua_gslua_gsPlayer_set_max_shield00);
 tolua_function(tolua_S,"set_max_hull",tolua_gslua_gsPlayer_set_max_hull00);
 tolua_function(tolua_S,"get_max_wall_shield",tolua_gslua_gsPlayer_get_max_wall_shield00);
 tolua_function(tolua_S,"set_hull_repair",tolua_gslua_gsPlayer_set_hull_repair00);
 tolua_function(tolua_S,"get_hull_repair",tolua_gslua_gsPlayer_get_hull_repair00);
 tolua_function(tolua_S,"get_shield_recharge",tolua_gslua_gsPlayer_get_shield_recharge00);
 tolua_function(tolua_S,"set_shield_recharge",tolua_gslua_gsPlayer_set_shield_recharge00);
 tolua_function(tolua_S,"get_fuel_dec",tolua_gslua_gsPlayer_get_fuel_dec00);
 tolua_function(tolua_S,"set_fuel_dec",tolua_gslua_gsPlayer_set_fuel_dec00);
 tolua_function(tolua_S,"get_max_speed",tolua_gslua_gsPlayer_get_max_speed00);
 tolua_function(tolua_S,"set_max_speed",tolua_gslua_gsPlayer_set_max_speed00);
 tolua_function(tolua_S,"get_player_acc",tolua_gslua_gsPlayer_get_player_acc00);
 tolua_function(tolua_S,"set_player_acc",tolua_gslua_gsPlayer_set_player_acc00);
 tolua_function(tolua_S,"get_turn_speed",tolua_gslua_gsPlayer_get_turn_speed00);
 tolua_function(tolua_S,"set_turn_speed",tolua_gslua_gsPlayer_set_turn_speed00);
 tolua_function(tolua_S,"set_ship_type",tolua_gslua_gsPlayer_set_ship_type00);
 tolua_function(tolua_S,"get_ship_type",tolua_gslua_gsPlayer_get_ship_type00);
 tolua_function(tolua_S,"set_max_radar_zoom",tolua_gslua_gsPlayer_set_max_radar_zoom00);
 tolua_function(tolua_S,"get_max_radar_zoom",tolua_gslua_gsPlayer_get_max_radar_zoom00);
 tolua_function(tolua_S,"set_weapon_storage",tolua_gslua_gsPlayer_set_weapon_storage00);
 tolua_function(tolua_S,"get_weapon_storage",tolua_gslua_gsPlayer_get_weapon_storage00);
 tolua_function(tolua_S,"get_locked_player",tolua_gslua_gsPlayer_get_locked_player00);
 tolua_function(tolua_S,"get_rocket_turn_speed",tolua_gslua_gsPlayer_get_rocket_turn_speed00);
 tolua_function(tolua_S,"set_rocket_turn_speed",tolua_gslua_gsPlayer_set_rocket_turn_speed00);
 tolua_function(tolua_S,"get_weapon_recharge",tolua_gslua_gsPlayer_get_weapon_recharge00);
 tolua_function(tolua_S,"set_weapon_recharge",tolua_gslua_gsPlayer_set_weapon_recharge00);
 tolua_function(tolua_S,"get_extra_power",tolua_gslua_gsPlayer_get_extra_power00);
 tolua_function(tolua_S,"set_extra_power",tolua_gslua_gsPlayer_set_extra_power00);
 tolua_function(tolua_S,"get_extra_recharge",tolua_gslua_gsPlayer_get_extra_recharge00);
 tolua_function(tolua_S,"set_extra_recharge",tolua_gslua_gsPlayer_set_extra_recharge00);
 tolua_function(tolua_S,"get_max_extra_power",tolua_gslua_gsPlayer_get_max_extra_power00);
 tolua_function(tolua_S,"get_active_weapon",tolua_gslua_gsPlayer_get_active_weapon00);
 tolua_function(tolua_S,"set_active_weapon",tolua_gslua_gsPlayer_set_active_weapon00);
 tolua_function(tolua_S,"get_extra",tolua_gslua_gsPlayer_get_extra01);
 tolua_function(tolua_S,"set_extra",tolua_gslua_gsPlayer_set_extra01);
 tolua_function(tolua_S,"get_weapon_slot",tolua_gslua_gsPlayer_get_weapon_slot00);
 tolua_function(tolua_S,"set_weapon_slot",tolua_gslua_gsPlayer_set_weapon_slot00);
 tolua_function(tolua_S,"get_avail_weapon_slots",tolua_gslua_gsPlayer_get_avail_weapon_slots00);
 tolua_function(tolua_S,"set_avail_weapon_slots",tolua_gslua_gsPlayer_set_avail_weapon_slots00);
 tolua_function(tolua_S,"set_weapon_bullets",tolua_gslua_gsPlayer_set_weapon_bullets00);
 tolua_function(tolua_S,"get_weapon_bullets",tolua_gslua_gsPlayer_get_weapon_bullets00);
 tolua_function(tolua_S,"activate_weapon_slot",tolua_gslua_gsPlayer_activate_weapon_slot00);
 tolua_function(tolua_S,"deactivate_weapon_slot",tolua_gslua_gsPlayer_deactivate_weapon_slot00);
 tolua_function(tolua_S,"set_ship_value",tolua_gslua_gsPlayer_set_ship_value00);
 tolua_function(tolua_S,"get_ship_value",tolua_gslua_gsPlayer_get_ship_value00);
 tolua_function(tolua_S,"set_status_mode",tolua_gslua_gsPlayer_set_status_mode00);
 tolua_function(tolua_S,"show_status_set_map",tolua_gslua_gsPlayer_show_status_set_map00);
 tolua_function(tolua_S,"show_status",tolua_gslua_gsPlayer_show_status00);
 tolua_function(tolua_S,"hide_status",tolua_gslua_gsPlayer_hide_status00);
 tolua_function(tolua_S,"get_handicap",tolua_gslua_gsPlayer_get_handicap00);
 tolua_function(tolua_S,"set_handicap",tolua_gslua_gsPlayer_set_handicap00);
 tolua_function(tolua_S,"get_recoil",tolua_gslua_gsPlayer_get_recoil00);
 tolua_function(tolua_S,"set_recoil",tolua_gslua_gsPlayer_set_recoil00);
 tolua_function(tolua_S,"set_dont_continue",tolua_gslua_gsPlayer_set_dont_continue00);
 tolua_function(tolua_S,"get_do_hooks",tolua_gslua_gsPlayer_get_do_hooks00);
 tolua_function(tolua_S,"set_do_hooks",tolua_gslua_gsPlayer_set_do_hooks00);
 tolua_function(tolua_S,"set_user_int",tolua_gslua_gsPlayer_set_user_int00);
 tolua_function(tolua_S,"set_user_float",tolua_gslua_gsPlayer_set_user_float00);
 tolua_function(tolua_S,"get_user_int",tolua_gslua_gsPlayer_get_user_int00);
 tolua_function(tolua_S,"get_user_float",tolua_gslua_gsPlayer_get_user_float00);
 tolua_function(tolua_S,"reinit",tolua_gslua_gsPlayer_reinit00);
 tolua_function(tolua_S,"set_standard_bullets",tolua_gslua_gsPlayer_set_standard_bullets00);
 tolua_function(tolua_S,"return_home",tolua_gslua_gsPlayer_return_home00);
 tolua_function(tolua_S,"set_weapon",tolua_gslua_gsPlayer_set_weapon00);
 tolua_function(tolua_S,"shoot",tolua_gslua_gsPlayer_shoot00);
 tolua_function(tolua_S,"hit",tolua_gslua_gsPlayer_hit00);
 tolua_function(tolua_S,"load_ship",tolua_gslua_gsPlayer_load_ship00);
 tolua_function(tolua_S,"load_image",tolua_gslua_gsPlayer_load_image00);
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"get_player_nr_by_sig",tolua_gslua_get_player_nr_by_sig00);
 tolua_function(tolua_S,"get_player_by_sig",tolua_gslua_get_player_by_sig00);
 tolua_array(tolua_S,"player",tolua_get_gslua_player,tolua_set_gslua_player);
 tolua_constant(tolua_S,"PLAYER_BASE",PLAYER_BASE);
 tolua_constant(tolua_S,"CARGO_BASE",CARGO_BASE);
 tolua_constant(tolua_S,"ENEMY_S_BASE",ENEMY_S_BASE);
 tolua_constant(tolua_S,"ENEMY_L_BASE",ENEMY_L_BASE);
 tolua_constant(tolua_S,"ENEMY_BASE_LEFT",ENEMY_BASE_LEFT);
 tolua_constant(tolua_S,"ENEMY_BASE_MIDDLE",ENEMY_BASE_MIDDLE);
 tolua_constant(tolua_S,"ENEMY_BASE_RIGHT",ENEMY_BASE_RIGHT);
 tolua_constant(tolua_S,"RELOAD_BASE",RELOAD_BASE);
 tolua_constant(tolua_S,"CUSTOM_BASE",CUSTOM_BASE);
 tolua_constant(tolua_S,"CARGO_NONE",CARGO_NONE);
 tolua_constant(tolua_S,"CARGO_NORMAL",CARGO_NORMAL);
 tolua_constant(tolua_S,"CARGO_OBJECT",CARGO_OBJECT);
 tolua_cclass(tolua_S,"Base","Base","",NULL);
 tolua_beginmodule(tolua_S,"Base");
 tolua_function(tolua_S,"get_x",tolua_gslua_Base_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Base_get_y00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_Base_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_Base_get_yspd00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Base_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Base_set_y00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_Base_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_Base_set_yspd00);
 tolua_function(tolua_S,"get_width",tolua_gslua_Base_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_Base_get_height00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Base_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Base_get_next00);
 tolua_function(tolua_S,"set_prev",tolua_gslua_Base_set_prev00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Base_set_next00);
 tolua_function(tolua_S,"get_land_x",tolua_gslua_Base_get_land_x00);
 tolua_function(tolua_S,"get_land_y",tolua_gslua_Base_get_land_y00);
 tolua_function(tolua_S,"get_land_w",tolua_gslua_Base_get_land_w00);
 tolua_function(tolua_S,"set_land_x",tolua_gslua_Base_set_land_x00);
 tolua_function(tolua_S,"set_land_y",tolua_gslua_Base_set_land_y00);
 tolua_function(tolua_S,"set_land_w",tolua_gslua_Base_set_land_w00);
 tolua_function(tolua_S,"set_type",tolua_gslua_Base_set_type00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Base_get_type00);
 tolua_function(tolua_S,"set_cargo_type",tolua_gslua_Base_set_cargo_type00);
 tolua_function(tolua_S,"get_cargo_type",tolua_gslua_Base_get_cargo_type00);
 tolua_function(tolua_S,"set_cargo_weight",tolua_gslua_Base_set_cargo_weight00);
 tolua_function(tolua_S,"get_cargo_weight",tolua_gslua_Base_get_cargo_weight00);
 tolua_function(tolua_S,"get_cargo_object",tolua_gslua_Base_get_cargo_object00);
 tolua_function(tolua_S,"set_cargo_object",tolua_gslua_Base_set_cargo_object00);
 tolua_function(tolua_S,"set_cargo_name",tolua_gslua_Base_set_cargo_name00);
 tolua_function(tolua_S,"get_cargo_name",tolua_gslua_Base_get_cargo_name00);
 tolua_function(tolua_S,"get_nr",tolua_gslua_Base_get_nr00);
 tolua_function(tolua_S,"get_player_on_base",tolua_gslua_Base_get_player_on_base00);
 tolua_function(tolua_S,"set_player_on_base",tolua_gslua_Base_set_player_on_base00);
 tolua_function(tolua_S,"set_refuel_percent",tolua_gslua_Base_set_refuel_percent00);
 tolua_function(tolua_S,"get_refuel_percent",tolua_gslua_Base_get_refuel_percent00);
 tolua_function(tolua_S,"get_refuel_player",tolua_gslua_Base_get_refuel_player00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsBase","gsBase","",NULL);
 tolua_beginmodule(tolua_S,"gsBase");
 tolua_function(tolua_S,"get_first_base",tolua_gslua_gsBase_get_first_base00);
 tolua_function(tolua_S,"get_last_base",tolua_gslua_gsBase_get_last_base00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsBase_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsBase_add00);
 tolua_function(tolua_S,"get_base",tolua_gslua_gsBase_get_base00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"bases",tolua_get_bases_ptr,tolua_set_bases_ptr);
 tolua_cclass(tolua_S,"Pixel","Pixel","",NULL);
 tolua_beginmodule(tolua_S,"Pixel");
 tolua_variable(tolua_S,"color",tolua_get_Pixel_color,tolua_set_Pixel_color);
 tolua_variable(tolua_S,"tcolor",tolua_get_Pixel_tcolor,tolua_set_Pixel_tcolor);
 tolua_variable(tolua_S,"lifetime",tolua_get_Pixel_lifetime,tolua_set_Pixel_lifetime);
 tolua_variable(tolua_S,"max_xab",tolua_get_Pixel_max_xab,tolua_set_Pixel_max_xab);
 tolua_variable(tolua_S,"max_yab",tolua_get_Pixel_max_yab,tolua_set_Pixel_max_yab);
 tolua_variable(tolua_S,"dspd",tolua_get_Pixel_dspd,tolua_set_Pixel_dspd);
 tolua_variable(tolua_S,"bounce",tolua_get_Pixel_bounce,tolua_set_Pixel_bounce);
 tolua_variable(tolua_S,"vdec",tolua_get_Pixel_vdec,tolua_set_Pixel_vdec);
 tolua_function(tolua_S,"get_x",tolua_gslua_Pixel_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Pixel_get_y00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Pixel_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Pixel_set_y00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_Pixel_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_Pixel_get_yspd00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_Pixel_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_Pixel_set_yspd00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Pixel_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Pixel_get_next00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"PIX_WHITE",PIX_WHITE);
 tolua_constant(tolua_S,"PIX_YELLOW",PIX_YELLOW);
 tolua_constant(tolua_S,"PIX_GREEN",PIX_GREEN);
 tolua_constant(tolua_S,"PIX_BLUE",PIX_BLUE);
 tolua_constant(tolua_S,"PIX_BLUEWHITE",PIX_BLUEWHITE);
 tolua_constant(tolua_S,"PIX_RED",PIX_RED);
 tolua_constant(tolua_S,"PIX_ALL",PIX_ALL);
 tolua_constant(tolua_S,"PIX_DEBRIS1",PIX_DEBRIS1);
 tolua_constant(tolua_S,"PIX_DEBRIS2",PIX_DEBRIS2);
 tolua_constant(tolua_S,"PIX_DEBRIS3",PIX_DEBRIS3);
 tolua_constant(tolua_S,"PIX_DEBRIS4",PIX_DEBRIS4);
 tolua_constant(tolua_S,"PIX_DEBRIS5",PIX_DEBRIS5);
 tolua_cclass(tolua_S,"gsPixelRain","gsPixelRain","",NULL);
 tolua_beginmodule(tolua_S,"gsPixelRain");
 tolua_function(tolua_S,"get_first",tolua_gslua_gsPixelRain_get_first00);
 tolua_function(tolua_S,"get_last",tolua_gslua_gsPixelRain_get_last00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsPixelRain_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsPixelRain_add00);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"level_info","level_info","",tolua_collect_level_info);
#else
 tolua_cclass(tolua_S,"level_info","level_info","",NULL);
#endif
 tolua_beginmodule(tolua_S,"level_info");
 tolua_variable(tolua_S,"map_style",tolua_get_level_info_map_style,NULL);
 tolua_variable(tolua_S,"onescreen_map",tolua_get_level_info_onescreen_map,tolua_set_level_info_onescreen_map);
 tolua_variable(tolua_S,"map_start_x",tolua_get_level_info_map_start_x,tolua_set_level_info_map_start_x);
 tolua_variable(tolua_S,"map_start_y",tolua_get_level_info_map_start_y,tolua_set_level_info_map_start_y);
 tolua_variable(tolua_S,"activate_extra_str",tolua_get_level_info_activate_extra_str,NULL);
 tolua_variable(tolua_S,"activate_extra_bar_str",tolua_get_level_info_activate_extra_bar_str,NULL);
 tolua_variable(tolua_S,"activate_ctf",tolua_get_level_info_activate_ctf,tolua_set_level_info_activate_ctf);
 tolua_variable(tolua_S,"activate_hh",tolua_get_level_info_activate_hh,tolua_set_level_info_activate_hh);
 tolua_variable(tolua_S,"activate_global_script",tolua_get_level_info_activate_global_script,tolua_set_level_info_activate_global_script);
 tolua_variable(tolua_S,"activate_race",tolua_get_level_info_activate_race,tolua_set_level_info_activate_race);
 tolua_variable(tolua_S,"activate_minigame",tolua_get_level_info_activate_minigame,tolua_set_level_info_activate_minigame);
 tolua_variable(tolua_S,"activate_singleplayer",tolua_get_level_info_activate_singleplayer,tolua_set_level_info_activate_singleplayer);
 tolua_variable(tolua_S,"min_version",tolua_get_level_info_min_version,tolua_set_level_info_min_version);
 tolua_variable(tolua_S,"max_players",tolua_get_level_info_max_players,tolua_set_level_info_max_players);
 tolua_variable(tolua_S,"name",tolua_get_level_info_name,NULL);
 tolua_variable(tolua_S,"desc",tolua_get_level_info_desc,NULL);
 tolua_variable(tolua_S,"missiondesc",tolua_get_level_info_missiondesc,NULL);
 tolua_variable(tolua_S,"leveldir",tolua_get_level_info_leveldir,NULL);
 tolua_variable(tolua_S,"levelfile",tolua_get_level_info_levelfile,NULL);
 tolua_variable(tolua_S,"mapfile",tolua_get_level_info_mapfile,NULL);
 tolua_variable(tolua_S,"initscriptfile",tolua_get_level_info_initscriptfile,NULL);
 tolua_variable(tolua_S,"scriptfile",tolua_get_level_info_scriptfile,NULL);
 tolua_variable(tolua_S,"author",tolua_get_level_info_author,NULL);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"FONT_ICE",FONT_ICE);
 tolua_constant(tolua_S,"FONT_BANK",FONT_BANK);
 tolua_constant(tolua_S,"FONT_IMPACT10",FONT_IMPACT10);
 tolua_constant(tolua_S,"FONT_IMPACT14",FONT_IMPACT14);
 tolua_constant(tolua_S,"FONT_LCD",FONT_LCD);
 tolua_constant(tolua_S,"FONT_KEYPUNCH",FONT_KEYPUNCH);
 tolua_constant(tolua_S,"FONT_VERDANA",FONT_VERDANA);
 tolua_constant(tolua_S,"FONT_MINIFONT",FONT_MINIFONT);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"UserStats","UserStats","",tolua_collect_UserStats);
#else
 tolua_cclass(tolua_S,"UserStats","UserStats","",NULL);
#endif
 tolua_beginmodule(tolua_S,"UserStats");
 tolua_function(tolua_S,"set_text",tolua_gslua_UserStats_set_text00);
 tolua_function(tolua_S,"set_textcolor",tolua_gslua_UserStats_set_textcolor00);
 tolua_function(tolua_S,"set_value",tolua_gslua_UserStats_set_value00);
 tolua_function(tolua_S,"set_value",tolua_gslua_UserStats_set_value01);
 tolua_function(tolua_S,"set_valuecolor",tolua_gslua_UserStats_set_valuecolor00);
 tolua_function(tolua_S,"set_plusx",tolua_gslua_UserStats_set_plusx00);
 tolua_function(tolua_S,"get_text",tolua_gslua_UserStats_get_text00);
 tolua_function(tolua_S,"get_textcolor",tolua_gslua_UserStats_get_textcolor00);
 tolua_function(tolua_S,"get_value",tolua_gslua_UserStats_get_value00);
 tolua_function(tolua_S,"get_valuecolor",tolua_gslua_UserStats_get_valuecolor00);
 tolua_function(tolua_S,"get_plusx",tolua_gslua_UserStats_get_plusx00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsGlobals","gsGlobals","",NULL);
 tolua_beginmodule(tolua_S,"gsGlobals");
 tolua_variable(tolua_S,"fps",tolua_get_gsGlobals_fps,NULL);
 tolua_variable(tolua_S,"fpst",tolua_get_gsGlobals_fpst,NULL);
 tolua_variable(tolua_S,"game_time",tolua_get_gsGlobals_game_time,NULL);
 tolua_variable(tolua_S,"overall_game_time",tolua_get_gsGlobals_overall_game_time,NULL);
 tolua_variable(tolua_S,"second_time",tolua_get_gsGlobals_second_time,NULL);
 tolua_variable(tolua_S,"splitscreen",tolua_get_gsGlobals_splitscreen,NULL);
 tolua_variable(tolua_S,"onescreen_map",tolua_get_gsGlobals_onescreen_map,NULL);
 tolua_variable(tolua_S,"players",tolua_get_gsGlobals_players,NULL);
 tolua_variable(tolua_S,"game_mode",tolua_get_gsGlobals_game_mode,NULL);
 tolua_variable(tolua_S,"singleplayer",tolua_get_gsGlobals_singleplayer,NULL);
 tolua_variable(tolua_S,"minigame",tolua_get_gsGlobals_minigame,NULL);
 tolua_variable(tolua_S,"network_game",tolua_get_gsGlobals_network_game,NULL);
 tolua_variable(tolua_S,"use_joystick",tolua_get_gsGlobals_use_joystick,NULL);
 tolua_variable(tolua_S,"joystick_installed",tolua_get_gsGlobals_joystick_installed,NULL);
 tolua_variable(tolua_S,"game_resolution_x",tolua_get_gsGlobals_game_resolution_x,NULL);
 tolua_variable(tolua_S,"game_resolution_y",tolua_get_gsGlobals_game_resolution_y,NULL);
 tolua_variable(tolua_S,"tile_w",tolua_get_gsGlobals_tile_w,NULL);
 tolua_variable(tolua_S,"tile_h",tolua_get_gsGlobals_tile_h,NULL);
 tolua_variable(tolua_S,"map_w",tolua_get_gsGlobals_map_w,NULL);
 tolua_variable(tolua_S,"map_h",tolua_get_gsGlobals_map_h,NULL);
 tolua_variable(tolua_S,"playscreen_width",tolua_get_gsGlobals_playscreen_width,NULL);
 tolua_variable(tolua_S,"playscreen_full_width",tolua_get_gsGlobals_playscreen_full_width,NULL);
 tolua_variable(tolua_S,"playscreen_split_width",tolua_get_gsGlobals_playscreen_split_width,NULL);
 tolua_variable(tolua_S,"playscreen_height",tolua_get_gsGlobals_playscreen_height,NULL);
 tolua_variable(tolua_S,"playscreen_full_height",tolua_get_gsGlobals_playscreen_full_height,NULL);
 tolua_variable(tolua_S,"playscreen1_sx",tolua_get_gsGlobals_playscreen1_sx,NULL);
 tolua_variable(tolua_S,"playscreen2_sx",tolua_get_gsGlobals_playscreen2_sx,NULL);
 tolua_variable(tolua_S,"panel_full_width",tolua_get_gsGlobals_panel_full_width,NULL);
 tolua_variable(tolua_S,"panel_full_height",tolua_get_gsGlobals_panel_full_height,NULL);
 tolua_variable(tolua_S,"panel_half_width",tolua_get_gsGlobals_panel_half_width,NULL);
 tolua_variable(tolua_S,"panel_half_height",tolua_get_gsGlobals_panel_half_height,NULL);
 tolua_variable(tolua_S,"panel_full_x",tolua_get_gsGlobals_panel_full_x,NULL);
 tolua_variable(tolua_S,"panel_full_y",tolua_get_gsGlobals_panel_full_y,NULL);
 tolua_variable(tolua_S,"col_white",tolua_get_gsGlobals_col_white,NULL);
 tolua_variable(tolua_S,"col_grey",tolua_get_gsGlobals_col_grey,NULL);
 tolua_variable(tolua_S,"col_dgrey",tolua_get_gsGlobals_col_dgrey,NULL);
 tolua_variable(tolua_S,"col_yellow",tolua_get_gsGlobals_col_yellow,NULL);
 tolua_variable(tolua_S,"col_red",tolua_get_gsGlobals_col_red,NULL);
 tolua_variable(tolua_S,"col_bred",tolua_get_gsGlobals_col_bred,NULL);
 tolua_variable(tolua_S,"col_blue",tolua_get_gsGlobals_col_blue,NULL);
 tolua_variable(tolua_S,"col_lblue",tolua_get_gsGlobals_col_lblue,NULL);
 tolua_variable(tolua_S,"col_green",tolua_get_gsGlobals_col_green,NULL);
 tolua_variable(tolua_S,"col_black",tolua_get_gsGlobals_col_black,NULL);
 tolua_variable(tolua_S,"col_orange",tolua_get_gsGlobals_col_orange,NULL);
 tolua_array(tolua_S,"tbl_blue",tolua_get_gslua_gsGlobals_tbl_blue,NULL);
 tolua_array(tolua_S,"tbl_white",tolua_get_gslua_gsGlobals_tbl_white,NULL);
 tolua_array(tolua_S,"tbl_bluewhite",tolua_get_gslua_gsGlobals_tbl_bluewhite,NULL);
 tolua_array(tolua_S,"tbl_green",tolua_get_gslua_gsGlobals_tbl_green,NULL);
 tolua_array(tolua_S,"tbl_yellow",tolua_get_gslua_gsGlobals_tbl_yellow,NULL);
 tolua_array(tolua_S,"tbl_red",tolua_get_gslua_gsGlobals_tbl_red,NULL);
 tolua_array(tolua_S,"tbl_all",tolua_get_gslua_gsGlobals_tbl_all,NULL);
 tolua_variable(tolua_S,"slow_computer",tolua_get_gsGlobals_slow_computer,NULL);
 tolua_variable(tolua_S,"activate_extra",tolua_get_gsGlobals_activate_extra,NULL);
 tolua_variable(tolua_S,"activate_extra_bar",tolua_get_gsGlobals_activate_extra_bar,NULL);
 tolua_variable(tolua_S,"extra_bar",tolua_get_gsGlobals_activate_extra_bar,NULL);
 tolua_variable(tolua_S,"capture_the_flag",tolua_get_gsGlobals_capture_the_flag,NULL);
 tolua_variable(tolua_S,"hunter_hunted",tolua_get_gsGlobals_hunter_hunted,NULL);
 tolua_variable(tolua_S,"race",tolua_get_gsGlobals_race,NULL);
 tolua_array(tolua_S,"color",tolua_get_gslua_gsGlobals_color,NULL);
 tolua_variable(tolua_S,"data_dir",tolua_get_gsGlobals_data_dir,NULL);
 tolua_variable(tolua_S,"ship_dir",tolua_get_gsGlobals_ship_dir,NULL);
 tolua_variable(tolua_S,"level_dir",tolua_get_gsGlobals_level_dir,NULL);
 tolua_variable(tolua_S,"map_dir",tolua_get_gsGlobals_map_dir,NULL);
 tolua_variable(tolua_S,"script_dir",tolua_get_gsGlobals_script_dir,NULL);
 tolua_variable(tolua_S,"hiscore_dir",tolua_get_gsGlobals_hiscore_dir,NULL);
 tolua_variable(tolua_S,"configfile_name",tolua_get_gsGlobals_configfile_name,NULL);
 tolua_variable(tolua_S,"datafile_name",tolua_get_gsGlobals_datafile_name,NULL);
 tolua_variable(tolua_S,"soundfile_name",tolua_get_gsGlobals_soundfile_name,NULL);
 tolua_variable(tolua_S,"shipdata_name",tolua_get_gsGlobals_shipdata_name,NULL);
 tolua_variable(tolua_S,"fontdata_name",tolua_get_gsGlobals_fontdata_name,NULL);
 tolua_variable(tolua_S,"menu_datafile_name",tolua_get_gsGlobals_menu_datafile_name,NULL);
 tolua_variable(tolua_S,"language_file_name",tolua_get_gsGlobals_language_file_name,NULL);
 tolua_variable(tolua_S,"keyboard_file_name",tolua_get_gsGlobals_keyboard_file_name,NULL);
 tolua_variable(tolua_S,"global_ini",tolua_get_gsGlobals_global_ini,NULL);
 tolua_variable(tolua_S,"global_sc",tolua_get_gsGlobals_global_sc,NULL);
 tolua_variable(tolua_S,"hiscore_file_name",tolua_get_gsGlobals_hiscore_file_name,NULL);
 tolua_variable(tolua_S,"def_ship",tolua_get_gsGlobals_def_ship,NULL);
 tolua_variable(tolua_S,"def_level",tolua_get_gsGlobals_def_level,NULL);
 tolua_array(tolua_S,"level_files",tolua_get_gslua_gsGlobals_level_files,NULL);
 tolua_variable(tolua_S,"selected_level",tolua_get_gsGlobals_selected_level,NULL);
 tolua_variable(tolua_S,"last_level",tolua_get_gsGlobals_last_level,NULL);
 tolua_variable(tolua_S,"last_splevel",tolua_get_gsGlobals_last_splevel,NULL);
 tolua_variable(tolua_S,"max_levels",tolua_get_gsGlobals_max_levels,NULL);
 tolua_function(tolua_S,"make_color",tolua_gslua_gsGlobals_make_color00);
 tolua_function(tolua_S,"get_level_num_from_filename",tolua_gslua_gsGlobals_get_level_num_from_filename00);
 tolua_function(tolua_S,"set_highscore",tolua_gslua_gsGlobals_set_highscore00);
 tolua_function(tolua_S,"set_highscore",tolua_gslua_gsGlobals_set_highscore01);
 tolua_function(tolua_S,"create_level_done_file",tolua_gslua_gsGlobals_create_level_done_file00);
 tolua_variable(tolua_S,"gravity_percent",tolua_get_gsGlobals_gravity_percent,tolua_set_gsGlobals_gravity_percent);
 tolua_variable(tolua_S,"level_gravity",tolua_get_gsGlobals_level_gravity,tolua_set_gsGlobals_level_gravity);
 tolua_variable(tolua_S,"particle_gravity",tolua_get_gsGlobals_particle_gravity,tolua_set_gsGlobals_particle_gravity);
 tolua_variable(tolua_S,"bomb_gravity",tolua_get_gsGlobals_bomb_gravity,tolua_set_gsGlobals_bomb_gravity);
 tolua_variable(tolua_S,"exit_level",tolua_get_gsGlobals_exit_level,tolua_set_gsGlobals_exit_level);
 tolua_variable(tolua_S,"max_craters",tolua_get_gsGlobals_max_craters,tolua_set_gsGlobals_max_craters);
 tolua_variable(tolua_S,"language",tolua_get_gsGlobals_language,tolua_set_gsGlobals_language);
 tolua_variable(tolua_S,"plus_x",tolua_get_gsGlobals_plus_x,tolua_set_gsGlobals_plus_x);
 tolua_variable(tolua_S,"plus_y",tolua_get_gsGlobals_plus_y,tolua_set_gsGlobals_plus_y);
 tolua_variable(tolua_S,"pixamount",tolua_get_gsGlobals_pixamount,tolua_set_gsGlobals_pixamount);
 tolua_variable(tolua_S,"soundvol",tolua_get_gsGlobals_soundvol,tolua_set_gsGlobals_soundvol);
 tolua_variable(tolua_S,"race_laps",tolua_get_gsGlobals_race_laps,tolua_set_gsGlobals_race_laps);
 tolua_variable(tolua_S,"shoot_speed",tolua_get_gsGlobals_shoot_speed,tolua_set_gsGlobals_shoot_speed);
 tolua_variable(tolua_S,"shoot_speed_bomb",tolua_get_gsGlobals_shoot_speed_bomb,tolua_set_gsGlobals_shoot_speed_bomb);
 tolua_variable(tolua_S,"player_respawn",tolua_get_gsGlobals_player_respawn,tolua_set_gsGlobals_player_respawn);
 tolua_variable(tolua_S,"player_indestructible",tolua_get_gsGlobals_player_indestructible,tolua_set_gsGlobals_player_indestructible);
 tolua_variable(tolua_S,"extra_amount",tolua_get_gsGlobals_extra_amount,tolua_set_gsGlobals_extra_amount);
 tolua_variable(tolua_S,"max_frags",tolua_get_gsGlobals_max_frags,tolua_set_gsGlobals_max_frags);
 tolua_variable(tolua_S,"max_time",tolua_get_gsGlobals_max_time,tolua_set_gsGlobals_max_time);
 tolua_variable(tolua_S,"do_all_hooks",tolua_get_gsGlobals_do_all_hooks,tolua_set_gsGlobals_do_all_hooks);
 tolua_variable(tolua_S,"computer_player",tolua_get_gsGlobals_computer_player,tolua_set_gsGlobals_computer_player);
 tolua_variable(tolua_S,"computer_player_strength",tolua_get_gsGlobals_computer_player_strength,tolua_set_gsGlobals_computer_player_strength);
 tolua_variable(tolua_S,"use_script_computer_player",tolua_get_gsGlobals_use_script_computer_player,tolua_set_gsGlobals_use_script_computer_player);
 tolua_variable(tolua_S,"use_user_stats",tolua_get_gsGlobals_use_user_stats,tolua_set_gsGlobals_use_user_stats);
 tolua_array(tolua_S,"user_stats",tolua_get_gslua_gsGlobals_user_stats,tolua_set_gslua_gsGlobals_user_stats);
 tolua_variable(tolua_S,"score_per_second",tolua_get_gsGlobals_score_per_second,tolua_set_gsGlobals_score_per_second);
 tolua_variable(tolua_S,"handicap",tolua_get_gsGlobals_handicap,tolua_set_gsGlobals_handicap);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"globals",tolua_get_globals_ptr,tolua_set_globals_ptr);
 tolua_constant(tolua_S,"LOCAL_SINGLE",LOCAL_SINGLE);
 tolua_constant(tolua_S,"LOCAL1",LOCAL1);
 tolua_constant(tolua_S,"LOCAL2",LOCAL2);
 tolua_constant(tolua_S,"LEFT",LEFT);
 tolua_constant(tolua_S,"RIGHT",RIGHT);
 tolua_constant(tolua_S,"SHOOT",SHOOT);
 tolua_constant(tolua_S,"THRUST",THRUST);
 tolua_constant(tolua_S,"NEXT_WEAPON",NEXT_WEAPON);
 tolua_constant(tolua_S,"PREV_WEAPON",PREV_WEAPON);
 tolua_constant(tolua_S,"WEAPON_1",WEAPON_1);
 tolua_constant(tolua_S,"WEAPON_2",WEAPON_2);
 tolua_constant(tolua_S,"WEAPON_3",WEAPON_3);
 tolua_constant(tolua_S,"WEAPON_4",WEAPON_4);
 tolua_constant(tolua_S,"WEAPON_5",WEAPON_5);
 tolua_constant(tolua_S,"WEAPON_6",WEAPON_6);
 tolua_constant(tolua_S,"EXTRA_ACTION",EXTRA_ACTION);
 tolua_constant(tolua_S,"SPECIAL",SPECIAL);
 tolua_constant(tolua_S,"TEMP1",TEMP1);
 tolua_constant(tolua_S,"TEMP2",TEMP2);
 tolua_constant(tolua_S,"TEMP3",TEMP3);
 tolua_constant(tolua_S,"SP_PAUSE",SP_PAUSE);
 tolua_constant(tolua_S,"SP_SCREENSHOT",SP_SCREENSHOT);
 tolua_constant(tolua_S,"SP_PANEL",SP_PANEL);
 tolua_constant(tolua_S,"SP_DEBUG",SP_DEBUG);
 tolua_constant(tolua_S,"SP_CHAT",SP_CHAT);
 tolua_constant(tolua_S,"TEMP9",TEMP9);
 tolua_constant(tolua_S,"LEFT_L1",LEFT_L1);
 tolua_constant(tolua_S,"RIGHT_L1",RIGHT_L1);
 tolua_constant(tolua_S,"SHOOT_L1",SHOOT_L1);
 tolua_constant(tolua_S,"THRUST_L1",THRUST_L1);
 tolua_constant(tolua_S,"NEXT_WEAPON_L1",NEXT_WEAPON_L1);
 tolua_constant(tolua_S,"PREV_WEAPON_L1",PREV_WEAPON_L1);
 tolua_constant(tolua_S,"WEAPON_1_L1",WEAPON_1_L1);
 tolua_constant(tolua_S,"WEAPON_2_L1",WEAPON_2_L1);
 tolua_constant(tolua_S,"WEAPON_3_L1",WEAPON_3_L1);
 tolua_constant(tolua_S,"WEAPON_4_L1",WEAPON_4_L1);
 tolua_constant(tolua_S,"WEAPON_5_L1",WEAPON_5_L1);
 tolua_constant(tolua_S,"WEAPON_6_L1",WEAPON_6_L1);
 tolua_constant(tolua_S,"EXTRA_ACTION_L1",EXTRA_ACTION_L1);
 tolua_constant(tolua_S,"SPECIAL_L1",SPECIAL_L1);
 tolua_constant(tolua_S,"TEMP1_L1",TEMP1_L1);
 tolua_constant(tolua_S,"TEMP2_L1",TEMP2_L1);
 tolua_constant(tolua_S,"TEMP3_L1",TEMP3_L1);
 tolua_constant(tolua_S,"TEMP4_L1",TEMP4_L1);
 tolua_constant(tolua_S,"TEMP5_L1",TEMP5_L1);
 tolua_constant(tolua_S,"TEMP6_L1",TEMP6_L1);
 tolua_constant(tolua_S,"TEMP7_L1",TEMP7_L1);
 tolua_constant(tolua_S,"TEMP8_L1",TEMP8_L1);
 tolua_constant(tolua_S,"TEMP9_L1",TEMP9_L1);
 tolua_constant(tolua_S,"LEFT_L2",LEFT_L2);
 tolua_constant(tolua_S,"RIGHT_L2",RIGHT_L2);
 tolua_constant(tolua_S,"SHOOT_L2",SHOOT_L2);
 tolua_constant(tolua_S,"THRUST_L2",THRUST_L2);
 tolua_constant(tolua_S,"NEXT_WEAPON_L2",NEXT_WEAPON_L2);
 tolua_constant(tolua_S,"PREV_WEAPON_L2",PREV_WEAPON_L2);
 tolua_constant(tolua_S,"WEAPON_1_L2",WEAPON_1_L2);
 tolua_constant(tolua_S,"WEAPON_2_L2",WEAPON_2_L2);
 tolua_constant(tolua_S,"WEAPON_3_L2",WEAPON_3_L2);
 tolua_constant(tolua_S,"WEAPON_4_L2",WEAPON_4_L2);
 tolua_constant(tolua_S,"WEAPON_5_L2",WEAPON_5_L2);
 tolua_constant(tolua_S,"WEAPON_6_L2",WEAPON_6_L2);
 tolua_constant(tolua_S,"EXTRA_ACTION_L2",EXTRA_ACTION_L2);
 tolua_constant(tolua_S,"SPECIAL_L2",SPECIAL_L2);
 tolua_constant(tolua_S,"TEMP1_L2",TEMP1_L2);
 tolua_constant(tolua_S,"TEMP2_L2",TEMP2_L2);
 tolua_constant(tolua_S,"TEMP3_L2",TEMP3_L2);
 tolua_constant(tolua_S,"TEMP4_L2",TEMP4_L2);
 tolua_constant(tolua_S,"TEMP5_L2",TEMP5_L2);
 tolua_constant(tolua_S,"TEMP6_L2",TEMP6_L2);
 tolua_constant(tolua_S,"TEMP7_L2",TEMP7_L2);
 tolua_constant(tolua_S,"TEMP8_L2",TEMP8_L2);
 tolua_constant(tolua_S,"TEMP9_L2",TEMP9_L2);
 tolua_cclass(tolua_S,"gsControl","gsControl","",NULL);
 tolua_beginmodule(tolua_S,"gsControl");
 tolua_function(tolua_S,"set_key",tolua_gslua_gsControl_set_key00);
 tolua_function(tolua_S,"get_key",tolua_gslua_gsControl_get_key00);
 tolua_function(tolua_S,"update",tolua_gslua_gsControl_update00);
 tolua_function(tolua_S,"get_pressed_key",tolua_gslua_gsControl_get_pressed_key00);
 tolua_function(tolua_S,"enable",tolua_gslua_gsControl_enable00);
 tolua_function(tolua_S,"disable",tolua_gslua_gsControl_disable00);
 tolua_function(tolua_S,"get_disabled",tolua_gslua_gsControl_get_disabled00);
 tolua_function(tolua_S,"left",tolua_gslua_gsControl_left00);
 tolua_function(tolua_S,"right",tolua_gslua_gsControl_right00);
 tolua_function(tolua_S,"thrust",tolua_gslua_gsControl_thrust00);
 tolua_function(tolua_S,"shoot",tolua_gslua_gsControl_shoot00);
 tolua_function(tolua_S,"special",tolua_gslua_gsControl_special00);
 tolua_function(tolua_S,"extra_action",tolua_gslua_gsControl_extra_action00);
 tolua_function(tolua_S,"nw",tolua_gslua_gsControl_nw00);
 tolua_function(tolua_S,"pw",tolua_gslua_gsControl_pw00);
 tolua_function(tolua_S,"w1",tolua_gslua_gsControl_w100);
 tolua_function(tolua_S,"w2",tolua_gslua_gsControl_w200);
 tolua_function(tolua_S,"w3",tolua_gslua_gsControl_w300);
 tolua_function(tolua_S,"w4",tolua_gslua_gsControl_w400);
 tolua_function(tolua_S,"w5",tolua_gslua_gsControl_w500);
 tolua_function(tolua_S,"w6",tolua_gslua_gsControl_w600);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"MAP_GRASS",MAP_GRASS);
 tolua_constant(tolua_S,"MAP_STONE",MAP_STONE);
 tolua_cclass(tolua_S,"gsMap","gsMap","",NULL);
 tolua_beginmodule(tolua_S,"gsMap");
 tolua_variable(tolua_S,"osd",tolua_get_gsMap_osd_ptr,tolua_set_gsMap_osd_ptr);
 tolua_function(tolua_S,"get_scroll_x",tolua_gslua_gsMap_get_scroll_x00);
 tolua_function(tolua_S,"get_scroll_y",tolua_gslua_gsMap_get_scroll_y00);
 tolua_function(tolua_S,"get_scroll_to_x",tolua_gslua_gsMap_get_scroll_to_x00);
 tolua_function(tolua_S,"get_scroll_to_y",tolua_gslua_gsMap_get_scroll_to_y00);
 tolua_function(tolua_S,"get_map_x",tolua_gslua_gsMap_get_map_x00);
 tolua_function(tolua_S,"get_map_y",tolua_gslua_gsMap_get_map_y00);
 tolua_function(tolua_S,"get_scroll_speed_x",tolua_gslua_gsMap_get_scroll_speed_x00);
 tolua_function(tolua_S,"get_scroll_speed_y",tolua_gslua_gsMap_get_scroll_speed_y00);
 tolua_function(tolua_S,"get_fscroll_x",tolua_gslua_gsMap_get_fscroll_x00);
 tolua_function(tolua_S,"get_fscroll_y",tolua_gslua_gsMap_get_fscroll_y00);
 tolua_function(tolua_S,"is_redraw_needed",tolua_gslua_gsMap_is_redraw_needed00);
 tolua_function(tolua_S,"request_redraw",tolua_gslua_gsMap_request_redraw00);
 tolua_function(tolua_S,"get_style",tolua_gslua_gsMap_get_style00);
 tolua_function(tolua_S,"restrict_scroll_x",tolua_gslua_gsMap_restrict_scroll_x00);
 tolua_function(tolua_S,"restrict_scroll_y",tolua_gslua_gsMap_restrict_scroll_y00);
 tolua_function(tolua_S,"scroll_to",tolua_gslua_gsMap_scroll_to00);
 tolua_function(tolua_S,"scroll_to",tolua_gslua_gsMap_scroll_to01);
 tolua_function(tolua_S,"scroll_to",tolua_gslua_gsMap_scroll_to02);
 tolua_function(tolua_S,"scroll",tolua_gslua_gsMap_scroll00);
 tolua_function(tolua_S,"redraw",tolua_gslua_gsMap_redraw00);
 tolua_function(tolua_S,"is_pixel",tolua_gslua_gsMap_is_pixel00);
 tolua_function(tolua_S,"get_pixel",tolua_gslua_gsMap_get_pixel00);
 tolua_function(tolua_S,"is_tile",tolua_gslua_gsMap_is_tile00);
 tolua_function(tolua_S,"is_on_screen",tolua_gslua_gsMap_is_on_screen00);
 tolua_function(tolua_S,"is_on_map",tolua_gslua_gsMap_is_on_map00);
 tolua_function(tolua_S,"draw_rectangle",tolua_gslua_gsMap_draw_rectangle00);
 tolua_function(tolua_S,"draw_filled_rectangle",tolua_gslua_gsMap_draw_filled_rectangle00);
 tolua_function(tolua_S,"draw_pixel",tolua_gslua_gsMap_draw_pixel00);
 tolua_function(tolua_S,"draw_circle",tolua_gslua_gsMap_draw_circle00);
 tolua_function(tolua_S,"draw_filled_circle",tolua_gslua_gsMap_draw_filled_circle00);
 tolua_function(tolua_S,"draw_ellipse",tolua_gslua_gsMap_draw_ellipse00);
 tolua_function(tolua_S,"draw_filled_ellipse",tolua_gslua_gsMap_draw_filled_ellipse00);
 tolua_function(tolua_S,"draw_line",tolua_gslua_gsMap_draw_line00);
 tolua_function(tolua_S,"draw_text",tolua_gslua_gsMap_draw_text00);
 tolua_function(tolua_S,"draw_sprite",tolua_gslua_gsMap_draw_sprite00);
 tolua_function(tolua_S,"fill_area",tolua_gslua_gsMap_fill_area00);
 tolua_endmodule(tolua_S);
 tolua_array(tolua_S,"playmap",tolua_get_gslua_playmap,tolua_set_gslua_playmap);
 tolua_constant(tolua_S,"EXP_SMALL_1",EXP_SMALL_1);
 tolua_constant(tolua_S,"EXP_SMALL_2",EXP_SMALL_2);
 tolua_constant(tolua_S,"EXP_SMALL_3",EXP_SMALL_3);
 tolua_constant(tolua_S,"EXP_SMALL_4",EXP_SMALL_4);
 tolua_constant(tolua_S,"EXP_MED_1",EXP_MED_1);
 tolua_constant(tolua_S,"EXP_BIG_1",EXP_BIG_1);
 tolua_constant(tolua_S,"EXP_BIG_2",EXP_BIG_2);
 tolua_cclass(tolua_S,"Explosion","Explosion","",NULL);
 tolua_beginmodule(tolua_S,"Explosion");
 tolua_function(tolua_S,"get_prev",tolua_gslua_Explosion_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Explosion_get_next00);
 tolua_function(tolua_S,"get_x",tolua_gslua_Explosion_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Explosion_get_y00);
 tolua_function(tolua_S,"get_width",tolua_gslua_Explosion_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_Explosion_get_height00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Explosion_get_type00);
 tolua_function(tolua_S,"get_starttime",tolua_gslua_Explosion_get_starttime00);
 tolua_function(tolua_S,"get_curframe",tolua_gslua_Explosion_get_curframe00);
 tolua_function(tolua_S,"get_maxframe",tolua_gslua_Explosion_get_maxframe00);
 tolua_function(tolua_S,"get_maxframet",tolua_gslua_Explosion_get_maxframet00);
 tolua_function(tolua_S,"get_player_sig",tolua_gslua_Explosion_get_player_sig00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsExplosion","gsExplosion","",NULL);
 tolua_beginmodule(tolua_S,"gsExplosion");
 tolua_function(tolua_S,"get_first",tolua_gslua_gsExplosion_get_first00);
 tolua_function(tolua_S,"get_last",tolua_gslua_gsExplosion_get_last00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsExplosion_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsExplosion_add00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Crater","Crater","",NULL);
 tolua_beginmodule(tolua_S,"Crater");
 tolua_function(tolua_S,"get_x",tolua_gslua_Crater_get_x00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Crater_set_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Crater_get_y00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Crater_set_y00);
 tolua_function(tolua_S,"get_size",tolua_gslua_Crater_get_size00);
 tolua_function(tolua_S,"set_size",tolua_gslua_Crater_set_size00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Crater_get_prev00);
 tolua_function(tolua_S,"set_prev",tolua_gslua_Crater_set_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Crater_get_next00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Crater_set_next00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsCrater","gsCrater","",NULL);
 tolua_beginmodule(tolua_S,"gsCrater");
 tolua_function(tolua_S,"get_first",tolua_gslua_gsCrater_get_first00);
 tolua_function(tolua_S,"get_last",tolua_gslua_gsCrater_get_last00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsCrater_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsCrater_add00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsCrater_remove00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsEffect","gsEffect","",NULL);
 tolua_beginmodule(tolua_S,"gsEffect");
 tolua_variable(tolua_S,"pixels",tolua_get_gsEffect_pixels_ptr,tolua_set_gsEffect_pixels_ptr);
 tolua_variable(tolua_S,"bullets",tolua_get_gsEffect_bullets_ptr,tolua_set_gsEffect_bullets_ptr);
 tolua_variable(tolua_S,"explosions",tolua_get_gsEffect_explosions_ptr,tolua_set_gsEffect_explosions_ptr);
 tolua_variable(tolua_S,"craters",tolua_get_gsEffect_craters_ptr,tolua_set_gsEffect_craters_ptr);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"effects",tolua_get_effects_ptr,tolua_set_effects_ptr);
 tolua_constant(tolua_S,"OSDBIT_SCREEN",OSDBIT_SCREEN);
 tolua_constant(tolua_S,"OSDBIT_MAP",OSDBIT_MAP);
 tolua_constant(tolua_S,"OSDBIT_PANEL",OSDBIT_PANEL);
 tolua_cclass(tolua_S,"UserImageAnim","UserImageAnim","",NULL);
 tolua_beginmodule(tolua_S,"UserImageAnim");
 tolua_function(tolua_S,"get_prev",tolua_gslua_UserImageAnim_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_UserImageAnim_get_next00);
 tolua_function(tolua_S,"get_x",tolua_gslua_UserImageAnim_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_UserImageAnim_get_y00);
 tolua_function(tolua_S,"get_loop",tolua_gslua_UserImageAnim_get_loop00);
 tolua_function(tolua_S,"get_curframe",tolua_gslua_UserImageAnim_get_curframe00);
 tolua_function(tolua_S,"get_curframet",tolua_gslua_UserImageAnim_get_curframet00);
 tolua_function(tolua_S,"get_maxframe",tolua_gslua_UserImageAnim_get_maxframe00);
 tolua_function(tolua_S,"get_maxframet",tolua_gslua_UserImageAnim_get_maxframet00);
 tolua_function(tolua_S,"set_curframe",tolua_gslua_UserImageAnim_set_curframe00);
 tolua_function(tolua_S,"set_maxframe",tolua_gslua_UserImageAnim_set_maxframe00);
 tolua_function(tolua_S,"set_curframet",tolua_gslua_UserImageAnim_set_curframet00);
 tolua_function(tolua_S,"set_maxframet",tolua_gslua_UserImageAnim_set_maxframet00);
 tolua_function(tolua_S,"get_startframe",tolua_gslua_UserImageAnim_get_startframe00);
 tolua_function(tolua_S,"set_startframe",tolua_gslua_UserImageAnim_set_startframe00);
 tolua_function(tolua_S,"get_image",tolua_gslua_UserImageAnim_get_image00);
 tolua_function(tolua_S,"set_image",tolua_gslua_UserImageAnim_set_image00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"OsdBitmap","OsdBitmap","",NULL);
 tolua_beginmodule(tolua_S,"OsdBitmap");
 tolua_function(tolua_S,"get_prev",tolua_gslua_OsdBitmap_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_OsdBitmap_get_next00);
 tolua_function(tolua_S,"get_x",tolua_gslua_OsdBitmap_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_OsdBitmap_get_y00);
 tolua_function(tolua_S,"set_x",tolua_gslua_OsdBitmap_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_OsdBitmap_set_y00);
 tolua_function(tolua_S,"get_width",tolua_gslua_OsdBitmap_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_OsdBitmap_get_height00);
 tolua_function(tolua_S,"get_mode",tolua_gslua_OsdBitmap_get_mode00);
 tolua_function(tolua_S,"get_sig",tolua_gslua_OsdBitmap_get_sig00);
 tolua_function(tolua_S,"get_active",tolua_gslua_OsdBitmap_get_active00);
 tolua_function(tolua_S,"get_timeout",tolua_gslua_OsdBitmap_get_timeout00);
 tolua_function(tolua_S,"get_last_clear_color",tolua_gslua_OsdBitmap_get_last_clear_color00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_OsdBitmap_get_prev01);
 tolua_function(tolua_S,"get_next",tolua_gslua_OsdBitmap_get_next01);
 tolua_function(tolua_S,"set_mode",tolua_gslua_OsdBitmap_set_mode00);
 tolua_function(tolua_S,"set_active",tolua_gslua_OsdBitmap_set_active00);
 tolua_function(tolua_S,"set_timeout",tolua_gslua_OsdBitmap_set_timeout00);
 tolua_function(tolua_S,"set_visible",tolua_gslua_OsdBitmap_set_visible00);
 tolua_function(tolua_S,"get_visible",tolua_gslua_OsdBitmap_get_visible00);
 tolua_function(tolua_S,"clear",tolua_gslua_OsdBitmap_clear00);
 tolua_function(tolua_S,"draw_pixel",tolua_gslua_OsdBitmap_draw_pixel00);
 tolua_function(tolua_S,"draw_rectangle",tolua_gslua_OsdBitmap_draw_rectangle00);
 tolua_function(tolua_S,"draw_filled_rectangle",tolua_gslua_OsdBitmap_draw_filled_rectangle00);
 tolua_function(tolua_S,"draw_line",tolua_gslua_OsdBitmap_draw_line00);
 tolua_function(tolua_S,"draw_circle",tolua_gslua_OsdBitmap_draw_circle00);
 tolua_function(tolua_S,"draw_filled_circle",tolua_gslua_OsdBitmap_draw_filled_circle00);
 tolua_function(tolua_S,"draw_ellipse",tolua_gslua_OsdBitmap_draw_ellipse00);
 tolua_function(tolua_S,"draw_filled_ellipse",tolua_gslua_OsdBitmap_draw_filled_ellipse00);
 tolua_function(tolua_S,"fill_area",tolua_gslua_OsdBitmap_fill_area00);
 tolua_function(tolua_S,"draw_text",tolua_gslua_OsdBitmap_draw_text00);
 tolua_function(tolua_S,"draw_typewriter_text",tolua_gslua_OsdBitmap_draw_typewriter_text00);
 tolua_function(tolua_S,"add_image",tolua_gslua_OsdBitmap_add_image00);
 tolua_function(tolua_S,"remove_image",tolua_gslua_OsdBitmap_remove_image00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsOsd","gsOsd","",NULL);
 tolua_beginmodule(tolua_S,"gsOsd");
 tolua_function(tolua_S,"get_first_osdbit",tolua_gslua_gsOsd_get_first_osdbit00);
 tolua_function(tolua_S,"get_last_osdbit",tolua_gslua_gsOsd_get_last_osdbit00);
 tolua_function(tolua_S,"add",tolua_gslua_gsOsd_add00);
 tolua_function(tolua_S,"add_label",tolua_gslua_gsOsd_add_label00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsOsd_remove00);
 tolua_function(tolua_S,"get_osdbit",tolua_gslua_gsOsd_get_osdbit00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsError","gsError","",NULL);
 tolua_beginmodule(tolua_S,"gsError");
 tolua_function(tolua_S,"log",tolua_gslua_gsError_log00);
 tolua_function(tolua_S,"log",tolua_gslua_gsError_log01);
 tolua_function(tolua_S,"log",tolua_gslua_gsError_log02);
 tolua_function(tolua_S,"log",tolua_gslua_gsError_log03);
 tolua_function(tolua_S,"log",tolua_gslua_gsError_log04);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"errors",tolua_get_errors_ptr,tolua_set_errors_ptr);
 tolua_cclass(tolua_S,"gsNetPlayer","gsNetPlayer","",NULL);
 tolua_beginmodule(tolua_S,"gsNetPlayer");
 tolua_variable(tolua_S,"player",tolua_get_gsNetPlayer_player_ptr,tolua_set_gsNetPlayer_player_ptr);
 tolua_variable(tolua_S,"network",tolua_get_gsNetPlayer_network,tolua_set_gsNetPlayer_network);
 tolua_variable(tolua_S,"name",tolua_get_gsNetPlayer_name,NULL);
 tolua_variable(tolua_S,"ship_file",tolua_get_gsNetPlayer_ship_file,NULL);
 tolua_variable(tolua_S,"shipimg_file",tolua_get_gsNetPlayer_shipimg_file,NULL);
 tolua_variable(tolua_S,"net_id",tolua_get_gsNetPlayer_net_id,tolua_set_gsNetPlayer_net_id);
 tolua_variable(tolua_S,"ready",tolua_get_gsNetPlayer_ready,tolua_set_gsNetPlayer_ready);
 tolua_variable(tolua_S,"in_game",tolua_get_gsNetPlayer_in_game,tolua_set_gsNetPlayer_in_game);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsChat","gsChat","",NULL);
 tolua_beginmodule(tolua_S,"gsChat");
 tolua_function(tolua_S,"add",tolua_gslua_gsChat_add00);
 tolua_function(tolua_S,"get_message",tolua_gslua_gsChat_get_message00);
 tolua_function(tolua_S,"get_player",tolua_gslua_gsChat_get_player00);
 tolua_function(tolua_S,"get_message_with_playername",tolua_gslua_gsChat_get_message_with_playername00);
 tolua_function(tolua_S,"get_color",tolua_gslua_gsChat_get_color00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsChat_get_count00);
 tolua_function(tolua_S,"clear",tolua_gslua_gsChat_clear00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsNetwork","gsNetwork","",NULL);
 tolua_beginmodule(tolua_S,"gsNetwork");
 tolua_variable(tolua_S,"chat",tolua_get_gsNetwork_chat,tolua_set_gsNetwork_chat);
 tolua_function(tolua_S,"is_server",tolua_gslua_gsNetwork_is_server00);
 tolua_function(tolua_S,"is_client",tolua_gslua_gsNetwork_is_client00);
 tolua_function(tolua_S,"get_netplayer",tolua_gslua_gsNetwork_get_netplayer00);
 tolua_function(tolua_S,"get_netplayer_by_id",tolua_gslua_gsNetwork_get_netplayer_by_id00);
 tolua_function(tolua_S,"get_netplayers",tolua_gslua_gsNetwork_get_netplayers00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"net",tolua_get_net_ptr,tolua_set_net_ptr);
 tolua_constant(tolua_S,"SOUND_MENU_CHANGE",SOUND_MENU_CHANGE);
 tolua_constant(tolua_S,"SOUND_MENU_SELECT",SOUND_MENU_SELECT);
 tolua_constant(tolua_S,"SOUND_MENU_CHANGEBAR",SOUND_MENU_CHANGEBAR);
 tolua_constant(tolua_S,"SOUND_MENU_NONO",SOUND_MENU_NONO);
 tolua_constant(tolua_S,"SOUND_MENU_KEYPRESSED",SOUND_MENU_KEYPRESSED);
 tolua_constant(tolua_S,"SOUND_MENU_SHOOT",SOUND_MENU_SHOOT);
 tolua_constant(tolua_S,"SOUND_MENU_DEAD",SOUND_MENU_DEAD);
 tolua_constant(tolua_S,"SOUND_EXTRA_TAKEN",SOUND_EXTRA_TAKEN);
 tolua_constant(tolua_S,"SOUND_CARGO_TAKEN",SOUND_CARGO_TAKEN);
 tolua_constant(tolua_S,"SOUND_CARGO_RETURNED",SOUND_CARGO_RETURNED);
 tolua_constant(tolua_S,"SOUND_DO_SWITCH",SOUND_DO_SWITCH);
 tolua_constant(tolua_S,"SOUND_BUMP_WALL",SOUND_BUMP_WALL);
 tolua_constant(tolua_S,"SOUND_SHIELD_HIT",SOUND_SHIELD_HIT);
 tolua_constant(tolua_S,"SOUND_HULL_HIT",SOUND_HULL_HIT);
 tolua_constant(tolua_S,"SOUND_WALL_HIT",SOUND_WALL_HIT);
 tolua_constant(tolua_S,"SOUND_SHIP_HIT",SOUND_SHIP_HIT);
 tolua_constant(tolua_S,"SOUND_SHOOT_EMPTY",SOUND_SHOOT_EMPTY);
 tolua_constant(tolua_S,"SOUND_SHOOT_SINGLE",SOUND_SHOOT_SINGLE);
 tolua_constant(tolua_S,"SOUND_SHOOT_DOUBLE",SOUND_SHOOT_DOUBLE);
 tolua_constant(tolua_S,"SOUND_SHOOT_BOMB",SOUND_SHOOT_BOMB);
 tolua_constant(tolua_S,"SOUND_SHOOT_BIGBOMB",SOUND_SHOOT_BIGBOMB);
 tolua_constant(tolua_S,"SOUND_SHOOT_ROCKET",SOUND_SHOOT_ROCKET);
 tolua_constant(tolua_S,"SOUND_SHOOT_FREEZER",SOUND_SHOOT_FREEZER);
 tolua_constant(tolua_S,"SOUND_SHOOT_SHRINKER",SOUND_SHOOT_SHRINKER);
 tolua_constant(tolua_S,"SOUND_SHOOT_PROBE",SOUND_SHOOT_PROBE);
 tolua_constant(tolua_S,"SOUND_SHOOT_MINE",SOUND_SHOOT_MINE);
 tolua_constant(tolua_S,"SOUND_SHOOT_GAS",SOUND_SHOOT_GAS);
 tolua_constant(tolua_S,"SOUND_SHOOT_BUMPER",SOUND_SHOOT_BUMPER);
 tolua_constant(tolua_S,"SOUND_SHOOT_SHOTGUN",SOUND_SHOOT_SHOTGUN);
 tolua_constant(tolua_S,"SOUND_SHOOT_ACID",SOUND_SHOOT_ACID);
 tolua_constant(tolua_S,"SOUND_BEAM_SHIP",SOUND_BEAM_SHIP);
 tolua_constant(tolua_S,"SOUND_BEAM_BULLET",SOUND_BEAM_BULLET);
 tolua_constant(tolua_S,"SOUND_EXPL_SMALL1",SOUND_EXPL_SMALL1);
 tolua_constant(tolua_S,"SOUND_EXPL_SMALL2",SOUND_EXPL_SMALL2);
 tolua_constant(tolua_S,"SOUND_EXPL_MEDIUM1",SOUND_EXPL_MEDIUM1);
 tolua_constant(tolua_S,"SOUND_EXPL_MEDIUM2",SOUND_EXPL_MEDIUM2);
 tolua_constant(tolua_S,"SOUND_EXPL_MEDIUM3",SOUND_EXPL_MEDIUM3);
 tolua_constant(tolua_S,"SOUND_EXPL_BIG1",SOUND_EXPL_BIG1);
 tolua_cclass(tolua_S,"gsSound","gsSound","",NULL);
 tolua_beginmodule(tolua_S,"gsSound");
 tolua_function(tolua_S,"get_play_music",tolua_gslua_gsSound_get_play_music00);
 tolua_function(tolua_S,"get_play_sound",tolua_gslua_gsSound_get_play_sound00);
 tolua_function(tolua_S,"play_sound",tolua_gslua_gsSound_play_sound00);
 tolua_function(tolua_S,"load_sound",tolua_gslua_gsSound_load_sound00);
 tolua_function(tolua_S,"replace_sound",tolua_gslua_gsSound_replace_sound00);
 tolua_function(tolua_S,"load_music",tolua_gslua_gsSound_load_music00);
 tolua_function(tolua_S,"start_music",tolua_gslua_gsSound_start_music00);
 tolua_function(tolua_S,"stop_music",tolua_gslua_gsSound_stop_music00);
 tolua_function(tolua_S,"destroy_music",tolua_gslua_gsSound_destroy_music00);
 tolua_function(tolua_S,"set_music_volume",tolua_gslua_gsSound_set_music_volume00);
 tolua_function(tolua_S,"get_music_volume",tolua_gslua_gsSound_get_music_volume00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"gsound",tolua_get_gsound_ptr,tolua_set_gsound_ptr);
 tolua_cclass(tolua_S,"Waypoint","Waypoint","",NULL);
 tolua_beginmodule(tolua_S,"Waypoint");
 tolua_function(tolua_S,"new",tolua_gslua_Waypoint_new00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Waypoint_set_next00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Waypoint_get_next00);
 tolua_function(tolua_S,"get_x",tolua_gslua_Waypoint_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Waypoint_get_y00);
 tolua_function(tolua_S,"get_speed",tolua_gslua_Waypoint_get_speed00);
 tolua_function(tolua_S,"get_pause",tolua_gslua_Waypoint_get_pause00);
 tolua_function(tolua_S,"get_curpause",tolua_gslua_Waypoint_get_curpause00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Waypoint_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Waypoint_set_y00);
 tolua_function(tolua_S,"set_speed",tolua_gslua_Waypoint_set_speed00);
 tolua_function(tolua_S,"set_pause",tolua_gslua_Waypoint_set_pause00);
 tolua_function(tolua_S,"set_curpause",tolua_gslua_Waypoint_set_curpause00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsWaypoint","gsWaypoint","",NULL);
 tolua_beginmodule(tolua_S,"gsWaypoint");
 tolua_function(tolua_S,"get_count",tolua_gslua_gsWaypoint_get_count00);
 tolua_function(tolua_S,"get_mode",tolua_gslua_gsWaypoint_get_mode00);
 tolua_function(tolua_S,"set_mode",tolua_gslua_gsWaypoint_set_mode00);
 tolua_function(tolua_S,"get_current",tolua_gslua_gsWaypoint_get_current00);
 tolua_function(tolua_S,"set_current",tolua_gslua_gsWaypoint_set_current00);
 tolua_function(tolua_S,"add",tolua_gslua_gsWaypoint_add00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsWaypoint_remove00);
 tolua_function(tolua_S,"remove_all",tolua_gslua_gsWaypoint_remove_all00);
 tolua_function(tolua_S,"get",tolua_gslua_gsWaypoint_get00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"D_LEFT",D_LEFT);
 tolua_constant(tolua_S,"D_RIGHT",D_RIGHT);
 tolua_constant(tolua_S,"D_UP",D_UP);
 tolua_constant(tolua_S,"D_DOWN",D_DOWN);
 tolua_constant(tolua_S,"E_NONE",E_NONE);
 tolua_constant(tolua_S,"E_USER",E_USER);
 tolua_constant(tolua_S,"E_TOWER_SINGLE",E_TOWER_SINGLE);
 tolua_constant(tolua_S,"E_TOWER_FLAK",E_TOWER_FLAK);
 tolua_constant(tolua_S,"E_TOWER_THREE",E_TOWER_THREE);
 tolua_constant(tolua_S,"E_TOWER_PULSE",E_TOWER_PULSE);
 tolua_constant(tolua_S,"E_TANK_MOBILETOWER",E_TANK_MOBILETOWER);
 tolua_constant(tolua_S,"E_TANK_FLYINGTOWER",E_TANK_FLYINGTOWER);
 tolua_constant(tolua_S,"E_SHIP_BLUE1",E_SHIP_BLUE1);
 tolua_constant(tolua_S,"E_SHIP_BLUE2",E_SHIP_BLUE2);
 tolua_constant(tolua_S,"E_SHIP_BIGRED",E_SHIP_BIGRED);
 tolua_constant(tolua_S,"E_SHIP_PROBE1",E_SHIP_PROBE1);
 tolua_constant(tolua_S,"E_SHIP_PROBE2",E_SHIP_PROBE2);
 tolua_constant(tolua_S,"E_SHIP_RELASH",E_SHIP_RELASH);
 tolua_constant(tolua_S,"E_SHIP_BIGBOSS",E_SHIP_BIGBOSS);
 tolua_cclass(tolua_S,"Enemy","Enemy","",NULL);
 tolua_beginmodule(tolua_S,"Enemy");
 tolua_variable(tolua_S,"wp",tolua_get_Enemy_wp_ptr,tolua_set_Enemy_wp_ptr);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Enemy_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Enemy_get_next00);
 tolua_function(tolua_S,"get_connected",tolua_gslua_Enemy_get_connected00);
 tolua_function(tolua_S,"set_connected",tolua_gslua_Enemy_set_connected00);
 tolua_function(tolua_S,"set_prev",tolua_gslua_Enemy_set_prev00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Enemy_set_next00);
 tolua_function(tolua_S,"set_type",tolua_gslua_Enemy_set_type00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Enemy_get_type00);
 tolua_function(tolua_S,"get_nr",tolua_gslua_Enemy_get_nr00);
 tolua_function(tolua_S,"set_nr",tolua_gslua_Enemy_set_nr00);
 tolua_function(tolua_S,"get_sig",tolua_gslua_Enemy_get_sig00);
 tolua_function(tolua_S,"set_sig",tolua_gslua_Enemy_set_sig00);
 tolua_function(tolua_S,"set_maxframe",tolua_gslua_Enemy_set_maxframe00);
 tolua_function(tolua_S,"get_maxframe",tolua_gslua_Enemy_get_maxframe00);
 tolua_function(tolua_S,"set_maxframet",tolua_gslua_Enemy_set_maxframet00);
 tolua_function(tolua_S,"get_maxframet",tolua_gslua_Enemy_get_maxframet00);
 tolua_function(tolua_S,"set_dispframe",tolua_gslua_Enemy_set_dispframe00);
 tolua_function(tolua_S,"get_dispframe",tolua_gslua_Enemy_get_dispframe00);
 tolua_function(tolua_S,"set_curframe",tolua_gslua_Enemy_set_curframe00);
 tolua_function(tolua_S,"get_curframe",tolua_gslua_Enemy_get_curframe00);
 tolua_function(tolua_S,"set_curframet",tolua_gslua_Enemy_set_curframet00);
 tolua_function(tolua_S,"get_curframet",tolua_gslua_Enemy_get_curframet00);
 tolua_function(tolua_S,"set_startframe",tolua_gslua_Enemy_set_startframe00);
 tolua_function(tolua_S,"get_startframe",tolua_gslua_Enemy_get_startframe00);
 tolua_function(tolua_S,"get_x",tolua_gslua_Enemy_get_x00);
 tolua_function(tolua_S,"get_y",tolua_gslua_Enemy_get_y00);
 tolua_function(tolua_S,"set_x",tolua_gslua_Enemy_set_x00);
 tolua_function(tolua_S,"set_y",tolua_gslua_Enemy_set_y00);
 tolua_function(tolua_S,"get_width",tolua_gslua_Enemy_get_width00);
 tolua_function(tolua_S,"get_height",tolua_gslua_Enemy_get_height00);
 tolua_function(tolua_S,"set_bound_x",tolua_gslua_Enemy_set_bound_x00);
 tolua_function(tolua_S,"set_bound_y",tolua_gslua_Enemy_set_bound_y00);
 tolua_function(tolua_S,"set_bound_w",tolua_gslua_Enemy_set_bound_w00);
 tolua_function(tolua_S,"set_bound_h",tolua_gslua_Enemy_set_bound_h00);
 tolua_function(tolua_S,"get_xradius",tolua_gslua_Enemy_get_xradius00);
 tolua_function(tolua_S,"set_xradius",tolua_gslua_Enemy_set_xradius00);
 tolua_function(tolua_S,"get_yradius",tolua_gslua_Enemy_get_yradius00);
 tolua_function(tolua_S,"set_yradius",tolua_gslua_Enemy_set_yradius00);
 tolua_function(tolua_S,"get_xspd",tolua_gslua_Enemy_get_xspd00);
 tolua_function(tolua_S,"get_yspd",tolua_gslua_Enemy_get_yspd00);
 tolua_function(tolua_S,"set_xspd",tolua_gslua_Enemy_set_xspd00);
 tolua_function(tolua_S,"set_yspd",tolua_gslua_Enemy_set_yspd00);
 tolua_function(tolua_S,"get_hit",tolua_gslua_Enemy_get_hit00);
 tolua_function(tolua_S,"get_maxhit",tolua_gslua_Enemy_get_maxhit00);
 tolua_function(tolua_S,"set_hit",tolua_gslua_Enemy_set_hit00);
 tolua_function(tolua_S,"set_maxhit",tolua_gslua_Enemy_set_maxhit00);
 tolua_function(tolua_S,"get_onehit",tolua_gslua_Enemy_get_onehit00);
 tolua_function(tolua_S,"get_hitby",tolua_gslua_Enemy_get_hitby00);
 tolua_function(tolua_S,"get_hitby_sx",tolua_gslua_Enemy_get_hitby_sx00);
 tolua_function(tolua_S,"get_hitby_sy",tolua_gslua_Enemy_get_hitby_sy00);
 tolua_function(tolua_S,"get_user",tolua_gslua_Enemy_get_user00);
 tolua_function(tolua_S,"set_user",tolua_gslua_Enemy_set_user00);
 tolua_function(tolua_S,"set_bullet_speed",tolua_gslua_Enemy_set_bullet_speed00);
 tolua_function(tolua_S,"get_bullet_speed",tolua_gslua_Enemy_get_bullet_speed00);
 tolua_function(tolua_S,"set_bullet_speed_x",tolua_gslua_Enemy_set_bullet_speed_x00);
 tolua_function(tolua_S,"get_bullet_speed_x",tolua_gslua_Enemy_get_bullet_speed_x00);
 tolua_function(tolua_S,"set_bullet_speed_y",tolua_gslua_Enemy_set_bullet_speed_y00);
 tolua_function(tolua_S,"get_bullet_speed_y",tolua_gslua_Enemy_get_bullet_speed_y00);
 tolua_function(tolua_S,"set_bullet_freq",tolua_gslua_Enemy_set_bullet_freq00);
 tolua_function(tolua_S,"get_bullet_freq",tolua_gslua_Enemy_get_bullet_freq00);
 tolua_function(tolua_S,"set_bullet_strength",tolua_gslua_Enemy_set_bullet_strength00);
 tolua_function(tolua_S,"get_bullet_strength",tolua_gslua_Enemy_get_bullet_strength00);
 tolua_function(tolua_S,"set_bullet_hull_hitp",tolua_gslua_Enemy_set_bullet_hull_hitp00);
 tolua_function(tolua_S,"get_bullet_hull_hitp",tolua_gslua_Enemy_get_bullet_hull_hitp00);
 tolua_function(tolua_S,"set_bullet_shield_hitp",tolua_gslua_Enemy_set_bullet_shield_hitp00);
 tolua_function(tolua_S,"get_bullet_shield_hitp",tolua_gslua_Enemy_get_bullet_shield_hitp00);
 tolua_function(tolua_S,"set_bullet_type",tolua_gslua_Enemy_set_bullet_type00);
 tolua_function(tolua_S,"get_bullet_type",tolua_gslua_Enemy_get_bullet_type00);
 tolua_function(tolua_S,"get_active",tolua_gslua_Enemy_get_active00);
 tolua_function(tolua_S,"set_active",tolua_gslua_Enemy_set_active00);
 tolua_function(tolua_S,"get_teleport_time",tolua_gslua_Enemy_get_teleport_time00);
 tolua_function(tolua_S,"set_teleport_time",tolua_gslua_Enemy_set_teleport_time00);
 tolua_function(tolua_S,"set_invisible",tolua_gslua_Enemy_set_invisible00);
 tolua_function(tolua_S,"get_invisible",tolua_gslua_Enemy_get_invisible00);
 tolua_function(tolua_S,"get_freezed",tolua_gslua_Enemy_get_freezed00);
 tolua_function(tolua_S,"get_shrinked",tolua_gslua_Enemy_get_shrinked00);
 tolua_function(tolua_S,"set_freezed",tolua_gslua_Enemy_set_freezed00);
 tolua_function(tolua_S,"set_shrinked",tolua_gslua_Enemy_set_shrinked00);
 tolua_function(tolua_S,"set_maxshrink",tolua_gslua_Enemy_set_maxshrink00);
 tolua_function(tolua_S,"get_maxshrink",tolua_gslua_Enemy_get_maxshrink00);
 tolua_function(tolua_S,"set_dont_continue",tolua_gslua_Enemy_set_dont_continue00);
 tolua_function(tolua_S,"get_do_hooks",tolua_gslua_Enemy_get_do_hooks00);
 tolua_function(tolua_S,"set_do_hooks",tolua_gslua_Enemy_set_do_hooks00);
 tolua_function(tolua_S,"get_current_sprite",tolua_gslua_Enemy_get_current_sprite00);
 tolua_function(tolua_S,"remove",tolua_gslua_Enemy_remove00);
 tolua_function(tolua_S,"get_remove",tolua_gslua_Enemy_get_remove00);
 tolua_function(tolua_S,"set_user_int",tolua_gslua_Enemy_set_user_int00);
 tolua_function(tolua_S,"set_user_float",tolua_gslua_Enemy_set_user_float00);
 tolua_function(tolua_S,"get_user_int",tolua_gslua_Enemy_get_user_int00);
 tolua_function(tolua_S,"get_user_float",tolua_gslua_Enemy_get_user_float00);
 tolua_function(tolua_S,"find_path",tolua_gslua_Enemy_find_path00);
 tolua_function(tolua_S,"hit",tolua_gslua_Enemy_hit00);
 tolua_function(tolua_S,"shoot",tolua_gslua_Enemy_shoot00);
 tolua_function(tolua_S,"move",tolua_gslua_Enemy_move00);
 tolua_function(tolua_S,"update",tolua_gslua_Enemy_update00);
 tolua_function(tolua_S,"draw",tolua_gslua_Enemy_draw00);
 tolua_function(tolua_S,"dead",tolua_gslua_Enemy_dead00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsEnemy","gsEnemy","",NULL);
 tolua_beginmodule(tolua_S,"gsEnemy");
 tolua_function(tolua_S,"get_count",tolua_gslua_gsEnemy_get_count00);
 tolua_function(tolua_S,"get_first_enemy",tolua_gslua_gsEnemy_get_first_enemy00);
 tolua_function(tolua_S,"get_enemy",tolua_gslua_gsEnemy_get_enemy00);
 tolua_function(tolua_S,"add",tolua_gslua_gsEnemy_add00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsEnemy_remove_for_script00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"enemies",tolua_get_enemies_ptr,tolua_set_enemies_ptr);
 tolua_constant(tolua_S,"DATA_RLE",DATA_RLE);
 tolua_constant(tolua_S,"DATA_BITMAP",DATA_BITMAP);
 tolua_cclass(tolua_S,"Frame","Frame","",NULL);
 tolua_beginmodule(tolua_S,"Frame");
 tolua_function(tolua_S,"get_kill",tolua_gslua_Frame_get_kill00);
 tolua_function(tolua_S,"get_w",tolua_gslua_Frame_get_w00);
 tolua_function(tolua_S,"get_h",tolua_gslua_Frame_get_h00);
 tolua_function(tolua_S,"get_disptime",tolua_gslua_Frame_get_disptime00);
 tolua_function(tolua_S,"set_disptime",tolua_gslua_Frame_set_disptime00);
 tolua_function(tolua_S,"get_type",tolua_gslua_Frame_get_type00);
 tolua_function(tolua_S,"get_data",tolua_gslua_Frame_get_data00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Image","Image","",NULL);
 tolua_beginmodule(tolua_S,"Image");
 tolua_function(tolua_S,"get_framecount",tolua_gslua_Image_get_framecount00);
 tolua_function(tolua_S,"get_prev",tolua_gslua_Image_get_prev00);
 tolua_function(tolua_S,"get_next",tolua_gslua_Image_get_next00);
 tolua_function(tolua_S,"set_next",tolua_gslua_Image_set_next00);
 tolua_function(tolua_S,"set_prev",tolua_gslua_Image_set_prev00);
 tolua_function(tolua_S,"get_sig",tolua_gslua_Image_get_sig00);
 tolua_function(tolua_S,"get_rle_frame",tolua_gslua_Image_get_rle_frame00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"gsImage","gsImage","",NULL);
 tolua_beginmodule(tolua_S,"gsImage");
 tolua_function(tolua_S,"get_first",tolua_gslua_gsImage_get_first00);
 tolua_function(tolua_S,"get_last",tolua_gslua_gsImage_get_last00);
 tolua_function(tolua_S,"get_count",tolua_gslua_gsImage_get_count00);
 tolua_function(tolua_S,"add",tolua_gslua_gsImage_add00);
 tolua_function(tolua_S,"load",tolua_gslua_gsImage_load00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsImage_remove00);
 tolua_function(tolua_S,"remove",tolua_gslua_gsImage_remove01);
 tolua_function(tolua_S,"get_image",tolua_gslua_gsImage_get_image00);
 tolua_endmodule(tolua_S);
 tolua_variable(tolua_S,"images",tolua_get_images_ptr,tolua_set_images_ptr);
 tolua_cclass(tolua_S,"gsPanel","gsPanel","",NULL);
 tolua_beginmodule(tolua_S,"gsPanel");
 tolua_variable(tolua_S,"osd",tolua_get_gsPanel_osd_ptr,tolua_set_gsPanel_osd_ptr);
 tolua_function(tolua_S,"set_debug",tolua_gslua_gsPanel_set_debug00);
 tolua_function(tolua_S,"get_debug",tolua_gslua_gsPanel_get_debug00);
 tolua_function(tolua_S,"set_active",tolua_gslua_gsPanel_set_active00);
 tolua_function(tolua_S,"get_active",tolua_gslua_gsPanel_get_active00);
 tolua_function(tolua_S,"set_radar_zoom",tolua_gslua_gsPanel_set_radar_zoom00);
 tolua_function(tolua_S,"get_radar_zoom",tolua_gslua_gsPanel_get_radar_zoom00);
 tolua_function(tolua_S,"update_all",tolua_gslua_gsPanel_update_all00);
 tolua_function(tolua_S,"update_lifes",tolua_gslua_gsPanel_update_lifes00);
 tolua_function(tolua_S,"update_shields",tolua_gslua_gsPanel_update_shields00);
 tolua_function(tolua_S,"update_weapons",tolua_gslua_gsPanel_update_weapons00);
 tolua_function(tolua_S,"update_bullets",tolua_gslua_gsPanel_update_bullets00);
 tolua_function(tolua_S,"update_fuel",tolua_gslua_gsPanel_update_fuel00);
 tolua_function(tolua_S,"update_frags",tolua_gslua_gsPanel_update_frags00);
 tolua_function(tolua_S,"update_radar",tolua_gslua_gsPanel_update_radar00);
 tolua_function(tolua_S,"update_extra_energy",tolua_gslua_gsPanel_update_extra_energy00);
 tolua_endmodule(tolua_S);
 tolua_array(tolua_S,"gpanel",tolua_get_gslua_gpanel,tolua_set_gslua_gpanel);
 tolua_endmodule(tolua_S);
 return 1;
}
