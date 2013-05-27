#ifndef WINDCOLLISION_H
#define WINDCOLLISION_H

#include "..\FlyGameEngine\Util\BoundingVolumes.h"
#include <vector>
#include "WindEmiter.h"

using namespace std;

extern "C"
{
	#include <Lua/lua.h>
	#include <Lua/lualib.h>
	#include <Lua/lauxlib.h>
}

class WindCollision
{
	private:
		lua_State* luaState;

	public:
		WindCollision();
		virtual ~WindCollision();

		void Initialize(lua_State* luaState);

		D3DXVECTOR3 PlayerVSWind(D3DXVECTOR3* localWind, BoundingSphere* playerSphere, vector<WindEmiter::WindSphere>* windSpheres);
};

#endif