#include "FlyState_Menu.h"
#include "..\FlyGame.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Core\Input.h"
#include "..\..\FlyGameEngine\Util\Camera.h"
#include "..\..\FlyGameEngine\Core\D3DShell.h"
#include "..\..\FlyGameEngine\Core\WindowShell.h"
#include "..\..\FlyGameEngine\Util\CollisionLib.h"
#include "..\..\FlyGameEngine\Util\MutexHandler.h"
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
	MENU_UI_MouseBtn,
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
		delete this->ui[i];
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
	
	this->entryInstance->GetCoreInstance()->Audio_LoadMenuSound();
	this->entryInstance->GetCoreInstance()->Audio_PlayMenuSound(FlySound_MenuSoundTrack);

	int w,h;
	this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
	this->mainMenuCam.SetOrthogonalMatrix((float)w, (float)h, 0.1f, 10.0f);
	this->mainMenuCam.Render();
	this->mainMenuCam.SetPosition(0.0f, 0.0f, -5.0f);
	
	MutexHandler::SetMutex(MutexResource_ALL, true);
	if(!ReadData())
		return false;
	MutexHandler::SetMutex(MutexResource_ALL);


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
	this->entryInstance->GetCoreInstance()->Gfx_SetCamera(&this->mainMenuCam, true);

	this->entryInstance->GetCoreInstance()->Gfx_Update();

	this->entryInstance->GetCoreInstance()->Audio_Update();

	for (int i = 0; i < (int)this->ui.size(); i++)
	{
		this->ui[i]->Update();
	}

	input();
}
void FlyState_Menu::render()
{
	this->entryInstance->GetCoreInstance()->Gfx_BeginDeferredScene();

	ViewFrustum f;
	this->mainMenuCam.ConstructViewFrustum(f);

	this->ui[MENU_UI_Background]->Render(f);
	this->ui[MENU_UI_LevelPanel]->Render(f);

	if(this->highlightBtn != -1)
		this->ui[this->highlightBtn]->Render(f);

	if(this->subMenu)
		this->subMenu->Render(f);

	this->ui[MENU_UI_MouseBtn]->Render(f);

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredSceneOrtho();
}
void FlyState_Menu::input()
{
	Input::self()->Frame();
	int x, y;

	Input::self()->GetMouseLocation(x, y);
	
	this->PickMenu();

	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
		this->entryInstance->setState(0);

	if(Input::self()->IsMouseButtonPressed(0) && this->highlightBtn != -1)
	{
		this->entryInstance->GetCoreInstance()->Audio_PlayMenuSound(FlySound_MenuClick);

		switch (this->highlightBtn)
		{
			case MENU_UI_ButtonLv1:
				this->subMenu = this->ui[MENU_UI_LevelInfo1];
			break;
			case MENU_UI_ButtonLv2:
				this->subMenu = this->ui[MENU_UI_LevelInfo2];
			break;
			case MENU_UI_ButtonLv3:
				this->subMenu = this->ui[MENU_UI_LevelInfo3];
			break;
			case MENU_UI_ButtonStart:
			{
				int cargoCount = this->entryInstance->getCargoCount(); 
				if(!this->subMenu)
					return;

				int id = this->subMenu->getID();
				if(id == this->ui[MENU_UI_LevelInfo1]->getID())
				{
					this->entryInstance->setLevelPath(L"..\\Resources\\Levels\\canyon.fgl");
					this->entryInstance->setState(Level_1);
				}
				else if(id == this->ui[MENU_UI_LevelInfo2]->getID())
				{
					//locked
					if(cargoCount < 2)
					{
						this->subMenu = this->ui[MENU_UI_LevelInfo3];
					}
					else
					{
						this->entryInstance->setLevelPath(L"..\\Resources\\Levels\\ocean.fgl");	
						this->entryInstance->setState(Level_2);
					}
				}
				else if(id == this->ui[MENU_UI_LevelInfo3]->getID())
				{
					//locked
					if(cargoCount < 4)
					{
						this->subMenu = this->ui[MENU_UI_LevelInfo3];
					}
					else
					{
						this->entryInstance->setLevelPath(L"..\\Resources\\Levels\\city.fgl");
						this->entryInstance->setState(Level_3);
					}
				}
			}
			break;
			case MENU_UI_ButtonQuit:
				PostQuitMessage(0);
			break;
			default:
				this->subMenu = 0;
			break;
		}
	}
	else if(Input::self()->IsMouseButtonPressed(0))
	{
		this->subMenu = 0;
	}

	
	FlyMesh* m = (FlyMesh*)this->ui[MENU_UI_MouseBtn];
	vec3 mPos;
	vec3 t1;
	vec3 t2;
	D3DXVec3TransformCoord(&t1, &(*m->GetTriangles())[0], &m->getWorld());
	D3DXVec3TransformCoord(&t2, &(*m->GetTriangles())[1], &m->getWorld());

	float hw = fabs(t2.x - t1.x) * 0.5f;
	float hh = fabs(t2.y - t1.y) * 0.5f;

	mPos.x = (float)x - (D3DShell::self()->getWidth()/2.0f) + hw;
	mPos.y = (float)-(y - D3DShell::self()->getHeight()/2) - (hh);
	mPos.z = m->getPosition().z;
	m->setPosition(mPos);

}



void FlyState_Menu::PickMenu()
{
	int mx = 0;
	int my = 0;
	int w = -1;
	int h = -1;
	vec3 v;
	vec3 rayOrigin;
	vec3 rayDir;

	this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
	Input::self()->GetMouseLocation(mx, my);
	Matrix P = this->mainMenuCam.GetOrthogonalMatrix();
	Matrix V = this->mainMenuCam.GetViewMatrix();
	

	// Compute picking ray in view space.
	v.x =  ( ( ( +2.0f * mx ) / w ) - 1.0f ) / P._11;
	v.y = -( ( ( +2.0f * my ) / h ) - 1.0f ) / P._22;
	v.z = 1.0f;

	int lastBtn = this->highlightBtn;

	for(int i = MENU_UI_ButtonLv1; i <= MENU_UI_ButtonQuit; i++)
	{
		if(i == MENU_UI_ButtonStart && !this->subMenu)
			continue;

		Matrix WV = this->ui[i]->getWorld() * V;


		Matrix invW;
		vec3 pInW;
		D3DXMatrixInverse(&invW, 0, &this->ui[i]->getWorld());
		D3DXVec3TransformCoord(&pInW, &v, &invW);


		//Do ray triangle intersect
		FlyMesh* temp = (FlyMesh*)ui[i];
		vector<vec3> *tris = temp->GetTriangles();
		if(tris)
		{
			for (int k = 0; k < (int)tris->size(); k+=3)
			{
				vec3 tri[3] = 
				{
					(*tris)[k],
					(*tris)[k + 1],
					(*tris)[k + 2]
				};

				if(RayVSTriangle(pInW, vec3(0.0f, 0.0f, 1.0f), tri))
				{
					if(lastBtn == -1)
						this->entryInstance->GetCoreInstance()->Audio_PlayMenuSound(FlySound_MenuHover);

					this->highlightBtn = i;
					return;
				}
				else
					this->highlightBtn = -1;
			}
		}
		
	}
	
}



bool FlyState_Menu::ReadData()
{
	wifstream in("..\\Resources\\Menu\\mainMenuLayout.fgl");
	if(!in.is_open())
		return false;
	
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

	std::vector<vec3>* t = ((FlyMesh*)this->uiBtn[MENU_UI_Background])->GetTriangles();

	//Get position offset
	this->ui[MENU_UI_Background]->Update();
	Matrix w = this->ui[MENU_UI_Background]->getWorld();
	Matrix v = this->mainMenuCam.GetViewMatrix();
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