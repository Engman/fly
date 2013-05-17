#include "FlyLevelEditor_impl.h"

#include "..\FlyGameEngine\Core\Entity.h"
#include "..\FlyGameEngine\Util\Camera.h"
#include "..\FlyGameEngine\Util\CollisionLib.h"
#include "..\FlyGameEngine\Core\D3DShell.h"
#include "..\FlyGameEngine\Util\Timer.h"


FlyLevelEditor_impl *gEditorInstance = 0;
bool selectedDrag = false;
int lastMouseX = 0;
int lastMouseY = 0;
bool gFlyMode = false;
float gSpeed = 0.01f;
bool gRotateCamera = false;
bool gMoveCameraScroll = false;
bool gKeyW = false;
bool gKeyA = false;
bool gKeyS = false;
bool gKeyD = false;




FlyLevelEditor* FLYCALL GetEditorInstance()
{
	if(!gEditorInstance)
		gEditorInstance = new FlyLevelEditor_impl();

	return gEditorInstance;
}



FlyLevelEditor_impl::FlyLevelEditor_impl()
{
	this->core = FlyEngineCreate();
	this->selected = NULL;
}
FlyLevelEditor_impl::~FlyLevelEditor_impl()
{
	this->selected = 0;
}



void FLYCALL FlyLevelEditor_impl::Terminate()
{
	this->core->Core_Shutdown();
	delete gEditorInstance;
}
bool FLYCALL FlyLevelEditor_impl::Initiate(HWND parent, int width, int height)
{
	//Some error checking
	if( !parent )			return false;
	if( width < 50 )		return false;
	if( height < 50 )		return false;
	if( !gEditorInstance)	return false;
	if( !this->core)		return false;

	FLY_ENGINE_INIT_DESC d;
	d.winWidth = width;
	d.parent = parent;
	d.winHeight = height;
	
	if(!this->core->Core_Initialize(d)) return false;

	this->core->Gfx_GetCamera()->SetPosition(0.0f, 100.0f, -100.0f);
	this->core->Gfx_GetCamera()->SetRotationX(45.0f);
	//this->core->Gfx_GetCamera()->


	CameraObject *topCam = new CameraObject();
	topCam->camera = new Camera();
	topCam->camera->SetPosition(0.0f, 100.0f, 0.0f);
	topCam->camera->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	topCam->camera->RelativePitch((float)D3DXToRadian(90.0f));
	topCam->name = L"Brids eye";
	this->cameras.push_back(topCam);


	CameraObject* frontCam = new CameraObject();
	frontCam->camera = new Camera();
	frontCam->camera->SetPosition(0.0f, 0.0f, -100.0f);
	frontCam->camera->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	frontCam->name = L"Gound level";
	this->cameras.push_back(frontCam);

	return true;
}
void FLYCALL FlyLevelEditor_impl::Frame()
{
	if(!this->core)			return;
	if(!gEditorInstance)	return;
	
	this->core->Gfx_Update();

			 if(gKeyW)	this->core->Gfx_GetCamera()->RelativeForward(gSpeed);
		else if(gKeyS)	this->core->Gfx_GetCamera()->RelativeForward(-gSpeed);
		else if(gKeyA)	this->core->Gfx_GetCamera()->RelativeRight(-gSpeed);
		else if(gKeyD)	this->core->Gfx_GetCamera()->RelativeRight(gSpeed);

	this->core->Gfx_BeginDeferredScene();
	
	ViewFrustum f;
	this->core->Gfx_GetCamera()->ConstructViewFrustum(f);

	if(this->theWorld.size() > 0)
		this->theWorld[0]->Render(f);

	for (int i = 0; i < (int)this->mesh.size(); i++)
	{
		this->mesh[i]->Update();
		this->mesh[i]->Render(f);
	}
	for (int i = 0; i < (int)this->levelPickups.size(); i++)
	{
		this->levelPickups[i]->Update();
		this->levelPickups[i]->Render(f);
	}

	for (int i = 0; i < (int)this->lights.size(); i++)
		this->lights[i]->Render(f);
	
	this->core->Gfx_EndDeferredScene();

}
void FLYCALL FlyLevelEditor_impl::Resize(int width, int height)
{
	this->core->Gfx_Resize(width, height);
}
bool FLYCALL FlyLevelEditor_impl::SetCamera(int id)
{
	if(this->core->Gfx_GetDefaultCamera()->GetID() == id)
	{
		this->core->Gfx_SetCamera(0);
		return true;
	}
	for (int i = 0; i < (int)this->cameras.size(); i++)
	{
		if(this->cameras[i]->camera->GetID() == id)
		{
			this->core->Gfx_SetCamera(this->cameras[i]->camera);
			return true;
		}
	}
	return false;
}
bool FLYCALL FlyLevelEditor_impl::Entity_LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded)
{
	int start = (int)this->mesh.size();
	if(!this->core->Geometry_Load(files, &this->mesh))
		return false;

	if(start == (int)this->mesh.size())
		return false;

	for (int i = start; i < (int)mesh.size(); i++)
	{
		(*loaded)[this->mesh[i]->getName()] = this->mesh[i]->getID();

		this->mesh[i]->setShader(this->core->Gfx_GetShader(FlyShader_Default));
	}

	return true;
}
bool FLYCALL FlyLevelEditor_impl::LoadTerrain(std::wstring& path)
{
	if(this->theWorld.size() > 0)
		this->theWorld.clear();

	if(!this->core->Geometry_Load(path.c_str(), &this->theWorld, FlyGeometry_Terrain))
		return false;

	this->theWorld[0]->setShader(this->core->Gfx_GetShader(FlyShader_Default));

	return true;
}
bool FLYCALL FlyLevelEditor_impl::Entity_Select(int id)
{
	if(id < 0)
		return false;

	for (int i = 0; i < (int)this->mesh.size(); i++)
	{
		if(this->mesh[i]->getID() == id)
		{
			this->selected = this->mesh[i];
			selectedDrag = false;
			return true;
		}
	}

	return false;
}
void FLYCALL FlyLevelEditor_impl::GetCameras(std::map<std::wstring, int>* outCameras)
{
	if(outCameras)
	{
		(*outCameras)[L"Perspective"] = this->core->Gfx_GetDefaultCamera()->GetID();
		for (int i = 0; i < (int)this->cameras.size(); i++)
		{
			(*outCameras)[this->cameras[i]->name] = this->cameras[i]->camera->GetID();
		}
	}
}
bool FLYCALL FlyLevelEditor_impl::GetSelected (std::wstring& name, int& id, float& rx, float& ry, float& rz, float& sx, float& sy, float& sz)
{
	if(!this->selected)
		return false;

	name = this->selected->getName();
	id = this->selected->getID();
	vec3 rot = this->selected->getRotation();
	vec3 scale = this->selected->getScale();

	rx = rot.x;
	ry = rot.y;
	rz = rot.z;
	
	sx = scale.x;
	sy = scale.y;
	sz = scale.z;

	return true;
}
void FLYCALL FlyLevelEditor_impl::SetRotation(float x, float y, float z)
{
	if(this->selected)
		this->selected->setRotation(vec3(x, y, z));
}
void FLYCALL FlyLevelEditor_impl::SetScale(float x, float y, float z)
{
	if(this->selected)
		this->selected->setScale(vec3(x, y, z));
}
bool FLYCALL FlyLevelEditor_impl::SetName(const std::wstring& name)
{
	for (int i = 0; i < (int)this->mesh.size(); i++)
	{
		if(this->mesh[i]->getName() == name)
			return false;
	}

	this->selected->setName(name);
	return true;
}
void FLYCALL FlyLevelEditor_impl::SetFlyMode(bool flyMode)
{
	gFlyMode = flyMode;
}
void FLYCALL FlyLevelEditor_impl::GetSpeed(float& speed)
{
	speed = gSpeed;
}
void FLYCALL FlyLevelEditor_impl::SetSpeed(float speed)
{
	gSpeed = speed;
}


bool FLYCALL FlyLevelEditor_impl::LoadLevel(const std::wstring& path, std::map<std::wstring, int>* meshEntity, std::map<std::wstring, int>* lights, std::map<std::wstring, int>* pickups)
{
	wifstream file(path);
	vec3 readVector;
	int readInt = 0;
	wstring readString = L"";
	int nrOfStuff = 0;

	vector<IShader*> shaders;
	this->core->Gfx_GetShader(shaders);

	if(!file.is_open())
	{
		return false;
	}

	file>>readString;
	file>>nrOfStuff;

	//First to load is always terrain
	file>>readString;
	this->core->Geometry_Load(readString.c_str(), &this->theWorld, FlyGeometry_Terrain);
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
		this->core->Geometry_Load(readString.c_str(), &this->mesh);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->mesh[i]->setPosition(readVector);
		file>>readVector.x;
		file>>readVector.y;
		file>>readVector.z;
		this->mesh[i]->setRotation(readVector);
		file>>readInt;

		this->mesh[i]->setShader(shaders[readInt]);

		(*meshEntity)[this->mesh[i]->getName()] = this->mesh[i]->getID();
	}

	file>>nrOfStuff;

	//Pick-ups
	for(int i = 0; i < nrOfStuff; i++)
	{
		file >> readString;
		this->core->Geometry_Load(readString.c_str(), &this->levelPickups);
		file >> readVector.x;
		file >> readVector.y;
		file >> readVector.z;
		this->levelPickups[i]->setPosition(readVector);
		file >> readVector.x;
		file >> readVector.y;
		file >> readVector.z;
		this->levelPickups[i]->setRotation(readVector);
		file >> readInt;

		this->levelPickups[i]->setShader(shaders[readInt]);
		(*pickups)[this->levelPickups[i]->getName()] = this->levelPickups[i]->getID();
	}
	
	file.close();
	return true;
}
bool FLYCALL FlyLevelEditor_impl::SaveLevel(const std::wstring& path, const std::wstring& levelName)
{
	wofstream out;
	out.open(path);

	if(!out.is_open())
		return false;

	out << levelName << "\n";

	

	//Save meshes
	out << (int)this->mesh.size() << " ";
	for (int i = 0; i < (int)this->mesh.size(); i++)
	{
		vec3 p = this->mesh[i]->getPosition();
		vec3 s = this->mesh[i]->getScale();
		vec3 r = this->mesh[i]->getRotation();
		out << "..\\Resources\\Models\\" << this->mesh[i]->getName() 
			<< " " << p.x << " " << p.y << " " << p.z
			<< " " << r.x << " " << r.y << " " << r.z
			<< " " << s.x << " " << s.y << " " << s.z
			<< " " << this->mesh[i]->getShader()->getType();
	}

	//Save pickups
	out << (int)this->levelPickups.size();
	for (int i = 0; i < (int)this->levelPickups.size(); i++)
	{
		vec3 p = this->levelPickups[i]->getPosition();
		vec3 s = this->levelPickups[i]->getScale();
		vec3 r = this->levelPickups[i]->getRotation();
		out << "..\\Resources\\Models\\" << this->levelPickups[i]->getName() 
			<< " " << p.x << " " << p.y << " " << p.z
			<< " " << r.x << " " << r.y << " " << r.z
			<< " " << s.x << " " << s.y << " " << s.z
			<< " " << this->levelPickups[i]->getShader()->getType();
	}

	return true;
}


void FLYCALL FlyLevelEditor_impl::OnKeyEvent(int key, bool released, bool ctrl, bool shift, bool alt)
{
	if(key == -1)
		return;

	if(gFlyMode)
	{
		if(released)
		{
				 if(key == KEY_W)	gKeyW = false;
			else if(key == KEY_S)	gKeyS = false;
			else if(key == KEY_A)	gKeyA = false;
			else if(key == KEY_D)	gKeyD = false;
		}
		else
		{
				 if(key == KEY_W)	gKeyW = true;
			else if(key == KEY_S)	gKeyS = true;
			else if(key == KEY_A)	gKeyA = true;
			else if(key == KEY_D)	gKeyD = true;
		}

	}
}
void FLYCALL FlyLevelEditor_impl::OnMouseBtnEvent(int key, bool released, bool ctrl, bool shift, bool alt)
{
	if(key == KEY_MOUSE_LBTN)
	{
		if(!released)
		{
			if(alt)
			{
				gRotateCamera = true;
			}
			else if (!alt && !shift && ! ctrl)
			{
				if(!selectedDrag)
					this->selected = this->core->Geometry_Pick(this->mesh);
				
				if(!this->selected)
					this->selected = this->core->Geometry_Pick(this->levelPickups);
				
				if(!this->selected)
					this->selected = this->core->Geometry_Pick(this->lights);
				
				if (this->selected)
					selectedDrag = true;
			}
			
		}
		else
		{
			selectedDrag = false;
			gRotateCamera = false;
		}
	}
	else if (key == KEY_MOUSE_RBTN)
	{
		if(released)
		{

		}
		else
		{

		}
	}
	else if (key == KEY_MOUSE_MBTN)
	{
		if(released)
		{
			gMoveCameraScroll = false;
		}
		else
		{
			if(alt)
				gMoveCameraScroll = true;
		}
	}
}
void FLYCALL FlyLevelEditor_impl::OnMouseMoveEvent(int cx, int cy, int rx, int ry, bool ctrl, bool shift, bool alt)
{
	if(gRotateCamera)
	{
		this->core->Gfx_GetCamera()->RelativeYaw((float)rx * 0.08f);
		this->core->Gfx_GetCamera()->RelativePitch((float)ry * 0.08f);
	}
	else if(gFlyMode)
	{
		this->core->Gfx_GetCamera()->RelativeYaw((float)rx);
		this->core->Gfx_GetCamera()->RelativePitch((float)ry);
		return;
	}
	else if(selectedDrag && this->selected)
	{
		Camera* cam			= this->core->Gfx_GetCamera();
		vec3 currentPos		= this->selected->getPosition();
		vec3 currentUp		= cam->GetUp();
		vec3 currentRight	= cam->GetRight() * 0.5f;

		if(rx < 0)
		{
			currentPos -= currentRight;
		}
		else if(rx > 0)
		{ 
			currentPos += currentRight;
		}
		if(ry < 0)
		{
			currentPos += currentUp;
		}
		else if(ry > 0)
		{
			currentPos -= currentUp;
		}

		this->selected->setPosition(currentPos);
	}

	if(gMoveCameraScroll)
	{
		if(rx < 0)
			this->core->Gfx_GetCamera()->RelativeRight(2.0f);
		else if(rx > 0)
			this->core->Gfx_GetCamera()->RelativeRight(-2.0f);
		else if(ry < 0)
			this->core->Gfx_GetCamera()->RelativeUp(-2.0f);
		else if(ry > 0)
			this->core->Gfx_GetCamera()->RelativeUp(2.0f);
	}
	lastMouseX = cx;
	lastMouseY = cy;
}
void FLYCALL FlyLevelEditor_impl::OnMouseScrollEvent(int delta, bool ctrl, bool shift, bool alt)
{
	if(!gFlyMode)
	{
		Camera* cam = this->core->Gfx_GetCamera();
		if(delta > 0)
			cam->RelativeForward(6.5f);
		else
			cam->RelativeForward(-6.5f);
	}
}





