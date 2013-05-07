#include "FlyLevelEditor_impl.h"

#include "..\FlyGameEngine\Core\Entity.h"
#include "..\FlyGameEngine\Util\Camera.h"
#include "..\FlyGameEngine\Core\D3DShell.h"


FlyLevelEditor_impl *gEditorInstance = 0;

FlyLevelEditor* FLYCALL GetEditorInstance()
{
	if(!gEditorInstance)
		gEditorInstance = new FlyLevelEditor_impl();

	return gEditorInstance;
}



FlyLevelEditor_impl::FlyLevelEditor_impl()
{
	this->core = FlyEngineCreate();
}

FlyLevelEditor_impl::~FlyLevelEditor_impl()
{
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

	Camera* topCam = new Camera();
	topCam->SetPosition(0.0f, 100.0f, 0.0f);
	topCam->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	topCam->RelativePitch(90.0f);
	this->cameras.push_back(topCam);
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
		this->mesh[i]->Render(f);
	for (int i = 0; i < (int)this->lights.size(); i++)
		this->lights[i]->Render(f);
	
	this->core->Gfx_EndDeferredScene();

}
void FLYCALL FlyLevelEditor_impl::Resize(int width, int height)
{
	this->core->Gfx_Resize(width, height);
}
void FLYCALL FlyLevelEditor_impl::SetCamera(CAMERA cam)
{
	switch (cam)
	{
		case FlyLevelEditor::CAM_DEFAULT:
			this->core->Gfx_SetCamera(NULL);
		break;
		case FlyLevelEditor::CAM_TOP:
			this->core->Gfx_SetCamera(this->cameras[(int)CAM_TOP]);
		break;
	}
}
bool FLYCALL FlyLevelEditor_impl::LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded)
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