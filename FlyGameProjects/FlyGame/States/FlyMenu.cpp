#include "FlyState_Menu.h"
#include "..\FlyGame.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Core\Input.h"
#include "..\..\FlyGameEngine\Util\Camera.h"
#include "..\..\FlyGameEngine\Core\D3DShell.h"
#include "..\..\FlyGameEngine\Core\WindowShell.h"
#include "..\..\FlyGameEngine\Util\CollisionLib.h"
#include <iostream>

enum MenuLayout
{
	MENU_UI_Background,
	MENU_UI_LevelPanel,
	MENU_UI_LevelInfo1,
	MENU_UI_LevelInfo2,
	MENU_UI_LevelInfo3,
	MENU_UI_ButtonLv1,
	MENU_UI_ButtonLv2,
	MENU_UI_ButtonLv3,
	MENU_UI_ButtonStart,
	MENU_UI_ButtonQuit,
	MENU_UI_ITEM_COUNT
};


FlyState_Menu::FlyState_Menu()
{
	this->subMenu		= 0;
	this->highlightBtn	= 0;
}
FlyState_Menu::~FlyState_Menu()
{
	for (int i = 0; i < (int)this->ui.size(); i++)
	{
		delete this->ui[i];
	}
}

void Read_V_VT_VN(Entity* obj, wifstream& in)
{
	vec3 temp;
	//position
	in >> temp.x;
	in >> temp.y;
	in >> temp.z;
	obj->setPosition(temp);
	//rotation
	in >> temp.x;
	in >> temp.y;
	in >> temp.z;
	obj->setRotation(temp);
	//scale
	in >> temp.x;
	in >> temp.y;
	in >> temp.z;
	obj->setScale(temp);
}
bool FlyState_Menu::Initiate(FlyGame* instance)
{
	if(!instance)
		return false;
	this->entryInstance = instance;
	
	if(!entryInstance->GetCoreInstance()->Input_Initialize())
		return false;
	
	wifstream in("..\\Resources\\Levels\\menuLayout.fgl");
	if(!in.is_open())
		return false;

	this->entryInstance->GetCoreInstance()->Gfx_GetDefaultCamera()->SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 0.1f, 10.0f);
	//this->entryInstance->GetCoreInstance()->Gfx_GetDefaultCamera()->SetOrthogonalMatrix(1920.0f, 1080.0f, 0.1f, 10.0f);
	this->entryInstance->GetCoreInstance()->Gfx_GetDefaultCamera()->SetPosition(vec3(0.0f, 0.0f, -5.0f));
	this->entryInstance->GetCoreInstance()->Gfx_GetDefaultCamera()->Render();
	
	FlyMesh::OBJECT_DESC d;
	wstring inData = L"";
	float sx = 1.0f;
	float sy = 1.0f;
	bool scaled = false;
	float offset = 0.0f;
	//__________________________________//
	//			Load menu items			//
	//__________________________________//
	do
	{
		in >> inData;
		if(!in.eof())
		{
			if(!this->entryInstance->GetCoreInstance()->Geometry_Load(inData.c_str(), &this->ui))
				return false;
			Read_V_VT_VN(this->ui[this->ui.size()-1], in);
			this->ui[this->ui.size()-1]->setShader(this->entryInstance->GetCoreInstance()->Gfx_GetShader(FlyShader_gBufferDefault));
			
			if(!scaled)
			{
				//Get scale of bg only
				sx = D3DShell::self()->getWidth()/(fabsf((*((FlyMesh*)this->ui[this->ui.size()-1])->GetTriangles())[0].x)*2);
				sy = D3DShell::self()->getHeight()/(fabsf((*((FlyMesh*)this->ui[this->ui.size()-1])->GetTriangles())[0].y)*2);
				
				scaled = true;
			}
			this->ui[this->ui.size()-1]->setScale(vec3(sx, sy, 1.0f));
			this->ui[this->ui.size()-1]->setBoundingSphere(0);

			switch ((int)this->ui.size()-1)
			{
				case MENU_UI_ButtonLv1:
				case MENU_UI_ButtonLv2:
				case MENU_UI_ButtonLv3:
				case MENU_UI_ButtonStart:
				case MENU_UI_ButtonQuit:
					this->uiBtn.push_back(this->ui[this->ui.size()-1]);
				break;
			}
		}
	} while(!in.eof());

	//Get position offset
	this->ui[MENU_UI_Background]->Update();
	Matrix w = this->ui[MENU_UI_Background]->getWorld();
	Matrix v = this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetViewMatrix();
	Matrix wv = w*v;
	vec3 u = (*((FlyMesh*)this->ui[MENU_UI_Background])->GetTriangles())[0];
	vec4 pnew4;
	D3DXVec3Transform(&pnew4, &u, &wv);
	vec3 p = vec3(pnew4.x, pnew4.y, 0.0f);

	for (int i = 2; i < (int)ui.size(); i++)
	{
		vec3 px = vec3(fabsf(this->ui[i]->getPosition().x - u.x), 0.0f, 0.0f);
		vec3 py = vec3(0.0f, fabsf(this->ui[i]->getPosition().y - u.y), 0.0f);
		D3DXVec3Scale(&px, &px, sx);
		D3DXVec3Scale(&py, &py, sy);
		vec3 np = p + px - py;
		np.z = this->ui[i]->getPosition().z;
		this->ui[i]->setPosition(np);
	}

	return true;
}
void FlyState_Menu::Frame() 
{
	update();
	render();
}
void FlyState_Menu::Release()
{  }
void FlyState_Menu::update()
{
	this->entryInstance->GetCoreInstance()->Gfx_Update();

	for (int i = 0; i < (int)this->ui.size(); i++)
	{
		this->ui[i]->Update();
	}
	//if(this->highlightBtn)
	//{
	//	this->highlightBtn->Update();
	//}


	input();
}
void FlyState_Menu::render()
{
	this->entryInstance->GetCoreInstance()->Gfx_BeginDeferredScene();

	ViewFrustum f;
	this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->SetPositionZ(-2.0f);
	this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->ConstructViewFrustum(f);

	this->ui[MENU_UI_Background]->Render(f);
	this->ui[MENU_UI_LevelPanel]->Render(f);
	//if(this->highlightBtn)
	//{
	//	this->highlightBtn->Render(f);
	//}
	for (int i = 0; i < this->uiBtn.size(); i++)
	{
		if(this->uiBtn[i] != this->highlightBtn)
			this->uiBtn[i]->Render(f);
	}
	//for (int i = 0; i < ui.size(); i++)
	//{
	//	ui[i]->Render(f);
	//}

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredSceneOrtho();
}
void FlyState_Menu::input()
{
	Input::self()->Frame();
	//int x, y;
	
	this->PickMenu();

	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
		PostQuitMessage(0);
	
}


void FlyState_Menu::PickMenu()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(WindowShell::self()->getHWND(), &p);

	Entity* retVal = 0;
	float pointX, pointY;
	D3DXMATRIX inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, rayOrigin, rayDirection;
	D3DXVECTOR3 origin = entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetPosition();

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)p.x) / (float)D3DShell::self()->getWidth()) - 1.0f;
	pointY = -(((2.0f * (float)p.y) / (float)D3DShell::self()->getHeight()) - 1.0f);
		
	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	pointX = pointX / this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetOrthogonalMatrix()._11;
	pointY = pointY / this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetOrthogonalMatrix()._22;	

	// Get the inverse of the view matrix.
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetViewMatrix());
	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	std::cout << "(" << direction.x << ", " << direction.y << direction.z << ")" << std::endl;
	std::cout << "(" << direction.x << ", " << direction.y << direction.z << ")" << std::endl;

	for(int i = 0; i < (int)this->uiBtn.size(); i++)
	{
		// Now get the inverse of the translated world matrix.
		D3DXMatrixInverse(&inverseWorldMatrix, NULL, &uiBtn[i]->getWorld());

		// Now transform the ray origin and the ray direction from view space to world space.
		D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

		// Normalize the ray direction.
		D3DXVec3Normalize(&rayDirection, &rayDirection);

		FlyMesh* temp = (FlyMesh*)uiBtn[i];
		vector<vec3> *tris = temp->GetTriangles();
		if(tris)
		{
			for (int i = 0; i < (int)tris->size(); i+=3)
			{
				vec3 tri[3] = 
				{
					(*tris)[i],
					(*tris)[i + 1],
					(*tris)[i + 2]
				};

				if(RayVSTriangle(rayOrigin, rayDirection, tri))
				{
					this->highlightBtn = uiBtn[i];
					return;
				}
				else
					this->highlightBtn = 0;
			}
		}
	}
}