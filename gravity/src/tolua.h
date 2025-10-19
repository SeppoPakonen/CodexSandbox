/* 
 * Minimal tolua stub for Gravity Strike compilation
 * This is a placeholder to allow compilation without the full tolua library
 * The full functionality will require the actual tolua++ library
 */

#ifndef tolua_h
#define tolua_h

// Define TOLUA_API if not already defined
#ifndef TOLUA_API
#define TOLUA_API LUA_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Basic tolua types and function declarations
typedef struct tolua_Error {
    int index;
    int array;
    const char* type;
} tolua_Error;

typedef struct tolua_cast {
    int eq; // type cast
    int arith; // arithmetic operation
    int compare; // comparison operation
} tolua_cast;

// Basic tolua function declarations
int tolua_open(lua_State* tolua_S);
void tolua_error(lua_State* tolua_S, const char* msg, tolua_Error* err);
int tolua_isnumber(lua_State* tolua_S, int lo, int def, tolua_Error* err);
int tolua_isstring(lua_State* tolua_S, int lo, int def, tolua_Error* err);
int tolua_istable(lua_State* tolua_S, int lo, int def, tolua_Error* err);
int tolua_isusertype(lua_State* tolua_S, int lo, const char* type, int def, tolua_Error* err);
int tolua_isnoobj(lua_State* tolua_S, int lo, tolua_Error* err);

void tolua_usertype(lua_State* tolua_S, const char* type);
void tolua_beginmodule(lua_State* tolua_S, const char* name);
void tolua_endmodule(lua_State* tolua_S);
void tolua_function(lua_State* tolua_S, const char* name, void* func);
void tolua_variable(lua_State* tolua_S, const char* name, int (*getter)(lua_State*), int (*setter)(lua_State*));
void tolua_constant(lua_State* tolua_S, const char* name, double value);

// Additional tolua function declarations needed by the generated code
void* tolua_tousertype(lua_State* tolua_S, int narg, int def);
void tolua_pushusertype(lua_State* tolua_S, void* ud, const char* type);
int tolua_pushnumber(lua_State* tolua_S, double num);
int tolua_pushstring(lua_State* tolua_S, const char* str);
int tolua_pushboolean(lua_State* tolua_S, int boolean);
int tolua_tonumber(lua_State* tolua_S, int narg, double def);
const char* tolua_tostring(lua_State* tolua_S, int narg, const char* def);
int tolua_toboolean(lua_State* tolua_S, int narg, int def);
int tolua_isboolean(lua_State* tolua_S, int lo, int def, tolua_Error* err);
int tolua_pushuserdata(lua_State* tolua_S, void* ud);
void tolua_myerror(lua_State* tolua_S, const char* msg, tolua_Error* err);

// Additional functions that are used in the codebase
int tolua_isusertable(lua_State* tolua_S, int lo, const char* type, int def, tolua_Error* err);
void tolua_module(lua_State* tolua_S, const char* name, int hasvar);
void tolua_array(lua_State* tolua_S, const char* name, void* getter, void* setter);
void tolua_cclass(lua_State* tolua_S, const char* lname, const char* name, const char* base, void* collector);
const char* tolua_tag(lua_State* tolua_S, const char* name);

// Define tolua++ macros
#define tolua_cast(x, y) (x)
#define tolua_len(x) lua_objlen(x)

#ifdef __cplusplus
}
#endif

#endif /* tolua_h */