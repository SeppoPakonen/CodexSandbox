#ifndef Lua_header
#define Lua_header

#include <string>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

class Lua
{
private:
	lua_State * state;
	bool closeOnDestruct;
	
public:
        lua_State *get_state() { return state; }
	void open(int stacksize = 0)
	{ 
		if(state) {
			close();
		}
		
		state = luaL_newstate(); 
	}
	
	void close(void)
	{ 
		if(state) {
			lua_close(state); 
			state = 0;
		}
	}
	
	Lua(int stacksize=0, bool c=true) : state(0), closeOnDestruct(c) 
	{
		open(stacksize); 
	}
	 
	Lua(lua_State * s, bool c=false) : state(s), closeOnDestruct(c) {}
	
	~Lua()
	{
		if(closeOnDestruct) {
			close();
		}
	}

	int gettop(void) { return lua_gettop(state); }
	void settop(int index) { lua_settop(state, index); }
	void pushvalue(int index) { lua_pushvalue(state, index); }
	void remove(int index) { lua_remove(state, index); }
	void insert(int index) { lua_insert(state, index); }
	void replace(int index) { lua_replace(state, index); }

	int lua_type(int index) { return ::lua_type(state, index); }
	const char * lua_typename(int t) { return ::lua_typename(state, t); }
	bool isnumber(int index) { return lua_isnumber(state, index)!=0; }
	bool isstring(int index) { return lua_isstring(state, index)!=0; }
	bool iscfunction(int i) { return lua_iscfunction(state, i)!=0; }
				
	bool equal(int i1, int i2) { return lua_compare(state, i1, i2, LUA_OPEQ)!=0; }
	bool lessthan(int i1, int i2) { return lua_compare(state, i1, i2, LUA_OPLT)!=0; }
	
	double tonumber(int index) { return lua_tonumber(state, index); }
	const char * tostring(int i) { return lua_tostring(state, i); }
	size_t strlen(int index) { return lua_rawlen(state, index); }
	lua_CFunction tocfunction(int i) { return lua_tocfunction(state, i); }

	void * touserdata(int i) { return lua_touserdata(state, i);	}
	const void * topointer(int i) { return lua_topointer(state, i); }
	
/*
** push functions (C -> stack)
*/

	void pushnil(void) { lua_pushnil(state); }
	void pushnumber(double n) { lua_pushnumber(state, n); }
	void pushlstring(const char * s, size_t len) 
	{ 
		lua_pushlstring(state, s, len);
	}

	void pushstring(const char * s) { lua_pushstring(state, s); }
	void pushcclosure(lua_CFunction fn, int n)
	{
		lua_pushcclosure(state, fn, n);
	}


/*
** get functions (Lua -> stack)
*/

	void getglobal(const char * name) { lua_getglobal(state, name); }
	void gettable(int index) { lua_gettable(state, index); }
	void rawget(int index) { lua_rawget(state, index); }
	void rawgeti(int index, int n) { lua_rawgeti(state, index, n); }
	void newtable(void) { lua_newtable(state); }
								
/*
** set functions (stack -> Lua)
*/
	void setglobal(const char * name) { lua_setglobal(state, name); }
	void settable(int index) { lua_settable(state, index); }
	void rawset(int index) { lua_rawset(state, index); }
	void rawseti(int index, int n) { lua_rawseti(state, index, n); }

	void getfenv(int index) { lua_getupvalue(state, index, 1); }
	int setfenv(int index) { return lua_setupvalue(state, index, 1) != NULL; }

/*
** "do" functions (run Lua code)
*/

	int call(int nargs, int nresults) 
	{ 
		lua_call(state, nargs, nresults);
    return 0;
	}

	int dofile(const char * filename) { return luaL_dofile(state, filename); }
	int dostring(const char * str) { return luaL_dostring(state, str); }
	int dobuffer(const char *buff, size_t size, const char * name)
	{
		return luaL_loadbuffer(state, buff, size, name) || lua_pcall(state, 0, LUA_MULTRET, 0);
	}	

/*
** Garbage-collection functions
*/
	int getgcthreshold(void) { return 0; /* Deprecated in Lua 5.1+ */ }
	int getgccount(void) { return 0; /* Deprecated in Lua 5.1+ */ }
	void setgcthreshold(int newthreshold)
	{
		/* Deprecated in Lua 5.1+, use lua_gc instead */
		lua_gc(state, LUA_GCCOLLECT, 0);
	}		

/*
** miscellaneous functions
*/
	void unref(int ref) { luaL_unref(state, LUA_REGISTRYINDEX, ref); }
		
	int next(int index) { return lua_next(state, index); }
	void concat(int n) { lua_concat(state, n); }

	void * newuserdata(size_t size) { return lua_newuserdata(state, size); }
	
/* 
** ===============================================================
** some useful macros
** ===============================================================
*/

//#define lua_pop(L,n)		lua_settop(L, -(n)-1)
	void pop(int n) { settop(-n-1); }

//#define lua_register(L,n,f)	(lua_pushcfunction(L, f), lua_setglobal(L, n))
	void registerfunction(const char * name, lua_CFunction fn)
	{
		pushcfunction(fn);
		setglobal(name);
	}
	
//#define lua_pushuserdata(L,u)	lua_pushusertag(L, u, 0)
	
//#define lua_pushcfunction(L,f)	lua_pushcclosure(L, f, 0)
	void pushcfunction(lua_CFunction fn)
	{
		pushcclosure(fn, 0);
	}
	
//#define lua_clonetag(L,t)	lua_copytagmethods(L, lua_newtag(L), (t))

//#define lua_isfunction(L,n)	(lua_type(L,n) == LUA_TFUNCTION)
	bool isfunction(int index) 
	{
		return lua_type(index) == LUA_TFUNCTION;
	}
	
	
//#define lua_istable(L,n)	(lua_type(L,n) == LUA_TTABLE)
	bool istable(int index) 
	{
		return lua_type(index) == LUA_TTABLE;
	}
	
//#define lua_isuserdata(L,n)	(lua_type(L,n) == LUA_TUSERDATA)
	bool isuserdata(int index) 
	{
		return lua_type(index) == LUA_TUSERDATA;
	}

//#define lua_isnil(L,n)		(lua_type(L,n) == LUA_TNIL)
	bool isnil(int index)
	{
		return lua_type(index) == LUA_TNIL;
	}
	
//#define lua_isnull(L,n)		(lua_type(L,n) == LUA_TNONE)
	bool isnull(int index)
	{
		return lua_type(index) == LUA_TNONE;
	}

//#define lua_getregistry(L)	lua_getref(L, LUA_REFREGISTRY)

// standard API ends

// lib functions
	void open_base(void) { luaL_requiref(state, "_G", luaopen_base, 1); lua_pop(state, 1); }
	void open_io(void) { luaL_requiref(state, LUA_IOLIBNAME, luaopen_io, 1); lua_pop(state, 1); }
	void open_string(void) { luaL_requiref(state, LUA_STRLIBNAME, luaopen_string, 1); lua_pop(state, 1); }
	void open_math(void) { luaL_requiref(state, LUA_MATHLIBNAME, luaopen_math, 1); lua_pop(state, 1); }
	void open_table(void) { luaL_requiref(state, LUA_TABLIBNAME, luaopen_table, 1); lua_pop(state, 1); }
	void open_debug(void) { luaL_requiref(state, LUA_DBLIBNAME, luaopen_debug, 1); lua_pop(state, 1); }
		
};

#endif
