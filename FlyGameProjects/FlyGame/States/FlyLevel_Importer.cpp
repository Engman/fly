#include "FlyState_Level.h"
#include "..\..\FlyGameEngine\Util\MutexHandler.h"


#if defined (_DEBUG) || defined (DEBUG)
#include <iostream>
#define C_PRINT(STR)\
		std::cout << STR
#else
#define C_PRINT
#endif


bool FlyState_Level::ReadLevel(const wchar_t* fileName)
{

	wifstream file(fileName);
	if(!file.is_open())	
		return false;


	vec3 readVector;
	D3DXVECTOR4 readVector4;
	int readInt = 0;
	wstring readString = L"";
	int nrOfStuff = 0;


	//Prepear shaders
	vector<IShader*> shaders;
	this->entryInstance->GetCoreInstance()->Gfx_GetShader(shaders);


			//Loading table:
	//---------------------------
	// - Terrain (0)
	// - Skybox (1)
	// - Water (2)
	// - Static objects (*)
	// - Pickups(3)
	// - Energy orbs
	// - Directional lights
	// - Point lights
	//---------------------------


	//Read the name of the file/level
	ReadString(file, readString);

	//Read number of "static" objects on level
	nrOfStuff = ReadInt(file);

	//Get the mutex
	MutexHandler::SetMutex(MutexResource_ALL, true);
	
	bool status = false;
	for(int i = 0; i < nrOfStuff; i++)
	{
		switch (i)
		{
			case 0:		//terrain
				status = _ImportTerrain(file, shaders);
			break;
			case 1:		//skybox/sphere/dome
				status = _ImportSkybox(file, shaders);
			break;
			case 2:		//water
				status = _ImportWater(file, shaders);
			break;
			default:	//Static
				status = _ImportStatic(file, shaders);
			break;
		}

		if(!status)
		{
			MutexHandler::SetMutex(MutexResource_ALL);
			return false;
		}
	}

	if(!_ImportPickups(file, shaders))	
	{ 
		MutexHandler::SetMutex(MutexResource_ALL); return false; 
	}
	if(!_ImportEnergy(file, shaders))	
	{ 
		MutexHandler::SetMutex(MutexResource_ALL); return false; 
	}
	if(!_ImportLights(file, shaders))	
	{ 
		MutexHandler::SetMutex(MutexResource_ALL); return false; 
	}
	if(!_ImportPlayer(file, shaders))	
	{ 
		MutexHandler::SetMutex(MutexResource_ALL); return false; 
	}


	file.close();
	/*________________________________END OF FILE_______________________________________*/

	
	//Cursor
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->cursor);
	this->cursor[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->cursor[0]->setScale(vec3(3,3,3));
	this->cursor[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));

	BoundingSphere* sphere = new BoundingSphere();
	sphere->center = vec3(0.0f, 0.0f, -1.0f);
	sphere->radius = 1200.0f;
	this->cursor[0]->setBoundingSphere(sphere);

	//UI
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\energyBar.fgm", &this->UIorthographic);
	this->UIorthographic[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->UIorthographic[0]->setScale(vec3((this->player.GetEnergy()/this->player.GetMaxEnergy())*20.0f,2.0f,1.0f));
	this->UIorthographic[0]->setPosition(vec3(0.0f, -250.0f, 0.0f));
	this->UIorthographic[0]->setBoundingSphere(0);

	MutexHandler::SetMutex(MutexResource_ALL);

	return true;
}



bool FlyState_Level::_ImportTerrain(wifstream& file, vector<IShader*>& shaders)
{
	wstring dummy = L"";
	if(!this->entryInstance->GetCoreInstance()->Geometry_Load(ReadString(file, dummy).c_str(), &this->theWorld, FlyGeometry_Terrain, 1, 5))
		return false;

	this->theWorld[0]->setShader(shaders[FlyShader_gBufferDefault]);

	return true;
}
bool FlyState_Level::_ImportSkybox(wifstream& file, vector<IShader*>& shaders)	
{
	wstring dummy = L"";
	if(!this->entryInstance->GetCoreInstance()->Geometry_Load(ReadString(file, dummy).c_str(), &this->skyBox))
		return false;

	this->skyBox[0]->setShader(shaders[FlyShader_gBufferNoDepth]);
	this->skyBox[0]->setScale(vec3(20 , 20 ,20));


	return true;
}
bool FlyState_Level::_ImportWater(wifstream& file, vector<IShader*>& shaders)	
{
	wstring dummy = L"";
	
	if(!this->entryInstance->GetCoreInstance()->Geometry_Load(ReadString(file, dummy).c_str(), &this->water, FlyGeometry_Water, 1, 3))
		return false;

	this->water[0]->setPosition(ReadVector3(file));
	this->water[0]->setRotation(ReadVector3(file));
	this->water[0]->setScale(ReadVector3(file));
	this->water[0]->setShader(shaders[FlyShader_gBufferBump]);

	return true;
}
bool FlyState_Level::_ImportStatic(wifstream& file, vector<IShader*>& shaders)	
{
	wstring dummy = L"";
	if(this->entryInstance->GetCoreInstance()->Geometry_Load(ReadString(file, dummy).c_str(), &this->levelEntities, FlyGeometry_Terrain, 1, 3))
	{
		int k = (int)this->levelEntities.size() - 1;
		this->levelEntities[k]->setPosition(ReadVector3(file));
		this->levelEntities[k]->setRotation(ReadVector3(file));
		this->levelEntities[k]->setScale(ReadVector3(file));
		this->levelEntities[k]->setShader(shaders[FlyShader_gBufferDefault]);
		dynamic_cast<Terrain*>(this->levelEntities[k])->TransformBoxes();
	}
	else
	{
		ReadVector3(file);
		ReadVector3(file);
		ReadVector3(file);
		return false;
	}
	return true;
}
bool FlyState_Level::_ImportPickups(wifstream& file, vector<IShader*>& shaders)	
{
	wstring fgmPath = L"";
	int nrOfStuff = ReadInt(file);

	for(int i = 0; i < nrOfStuff; i++)
	{
		//ReadString(file, fgmPath);
		fgmPath = L"..\\Resources\\Models\\cargo.fgm";
		vec3 p = ReadVector3(file);
		vec3 r = ReadVector3(file);
		vec3 s = ReadVector3(file);
		if(!this->pickups[i].Initialize(this->entryInstance, fgmPath, p, r, s, FlyShader_gBufferNoCull))
			return false;
	}
	return true;
}
bool FlyState_Level::_ImportEnergy(wifstream& file, vector<IShader*>& shaders)
{
	int nrOfStuff = ReadInt(file);
	wstring readString = L"";
	for(int i = 0; i < nrOfStuff; i++)
	{
		readString = L"..\\Resources\\Models\\energy.fgm";

		this->energy.push_back(EnergyPickup());
		vec3 p = ReadVector3(file);
		vec3 r = ReadVector3(file);
		vec3 s = ReadVector3(file);
		if(!this->energy[i].Initialize(this->entryInstance, readString, p, r, s, FlyShader_gBufferNoCull))
			return false;
	}

	return true;
}
bool FlyState_Level::_ImportLights(wifstream& file, vector<IShader*>& shaders)
{
	// Directional lights

	DirectionalLightProxy dirLightProxy;
	D3DXVECTOR3 pos; 
	D3DXVECTOR3 rot = vec3(0.0f, 0.0f, 0.0f);
	bool hasShadow;

	int nrOfStuff = ReadInt(file);
	for(int i= 0 ; i<nrOfStuff; i++)
	{
		dirLightProxy.ambient = ReadVector4(file);
		dirLightProxy.diffuse = ReadVector4(file);
		dirLightProxy.specular = ReadVector4(file);
		dirLightProxy.direction = D3DXVECTOR4(ReadVector3(file), 0);
	
		pos = ReadVector3(file);
		D3DXVec4Normalize(&dirLightProxy.direction, &dirLightProxy.direction);
		file>>hasShadow;

		D3DXMATRIX viewMatrix, projectionMatrix;
		//D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(1000, 200, 0), &D3DXVECTOR3(0,-1,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixLookAtLH(&viewMatrix, &pos, &D3DXVECTOR3(dirLightProxy.direction.x, dirLightProxy.direction.y, dirLightProxy.direction.z ), &D3DXVECTOR3(0.0, 1.0, 0.0));

		D3DXMatrixPerspectiveFovLH(&projectionMatrix,(float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 10000);
		//int w, h;this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
		//D3DXMatrixOrthoLH(&projectionMatrix, (float)w, (float)h, 0.1, 10000);

		Camera ShadowCamera; 
		ShadowCamera.SetPosition(vec3(0, 800, 0));
		ShadowCamera.SetRotation(vec3( 90, 0 , 0));

		int w = 0;
		int h = 0;
		this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
		ShadowCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, (float)w/h, 0.2f, 4000.0f);
		//ShadowCamera.SetOrthogonalMatrix(200, 400, 0.2f, 4000.0f); 
		ShadowCamera.Render(); 

		viewMatrix = ShadowCamera.GetViewMatrix(); 
		projectionMatrix = ShadowCamera.GetProjectionMatrix(); 
  
		LightViewProj *lightViewProj = new LightViewProj(); 
		lightViewProj->lView = viewMatrix;
		lightViewProj->lProj = projectionMatrix;
  
		DirectionLight* light = new DirectionLight(Type::LIGHT);
		DirectionLight::DIRLIGHT_DESC dirLight; 
		dirLight.data = dirLightProxy; 
		dirLight.viewProj = *lightViewProj;
		dirLight.shader =  shaders[FlyShader_DirLight];
 
		light->Initialize(dirLight);
  
		dirLights.push_back(light);

		if(hasShadow)
		{
			//this->lightCamera.SetProjectionMatrix(projectionMatrix);
			//this->lightCamera.SetViewMatrix(viewMatrix); 
			shadowViews.push_back(lightViewProj);
		}
  
	}

	// Pointlights
	nrOfStuff = ReadInt(file);
	for (int i = 0; i < nrOfStuff; i++)
	{
		PointLight* light = new PointLight(Type::LIGHT);
		PointLightProxy lightProxy; 
		lightProxy.ambient		= ReadVector4(file);
		lightProxy.diffuse		= ReadVector4(file);
		lightProxy.specular		= ReadVector4(file);
		lightProxy.posRange		= ReadVector4(file);
		light->Initialize(lightProxy,  shaders[FlyShader_PointLightBack]);
		pointLights.push_back(light);
	}

	return true;
}
bool FlyState_Level::_ImportPlayer(wifstream& file, vector<IShader*>& shaders)
{
	if(!this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\characterGlowAnim.fgm", this->player.GetModel(), FlyGeometry_AnimatedMesh, 1, 3))
		return false;
	
	this->player.GetModel()->at(0)->setPosition(ReadVector3(file));
	this->player.GetModel()->at(0)->setRotation(ReadVector3(file));
	this->player.GetModel()->at(0)->setScale(ReadVector3(file));
	this->player.GetModel()->at(0)->setShader(shaders[FlyShader_gBufferDefault]);

	this->mainCamera.SetPosition(this->player.GetPosition());
	this->mainCamera.SetRotation(this->player.GetRotation());


	int w = 0;
	int h = 0;
	this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
	this->mainCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, (float)w/h, 0.2f, 4000.0f);
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);

	//Set player bounding info
	BoundingSphere* playerSphere = new BoundingSphere;
	playerSphere->center = this->mainCamera.GetPosition();
	playerSphere->radius = 1.5f;
	this->player.SetBoundingSphere(playerSphere);

	return true;
}


vec4 FlyState_Level::ReadVector4(wifstream& in)
{
	vec4 ret = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	in >> ret.x;
	in >> ret.y;
	in >> ret.z;
	in >> ret.w;

	return ret;
}
vec3 FlyState_Level::ReadVector3(wifstream& in)
{
	vec3 ret = vec3(0.0f, 0.0f, 0.0f);

	in >> ret.x;
	in >> ret.y;
	in >> ret.z;

	return ret;
}
int FlyState_Level::ReadInt(wifstream& in)
{
	int i = 0;
	in >> i;
	return i;
}
wstring& FlyState_Level::ReadString(wifstream& in, wstring& outStr)
{
	in >> outStr;
	return outStr;
}



