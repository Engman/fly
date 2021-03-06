#include "FlyLevelEditor_impl.h"

#include "..\FlyGameEngine\Core\Entity.h"
#include "..\FlyGameEngine\Util\Camera.h"
#include "..\FlyGameEngine\Util\CollisionLib.h"
#include "..\FlyGameEngine\Core\D3DShell.h"
#include "..\FlyGameEngine\Util\Timer.h"


FlyLevelEditor_impl *gEditorInstance = 0;
bool selectedDrag = false;


FlyLevelEditor* FLYCALL GetEditorInstance()
{
	if(!gEditorInstance)
		gEditorInstance = new FlyLevelEditor_impl();

	return gEditorInstance;
}



FlyLevelEditor_impl::FlyLevelEditor_impl()
{
	this->core = FlyEngineCreate();
	this->selected = 0;
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

	this->core->Input_Initialize();
	this->core->Input_Activate();

	Input::self()->subscribeMouseBtnDown(this, &FlyLevelEditor_impl::KeyDownEvent);
	Input::self()->subscribeMouseBtnUp(this, &FlyLevelEditor_impl::KeyReleaseEvent);
	Input::self()->subscribeMouseMove(this, &FlyLevelEditor_impl::MouseMoveEvent);
	Input::self()->subscribeMouseScroll(this, &FlyLevelEditor_impl::MouseScrollEvent);

	CameraObject *topCam = new CameraObject();
	topCam->camera = new Camera();
	topCam->camera->SetPosition(0.0f, 100.0f, 0.0f);
	topCam->camera->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	topCam->camera->RelativePitch(90.0f);
	topCam->name = L"Top view Camera";
	this->cameras.push_back(topCam);


	CameraObject* frontCam = new CameraObject();
	frontCam->camera = new Camera();
	frontCam->camera->SetPosition(0.0f, 0.0f, -100.0f);
	frontCam->camera->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	frontCam->name = L"Front view Camera";
	this->cameras.push_back(frontCam);

	return true;
}
void FLYCALL FlyLevelEditor_impl::Frame()
{
	if(!this->core)			return;
	if(!gEditorInstance)	return;
	
	this->core->Gfx_Update();
	this->core->Gfx_BeginDeferredScene();
	
	ViewFrustum f;
	this->core->Gfx_GetCamera()->ConstructViewFrustum(f);

	for (int i = 0; i < (int)this->mesh.size(); i++)
	{
		this->mesh[i]->Update();
		this->mesh[i]->Render(f);
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
		(*outCameras)[L"First person view"] = this->core->Gfx_GetDefaultCamera()->GetID();
		for (int i = 0; i < (int)this->cameras.size(); i++)
		{
			(*outCameras)[this->cameras[i]->name] = this->cameras[i]->camera->GetID();
		}
	}
}
void FLYCALL FlyLevelEditor_impl::GetSelected (std::wstring& name, int& id)
{
	if(this->selected->getID() == -1)
		MessageBox(0, L"Negativ one", L"", 0);
	else
		MessageBox(0, L"none", L"", 0);

	if(this->selected)
	{
		name = this->selected->getName();
		id = this->selected->getID();
	}
	else
	{
		name = L"";
		id = -1;
	}
}




void FlyLevelEditor_impl::MouseScrollEvent(int &delta)
{
	Camera* cam = this->core->Gfx_GetCamera();
	if(delta == 1)
		cam->RelativeForward(3.5f);
	else
		cam->RelativeForward(-3.5f);
}
void FlyLevelEditor_impl::KeyDownEvent(Input::MouseBtnData& data)
{
	if(data.MousePos_clientX >= 0 && data.MousePos_clientX < D3DShell::self()->getWidth() &&
				data.MousePos_clientY >= 0 && data.MousePos_clientY < D3DShell::self()->getHeight())
	{
		if (data.key == Input::KeyCodes::M_LeftBtn)
		{
			//if (!this->selected)
			//{
			if(!selectedDrag)
				this->selected = this->core->Geometry_Pick(this->mesh, data.MousePos_clientX, data.MousePos_clientY);
			//}
			if (this->selected)
			{
				selectedDrag = true;
			}
		}
	}
}
void FlyLevelEditor_impl::KeyReleaseEvent(Input::MouseBtnData& data)
{
	if (data.key == Input::KeyCodes::M_LeftBtn)
	{
		if(this->selected && selectedDrag)
		{
			selectedDrag = false;
		}
	}
}
void FlyLevelEditor_impl::MouseMoveEvent(Input::MouseMoveData& data)
{
	if(selectedDrag && this->selected)
	{
		Camera* cam = this->core->Gfx_GetCamera();
		vec3 currentPos		= this->selected->getPosition();
		vec3 currentUp		= cam->GetUp();
		vec3 currentRight	= cam->GetRight();
		//vec3 currentFront	= this->selected->getFront();

		if(data.relativeX < 0)
		{
			currentPos -= currentRight;
		}
		if(data.relativeX > 0)
		{ 
			currentPos += currentRight;
		}
		if(data.relativeY < 0)
		{
			currentPos += currentUp;
		}
		if(data.relativeY > 0)
		{
			currentPos -= currentUp;
		}
		this->selected->setPosition(currentPos);
	}
}

