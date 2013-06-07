#include "WindCollision.h"

/** WIND SPHERE LIB DEFINE */
int GetWindArray(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	vector<WindEmiter::WindSphere>* temp = (vector<WindEmiter::WindSphere>*)lua_touserdata(luaState, 1);
	int index = (int)lua_tointeger(luaState, 2);
	lua_pop(luaState, 2);
	luaL_argcheck(luaState, temp != NULL, 1, "`array' expected");
	lua_pushlightuserdata(luaState, (void*)&(temp->at(index))); // return value

	return 1; // number of return val
}
int SizeWindArray(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	vector<WindEmiter::WindSphere>* temp = (vector<WindEmiter::WindSphere>*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	luaL_argcheck(luaState, temp != NULL, 1, "`array' expected");
	lua_pushnumber(luaState, temp->size()); // return value

	return 1; // number of return val
}
static const struct luaL_Reg windArrayLib[] = 
{
    {"get", GetWindArray},
	{"size", SizeWindArray},
    {NULL, NULL}
};

int luaopen_windarraylib(lua_State* luaState)
{
	  luaL_newlib(luaState, windArrayLib);
	  return 1;
}
/** WIND SPHERE LIB DEFINE END*/

/** WINDSPHERE LIB DEFINE */
int GetPosition(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	WindEmiter::WindSphere* temp = (WindEmiter::WindSphere*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushlightuserdata(luaState, (void*)&(temp->sphere.center)); // return value

	return 1; // number of return val
}
int GetRadius(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	WindEmiter::WindSphere* temp = (WindEmiter::WindSphere*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushnumber(luaState, temp->sphere.radius); // return value

	return 1; // number of return val
}
int GetDirection(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	WindEmiter::WindSphere* temp = (WindEmiter::WindSphere*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushlightuserdata(luaState, (void*)&(temp->direction)); // return value

	return 1; // number of return val
}
static const struct luaL_Reg windSphereLib[] = 
{
    {"position", GetPosition},
	{"radius", GetRadius},
	{"direction", GetDirection},
    {NULL, NULL}
};
int luaopen_windspherelib(lua_State* luaState)
{
	  luaL_newlib(luaState, windSphereLib);
	  return 1;
}
 /** WINDSPHERE LIB DEFINE END */

/** ARRAY LIB DEFINE */
int GetArray(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	vector<D3DXVECTOR3>* temp = (vector<D3DXVECTOR3>*)lua_touserdata(luaState, 1);
	int index = lua_tointeger(luaState, 2);
	lua_pop(luaState, 2);
	luaL_argcheck(luaState, temp != NULL, 1, "`array' expected");
	lua_pushlightuserdata(luaState, (void*)&(temp->at(index))); // return value

	return 1; // number of return val
}
int SizeArray(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	vector<D3DXVECTOR3>* temp = (vector<D3DXVECTOR3>*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	luaL_argcheck(luaState, temp != NULL, 1, "`array' expected");
	lua_pushnumber(luaState, temp->size()); // return value

	return 1; // number of return val
}
static const struct luaL_Reg arrayLib[] = 
{
    {"get", GetArray},
	{"size", SizeArray},
    {NULL, NULL}
};

int luaopen_vectorlib(lua_State* luaState)
{
	  luaL_newlib(luaState, arrayLib);
	  return 1;
}
/** ARRAY LIB DEFINE END*/

/** VECTOR3 LIB DEFINE */
int GetX(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	D3DXVECTOR3* temp = (D3DXVECTOR3*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushnumber(luaState, temp->x); // return value

	return 1; // number of return val
}
int GetY(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	D3DXVECTOR3* temp = (D3DXVECTOR3*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushnumber(luaState, temp->y); // return value

	return 1; // number of return val
}
int GetZ(lua_State* luaState)
{
	int argc = lua_gettop(luaState);

	D3DXVECTOR3* temp = (D3DXVECTOR3*)lua_touserdata(luaState, 1);
	lua_pop(luaState, 1);
	lua_pushnumber(luaState, temp->z); // return value

	return 1; // number of return val
}
static const struct luaL_Reg vector3Lib[] = 
{
    {"x", GetX},
	{"y", GetY},
	{"z", GetZ},
    {NULL, NULL}
};
int luaopen_vector3lib(lua_State* luaState)
{
	  luaL_newlib(luaState, vector3Lib);
	  return 1;
}
 /** VECTOR3 LIB DEFINE END */

WindCollision::WindCollision()
{

}

WindCollision::~WindCollision()
{

}

void WindCollision::Initialize(lua_State* luaState)
{
	this->luaState = luaState;

	luaL_requiref(this->luaState, "array", luaopen_vectorlib, 1);
	luaL_requiref(this->luaState, "vector3", luaopen_vector3lib, 1);
	luaL_requiref(this->luaState, "windArray", luaopen_windarraylib, 1);
	luaL_requiref(this->luaState, "windSphere", luaopen_windspherelib, 1);

	lua_pop(this->luaState, 4);
}

D3DXVECTOR3 WindCollision::PlayerVSWind(D3DXVECTOR3* localWind, BoundingSphere* playerSphere, vector<WindEmiter::WindSphere>* windSpheres)
{
	int rv = luaL_loadfile(luaState, "../../FlyGameProjects/FlyGame/LuaScript/windCollision.lua");
	rv = lua_pcall(luaState, 0, 0, 0);

	lua_getglobal(luaState, "WindCollision");
	lua_pushlightuserdata(luaState, (void*)playerSphere);
	lua_pushlightuserdata(luaState, (void*)localWind);
	lua_pushlightuserdata(luaState, (void*)windSpheres);

	int argc = lua_gettop(luaState);

	rv = lua_pcall(luaState, 3, LUA_MULTRET, 0);

	argc = lua_gettop(luaState);

	D3DXVECTOR3 tempVel;

	tempVel.x = (float)lua_tonumber(luaState, 1);
	tempVel.y = (float)lua_tonumber(luaState, 2);
	tempVel.z = (float)lua_tonumber(luaState, 3);
	
	lua_pop(luaState, 3);

	return tempVel;
}