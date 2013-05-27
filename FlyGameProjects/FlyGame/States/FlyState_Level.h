#ifndef FLY_LEVEL_H
#define FLY_LEVEL_H

#include "IFlySystemState.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Util\Camera.h"
#include "..\..\FlyGameEngine\Core\Mesh\Terrain.h"
#include "..\..\FlyGameEngine\Core\Input.h"
#include "..\..\FlyGameEngine\Util\Timer.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\GameObjects\FlyPlayer.h"
#include "..\..\FlyGameEngine\Core\Light\DirectionLight.h"
#include "..\..\FlyGameEngine\Util\Proxy.h"
#include "../../FlyGameEngine/Core/ParticleEngineSystem.h"
#include "../../FlyGameEngine/Core/ParticleCollisionSystem.h"
#include "../../FlyGameEngine/Core/ParticlePickupSystem.h"
#include "../GameObjects/PauseMenu.h"
#include "../GameObjects/FlyPickup.h"
#include "../WindCollision.h"


class FlyState_Level		:public IFlySystemState
{
	private:
		vector<Entity*> levelEntities;
		vector<Entity*> energyPickups;
		vector<Entity*> theWorld;
		vector<Entity*> skyBox;
		vector<Entity*> dirLights;
		vector<Entity*> cursor;
		vector<Entity*> UIorthographic; 
		vector<BaseBuffer*> shadowViews;

		FlyPlayer player;
		FlyPickup pickups[3];

		PauseMenu pauseMenu;

		WindCollision windCollision;

		Camera mainCamera;
		Camera menuCamera;
		Camera lightCamera;
		Timer* mainTimer;

		vec3 worldWind;
		vec3 localWind;

		ParticleEngineSystem engineParticlesLeft;
		ParticleEngineSystem engineParticlesRight;
		ParticleCollisionSystem collisionParticle;
		ParticlePickupSystem pickupParticle;

		int state; // 0 = MainGame, 1 = PauseMenu

		bool ReadLevel(const wchar_t* fileName);

		//void ReadEntity(Entity* entity, wifstream& in);
		vec4 ReadVector4(wifstream& in);
		vec3 ReadVector3(wifstream& in);
		int ReadInt(wifstream& in);

		vec3 SlideCollision(vec3 oldPosition, vec3 velocity, int iterations, vec3 safeLocation);


		bool Update();
		bool UpdatePlayer();
		bool Render();

		bool MenuRender();
		bool MenuUpdate();

		void Input();
		
	public:
		FlyState_Level();
		virtual~FlyState_Level();
	
		bool Initiate(FlyGame*) override;
		void Frame() override;

		void Release() override;
};


#endif

