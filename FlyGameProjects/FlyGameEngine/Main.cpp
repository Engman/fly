#include "FlyEngine\FlyEngine.h"
#include "Core\Input.h"
#include "Core\Entity.h"
#include "Util\Camera.h"
#include "Core\stdafx.h"



class Lukas
{
	public:
		static FlyEngine *fly;
		static std::vector<Entity*> obj;

	public:
		Lukas()
		{
			this->fly = FlyEngineCreate();
		}
		virtual~Lukas()
		{
			//this->fly->Core_Shutdown();
			for (int i = 0; i < (int)this->obj.size(); i++)
			{
				delete this->obj[i];
			}
		}

		static void Update()
		{
			Lukas::fly->Gfx_Update();
		}
		static void Render()
		{
			ViewFrustum f;
			fly->Gfx_GetCamera()->ConstructViewFrustum(f);
			for (int i = 0; i < (int)Lukas::obj.size(); i++)
			{
				Lukas::obj[i]->Render(f);
			}
		}
		//void Keydown(Input::KeyCodes::Key key)
		//{
		//	Camera *c = Lukas::fly->Gfx_GetCamera();
		//	if(key == Input::KeyCodes::K_W)
		//		c->RelativeForward(0.3f);
		//	else if(key == Input::KeyCodes::K_S)
		//		c->RelativeForward(-0.3f);
		//	else if(key == Input::KeyCodes::K_A)
		//		c->RelativeRight(-0.1f);
		//	else if(key == Input::KeyCodes::K_D)
		//		c->RelativeRight(0.1f);
		//	else if(key == Input::KeyCodes::K_Space)
		//		c->RelativeUp(0.3f);
		//	else if(key == Input::KeyCodes::K_Ctrl)
		//		c->RelativeUp(-0.3f);
		//	else if(key == Input::KeyCodes::K_Escape)
		//		PostQuitMessage(0);
		//}
		//void MouseMove(Input::MouseMoveData d)
		//{
		//	Camera *c = Lukas::fly->Gfx_GetCamera();
		//	c->RelativeYaw((float)D3DXToRadian(d.relativeX*0.2)); //* this->gtime->DeltaTime() * 150);
		//	c->RelativePitch((float)D3DXToRadian(d.relativeY*0.2)); //* this->gtime->DeltaTime() * 150);
		//}
};

std::vector<Entity*> Lukas::obj = std::vector<Entity*>();
FlyEngine* Lukas::fly = NULL;

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{

	#if defined(DEBUG) || defined(_DEBUG)
		FIND_MEMORY_LEAKS;
	#endif

	Lukas lukor;
	FLY_ENGINE_INIT_DESC desc;
	desc.handle = hInst;
	desc.winWidth = 1200;
	desc.winHeight = 600;
	desc.showSplash = true;
	desc.deferredRenderFunc = Lukas::Render;
	desc.deferredUpdateFunc = Lukas::Update;
	if(!Lukas::fly->Core_Initialize(desc))
		return cmdShow;
	
	Lukas::fly->Input_Initialize();
	Lukas::fly->Input_Activate();
	/*Input::self()->subscribeKeyDown<Lukas>(&lukor, &Lukas::Keydown);
	Input::self()->subscribeMouseMove<Lukas>(&lukor, &Lukas::MouseMove);*/
	
	
	vector<const wchar_t*> files;
	files.push_back(L"shadow_test.fgm");
	Lukas::fly->Geometry_Load(files, &Lukas::obj);
	vector<IShader*> sh;
	Lukas::fly->Gfx_GetShader(sh);
	Lukas::obj[0]->setShader(sh[1]);
	

	Lukas::fly->Core_Run();
	Lukas::fly->Core_Shutdown();


	return cmdShow;

}
