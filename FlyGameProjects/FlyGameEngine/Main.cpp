#include "Core\Application.h"
#include "FlyEngine\FlyEngine.h"





class Lukas
{
	public:
		static FlyEngine *fly;
		static std::vector<SmartPtrStd<FlyMesh>> obj;

	public:
		Lukas()
		{
			this->fly = FlyEngineCreate();
		}
		virtual~Lukas()
		{
			//this->fly->Core_Shutdown();
		}

		static void Update()
		{
			Lukas::fly->Gfx_Update();
		}
		static void Render()
		{
			for (int i = 0; i < (int)Lukas::obj.size(); i++)
			{
				Lukas::obj[i]->Render();
			}
		}
		void Keydown(Input::KeyCodes::Key key)
		{
			FlyCamera *c = Lukas::fly->Gfx_GetCamera();
			if(key == Input::KeyCodes::K_W)
				c->Move(0.3f);
			else if(key == Input::KeyCodes::K_S)
				c->Move(-0.3f);
			else if(key == Input::KeyCodes::K_A)
				c->Strafe(-0.1f);
			else if(key == Input::KeyCodes::K_D)
				c->Strafe(0.1f);
			else if(key == Input::KeyCodes::K_Space)
				c->Ascend(0.3f);
			else if(key == Input::KeyCodes::K_Ctrl)
				c->Ascend(-0.3f);
			else if(key == Input::KeyCodes::K_Escape)
				PostQuitMessage(0);
		}
		void MouseMove(Input::MouseMoveData d)
		{
			FlyCamera *c = Lukas::fly->Gfx_GetCamera();
			c->Yaw((float)D3DXToRadian(d.relativeX*0.2)); //* this->gtime->DeltaTime() * 150);
			c->Pitch((float)D3DXToRadian(d.relativeY*0.2)); //* this->gtime->DeltaTime() * 150);
		}
};

std::vector<SmartPtrStd<FlyMesh>> Lukas::obj = std::vector<SmartPtrStd<FlyMesh>>();
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
	Input::self()->subscribeKeyDown<Lukas>(&lukor, &Lukas::Keydown);
	Input::self()->subscribeMouseMove<Lukas>(&lukor, &Lukas::MouseMove);
	
	
	Lukas::fly->Geometry_Load(L"..\\Resources\\Models\\shadow_test.fgm", Lukas::obj);
	vector<IShader*> sh;
	Lukas::fly->Gfx_GetShader(sh);
	Lukas::obj[0]->setShader(sh[1]);
	

	Lukas::fly->Core_Run();


	Lukas::fly->Core_Shutdown();


	return cmdShow;

}
