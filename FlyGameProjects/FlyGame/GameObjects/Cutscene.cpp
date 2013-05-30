#include "Cutscene.h"

#include "..\..\FlyGameEngine\Core\Input.h"
#include "..\..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\..\FlyGameEngine\Core\Mesh\Plane.h"
#include "..\..\FlyGameEngine\Core\WindowShell.h"
#include "..\..\FlyGameEngine\Util\Camera.h"
#include "..\..\FlyGameEngine\Util\Importer\ResourceImporter.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"


void doScene();
void doInput();
bool doLoad(const wchar_t* scene);

int current = 0;
vector<SmartPtrStd<Entity>> parts;
Camera cam;
bool done = false;
bool keyPressed = false;
bool rendered = false;
FlyEngine* core;
float transitionTime = 1.0f;


bool FlyCutscene::RunCutscene(FlyCutsceneType scene, FlyEngine* _core)
{
	if(!_core)
		return false;
	core = _core;


	const wchar_t* p = L"";
	
	switch (scene)
	{
		case FlyCutsceneType_Intro:
			p = L"..\\Resources\\cutscene\\intro\\dat.txt";
		break;

		case FlyCutsceneType_Level1:
			p = L"..\\Resources\\cutscene\\level1\\dat.txt";
		break;

		case FlyCutsceneType_Level2:
			p = L"..\\Resources\\cutscene\\level2\\dat.txt";
		break;

		case FlyCutsceneType_Level3:
			p = L"..\\Resources\\cutscene\\level3\\dat.txt";
		break;

		default:
			return false;
	}

	if(!doLoad(p))
		return false;

	doScene();

	return true;
}

void doScene()
{
	ViewFrustum f;
	cam.ConstructViewFrustum(f);

	while (!done)
	{
		doInput();

		if(!rendered && current < (int)parts.size())
		{
			core->Gfx_BeginDeferredScene();
			parts[current]->Render(f);
			core->Gfx_EndDeferredSceneOrtho();
			rendered = true;
		}
	}

	for (int i = 0; i < (int)parts.size(); i++)
	{
		if(parts[i]->getMaterial())
		{
			MaterialHandler::RemoveMaterial(parts[i]->getMaterial()->GetID());
			parts[i].Destroy();
		}
	}
	parts.clear();
	core = 0;
	current = 0;
	done = false;
	keyPressed = false;
}
void doInput()
{
	Input::self()->Frame();

	if(Input::self()->IsMouseButtonPressed(0) && !keyPressed)
	{
		keyPressed = true;
	}
	else if(!Input::self()->IsMouseButtonPressed(0) && keyPressed) //Released
	{
		keyPressed = false;
		current ++;

		if(current == (int)parts.size())
			done = true;

		rendered = false;
	}
	
}
bool doLoad(const wchar_t* scene)
{
	wifstream in(scene);
	wstring data;

	if(!in.is_open())
		return false;

	float width = D3DShell::self()->getWidth()*0.5f;
	float height = D3DShell::self()->getHeight()*0.5f;

	SmartPtrStd<vector<VERTEX::VertexPNT>> v = new vector<VERTEX::VertexPNT>(6);
	VERTEX::VertexPNT mesh[] =
	{
		{ D3DXVECTOR4(-width, +height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(+width, +height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(-width, -height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(-width, -height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(+width, +height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(+width, -height, 0.0f, 1.0f)	,D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) }
	};
	for (int i = 0; i < 6; i++)
	{
		(*v)[i] = mesh[i];
	}
	
	while (!in.eof())
	{
		wchar_t temp[255];
		in.getline(temp, 255, L'\n');
		data = temp;
		if(data.size() > 0)
		{
			ObjectMaterial::OBJECT_MATERIAL_DESC md;
			md.dc				= D3DShell::self()->getDeviceContext();
			md.device			= D3DShell::self()->getDevice();
			md.name				= L"data";
			md.ambient			= vec4(1.0f, 1.0f, 1.0f, 1.0f);
			md.diffuse			= vec4(1.0f, 1.0f, 1.0f, 1.0f);
			md.specular			= vec4(1.0f, 1.0f, 1.0f, 1.0f);
			md.specualarPow		= 5;
			md.ambientTexture	= L"";
			md.diffuseTexture	= data;
			md.specularTexture	= L"";
			md.glowTexture		= L"";
			md.normalTexture	= L"";
			int i = MaterialHandler::AddMaterial(md);

			if(i == -1)			return false;

			FlyMesh::OBJECT_DESC fd;
			fd.device			= D3DShell::self()->getDevice();
			fd.deviceContext	= D3DShell::self()->getDeviceContext();
			fd.material_id		= i;
			fd.name				= data;
			fd.vCount			= 6;
			fd.vertecies		= v;
			fd.shader			= core->Gfx_GetShader(FlyShader_gBufferDefault);
			FlyMesh* a = new FlyMesh();
			if(!a->Initialize(fd))
				return false;
			parts.push_back(a);
		}
	}

	in.close();

	if(parts.size() == 0)
		return false;

	cam.SetPosition(0.0f, 0.0f, -2.0f);
	cam.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 0.1f, 10.0f);
	cam.Render();
	core->Gfx_SetCamera(&cam, true);

	return true;
}