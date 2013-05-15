#include "FlyState_Level.h"
#include "..\FlyGame.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Util\CollisionLib.h"

#include <fstream>
#include <iostream>
#include <string>

FlyState_Level::FlyState_Level()
{

}
FlyState_Level::~FlyState_Level()
{

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

	this->mainCamera.Render();
}
void FlyState_Level::Frame()
{
	this->mainTimer->Frame();

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
	vec3 oldPosition = this->player.GetPosition();

	//Move camera and bird
	this->mainCamera.SetPosition(oldPosition);
	this->Input();

	this->mainCamera.RelativeForward(this->player.GetVelocity().z);
	this->mainCamera.RelativeRight(this->player.GetVelocity().x);
	this->mainCamera.RelativeUp(this->player.GetVelocity().y);
	this->mainCamera.SetPositionY(this->mainCamera.GetPosition().y-0.01f);
	if(!Input::self()->IsButtonPressed(DIK_A))
		this->mainCamera.RelativeYaw(this->player.GetVelocity().x*3.0f);
 
	this->player.SetPosition(this->mainCamera.GetPosition());
	this->player.GetBoundingSphere()->center = this->player.GetPosition();
	//Check for slide collision for birds location
	D3DXVECTOR3 velocity = this->player.GetPosition() - oldPosition;

	this->player.SetPosition(SlideCollision(oldPosition, velocity, 200, oldPosition));

	this->player.GetBoundingSphere()->center = this->player.GetPosition();
	this->mainCamera.SetPosition(this->player.GetPosition());

	this->mainCamera.Render();
	vec3 newCameraPos = this->player.GetPosition() - this->mainCamera.GetForward()*10.0f + this->mainCamera.GetUp()*0.7f;
	this->mainCamera.SetPosition(newCameraPos.x, newCameraPos.y, newCameraPos.z);

	//Tilt the player model according to sideways velocity
	if(this->player.GetVelocity().x < -0.001f && !Input::self()->IsButtonPressed(DIK_A))
	{
		float howMuch = this->player.GetVelocity().x*-10.0f;
		this->player.SetRotation(vec3((float)D3DX_PI*0.25f*howMuch, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	else if(this->player.GetVelocity().x > 0.001f)
	{
		float howMuch = this->player.GetVelocity().x*10.0f;
		this->player.SetRotation(vec3((float)-D3DX_PI*0.25f*howMuch, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	else
	{
		this->player.SetRotation(vec3(0.0f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	//END

	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		if(SphereVSSphere(*this->levelPickups[i]->getBoundingSphere(), *this->player.GetBoundingSphere()))
		{
			this->levelPickups.erase(this->levelPickups.begin()+i);
			i--;
		}
	}

	return true;
}

bool FlyState_Level::Render()
{
	this->entryInstance->GetCoreInstance()->Gfx_Update();
			
	this->entryInstance->GetCoreInstance()->Gfx_BeginDeferredScene();

	ViewFrustum f;
	this->mainCamera.ConstructViewFrustum(f);

	this->theWorld[0]->Render(f);

	this->player.Render(f);

	for(unsigned int i = 0; i < this->levelEntities.size(); i++)
	{	
		this->levelEntities[i]->Render(f);
	}
	for(unsigned int i = 0; i < this->levelPickups.size(); i++)
	{
		this->levelPickups[i]->Render(f);
	}

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredScene();

	return true;
}

bool FlyState_Level::MenuRender()
{
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

	this->cursor[0]->setPosition(vec3((float)mouseX-600.0f, -(float)(mouseY-300.0f) , -1.0f));

	return true;
}

void FlyState_Level::Input()
{
	Input::self()->Frame();

	int mouseX, mouseY;

	Input::self()->GetMouseRelative(mouseX, mouseY);

	//Advanced
	/*if(this->mainCamera.GetForward().y < 0.0f)
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	}
	if(this->mainCamera.GetForward().y > 0.0f)
	{
		if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.001f));
	}*/

	/*if(mouseY < 0)
	{
		this->mainCamera.RelativePitch(0.1f*mouseY);
	}
	if(mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.1f*mouseY);
	}*/
	/*if(mouseX < 0)
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
	}*/
	//Advanced
	if(Input::self()->IsButtonPressed(DIK_W))
	{
		this->mainCamera.RelativePitch(0.4f);
	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		this->mainCamera.RelativePitch(-0.4f);
	}
	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		this->player.GetBoundingSphere()->radius = 1.0f;
	}
	
	if(Input::self()->IsButtonPressed(DIK_D))
	{
		//this->mainCamera.RelativePitch(-0.2f);
	}
	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		this->player.GetBoundingSphere()->radius = 1.0f;
	}

	if(Input::self()->IsButtonPressed(DIK_A))
	{
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
		this->player.SetRotation(vec3(this->player.GetRotation().x-this->player.GetVelocity().x*0.4f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	else if(Input::self()->IsMouseButtonPressed(1))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().x < -0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
	}

	if(Input::self()->IsMouseButtonPressed(0))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
		if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().x > 0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
	}

	//Casual
	/*if(mouseY < 0)
	{
		this->mainCamera.RelativePitch(0.1f*mouseY);
	}
	if(mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.1f*mouseY);
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

	if(Input::self()->IsButtonPressed(DIK_LCONTROL))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
	}
	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.001f));
	}*/
	
	

	if(Input::self()->IsButtonPressed(DIK_F1))
	{
		this->player.SetPosition(vec3(0.0f, 10.0f, 0.0f));
	}
	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
	{
		this->state = this->state = 1;
	}
}

bool FlyState_Level::ReadLevel(const wchar_t* fileName)
{
	wifstream file(fileName);
	vec3 readVector;
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
						
	this->theWorld[0]->setShader(shaders[readInt]);

	//Read rest of objects
	for(int i = 0; i < nrOfStuff-1; i++)
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

		this->levelEntities[i]->setShader(shaders[readInt]);
	}

	file>>nrOfStuff;

	//Pick-ups
	for(int i = 0; i < nrOfStuff; i++)
	{
		file>>readString;
		this->entryInstance->GetCoreInstance()->Geometry_Load(readString.c_str(), &this->levelPickups);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelPickups[i]->setPosition(readVector);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->levelPickups[i]->setRotation(readVector);
		file>>readInt;

		this->levelPickups[i]->setShader(shaders[readInt]);
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
	this->mainCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, 1200.0f/600.0f, 8.0f, 400.0f);
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainCamera);
	
	file.close();

	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\character.fgm", this->player.GetModel());
	this->player.GetModel()->at(0)->setShader(shaders[1]);
	this->player.SetPosition(this->mainCamera.GetPosition());

	BoundingSphere* sphere1 = new BoundingSphere;
	BoundingSphere* sphere2 = new BoundingSphere;
	BoundingSphere* sphere3 = new BoundingSphere;
	

	sphere1->center = vec3(0.0f, 10.0f, 0.0f);
	sphere1->radius = 1.0f;
	this->levelPickups[0]->setBoundingSphere(sphere1);

	sphere2->center = vec3(10.0f, 10.0f, 10.0f);
	sphere2->radius = 1.0f;
	this->levelPickups[1]->setBoundingSphere(sphere2);

	sphere3->center = vec3(0.0f, 10.0f, 5.0f);
	sphere3->radius = 1.0f;
	this->levelPickups[2]->setBoundingSphere(sphere3);

	BoundingSphere* sphere4 = new BoundingSphere;
	sphere4->center = this->mainCamera.GetPosition();
	sphere4->radius = 2.0f;
	this->player.SetBoundingSphere(sphere4);

	//Game Menu
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\in_game_menu.fgm", &this->gameMenu);
	this->gameMenu[0]->setShader(shaders[1]);
	this->gameMenu[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));

	//Cursor
	this->entryInstance->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->cursor);
	this->cursor[0]->setShader(shaders[1]);
	this->cursor[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));
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

	if (D3DXVec3Length(&velocity) <= D3DXVec3Length(&this->player.GetVelocity())*0.01f)
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

	for(unsigned int u = 0; u < this->levelEntities.size(); u++)
	{
		tempEntity = this->levelEntities[u];
		triangleList = *(dynamic_cast<FlyMesh*>(tempEntity)->GetTriangles());

		D3DXMATRIX rotation, translation, world, invWorld;
		D3DXMatrixRotationYawPitchRoll(&rotation, this->levelEntities[u]->getRotation().y, this->levelEntities[u]->getRotation().x, this->levelEntities[u]->getRotation().z);
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
 
			if(stuckD <= this->player.GetBoundingSphere()->radius) return safeLocation - pNormal*0.1f;			

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

			D3DXVECTOR3 normalizedVelocity;
			D3DXVec3Normalize(&normalizedVelocity,&(velocity));

			// Plane origin/normal
			D3DXVECTOR3 pOrigin = triangleList.at(i);
			D3DXVECTOR3 pNormal;
			D3DXVec3Cross(&pNormal, &(triangleList.at(i+1) - triangleList.at(i)), &(triangleList.at(i+2) - triangleList.at(i)));
			D3DXVec3Normalize(&pNormal, &pNormal);


			// Determine the distance from the plane to the source
			D3DXVECTOR3 sphereIntersectionPoint = oldPosition - pNormal*this->player.GetBoundingSphere()->radius;
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
     
				distToSphereIntersection = intersectSphere(polyIPoint, -normalizedVelocity, oldPosition, this->player.GetBoundingSphere()->radius);  
                  
				// we cannot know if the ray will actually hit the sphere so we need this check
				if (distToSphereIntersection > 0) 
				{ 	
					//calculate true ellipsoid intersection point
					sphereIntersectionPoint = polyIPoint + distToSphereIntersection*-normalizedVelocity;
				}
			}

			float stuckD = D3DXVec3Length(&(polyIPoint-oldPosition));
 
			if(stuckD <= this->player.GetBoundingSphere()->radius) return safeLocation + pNormal*0.1f;		

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


	if(!foundCollision)
	{
		float velocityLength = D3DXVec3Length(&velocity);
		D3DXVECTOR3 normVelo;
		D3DXVec3Normalize(&normVelo, & velocity);
		D3DXVECTOR3 V = normVelo*(velocityLength - D3DXVec3Length(&this->player.GetVelocity())*0.01f); 
     
		// return the final position
		return oldPosition + V;
	}

	D3DXVECTOR3 newPosition;

	if(nearestDistance >= D3DXVec3Length(&this->player.GetVelocity())*0.01f)
	{
		D3DXVECTOR3 normVelo;
		D3DXVec3Normalize(&normVelo, & velocity);
		D3DXVECTOR3 V = normVelo*(nearestDistance - D3DXVec3Length(&this->player.GetVelocity())*0.01f);

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
	for(unsigned int i = 0; i < this->gameMenu.size(); i++)
	{
		delete this->gameMenu[i];
	}
	for(unsigned int i = 0; i < this->cursor.size(); i++)
	{
		delete this->cursor[i];
	}

	this->player.Release();
	
}