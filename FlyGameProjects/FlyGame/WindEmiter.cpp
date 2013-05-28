#include "WindEmiter.h"

static std::vector<WindEmiter*> WindHolder;



bool WindEmiter::initialize(char* scriptFile, ViewFrustum frustum, D3DXVECTOR3 pos)
{
	this->scriptFile = scriptFile;
	this->viewFrustum = frustum;
	this->position = pos; 
	WindHolder.push_back(this);
	//int rv = luaL_loadfile(luaState, scriptFile);
	int rv = luaL_loadfile(luaState, "test.lua");
	rv = lua_pcall(luaState, 0, 0, 0);

	return true;
}

bool WindEmiter::Update(BoundingSphere playerSphere, D3DXVECTOR3 vel)
{
	callLua_uppdateWindSpheres();
	return true;
}


void WindEmiter::callLua_uppdateWindSpheres()
{
	//function uppdateWindSpheres(spheres)
	lua_getglobal(luaState, "uppdateWindSpheres");
	lua_pushlightuserdata(luaState, (void*)&windSpheres);
}