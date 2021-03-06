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
#include "../../FlyGameEngine/Core/ParticleSystem.h"


class FlyState_Level		:public IFlySystemState
{
	private:
		vector<Entity*> levelEntities;
		vector<Entity*> levelPickups;
		vector<Entity*> energyPickups;
		vector<Entity*> theWorld;
		vector<Entity*> skyBox;
		vector<Entity*> dirLights;
		vector<Entity*> gameMenu;
		vector<Entity*> cursor;
		vector<Entity*> UIorthographic; 
		vector<BaseBuffer*> shadowViews;
		FlyPlayer player;

		Camera mainCamera;
		Camera menuCamera;
		Timer* mainTimer;

		ParticleSystem engineParticlesLeft;
		ParticleSystem engineParticlesRight;
		ParticleSystem collisionParticle;

		int state;

		bool ReadLevel(const wchar_t* fileName);

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
	
		void Initiate(FlyGame*) override;
		void Frame() override;

		void Release() override;
};


#endif

