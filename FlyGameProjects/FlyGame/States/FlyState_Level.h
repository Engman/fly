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
#include "..\..\FlyGameEngine\Core\Light\PointLight.h"
#include "..\..\FlyGameEngine\Util\Proxy.h"
#include "../../FlyGameEngine/Core/ParticleEngineSystem.h"
#include "../../FlyGameEngine/Core/ParticleCollisionSystem.h"
#include "../../FlyGameEngine/Core/ParticlePickupSystem.h"
#include "../GameObjects/PauseMenu.h"
#include "../GameObjects/FlyPickup.h"
#include "../WindCollision.h"
#include "..\GameObjects\EnergyPickup.h"



class FlyState_Level		:public IFlySystemState
{
	private:
		vector<Entity*> levelEntities;
		vector<Entity*> theWorld;
		vector<Entity*> skyBox;
		vector<Entity*> water; 
		vector<Entity*> dirLights;
		vector<Entity*> pointLights; 
		vector<Entity*> cursor;
		vector<Entity*> UIorthographic; 
		vector<Camera*> shadowViews;

		FlyPlayer player;
		FlyPickup pickups[3];
		vector<EnergyPickup> energy;

		PauseMenu pauseMenu;

		WindCollision windCollision;

		Camera mainCamera;
		Camera menuCamera;
		//Camera lightCamera;
		Timer* mainTimer;

		vec3 worldWind;
		vec3 localWind;
		vec3 playerStartPosition;

		float deathTimer;
		int lastState;

		ParticleEngineSystem engineParticlesLeft;
		ParticleEngineSystem engineParticlesRight;
		ParticleCollisionSystem collisionParticle;
		ParticlePickupSystem pickupParticle;

		int state;							// 0 = MainGame, 1 = PauseMenu
		InuptControlScheme controlScheme;	//See IFlySystemState.h
		

		
		

	private:
		vec3 SlideCollision(vec3 oldPosition, vec3 velocity, int iterations, vec3 safeLocation);

		bool ReadLevel(const wchar_t* fileName);
		
		bool Update();
		bool UpdatePlayer();
		bool Render();

		bool MenuRender();
		bool MenuUpdate();

		/* Main input function to update */
		void _Input();
		/* Help function for casual contro scheme */
		void _InputCasual();
		/* Help function for advance contro scheme */
		void _InputAdvance();
		/* Debug input function */
		void _InputDebug();

		
		bool _ImportTerrain(wifstream& file, vector<IShader*>& shaders);
		bool _ImportSkybox(wifstream& file, vector<IShader*>& shaders);
		bool _ImportWater(wifstream& file, vector<IShader*>& shaders);
		bool _ImportStatic(wifstream& file, vector<IShader*>& shaders);
		bool _ImportPickups(wifstream& file, vector<IShader*>& shaders);
		bool _ImportEnergy(wifstream& file, vector<IShader*>& shaders);
		bool _ImportLights(wifstream& file, vector<IShader*>& shaders);
		bool _ImportPlayer(wifstream& file, vector<IShader*>& shaders);

		vec4 ReadVector4(wifstream& in);
		vec3 ReadVector3(wifstream& in);
		int ReadInt(wifstream& in);
		wstring& ReadString(wifstream& in, wstring& outStr);

	public:
		FlyState_Level();
		virtual~FlyState_Level();
	
		bool Initiate(FlyGame*) override;
		void Frame() override;

		void Release() override;

		void PlayLevelSound(bool play);
};


#endif

