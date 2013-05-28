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
	
	if(!entryInstance->GetCoreInstance()->Input_Initialize())
		return false;
	
	if(!ReadData())
		return false;

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
	this->ui[MENU_UI_MouseBtn]->Render(f);
	//if(this->highlightBtn)
	//{
	//	this->highlightBtn->Render(f);
	//
	//}
	for (int i = 0; i < (int)this->uiBtn.size(); i++)
	{
		if(this->uiBtn[i] != this->highlightBtn)
			this->uiBtn[i]->Render(f);
	}
	//for (int i = 0; i < (int)ui.size(); i++)
	//{
	//	ui[i]->Render(f);
	//}

	this->entryInstance->GetCoreInstance()->Gfx_EndDeferredSceneOrtho();
}
void FlyState_Menu::input()
{
	Input::self()->Frame();
	int x, y;

	Input::self()->GetMouseLocation(x, y);
	
	this->PickMenu();

	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
		PostQuitMessage(0);

	
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
	int mx, my;
	int w, h;
	Camera* cam = this->entryInstance->GetCoreInstance()->Gfx_GetCamera();
	Input::self()->GetMouseLocation(mx, my);
	this->entryInstance->GetCoreInstance()->Core_Dimensions(w, h);
	vec3 picRayOrig;
	vec3 picRayDir;
	Matrix proj = this->entryInstance->GetCoreInstance()->Gfx_GetCamera()->GetOrthogonalMatrix();
	
	 // Compute the vector of the pick ray in screen space
	vec3 v;
	v.x = ( ( ( 2.0f * mx ) / w ) - 1 ) / proj._11;
	v.y = -( ( ( 2.0f * my ) / h ) - 1 ) / proj._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	const D3DXMATRIX matView = cam->GetViewMatrix();
	const D3DXMATRIX matWorld = cam->GetWorldMatrix();
	D3DXMATRIX mWorldView = matWorld * matView;
	D3DXMATRIX m;
	D3DXMatrixInverse( &m, NULL, &mWorldView );
	
	// Transform the screen space pick ray into 3D space
	picRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	picRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	picRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	picRayOrig.x = m._41;
	picRayOrig.y = m._42;
	picRayOrig.z = m._43;

	for(int i = 0; i < (int)this->uiBtn.size(); i++)
	{
		// Now get the inverse of the translated world matrix.
		//D3DXMatrixInverse(&iw, NULL, &(uiBtn[i]->getWorld()));
	
		// Now transform the ray origin and the ray direction from view space to world space.
		//D3DXVec3TransformCoord(&rayOrigin, &origin, &iw);
		//D3DXVec3TransformNormal(&rayDirection, &direction, &iw);
	
		// Normalize the ray direction.
		//D3DXVec3Normalize(&rayDirection, &rayDirection);
	
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
	
				if(RayVSTriangle(picRayOrig, picRayDir, tri))
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


bool FlyState_Menu::ReadData()
{
	wifstream in("..\\Resources\\Menu\\mainMenuLayout.fgl");
	if(!in.is_open())
		return false;

	this->entryInstance->GetCoreInstance()->Gfx_GetDefaultCamera()->SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 0.1f, 10.0f);
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