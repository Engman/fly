#include "FlyState_Level.h"
#include "..\FlyGame.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Util\CollisionLib.h"
#include "..\..\FlyGameEngine\Core\Light\DirectionLight.h"
#include "..\..\FlyGameEngine\Util\Proxy.h"


#include <fstream>
#include <iostream>
#include <string>

FlyState_Level::FlyState_Level()
{

}
FlyState_Level::~FlyState_Level()
{
	delete this->mainTimer;
	//this->entryInstance->GetCoreInstance()->Audio_Shutdown();
}

void FlyState_Level::Initiate(FlyGame* instance)
{
	this->state = 0;

	this->entryInstance = instance;

	this->ReadLevel(L"..\\Resources\\Levels\\LevelTest.txt");

	this->entryInstance->GetCoreInstance()->Input_Initialize();
	this->entryInstance->GetCoreInstance()->Input_Activate();

	this->mainTimer = new Timer();
	this->mainTimer->Initialize();

	this->menuCamera.SetPosition(0.0f, 0.0f, -3.0f);
	this->menuCamera.SetOrthogonalMatrix(1200.0f, 600.0f, 0.1f, 10.0f);

	this->mainCamera.SetOrthogonalMatrix(1200.0f, 600.0f, 0.1f, 10.0f);

	this->mainCamera.Render();
	this->menuCamera.Render();
	this->entryInstance->GetCoreInstance()->Audio_Initialize();
	//this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Collision);
	this->engineParticlesLeft.Initialize();
	this->engineParticlesRight.Initialize();
	this->collisionParticle.Initialize();
	
	this->entryInstance->GetLuaState();

}
void FlyState_Level::Frame()
{
	this->mainTimer->Frame();

	/*if(Input::self()->IsButtonPressed(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}*/

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

	//Collision against pickups
	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		if(SphereVSSphere(*this->levelPickups[i]->getBoundingSphere(), *this->player.GetBoundingSphere()))
		{
			this->levelPickups.erase(this->levelPickups.begin()+i);
			i--;
			this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);
		}
	}
	//Collision against energy
	for(unsigned int i = 0; i < this->energyPickups.size(); i++)
	{
		if(SphereVSSphere(*this->energyPickups[i]->getBoundingSphere(), *this->player.GetBoundingSphere()))
		{
			this->energyPickups.erase(this->energyPickups.begin()+i);
			i--;
			this->player.DeductEnergy(-1000);
			this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);
		}
	}
	
	//Check if all energy is lost
	if(this->player.GetEnergy() < 0.0f)
	{
		//this->player.SetPosition(vec3(0.0f, 10.0f, -30.0f));
		this->player.DeductEnergy(-10000.0f);
	}
	
	//Update skybox position
	this->skyBox[0]->setPosition(this->player.GetPosition());
	this->skyBox[0]->Update();

	this->collisionParticle.Frame(this->player.GetPosition(), this->mainTimer->GetDeltaTime()*10.0f);
	this->collisionParticle.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
	//this->collisionParticle.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
	
	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		//if(typeid(this->levelPickups[i]) == typeid(FlyMeshAnimated) )
			((FlyMeshAnimated*)this->levelPickups[i])->UpdateAnimation(1); 
		
		if(SphereVSSphere(*this->levelPickups[i]->getBoundingSphere(), *this->player.GetBoundingSphere()))
		{
			this->levelPickups.erase(this->levelPickups.begin()+i);
			i--;
			//play sound
			this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Collision);
		}
	}
	this->player.GetModel()->at(0)->Update();
	
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
	this->Input();

	float downVelocity = D3DXVec3Dot(&this->mainCamera.GetForward(), &vec3(0.0f, 1.0f, 0.0f));

	if(downVelocity < 0.0f && downVelocity > -1.0f && this->player.GetVelocity().z < this->player.GetMaxVelocity().z+0.5f*(1.0f - downVelocity))
	{
		this->player.SetVelocity(this->player.GetVelocity()+vec3(0.0f, 0.0f, 0.0005f));
	}
	else if(downVelocity > 0.0f && downVelocity < 1.0f && this->player.GetVelocity().z > this->player.GetMaxVelocity().z)
	{
		this->player.SetVelocity(this->player.GetVelocity()-vec3(0.0f, 0.0f, 0.0005f));
	}

	this->mainCamera.RelativeForward(this->player.GetVelocity().z);
	this->mainCamera.RelativeRight(this->player.GetVelocity().x);
	//this->mainCamera.RelativeUp(this->player.GetVelocity().y);
	this->mainCamera.RelativePitch(0.4f*this->player.GetVelocity().y*this->player.GetVelocity().z*50.0f);
	this->mainCamera.SetPositionY(this->mainCamera.GetPosition().y-0.1f*fabs(downVelocity));
	this->mainCamera.RelativeYaw(this->player.GetVelocity().x*3.0f*this->mainCamera.GetUp().y);
 
	this->player.SetPosition(this->mainCamera.GetPosition());
	this->player.GetBoundingSphere()->center = this->player.GetPosition();
	//Check for slide collision for birds location
	D3DXVECTOR3 velocity = this->player.GetPosition() - oldPosition;
	vec3 newPlayerPosition = SlideCollision(vec3(oldPosition.x/this->player.GetEllipse().radiusVector.x, oldPosition.y/this->player.GetEllipse().radiusVector.y, oldPosition.z/this->player.GetEllipse().radiusVector.z), vec3(velocity.x/this->player.GetEllipse().radiusVector.x, velocity.y/this->player.GetEllipse().radiusVector.y, velocity.z/this->player.GetEllipse().radiusVector.z), 200, vec3(oldPosition.x/this->player.GetEllipse().radiusVector.x, oldPosition.y/this->player.GetEllipse().radiusVector.y, oldPosition.z/this->player.GetEllipse().radiusVector.z));
	this->player.SetPosition(vec3(newPlayerPosition.x*this->player.GetEllipse().radiusVector.x, newPlayerPosition.y*this->player.GetEllipse().radiusVector.y, newPlayerPosition.z*this->player.GetEllipse().radiusVector.z));

	this->player.GetBoundingSphere()->center = this->player.GetPosition();
	this->mainCamera.SetPosition(this->player.GetPosition());

	this->mainCamera.Render();
	vec3 newCameraPos = this->player.GetPosition() - this->mainCamera.GetForward()*10.0f + this->mainCamera.GetUp()*1.5f;
	this->mainCamera.SetPosition(newCameraPos.x, newCameraPos.y, newCameraPos.z);

	

	//Tilt the player model according to sideways velocity
	if(this->player.GetVelocity().x < -0.001f)
	{
		float rX = this->mainCamera.GetForward().x;
		float rY = this->mainCamera.GetForward().y*this->player.GetVelocity().x*-5.0f;
		float rZ = D3DXVec3Length(&vec3(this->mainCamera.GetForward().x, 0.0f, this->mainCamera.GetForward().z))*upsideDown*this->player.GetVelocity().x*-5.0f;

		//this->player.RollRight(this->mainCamera.GetForward());
		float howMuch = this->player.GetVelocity().x*-10.0f;
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
		float rY = this->mainCamera.GetForward().y*this->player.GetVelocity().x*5.0f;
		float rZ = D3DXVec3Length(&vec3(this->mainCamera.GetForward().x, 0.0f, this->mainCamera.GetForward().z))*upsideDown*this->player.GetVelocity().x*5.0f;

		//this->player.RollRight(this->mainCamera.GetForward());
		float howMuch = this->player.GetVelocity().x*-10.0f;
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
		this->engineParticlesLeft.SetPosition(this->player.GetPosition() - this->mainCamera.GetRight()*1.3f);
		this->engineParticlesRight.SetRotation(this->mainCamera.GetRotation()*((float)D3DX_PI/180.0f));
		this->engineParticlesRight.SetPosition(this->player.GetPosition() + this->mainCamera.GetRight()*1.3f);
	}
	//END

	//Engines
	if(Input::self()->IsButtonPressed(DIK_W))
	{
		this->engineParticlesLeft.EmitParticles(16, this->player.GetPosition() - oldPosition);
		this->engineParticlesRight.EmitParticles(16, this->player.GetPosition() - oldPosition);
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		this->engineParticlesLeft.EmitParticles(16, this->player.GetPosition() - oldPosition);
		this->engineParticlesRight.EmitParticles(16, this->player.GetPosition() - oldPosition);
		if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.001f));
	}

	this->engineParticlesLeft.Frame(this->player.GetPosition() - oldPosition, this->mainTimer->GetDeltaTime());
	this->engineParticlesRight.Frame(this->player.GetPosition() - oldPosition, this->mainTimer->GetDeltaTime());
	//Engines end

	this->player.Update();

	//AudioClass::self()->uppdateSounds();

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

	this->player.Render(f);

	this->engineParticlesLeft.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->engineParticlesLeft.Render(f);
	this->engineParticlesRight.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->engineParticlesRight.Render(f);
	this->collisionParticle.SetParticleShader(shaders[FlyShader_gBufferDefault]);
	this->collisionParticle.Render(f);

	for(unsigned int i = 0; i < this->levelEntities.size(); i++)
	{	
		this->levelEntities[i]->setShader(shaders[FlyShader_gBufferDefault]);
		this->levelEntities[i]->Render(f);
	}
	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		this->levelPickups[i]->Render(f);
	}
	for(unsigned int i = 0; i < this->energyPickups.size(); i++)
	{
		this->energyPickups[i]->setShader(shaders[FlyShader_gBufferDefault]);
		this->energyPickups[i]->Render(f);
	}

	for(unsigned int i = 0; i < this->dirLights.size(); i++)
	{
		this->dirLights[i]->Render(f);
	}

	for(unsigned int i = 0; i <(int) this->shadowViews.size(); i++)
	{
		Camera cam;

		LightViewProj* shadow = (LightViewProj*)(shadowViews[0])->Map();

		cam.SetViewMatrix(shadow->lView);
		cam.SetProjectionMatrix(shadow->lProj);
		cam.Render();
		cam.ConstructViewFrustum(f);
		shadowViews[0]->Unmap();

		for(unsigned int i = 0; i <(int) this->levelEntities.size(); i++)
		{	
			this->levelEntities[i]->setShader(shaders[FlyShader_Shadow]);
			this->levelEntities[i]->Render(f);
		}

		this->theWorld[0]->setShader(shaders[FlyShader_Shadow]);
		this->theWorld[0]->Render(f);

	}
	this->entryInstance->GetCoreInstance()->Gfx_DrawSkyBox();
	this->entryInstance->GetCoreInstance()->Gfx_DrawGbuffer();
	
	this->menuCamera.ConstructViewFrustum(f);
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->menuCamera);

	/*for(int i =0; i<(int)UIorthographic.size(); i++ )
	{
		this->UIorthographic[i]->Render(f);
	}*/
	this->entryInstance->GetCoreInstance()->Gfx_DrawGbufferOrtho();
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);

	//this->entryInstance->GetCoreInstance()->Gfx_DrawLighting();
	this->entryInstance->GetCoreInstance()->Gfx_DrawBlur();
	this->entryInstance->GetCoreInstance()->Gfx_DrawShadows(&shadowViews);
	this->entryInstance->GetCoreInstance()->Gfx_DrawFinalPicture(&shadowViews);
	

	return true;
}

bool FlyState_Level::MenuRender()
{
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->menuCamera);

	this->entryInstance->GetCoreInstance()->Gfx_Update();
			
	this->entryInstance->GetCoreInstance()->Gfx_BeginDeferredScene();

	ViewFrustum f;
	this->menuCamera.ConstructViewFrustum(f);
	this->gameMenu[0]->Render(f);
	this->cursor[0]->Render(f);

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredSceneOrtho();

	return true;
}
bool FlyState_Level::MenuUpdate()
{
	Input::self()->Frame();

	int mouseX, mouseY;

	Input::self()->GetMouseLocation(mouseX, mouseY);

	if(Input::self()->IsMouseButtonPressed(0) && mouseY > 410 && mouseY < 440)
	{
		PostQuitMessage(0);
	}

	if(Input::self()->IsMouseButtonPressed(0) && mouseY > 160 && mouseY < 190)
	{
		this->state = 0;
	}

	this->cursor[0]->setPosition(vec3(mouseX-600.0f, -(mouseY-300.0f) , -1.0f));

	return true;
}

void FlyState_Level::Input()
{
	Input::self()->Frame();

	int mouseX, mouseY;

	Input::self()->GetMouseRelative(mouseX, mouseY);

	//Advanced
	//if(Input::self()->IsButtonPressed(DIK_W))
	//{
	//	if(this->player.GetVelocity().y < this->player.GetMaxVelocity().y)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.004f, 0.0f));
	//}
	//else
	//{
	//	if(this->player.GetVelocity().y > 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.002f, 0.0f));
	//}
	//if(Input::self()->IsButtonPressed(DIK_S))
	//{
	//	if(this->player.GetVelocity().y > -this->player.GetMaxVelocity().y)
	//		this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.004f, 0.0f));
	//}
	//else
	//{
	//	if(this->player.GetVelocity().y < 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.002f, 0.0f));
	//}
	//
	//if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	//{
	//	this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
	//}
	//else if(this->player.GetEllipse().radiusVector.x == 0.5f)
	//{
	//	this->player.SetEllipseVector(vec3(2.0f, 0.5f, 2.0f));
	//	this->player.SetSmall(true);
	//}

	//if(Input::self()->IsButtonPressed(DIK_A))
	//{
	//	if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.002f, 0.0f, 0.0f));
	//	//this->player.SetRotation(vec3(this->player.GetRotation().x-this->player.GetVelocity().x*0.4f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	//}
	//if(Input::self()->IsMouseButtonPressed(1))
	//{
	//	if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	//	if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	if(this->player.GetVelocity().x < -0.001f)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().z > 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.0f, 0.0005f));
	//	if(this->player.GetVelocity().z < 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.0005f));
	//}

	//if(Input::self()->IsButtonPressed(DIK_D))
	//{
	//	if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.002f, 0.0f, 0.0f));
	//	//this->player.SetRotation(vec3(this->player.GetRotation().x-this->player.GetVelocity().x*0.4f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	//}
	//if(Input::self()->IsMouseButtonPressed(0))
	//{
	//	if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	//	if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	if(this->player.GetVelocity().x > 0.001f)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
	//	if(this->player.GetVelocity().z > 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.0f, 0.00005f));
	//	if(this->player.GetVelocity().z < 0.0f)
	//		this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.00005f));
	//}


	//Casual
	if(mouseY < 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseX < 0)
	{	
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(mouseX*0.0005f, 0.0f, 0.0f));
		
	}
	if(mouseX > 0)
	{
		if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(mouseX*0.0005f, 0.0f, 0.0f));

	}
	if(mouseX == 0)
	{
		if(this->player.GetVelocity().x < -0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0005f, 0.0f, 0.0f));
		else if(this->player.GetVelocity().x > 0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.0005f, 0.0f, 0.0f));
	}

	if(Input::self()->IsButtonPressed(DIK_W))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.001f));
	}

	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
	}
	else if(this->player.GetEllipse().radiusVector.x == 0.5f)
	{
		this->player.SetEllipseVector(vec3(2.0f, 0.5f, 2.0f));
		this->player.SetSmall(true);
	}
	
	//Debug
	/*if(Input::self()->IsButtonPressed(DIK_W))
	{
		this->mainCamera.RelativeForward(0.2f);
		this->player.UpdateAnimation(1);
		this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);

	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		this->mainCamera.RelativeForward(-0.2f);
		this->player.StopAnimation(1);
	}

	if(Input::self()->IsButtonPressed(DIK_A))
	{
		this->mainCamera.RelativeRight(-0.2f);
		this->player.UpdateAnimation(2);
		this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);
	}
	if(Input::self()->IsButtonPressed(DIK_D))
	{
		this->mainCamera.RelativeRight(0.2f);
		this->player.StopAnimation(2);
	}
	if(mouseY < 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseX < 0)
	{	
		this->mainCamera.RelativeYaw(0.05f*mouseX);
	}
	if(mouseX > 0)
	{
		this->mainCamera.RelativeYaw(0.05f*mouseX);
	}*/


	if(Input::self()->IsButtonPressed(DIK_F1))
	{
		this->player.SetPosition(vec3(0.0f, 10.0f, 0.0f));
	}
	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
	{
		this->state = 1;
	}
}

bool FlyState_Level::ReadLevel(const wchar_t* fileName)
{
	wifstream file(fileName);
	vec3 readVector;
	D3DXVECTOR4 readVector4;
	int readInt = 0;
	wstring readString = L"";
	int nrOfStuff = 0;

	vector<IShader*> shaders;
	this->entryInstance->GetCoreInstance()->Gfx_GetShader(shaders);

	if(!file.is_open())
	{
		return false;
	}

	file>>readString;
	file>>nrOfStuff;

	//First to load is always terrain
	file>>readString;
	this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->theWorld, FlyGeometry_Terrain);
	file>>readVector.x;
	file>>readVector.y;
	file>>readVector.z;
	this->theWorld[0]->setPosition(readVector);
	file>>readVector.x;
	file>>readVector.y;
	file>>readVector.z;
	this->theWorld[0]->setRotation(readVector);
	file>>readInt;		
	
	readVector = D3DXVECTOR3(1,1,1);
	this->theWorld[0]->setScale(readVector);
	this->theWorld[0]->setShader(shaders[readInt]);


	//second is the skybox
	file>>readString;
	this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->skyBox);
	file>>readVector.x;
	file>>readVector.y;
	file>>readVector.z;
	this->skyBox[0]->setPosition(readVector);
	file>>readVector.x;
	file>>readVector.y;
	file>>readVector.z;
	this->skyBox[0]->setRotation(readVector);
	file>>readInt;		

	readVector = D3DXVECTOR3(20,20,20);
	this->skyBox[0]->setScale(readVector);
	this->skyBox[0]->setShader(shaders[readInt]);

	//read in the water


	////Read rest of objects
	for(int i = 0; i < nrOfStuff-2; i++)
	{
		file>>readString;
		this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->levelEntities);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelEntities[i]->setPosition(readVector);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelEntities[i]->setRotation(readVector);
		file>>readInt;
		readVector = D3DXVECTOR3(1,1,1);
		this->levelEntities[i]->setScale(readVector);
		this->levelEntities[i]->setShader(shaders[readInt]);
	}

	file>>nrOfStuff;

	//Pick-ups
	for(int i = 0; i < nrOfStuff; i++)
	{
		file>>readString;
		this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->levelPickups, FlyGeometry_AnimatedMesh);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelPickups[i]->setPosition(readVector);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelPickups[i]->setRotation(readVector);
		file>>readInt;
		readVector = D3DXVECTOR3(1,1,1);
		this->levelPickups[i]->setScale(readVector);
		this->levelPickups[i]->setShader(shaders[readInt]);
	}

	file>>nrOfStuff;

	//Energy
	for(int i = 0; i < nrOfStuff; i++)
	{
		file>>readString;
		this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->energyPickups);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->energyPickups[i]->setPosition(readVector);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->energyPickups[i]->setRotation(readVector);
		file>>readInt;
		readVector = D3DXVECTOR3(1,1,1);
		this->energyPickups[i]->setScale(readVector);


		this->energyPickups[i]->setShader(shaders[readInt]);
		BoundingSphere* energySphere = new BoundingSphere;
	


		energySphere->center = this->energyPickups[i]->getPosition();
		energySphere->radius = 2.0f;
		this->energyPickups[i]->setBoundingSphere(energySphere);
	}

	file>>readString;
	file>>nrOfStuff;

	DirectionalLightProxy dirLightProxy;
	D3DXVECTOR3 pos; 
	D3DXVECTOR3 rot;
	bool hasShadow;
	for(int i= 0 ; i<nrOfStuff; i++)
	{

		
		file>>readVector4.x;
		file>>readVector4.y;
		file>>readVector4.z;
		file>>readVector4.w; 
		dirLightProxy.ambient = readVector4;
		file>>readVector4.x;
		file>>readVector4.y;
		file>>readVector4.z;
		file>>readVector4.w; 
		dirLightProxy.diffuse = readVector4;
		file>>readVector4.x;
		file>>readVector4.y;
		file>>readVector4.z;
		file>>readVector4.w; 
		dirLightProxy.specular = readVector4;

		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		dirLightProxy.direction = D3DXVECTOR4(readVector, 0);
	
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		pos = readVector;

		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		rot = readVector;


		file>>hasShadow;
		file>>readInt;

		Camera lightCam;
		lightCam.SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
		lightCam.SetPosition(pos);
		lightCam.SetRotation(rot.x, rot.y, rot.z);
		lightCam.Render();
		
		LightViewProj lightViewProj; 
		lightViewProj.lView = lightCam.GetViewMatrix();
		lightViewProj.lProj = lightCam.GetProjectionMatrix();
		
		BaseBuffer* dirLightViewProj = new BaseBuffer();
		BaseBuffer::BUFFER_INIT_DESC viewProjBufferDesc;
		viewProjBufferDesc.dc = D3DShell::self()->getDeviceContext();
		viewProjBufferDesc.device = D3DShell::self()->getDevice();
		viewProjBufferDesc.elementSize = sizeof(LightViewProj);
		viewProjBufferDesc.nrOfElements = 1;
		viewProjBufferDesc.data = &lightViewProj;
		viewProjBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
		viewProjBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

		if(FAILED(dirLightViewProj->Initialize(viewProjBufferDesc)))
		{

		}
		DirectionLight* light = new DirectionLight(Type::LIGHT);
		DirectionLight::DIRLIGHT_DESC dirLight; 
		dirLight.data = dirLightProxy; 
		dirLight.viewProj = lightViewProj;
		dirLight.shader =  shaders[FlyShader_DirLight];
	
		light->Initialize(dirLight);
		
		dirLights.push_back(light);

		if(hasShadow)
		{
			shadowViews.push_back(dirLightViewProj);
		}
		
	}


	//Read player position
	vec3 player;
	
	file>>player.x;
	file>>player.y;
	file>>player.z;
	this->mainCamera.SetPosition(player);
	file>>player.x;
	file>>player.y;
	file>>player.z;
	this->mainCamera.SetRotation(player.x, player.y, player.z);
	this->mainCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, 1200.0f/600.0f, 1.0f, 400.0f);

	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);
	
	file.close();
	//Done reading info

	//Set player model and player info
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\zcharacter_anim.fgm", this->player.GetModel(), FlyGeometry_AnimatedMesh);
	readVector = D3DXVECTOR3(1,1,1);
	this->player.GetModel()->at(0)->setScale(readVector);
	this->player.GetModel()->at(0)->setShader(shaders[FlyShader_gBufferDefault]);
	this->player.SetPosition(this->mainCamera.GetPosition());

	//Set level pickup bounding info
	BoundingSphere* sphere1 = new BoundingSphere;
	BoundingSphere* sphere2 = new BoundingSphere;
	BoundingSphere* sphere3 = new BoundingSphere;
	
	sphere1->center = this->levelPickups[0]->getPosition();
	sphere1->radius = 2.0f;
	this->levelPickups[0]->setBoundingSphere(sphere1);

	sphere2->center = this->levelPickups[1]->getPosition();
	sphere2->radius = 2.0f;
	this->levelPickups[1]->setBoundingSphere(sphere2);

	sphere3->center = this->levelPickups[2]->getPosition();
	sphere3->radius = 2.0f;
	this->levelPickups[2]->setBoundingSphere(sphere3);

	//Set player bounding info
	BoundingSphere* playerSphere = new BoundingSphere;
	playerSphere->center = this->mainCamera.GetPosition();
	playerSphere->radius = 2.0f;
	this->player.SetBoundingSphere(playerSphere);

	//Game Menu
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\in_game_menu.fgm", &this->gameMenu);

	this->gameMenu[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->gameMenu[0]->setScale(vec3(1,1,1));
	this->gameMenu[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));

	//Cursor
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->cursor);

	this->cursor[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->cursor[0]->setScale(vec3(1,1,1));
	this->cursor[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));

	//UI
	vector<const wchar_t*> UIpaths;
	UIpaths.push_back(L"..\\Resources\\Models\\mouse_cursor.fgm");
	UIpaths.push_back(L"..\\Resources\\Models\\menu.fgm");

	this->entryInstance->GetCoreInstance()->Geometry_Load(UIpaths, &this->UIorthographic);

	this->UIorthographic[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->UIorthographic[0]->setScale(vec3(3,3,1));
	this->UIorthographic[0]->setPosition(vec3(-100.0f, 50.0f, 0.0f));

	//UI
	
	this->UIorthographic[1]->setShader(shaders[FlyShader_gBufferDefault]);
	this->UIorthographic[1]->setScale(vec3(10,10,1));
	this->UIorthographic[1]->setPosition(vec3(0.0f, 0.0f, 0.0f));
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
		tempEntity = this->levelEntities[u];
		triangleList = *(dynamic_cast<FlyMesh*>(tempEntity)->GetTriangles());

		D3DXMATRIX rotation, translation, world, invWorld;
		D3DXMatrixTranslation(&rotation, this->levelEntities[u]->getRotation().y, this->levelEntities[u]->getRotation().x, this->levelEntities[u]->getRotation().z);
		D3DXMatrixTranslation(&translation, this->levelEntities[u]->getPosition().x, this->levelEntities[u]->getPosition().y, this->levelEntities[u]->getPosition().z);
		float determinant;

		D3DXMatrixIdentity(&world);

		world *= rotation;
		world *= translation;

		D3DXMatrixInverse(&invWorld, &determinant, &world);

		D3DXVECTOR4 transformedOldFour;
		D3DXVECTOR3 transformedOld = *D3DXVec3Transform(&transformedOldFour, &oldPosition, &invWorld);
		D3DXVECTOR3 transformedVelocity = *D3DXVec4Transform(&transformedOldFour, &D3DXVECTOR4(velocity, 0.0f), &invWorld);

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
 
			if(d>0.001f)
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
				D3DXVECTOR3 collisionParticle = *D3DXVec3Transform(&tempVectorFourCol, &polyIPoint, &world);
				this->collisionParticle.SetPosition(vec3(collisionParticle.x*this->player.GetEllipse().radiusVector.x, collisionParticle.y*this->player.GetEllipse().radiusVector.y, collisionParticle.z*this->player.GetEllipse().radiusVector.z));
				this->collisionParticle.EmitParticles(this->mainTimer->GetDeltaTime()*1000.0f, vec3(normalizedVelocity));

				this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);

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
				
				this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);

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

	
	this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);
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
	float howMuchEnergy = (90.0f - angle)*D3DXVec3Length(&this->player.GetVelocity());
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
	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		delete this->levelPickups[i];
	}
	for(unsigned int i = 0; i < this->energyPickups.size(); i++)
	{
		delete this->energyPickups[i];
	}
	for(unsigned int i = 0; i < this->gameMenu.size(); i++)
	{
		delete this->gameMenu[i];
	}
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

	delete this->mainTimer;

	this->player.Release();
}