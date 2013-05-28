#ifndef WINDEMITER_H
#define WINDEMITER_H

#include "../FlyGameEngine/Util/misc.h"
#include "../FlyGameEngine/Util/SmartPtrs.h"
#include "../FlyGameEngine/Util/BoundingVolumes.h"
#include "../FlyGameEngine/Util/CollisionLib.h"
#include <vector>

extern "C"
{
#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>
}
class WindEmiter
{
public: 
	struct WindSphere
	{
		BoundingSphere sphere;
		D3DXVECTOR3 direction; 
		WindSphere() { };
	};

private:

	
	lua_State* luaState;
	ViewFrustum viewFrustum;
	D3DXVECTOR3 position; 
	SmartPtrStd<std::vector<WindSphere>> windSpheres;
	int spherePerSec; 
	int 

	char* scriptFile;
	void callLua_uppdateWindSpheres();

public:
	WindEmiter();
	~WindEmiter();

	bool initialize(char* scriptFile, ViewFrustum frustum, D3DXVECTOR3 pos);
	bool Update(BoundingSphere playerSphere, D3DXVECTOR3 vel);


};
#endif