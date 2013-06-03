#include "FlyState_Level.h"
#include "..\FlyGame.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Util\CollisionLib.h"
#include "..\..\FlyGameEngine\Core\Light\DirectionLight.h"
#include "..\..\FlyGameEngine\Core\Light\PointLight.h"
#include "..\..\FlyGameEngine\Util\Proxy.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyWaterMesh.h"


#include <fstream>
#include <iostream>
#include <string>

FlyState_Level::FlyState_Level()
{ }
FlyState_Level::~FlyState_Level()
{
	
}


bool FlyState_Level::Initiate(FlyGame* instance)
{
	this->state = 0;
	this->lastState = 0; 

	this->controlScheme = CONTROL_Debug;
	//this->controlScheme = CONTROL_Casual;

	this->entryInstance = instance;


	//Read level data
	if(!this->ReadLevel(this->entryInstance->getLevel()))
		return false;
	//Read level data
	//if(!this->ReadLevel(L"..\\Resources\\Levels\\testLinda2.fgl"))
	//	return false;
	

	//If the lvl was not completed you will continue with the cargo you already picked up
	if(!this->entryInstance->isLvlCompleted())
	{	
		std::vector<bool> taken = this->entryInstance->getLvlSavedData(); 

		for(int i =0; i<CARGO_COUNT; i++)
		{
			this->pickups[i].SetPickTaken((bool)taken[i]);  
		}

	}
	//Timer activation
	this->mainTimer = new Timer();
	if(!this->mainTimer->Initialize())
		return false;

	//Camera setup
	int w, h;
	entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
	this->menuCamera.SetPosition(0.0f, 0.0f, -3.0f);
	this->menuCamera.SetOrthogonalMatrix((float)w, (float)h, 0.1f, 10.0f);
	this->mainCamera.SetOrthogonalMatrix((float)w, (float)h, 0.1f, 10.0f);
	this->mainCamera.Render();
	this->menuCamera.Render();


	//Particle effects
	if(!this->engineParticlesLeft.Initialize())		return false;
	if(!this->engineParticlesRight.Initialize())	return false;
	if(!this->collisionParticle.Initialize())		return false;
	if(!this->pickupParticle.Initialize())			return false;
	

	//Menu when paused
	if(!this->pauseMenu.Initialize(this->entryInstance, (float)h, (float)w))
		return false;

	//Wind vairables
	this->worldWind = vec3(0.0f, 0.0f, 0.0f);
	this->localWind = vec3(0.0f, 0.0f, 0.0f); 

	//Wind collision with lua
	this->windCollision.Initialize(this->entryInstance->GetLuaState());

	//Audio initialization
	const char* path = "..\\Resources\\Sound\\level_background.mp3";	
	this->entryInstance->GetCoreInstance()->Audio_LoadLevelSound(path);
	this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_LevelSoundTrack);
	this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wind);

	return true;
}
void FlyState_Level::Frame()
{
	this->mainTimer->Frame();

	
	if(this->lastState != this->state)
	{
		this->entryInstance->GetCoreInstance()->Audio_ToggleSoundTrack(); 
	}

	this->lastState = this->state;

	if(this->state == 0)
	{
		this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);

		this->Update();
		this->Render();
		 
	}
	else if(this->state == 1)
	{
		this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->menuCamera);

		this->MenuUpdate();
		this->MenuRender();
	}
	
}

bool FlyState_Level::Update()
{
	//Moves and collides player against the world/tilts player model accordingly
	this->UpdatePlayer();

	player.setCloseCargo(false);
	//Collision against pickups
	for(unsigned int i = 0; i < 3; i++)
	{
		if(!this->pickups[i].GetTaken())
		{
			this->pickups[i].Update(this->mainTimer->GetDeltaTime());
			D3DXVECTOR3 dir;
			dir = player.GetPosition() - pickups[i].GetPosition();
			float distance = D3DXVec3Length(&dir); 
			if(distance<2000)
				player.setCloseCargo(true);
			if(SphereVSSphere(*this->pickups[i].GetBoundingSphere(), *this->player.GetBoundingSphere()))
			{
				this->pickupParticle.SetPosition(this->pickups[i].GetPosition());
				this->pickupParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f,vec3(0.0f, 0.0f, 1.0f));
				this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_CargoPickup);
				this->pickups[i].SetPickTaken(true);

				//Save data to file
				std::vector<bool> taken = this->entryInstance->getLvlSavedData(); 
				if(taken[i] != 1)
					this->entryInstance->setLvlSaveData(i); 
			}
		}

	}
	//Collision against energy
	for(unsigned int i = 0; i < this->energy.size(); i++)
	{
		if(!this->energy[i].GetTaken() )
		{
			this->energy[i].Update(this->mainTimer->GetDeltaTime());
			if(SphereVSSphere(*this->energy[i].GetBoundingSphere(), *this->player.GetBoundingSphere()))
			{
				this->pickupParticle.SetPosition(this->energy[i].GetPosition());
				this->pickupParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f,vec3(0.0f, 0.0f, 1.0f));
				this->energy[i].SetPickTaken(true);
				this->player.DeductEnergy(-1000);
				this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_EnergyPickup);
			}
		}
	}
	
	//Check if all energy is lost
	if(this->player.GetEnergy() < 0.0f)
	{
		//this->state = 1; 
		//return true; 
		this->player.DeductEnergy(-10000.0f);
	}
	
	//Update skybox position
	this->skyBox[0]->setPosition(this->player.GetPosition());
	this->skyBox[0]->Update();

	//Particle updates
	this->collisionParticle.Frame(this->player.GetPosition(), this->mainTimer->GetDeltaTime()*10.0f);
	this->collisionParticle.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
	this->pickupParticle.Frame(this->player.GetPosition(), this->mainTimer->GetDeltaTime()*5.0f);
	this->pickupParticle.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
	
	//this->entryInstance->GetCoreInstance()->Audio_PlaySound(); volume needs to be calculated from current velocity this->player.GetVelocity().z/this->player.GetMaxVelocity().z;	

	
	this->player.GetModel()->at(0)->Update();
	
	float en = (this->player.GetEnergy()/this->player.GetMaxEnergy())*20.0f;
	this->UIorthographic[0]->setScale(vec3(en,1.0f,1.0f));
	this->UIorthographic[0]->Update();

	//play a warning sound when the energy is low
	if(this->player.GetEnergy()<1000)
	{
		this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_LowEnergy);
	}

	float playerVel = this->player.GetVelocity().z/this->player.GetMaxVelocity().z;
	this->entryInstance->GetCoreInstance()->Audio_Update(this->player.GetPosition(),playerVel);

	((FlyWaterMesh*)this->water[0])->UpdateWater(this->mainTimer->GetDeltaTime());

	return true;
}
bool FlyState_Level::UpdatePlayer()
{
	vec3 oldPosition = this->player.GetPosition();
	float upsideDown = 1.0f;

	if(this->mainCamera.GetUp().y < 0.0f)
	{
		upsideDown = -1.0f;
	}

	//Move camera and bird
	this->mainCamera.SetPosition(oldPosition);
	this->_Input();

	float downVelocity = D3DXVec3Dot(&this->mainCamera.GetForward(), &vec3(0.0f, 1.0f, 0.0f));

	if(downVelocity < 0.0f && downVelocity > -1.0f && this->player.GetVelocity().z < this->player.GetMaxVelocity().z+1.0f*-downVelocity)
	{
		this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.004f));
	}
	else if(downVelocity > 0.0f && downVelocity < 1.0f && this->player.GetVelocity().z > -this->player.GetMaxVelocity().z-1.0f*downVelocity)
	{
		this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.004f));
	}

	////Collision against world wind walls
	//Terrain* tempTerrain = dynamic_cast<Terrain*>(this->theWorld[0]);

	//if(this->mainCamera.GetPosition().x <= tempTerrain->GetBoundingBox().minPoint.x+2.0f)
	//{
	//	this->worldWind.x += 0.032f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else if(this->mainCamera.GetPosition().x >= tempTerrain->GetBoundingBox().maxPoint.x-2.0f)
	//{
	//	this->worldWind.x -= 0.032f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	if(this->worldWind.x < 0.0f)
	//	{
	//		this->worldWind.x += 0.032f;
	//	}
	//	else if(this->worldWind.x > 0.0f)
	//	{
	//		this->worldWind.x -= 0.032f;
	//	}
	//}
	//if(this->mainCamera.GetPosition().z <= tempTerrain->GetBoundingBox().minPoint.z+2.0f)
	//{
	//	this->worldWind.z += 0.064f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else if(this->mainCamera.GetPosition().z >= tempTerrain->GetBoundingBox().maxPoint.z-2.0f)
	//{
	//	this->worldWind.z -= 0.032f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	if(this->worldWind.z < 0.0f)
	//	{
	//		this->worldWind.z += 0.032f;
	//	}
	//	else if(this->worldWind.z > 0.0f)
	//	{
	//		this->worldWind.z -= 0.032f;
	//	}
	//}
	//if(this->mainCamera.GetPosition().y <= tempTerrain->GetBoundingBox().minPoint.y+2.0f)
	//{
	//	this->worldWind.y += 0.032f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else if(this->mainCamera.GetPosition().y >= tempTerrain->GetBoundingBox().maxPoint.y-2.0f)
	//{
	//	this->worldWind.y -= 0.032f;
	//	if(this->player.GetVelocity().z < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().z > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.032f));
	//	if(this->player.GetVelocity().x < 0.0f) this->player.SetVelocity(this->player.GetVelocity()+vec3(0.032f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().x > 0.0f) this->player.SetVelocity(this->player.GetVelocity()-vec3(0.032f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	if(this->worldWind.y < 0.0f)
	//	{
	//		this->worldWind.y += 0.032f;
	//	}
	//	else if(this->worldWind.y > 0.0f)
	//	{
	//		this->worldWind.y -= 0.032f;
	//	}
	//}

	//Wind collision
	//this->localWind = this->windCollision.PlayerVSWind(&this->localWind, this->player.GetBoundingSphere(), spheres);
	//this->mainCamera.SetPosition(this->mainCamera.GetPosition()+this->localWind);
	//this->mainCamera.SetPosition(this->mainCamera.GetPosition()+this->worldWind);

	//Move camera according to player velocities and add gravity
	this->mainCamera.RelativeForward(this->player.GetVelocity().z);
	this->mainCamera.RelativeRight(this->player.GetVelocity().x);
	this->mainCamera.RelativePitch(0.8f*this->player.GetVelocity().y*this->player.GetVelocity().z*100.0f);
	//this->mainCamera.SetPositionY(this->mainCamera.GetPosition().y-0.4f*fabs(downVelocity));
	this->mainCamera.RelativeYaw(this->player.GetVelocity().x*6.0f*this->mainCamera.GetUp().y);

	this->player.SetPosition(this->mainCamera.GetPosition());
	this->player.GetBoundingSphere()->center = this->player.GetPosition();

	
	//Check for slide collision for bird's location
	D3DXVECTOR3 velocity = this->player.GetPosition() - oldPosition;
	vec3 newPlayerPosition = SlideCollision(vec3(oldPosition.x/this->player.GetEllipse().radiusVector.x, oldPosition.y/this->player.GetEllipse().radiusVector.y, oldPosition.z/this->player.GetEllipse().radiusVector.z), vec3(velocity.x/this->player.GetEllipse().radiusVector.x, velocity.y/this->player.GetEllipse().radiusVector.y, velocity.z/this->player.GetEllipse().radiusVector.z), 200, vec3(oldPosition.x/this->player.GetEllipse().radiusVector.x, oldPosition.y/this->player.GetEllipse().radiusVector.y, oldPosition.z/this->player.GetEllipse().radiusVector.z));
	this->player.SetPosition(vec3(newPlayerPosition.x*this->player.GetEllipse().radiusVector.x, newPlayerPosition.y*this->player.GetEllipse().radiusVector.y, newPlayerPosition.z*this->player.GetEllipse().radiusVector.z));

	this->player.GetBoundingSphere()->center = this->player.GetPosition();
	this->mainCamera.SetPosition(this->player.GetPosition());

	//Position camera behind player
	this->mainCamera.Render();
	vec3 newCameraPos = this->player.GetPosition() - this->mainCamera.GetForward()*10.0f + this->mainCamera.GetUp()*1.5f;
	this->mainCamera.SetPosition(newCameraPos.x, newCameraPos.y, newCameraPos.z);

	

	//Tilt the player model according to sideways velocity
	if(this->player.GetVelocity().x < -0.001f)
	{
		float rX = this->mainCamera.GetForward().x;
		float rY = this->mainCamera.GetForward().y*this->player.GetVelocity().x*-2.5f;
		float rZ = D3DXVec3Length(&vec3(this->mainCamera.GetForward().x, 0.0f, this->mainCamera.GetForward().z))*upsideDown*this->player.GetVelocity().x*-2.5f;

		float howMuch = this->player.GetVelocity().x*-5.0f;
		this->player.SetRotation(vec3((float)D3DX_PI*0.25f*rZ, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f)+(float)D3DX_PI*0.25f*rY, -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));

		this->engineParticlesLeft.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		Matrix engineRotation;
		D3DXMatrixRotationYawPitchRoll(&engineRotation, howMuch*this->mainCamera.GetForward().y*0.4f, howMuch*this->mainCamera.GetForward().x*0.4f, howMuch*this->mainCamera.GetForward().z*0.4f);
		vec4 engineFour(this->mainCamera.GetRight(), 0.0f);
		vec3 engineTransform = *D3DXVec4Transform(&engineFour, &engineFour, &engineRotation);
		this->engineParticlesLeft.SetPosition(this->player.GetPosition() - engineTransform*1.3f);
		this->engineParticlesRight.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		this->engineParticlesRight.SetPosition(this->player.GetPosition() + engineTransform*1.3f);
	}
	else if(this->player.GetVelocity().x > 0.001f)
	{
		float rX = this->mainCamera.GetForward().x;
		float rY = this->mainCamera.GetForward().y*this->player.GetVelocity().x*2.5f;
		float rZ = D3DXVec3Length(&vec3(this->mainCamera.GetForward().x, 0.0f, this->mainCamera.GetForward().z))*upsideDown*this->player.GetVelocity().x*2.5f;

		float howMuch = this->player.GetVelocity().x*-5.0f;
		this->player.SetRotation(vec3((float)-D3DX_PI*0.25f*rZ, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f)-(float)D3DX_PI*0.25f*rY, -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));

		this->engineParticlesLeft.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		Matrix engineRotation;
		D3DXMatrixRotationYawPitchRoll(&engineRotation, howMuch*this->mainCamera.GetForward().y*0.4f, howMuch*this->mainCamera.GetForward().x*0.4f, howMuch*this->mainCamera.GetForward().z*0.4f);
		vec4 engineFour(this->mainCamera.GetRight(), 0.0f);
		vec3 engineTransform = *D3DXVec4Transform(&engineFour, &engineFour, &engineRotation);
		this->engineParticlesLeft.SetPosition(this->player.GetPosition() - engineTransform*1.3f);
		this->engineParticlesRight.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		this->engineParticlesRight.SetPosition(this->player.GetPosition() + engineTransform*1.3f);
	}
	else
	{
		this->player.SetRotation(vec3(0.0f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
		this->engineParticlesLeft.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		this->engineParticlesLeft.SetPosition(this->player.GetPosition() - this->mainCamera.GetRight()*1.3f);
		this->engineParticlesRight.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		this->engineParticlesRight.SetPosition(this->player.GetPosition() + this->mainCamera.GetRight()*1.3f);
	}
	//END

	if(!Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		//Engines
		if(Input::self()->IsButtonPressed(DIK_W))
		{
			this->engineParticlesLeft.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, this->player.GetPosition() - oldPosition);
			this->engineParticlesRight.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, this->player.GetPosition() - oldPosition);
			if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
				this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
			this->player.DeductEnergy(5);
			this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Thrust);
		}
		if(Input::self()->IsButtonPressed(DIK_S))
		{
			this->engineParticlesLeft.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, this->player.GetPosition() - oldPosition);
			this->engineParticlesRight.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, this->player.GetPosition() - oldPosition);
			if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
				this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.001f));
			this->player.DeductEnergy(5);
			this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Thrust);
		}
	}

	this->engineParticlesLeft.Frame(this->player.GetPosition() - oldPosition, this->mainTimer->GetDeltaTime());
	this->engineParticlesRight.Frame(this->player.GetPosition() - oldPosition, this->mainTimer->GetDeltaTime());
	//Engines end
	

	this->player.Update();


	return true;
}


bool FlyState_Level::Render()
{
	this->entryInstance->GetCoreInstance()->Gfx_Update();

	vector<IShader*> shaders;
	this->entryInstance->GetCoreInstance()->Gfx_GetShader(shaders);
	
	ViewFrustum f;
	this->mainCamera.ConstructViewFrustum(f);
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);

	
	this->theWorld[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->theWorld[0]->Render(f);
	this->skyBox[0]->Render(f);
	this->water[0]->Render(f);
	this->player.GetModel()->at(0)->setShader(shaders[FlyShader_gBufferNoCull]);
	this->player.Render(f);

	this->engineParticlesLeft.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->engineParticlesLeft.Render(f);
	this->engineParticlesRight.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->engineParticlesRight.Render(f);
	this->collisionParticle.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->collisionParticle.Render(f);
	this->pickupParticle.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->pickupParticle.Render(f);

	for(unsigned int i = 0; i < this->levelEntities.size(); i++)
	{
		this->levelEntities[i]->setShader(shaders[FlyShader_gBufferDefault]);
		this->levelEntities[i]->Render(f);
	}
	for(unsigned int i = 0; i < 3; i++)
	{
		if(!this->pickups[i].GetTaken())
		{
			this->pickups[i].SetShader(shaders[FlyShader_gBufferNoCull]);
			this->pickups[i].Render(f);
		}
	}
	for(unsigned int i = 0; i < this->energy.size(); i++)
	{
		if(!this->energy[i].GetTaken())
		{
			this->energy[i].SetShader(shaders[FlyShader_gBufferNoCull]);
			this->energy[i].Render(f);
		}
	}

	for(unsigned int i = 0; i < this->dirLights.size(); i++)
	{
		this->dirLights[i]->Render(f);
	}
	for(unsigned int i = 0; i< this->pointLights.size(); i++)
	{
		D3DXVECTOR3 cameraPos = this->mainCamera.GetPosition(); 
		D3DXVECTOR3 lightPos = this->pointLights[i]->getPosition(); 
		D3DXVECTOR3 camToLight = cameraPos - lightPos; 
		float distance = D3DXVec3Length(&camToLight); 
		if(distance<((PointLight*)this->pointLights[i])->getRadie())
		{
 			this->pointLights[i]->setShader(shaders[FlyShader_PointLightFront]);	
		}
		else
		{	
			this->pointLights[i]->setShader(shaders[FlyShader_PointLightBack]);
		}
		this->pointLights[i]->Render(f);
	}

	for(unsigned int i = 0; i <(int) this->shadowViews.size(); i++)
	{
		//cull objects from the shadowCamera view
		Camera ShadowCamera;
		ShadowCamera.SetPosition(500.0, 200.0, 0.0);
		ShadowCamera.SetViewMatrix(shadowViews[0]->lView);
		ShadowCamera.SetProjectionMatrix(shadowViews[0]->lProj);
		ShadowCamera.SetProjectionMatrix(1200.0f, 600.0f, 0.1f, 1000.0f);
		ShadowCamera.Render();
		
		ShadowCamera.ConstructViewFrustum(f);

		for(unsigned int i = 0; i <(int) this->levelEntities.size(); i++)
		{	
			this->levelEntities[i]->setShader(shaders[FlyShader_Shadow]);
			this->levelEntities[i]->Render(f);
		}

		this->theWorld[0]->setShader(shaders[FlyShader_Shadow]);
		this->theWorld[0]->Render(f);
		this->player.GetModel()->at(0)->setShader(shaders[FlyShader_Shadow]);
		this->player.Render(f);
	}

	bool glowOn = player.getCloseCargo(); 

	this->entryInstance->GetCoreInstance()->Gfx_DrawSkyBox();
	
	this->entryInstance->GetCoreInstance()->Gfx_DrawGbuffer(((FlyWaterMesh*)this->water[0])->getWaterBuffer(), glowOn);
	
	//switch back to player camera view
	this->menuCamera.ConstructViewFrustum(f);
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->menuCamera);

	for(int i =0; i<(int)UIorthographic.size(); i++ )
	{
		this->UIorthographic[i]->Render(f);
	}
	this->entryInstance->GetCoreInstance()->Gfx_DrawGbufferOrtho();
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);

	this->entryInstance->GetCoreInstance()->Gfx_DrawLighting();
	this->entryInstance->GetCoreInstance()->Gfx_DrawBlur();
	this->entryInstance->GetCoreInstance()->Gfx_DrawShadows(&shadowViews);
	this->entryInstance->GetCoreInstance()->Gfx_DrawFinalPicture(&shadowViews);
	

	return true;
}

bool FlyState_Level::MenuRender()
{

	this->entryInstance->GetCoreInstance()->Gfx_Update();
			
	this->entryInstance->GetCoreInstance()->Gfx_BeginDeferredScene();

	ViewFrustum f;
	this->menuCamera.ConstructViewFrustum(f);
	this->pauseMenu.Render(f, this->pickups[0].GetTaken(), this->pickups[1].GetTaken(), this->pickups[2].GetTaken());
	this->cursor[0]->Render(f);

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredSceneOrtho();

	return true;
}
bool FlyState_Level::MenuUpdate()
{
	int mouseX, mouseY;

	Input::self()->Frame();

	Input::self()->GetMouseLocation(mouseX, mouseY);

	this->cursor[0]->setPosition(vec3(mouseX-600.0f, -(mouseY-300.0f) , 0.0f));
	this->cursor[0]->Update();

	switch(this->pauseMenu.Update(mouseX, mouseY, this->entryInstance))
	{
		case 1:
			this->state = 0;
		break;

		case 2:
			this->entryInstance->setState(Menu);
		break;

		default:

		break;
	}

	return true;
}

	
vec3 FlyState_Level::SlideCollision(vec3 oldPosition, vec3 velocity, int iterations, vec3 safeLocation)
{
	float maxSpeed = 0.3f;
	float speed = 0.3f;

	if(iterations == 0)
	{
		return safeLocation;
	}

	if (D3DXVec3Length(&velocity) <= D3DXVec3Length(&this->player.GetVelocity())*0.005f)
	{
		return oldPosition;
	}

	D3DXVECTOR3 destinationPoint = oldPosition + velocity;

	//Slide along plane of a collided triangle
	Entity* tempEntity;
	vector<D3DXVECTOR3> triangleList;

	bool foundCollision = false;
	bool stuck = false;
	float nearestDistance = -1.0;
	D3DXVECTOR3 nearestCollider[3];
	D3DXVECTOR3 nearestSphereIntersectionPoint;
	D3DXVECTOR3 nearestPolygonIntersectionPoint;
	D3DXMATRIX nearestPolygonTransform;
	D3DXMatrixIdentity(&nearestPolygonTransform);

	for(unsigned int u = 0; u < this->levelEntities.size(); u++)
	{
		D3DXMATRIX rotation, translation, scale, world, invWorld;
		D3DXMatrixRotationYawPitchRoll(&rotation, this->levelEntities[u]->getRotation().y, this->levelEntities[u]->getRotation().x, this->levelEntities[u]->getRotation().z);
		D3DXMatrixTranslation(&translation, this->levelEntities[u]->getPosition().x/this->player.GetEllipse().radiusVector.x, this->levelEntities[u]->getPosition().y/this->player.GetEllipse().radiusVector.y, this->levelEntities[u]->getPosition().z/this->player.GetEllipse().radiusVector.z);
		D3DXMatrixScaling(&scale, this->levelEntities[u]->getScale().x, this->levelEntities[u]->getScale().y, this->levelEntities[u]->getScale().z);
		float determinant;

		D3DXMatrixIdentity(&world);

		world *= scale;
		world *= rotation;
		world *= translation;	

		D3DXMatrixInverse(&invWorld, &determinant, &world);

		D3DXVECTOR4 transformedOldFour;
		D3DXVECTOR3 transformedOld = *D3DXVec3Transform(&transformedOldFour, &oldPosition, &invWorld);
		D3DXVECTOR3 transformedVelocity = *D3DXVec4Transform(&transformedOldFour, &D3DXVECTOR4(velocity, 0.0f), &invWorld);

		tempEntity = this->levelEntities[u];
		vector<vector<D3DXVECTOR3>*> entitiesBoxList = dynamic_cast<Terrain*>(tempEntity)->GetCollidedBoxes(*this->player.GetBoundingSphere());

		for(unsigned int j = 0; j < entitiesBoxList.size(); j++)
		{
			triangleList = *entitiesBoxList[j];

			for(unsigned int i = 0; i < triangleList.size(); i+=3)
			{
				triangleList.at(i).x /= this->player.GetEllipse().radiusVector.x;
				triangleList.at(i+1).x /= this->player.GetEllipse().radiusVector.x;
				triangleList.at(i+2).x /= this->player.GetEllipse().radiusVector.x;

				triangleList.at(i).z /= this->player.GetEllipse().radiusVector.z;
				triangleList.at(i+1).z /= this->player.GetEllipse().radiusVector.z;
				triangleList.at(i+2).z /= this->player.GetEllipse().radiusVector.z;

				triangleList.at(i).y /= this->player.GetEllipse().radiusVector.y;
				triangleList.at(i+1).y /= this->player.GetEllipse().radiusVector.y;
				triangleList.at(i+2).y /= this->player.GetEllipse().radiusVector.y;

				D3DXVECTOR3 normalizedVelocity;
				D3DXVec3Normalize(&normalizedVelocity,&(transformedVelocity));

				// Plane origin/normal
				D3DXVECTOR3 pOrigin = triangleList.at(i);
				D3DXVECTOR3 pNormal;
				D3DXVec3Cross(&pNormal, &(triangleList.at(i+1) - triangleList.at(i)), &(triangleList.at(i+2) - triangleList.at(i)));
				D3DXVec3Normalize(&pNormal, &pNormal);


				// Determine the distance from the plane to the source
				D3DXVECTOR3 sphereIntersectionPoint = transformedOld - pNormal*this->player.GetBoundingSphere()->radius;
				float pDist;
  				D3DXVECTOR3 planeIntersectionPoint;

				D3DXVECTOR3 direction = pOrigin - sphereIntersectionPoint;
				float d = D3DXVec3Dot(&direction, &pNormal);
 
				if(d>0.00005f)
				{
							// find plane intersection point by shooting a ray from the 
			// sphere intersection point along the planes normal.
					pDist = intersect(pOrigin, pNormal, sphereIntersectionPoint, pNormal);
                 
					// calculate plane intersection point
					planeIntersectionPoint = sphereIntersectionPoint + pDist*pNormal;
				}
				else
				{
					// shoot ray along the velocity vector
     				pDist = intersect(pOrigin, pNormal, sphereIntersectionPoint, normalizedVelocity);
               
     				// calculate plane intersection point
     				planeIntersectionPoint = sphereIntersectionPoint + pDist*normalizedVelocity;
				}

						// find polygon intersection point. By default we assume its equal to the 
		// plane intersection point. In that case we already know the distance to it.
				D3DXVECTOR3 polyIPoint = planeIntersectionPoint;
				float distToSphereIntersection = pDist;
     
				if(!CheckPointInTriangle(planeIntersectionPoint,triangleList.at(i),triangleList.at(i+1),triangleList.at(i+2))) 
				{ 
					// if not in triangle
					polyIPoint = closestPointOnTriangle(triangleList.at(i), triangleList.at(i+1), triangleList.at(i+2), planeIntersectionPoint);	
     
					distToSphereIntersection = intersectSphere(polyIPoint, -normalizedVelocity, transformedOld, this->player.GetBoundingSphere()->radius);  
                  
					// we cannot know if the ray will actually hit the sphere so we need this check
					if (distToSphereIntersection > 0) 
					{ 	
						//calculate true ellipsoid intersection point
						sphereIntersectionPoint = polyIPoint + distToSphereIntersection*-normalizedVelocity;
					}
				}


				float stuckD = D3DXVec3Length(&(polyIPoint-transformedOld));
 
				if(stuckD <= 1.0f)
				{	
			
					if(this->player.GetSmall())
					{
						oldPosition.x *= this->player.GetEllipse().radiusVector.x;
						oldPosition.y *= this->player.GetEllipse().radiusVector.y;
						oldPosition.z *= this->player.GetEllipse().radiusVector.z;

						velocity.x *= this->player.GetEllipse().radiusVector.x;
						velocity.y *= this->player.GetEllipse().radiusVector.y;
						velocity.z *= this->player.GetEllipse().radiusVector.z;

						this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
						this->player.SetSmall(false);

						oldPosition.x /= this->player.GetEllipse().radiusVector.x;
						oldPosition.y /= this->player.GetEllipse().radiusVector.y;
						oldPosition.z /= this->player.GetEllipse().radiusVector.z;

						velocity.x /= this->player.GetEllipse().radiusVector.x;
						velocity.y /= this->player.GetEllipse().radiusVector.y;
						velocity.z /= this->player.GetEllipse().radiusVector.z;

						return SlideCollision(oldPosition, velocity, iterations, oldPosition);
					}

					D3DXVECTOR4 tempVectorFourCol;
					D3DXVECTOR3 collisionParticle = *D3DXVec3Transform(&tempVectorFourCol, &polyIPoint, &world);
					this->collisionParticle.SetPosition(vec3(collisionParticle.x*this->player.GetEllipse().radiusVector.x, collisionParticle.y*this->player.GetEllipse().radiusVector.y, collisionParticle.z*this->player.GetEllipse().radiusVector.z));
					this->collisionParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, vec3(normalizedVelocity));

					this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Collision);

					this->player.DeductEnergy(10);

					return safeLocation + pNormal*0.1f;
				}

				if((distToSphereIntersection > 0) && (distToSphereIntersection <= D3DXVec3Length(&transformedVelocity))) 
				{ 
					// if first hit, or closest hit so far
					if ((foundCollision == false) || (distToSphereIntersection < nearestDistance))  
					{
						nearestDistance = distToSphereIntersection;
						nearestSphereIntersectionPoint = sphereIntersectionPoint;
						nearestPolygonIntersectionPoint = polyIPoint;
						foundCollision = true;
						nearestPolygonTransform = world;
					}
				} 	
			}
		}
	}

	tempEntity = this->theWorld[0];

	vector<vector<D3DXVECTOR3>*> terrainBoxList = dynamic_cast<Terrain*>(tempEntity)->GetCollidedBoxes(*this->player.GetModel()->at(0)->getBoundingSphere());

	for(unsigned int u = 0; u < terrainBoxList.size(); u++)
	{
		triangleList = *terrainBoxList[u];

		for(unsigned int i = 0; i < triangleList.size(); i+=3)
		{

			triangleList.at(i).x /= this->player.GetEllipse().radiusVector.x;
			triangleList.at(i+1).x /= this->player.GetEllipse().radiusVector.x;
			triangleList.at(i+2).x /= this->player.GetEllipse().radiusVector.x;

			triangleList.at(i).z /= this->player.GetEllipse().radiusVector.z;
			triangleList.at(i+1).z /= this->player.GetEllipse().radiusVector.z;
			triangleList.at(i+2).z /= this->player.GetEllipse().radiusVector.z;

			triangleList.at(i).y /= this->player.GetEllipse().radiusVector.y;
			triangleList.at(i+1).y /= this->player.GetEllipse().radiusVector.y;
			triangleList.at(i+2).y /= this->player.GetEllipse().radiusVector.y;

			D3DXVECTOR3 normalizedVelocity;
			D3DXVec3Normalize(&normalizedVelocity,&(velocity));

			// Plane origin/normal
			D3DXVECTOR3 pOrigin = triangleList.at(i);
			D3DXVECTOR3 pNormal;
			D3DXVec3Cross(&pNormal, &(triangleList.at(i+1) - triangleList.at(i)), &(triangleList.at(i+2) - triangleList.at(i)));
			D3DXVec3Normalize(&pNormal, &pNormal);


			// Determine the distance from the plane to the source
			D3DXVECTOR3 sphereIntersectionPoint = oldPosition - pNormal;
			float pDist;
  			D3DXVECTOR3 planeIntersectionPoint;

			D3DXVECTOR3 direction = pOrigin - sphereIntersectionPoint;
			float d = D3DXVec3Dot(&direction, &pNormal);
 
			if(d>0.0005f)
			{
					// find plane intersection point by shooting a ray from the 
			// sphere intersection point along the planes normal.
				pDist = intersect(pOrigin, pNormal, sphereIntersectionPoint, pNormal);
                 
				// calculate plane intersection point
				planeIntersectionPoint = sphereIntersectionPoint + pDist*pNormal;
			}
			else
			{
				// shoot ray along the velocity vector
     			pDist = intersect(pOrigin, pNormal, sphereIntersectionPoint, normalizedVelocity);
               
     			// calculate plane intersection point
     			planeIntersectionPoint = sphereIntersectionPoint + pDist*normalizedVelocity;
			}

				// find polygon intersection point. By default we assume its equal to the 
		// plane intersection point. In that case we already know the distance to it.
			D3DXVECTOR3 polyIPoint = planeIntersectionPoint;
			float distToSphereIntersection = pDist;
     
			if(!CheckPointInTriangle(planeIntersectionPoint,triangleList.at(i),triangleList.at(i+1),triangleList.at(i+2))) 
			{ 
				// if not in triangle
				polyIPoint = closestPointOnTriangle(triangleList.at(i), triangleList.at(i+1), triangleList.at(i+2), planeIntersectionPoint);	
     
				distToSphereIntersection = intersectSphere(polyIPoint, -normalizedVelocity, oldPosition, 1.0f);  
                  
				// we cannot know if the ray will actually hit the sphere so we need this check
				if (distToSphereIntersection > 0) 
				{ 	
					//calculate true ellipsoid intersection point
					sphereIntersectionPoint = polyIPoint + distToSphereIntersection*-normalizedVelocity;
				}
			}

			float stuckD = D3DXVec3Length(&(polyIPoint-oldPosition));
 
			if(stuckD <= 1.0f)
			{	
				//this->entryInstance->GetCoreInstance()->PlaySound(L"path");

				if(this->player.GetSmall())
				{
					oldPosition.x *= this->player.GetEllipse().radiusVector.x;
					oldPosition.y *= this->player.GetEllipse().radiusVector.y;
					oldPosition.z *= this->player.GetEllipse().radiusVector.z;

					velocity.x *= this->player.GetEllipse().radiusVector.x;
					velocity.y *= this->player.GetEllipse().radiusVector.y;
					velocity.z *= this->player.GetEllipse().radiusVector.z;

					this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
					this->player.SetSmall(false);

					oldPosition.x /= this->player.GetEllipse().radiusVector.x;
					oldPosition.y /= this->player.GetEllipse().radiusVector.y;
					oldPosition.z /= this->player.GetEllipse().radiusVector.z;

					velocity.x /= this->player.GetEllipse().radiusVector.x;
					velocity.y /= this->player.GetEllipse().radiusVector.y;
					velocity.z /= this->player.GetEllipse().radiusVector.z;

					return SlideCollision(oldPosition, velocity, iterations, oldPosition);
				}

				D3DXVECTOR4 tempVectorFourCol;
				D3DXVECTOR3 collisionParticle = *D3DXVec3Transform(&tempVectorFourCol, &polyIPoint, &nearestPolygonTransform);
				this->collisionParticle.SetPosition(vec3(collisionParticle.x*this->player.GetEllipse().radiusVector.x, collisionParticle.y*this->player.GetEllipse().radiusVector.y, collisionParticle.z*this->player.GetEllipse().radiusVector.z));
				this->collisionParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, vec3(normalizedVelocity));

				this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Collision);

				this->player.DeductEnergy(20);

				return safeLocation + pNormal*0.1f;
			}

			if((distToSphereIntersection > 0) && (distToSphereIntersection <= D3DXVec3Length(&velocity))) 
			{ 
				// if first hit, or closest hit so far
				if ((foundCollision == false) || (distToSphereIntersection < nearestDistance))  
				{
					nearestDistance = distToSphereIntersection;
					nearestSphereIntersectionPoint = sphereIntersectionPoint;
					nearestPolygonIntersectionPoint = polyIPoint;
					foundCollision = true;
					D3DXMatrixIdentity(&nearestPolygonTransform);
				}
			} 	
		}
	}

	this->player.SetSmall(false);

	if(!foundCollision)
	{
		float velocityLength = D3DXVec3Length(&velocity);
		D3DXVECTOR3 normVelo;
		D3DXVec3Normalize(&normVelo, & velocity);
		D3DXVECTOR3 V = normVelo*(velocityLength - D3DXVec3Length(&this->player.GetVelocity())*0.005f); 
     
		// return the final position
		return oldPosition + V;
	}

	this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Collision);
	D3DXVECTOR3 newPosition;

	if(nearestDistance >= D3DXVec3Length(&this->player.GetVelocity())*0.01f)
	{
		D3DXVECTOR3 normVelo;
		D3DXVec3Normalize(&normVelo, & velocity);
		D3DXVECTOR3 V = normVelo*(nearestDistance - D3DXVec3Length(&this->player.GetVelocity())*0.005f);

		newPosition = oldPosition + V;
	}
	else
	{
		newPosition = oldPosition;
	}


	D3DXVECTOR4 tempVectorFour;
	D3DXVECTOR3 slidePlaneOrigin = *D3DXVec3Transform(&tempVectorFour, &nearestPolygonIntersectionPoint, &nearestPolygonTransform);
	D3DXVECTOR3 slidePlaneNormal = newPosition - slidePlaneOrigin;
	D3DXVec3Normalize(&slidePlaneNormal, &slidePlaneNormal);

    this->collisionParticle.SetPosition(vec3(slidePlaneOrigin.x*this->player.GetEllipse().radiusVector.x, slidePlaneOrigin.y*this->player.GetEllipse().radiusVector.y, slidePlaneOrigin.z*this->player.GetEllipse().radiusVector.z)); 
	this->collisionParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, -slidePlaneNormal);

    // We now project the destination point onto the sliding plane
    float l = intersect(slidePlaneOrigin, slidePlaneNormal, destinationPoint, slidePlaneNormal); 
    
    // We can now calculate a new destination point on the sliding plane
    D3DXVECTOR3 newDestinationPoint;
    newDestinationPoint.x = destinationPoint.x + l*slidePlaneNormal.x;
    newDestinationPoint.y = destinationPoint.y + l*slidePlaneNormal.y;
    newDestinationPoint.z = destinationPoint.z + l*slidePlaneNormal.z;
    
    // Generate the slide vector, which will become our new velocity vector
    // for the next iteration
    D3DXVECTOR3 newVelocityVector = newDestinationPoint - slidePlaneOrigin;

	safeLocation = newPosition - newVelocityVector*0.1f;
	D3DXVECTOR3 normVelocity2;
	D3DXVec3Normalize(&normVelocity2, &velocity);
	float angle = (D3DXVec3Dot(&normVelocity2, &-slidePlaneNormal)-1.0f)*90.0f;
	float howMuchEnergy = (90.0f - angle)*D3DXVec3Length(&velocity);
	this->player.DeductEnergy(howMuchEnergy);

	return SlideCollision(newPosition, newVelocityVector, iterations - 1, safeLocation);
}




void FlyState_Level::Release()
{
	for(unsigned int i = 0; i < this->theWorld.size(); i++)
	{
		delete this->theWorld[i];
	}
	for(unsigned int i = 0; i < this->levelEntities.size(); i++)
	{
		delete this->levelEntities[i];
	}
	for(int i = 0; i < 3; i++)
	{
		this->pickups[i].Release();
	}
	for(unsigned int i = 0; i < this->energy.size(); i++)
	{
		this->energy[i].Release();
	}

	this->pauseMenu.Release();

	for(unsigned int i = 0; i < this->cursor.size(); i++)
	{
		delete this->cursor[i];
	}
	for(unsigned int i = 0; i < this->skyBox.size(); i++)
	{
		delete this->skyBox[i];
	}
	for(unsigned int i = 0; i < this->dirLights.size(); i++)
	{
		delete this->dirLights[i];
	}
	for(unsigned int i = 0; i < this->shadowViews.size(); i++)
	{
		delete this->shadowViews[i];
	}
	for(unsigned int i = 0; i < this->UIorthographic.size(); i++)
	{
		delete this->UIorthographic[i];
	}

	delete this->mainTimer;

	this->player.Release();
}
