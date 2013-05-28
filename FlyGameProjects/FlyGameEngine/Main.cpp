#include "FlyEngine\FlyEngine.h"
#include "Core\Input.h"
#include "Core\Entity.h"
#include "Util\Camera.h"
#include "Core\stdafx.h"
#include "Core\Light\LightHolder.h"


class Lukas
{
	public:
		static FlyEngine *fly;
		static std::vector<Entity*> skyBox; 
		static std::vector<Entity*> obj;
		static std::vector<Entity*> dirLight;
		static std::vector<BaseBuffer*> shadowViews;

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
			for (int i = 0; i < (int)this->dirLight.size(); i++)
			{
				delete this->dirLight[i];
			}
			for (int i = 0; i < (int)this->skyBox.size(); i++)
			{
				delete this->skyBox[i];
			}
		}

		static void Update()
		{
			Lukas::fly->Gfx_Update();
			Camera* cam = Lukas::fly->Gfx_GetCamera();

			D3DXVECTOR3 pos =cam->GetPosition();
			Lukas::skyBox[0]->setPosition(pos);

		}
		static void Render()
		{
			vector<IShader*> sh;
			Lukas::fly->Gfx_GetShader(sh);
			
			
			ViewFrustum f;
			fly->Gfx_GetCamera()->ConstructViewFrustum(f);
			
			Lukas::skyBox[0]->Render(f);
			
			for (int i = 0; i < (int)Lukas::obj.size(); i++)
			{
				Lukas::obj[i]->setShader(sh[FlyShader_gBufferDefault]);
				Lukas::obj[i]->Render(f);
			}
			/*for (int i = 0; i < (int)Lukas::obj.size(); i++)
			{
				Lukas::obj[i]->setShader(sh[FlyShader_Shadow]);
				Lukas::obj[i]->Render(f);
			}*/
			std::vector<BaseBuffer*>  shadowViews; 
			for (int i = 0; i < (int)Lukas::dirLight.size(); i++)
			{
				Lukas::dirLight[i]->Render(f);

				if(((DirectionLight*)dirLight[i])->castShadow())
				{
					//shadowViews.push_back(((DirectionLight*)dirLight[i])->getLightViewProj());
				}
			}

			for(int i= 0; i<(int)shadowViews.size();i++ )
			{
				Lukas::obj[i]->setShader(sh[FlyShader_Shadow]);
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
std::vector<Entity*> Lukas::dirLight = std::vector<Entity*>();
std::vector<Entity*> Lukas::skyBox = std::vector<Entity*>();
std::vector<BaseBuffer*> Lukas::shadowViews = std::vector<BaseBuffer*>();

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

	//Input::self()->subscribeKeyDown<Lukas>(&lukor, &Lukas::Keydown);
	//Input::self()->subscribeMouseMove<Lukas>(&lukor, &Lukas::MouseMove);
	//Input::self()->subscribeMouseBtnDown(&lukor, &Lukas::mkeydown);

	
	
	vector<const wchar_t*> files;
	files.push_back(L"shadow_test.fgm");
	Lukas::fly->Geometry_Load(files, &Lukas::obj);
	vector<IShader*> sh;
	Lukas::fly->Gfx_GetShader(sh);

	Lukas::obj[0]->setShader(sh[FlyShader_gBufferDefault]);
	//Lukas::obj[1]->setShader(sh[FlyShader_gBufferDefault]);



	files.clear();
	files.push_back(L"AAskydome.fgm");
	Lukas::fly->Geometry_Load(files, &Lukas::skyBox);
	Lukas::skyBox[0]->setShader(sh[FlyShader_gBufferNoDepth]);


	DirectionalLightProxy dirLightProxy;
	dirLightProxy.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	dirLightProxy.diffuse	= D3DXVECTOR4(1.0f,0.0f,0.0f,1);
	dirLightProxy.specular	= D3DXVECTOR4(0,0,1,1);
	D3DXVECTOR4 dir(1,-3,-1,0);
	D3DXVec4Normalize(&dir, &dir); 
	dirLightProxy.direction= dir;


	DirectionalLightProxy dirLightProxy2;
	dirLightProxy2.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	dirLightProxy2.diffuse	= D3DXVECTOR4(0.0f,0.0f,1.0f,1);
	dirLightProxy2.specular	= D3DXVECTOR4(0,0,1,1);
	dir = D3DXVECTOR4(1,3,1,0);
	D3DXVec4Normalize(&dir, &dir); 
	dirLightProxy2.direction= dir;


	DirectionLight light(Type::LIGHT);
	bool hasShadow = true;

	//light.Initialize(dirLightProxy, sh[FlyShader_DirLight], hasShadow);

	Lukas::dirLight.push_back(&light);

	DirectionLight light2(Type::LIGHT);
	//light2.Initialize(dirLightProxy2, sh[FlyShader_DirLight], hasShadow);

	Lukas::dirLight.push_back(&light2);

	//Lukas::shadowViews.push_back(light2.getLightViewProj()); 

	Lukas::fly->Core_Run();
	Lukas::fly->Core_Shutdown();


	return cmdShow;

}
