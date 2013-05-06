#ifndef __ENGINE_WRAPPER_H__
#define __ENGINE_WRAPPER_H__



#include <Windows.h>
#include <string>
#include <vector>

#include "..\Util\SmartPtrs.h"
#include "..\Util\FlyCamera.h"
#include "..\Core\Mesh\FlyMesh.h"

class FlyMesh;
class IShader;

using namespace std;


#ifdef FLYDLL
#define FLY_EXPORT __declspec(dllexport)
#else
#define FLY_EXPORT
#endif
#define FLYCALL  __stdcall

typedef void(*ForwardUpdateFunc)();
typedef void(*ForwardRenderFunc)();
typedef void(*DeferredUpdateFunc)();
typedef void(*DeferredRenderFunc)();



struct FLY_ENGINE_INIT_DESC
{
	HWND parent;
	HINSTANCE handle;
	int winWidth;
	int winHeight;
	int winPosX;
	int winPosY;
	bool multisampling;
	bool vSync;
	bool fullscreen;
	bool showSplash;
	const wchar_t* windowName;
	ForwardUpdateFunc forwardUpdateFunc;
	ForwardRenderFunc forwardRenderFunc;
	DeferredUpdateFunc deferredUpdateFunc;
	DeferredRenderFunc deferredRenderFunc;

	FLY_ENGINE_INIT_DESC()
		:parent(0), handle(0), winHeight(0), winWidth(0), winPosX(0), winPosY(0), 
		multisampling(0), vSync(0), fullscreen(0), windowName(L"FlyEngine"),
		forwardRenderFunc(0), forwardUpdateFunc(0),
		deferredRenderFunc(0), deferredUpdateFunc(0)
	{}
};

enum FlyEngineGeometry
{
	Sphere,
	Cube,
	Plane,
	Line,
};



class FlyEngine
{
	public:
		/** Starts the core and begins update and render */
		virtual bool		FLYCALL		Core_Run() = 0;
		/** Initializes the core with existing hwnd */
		virtual bool		FLYCALL		Core_Initialize(FLY_ENGINE_INIT_DESC& initDesc) = 0;
		/** Shuts down engine */
		virtual void		FLYCALL		Core_Shutdown() = 0;

		

		/** Updates a sceen */
		virtual void		FLYCALL		Gfx_Update() = 0;
		/** Begin forward scene */
		virtual void		FLYCALL		Gfx_BeginForwardScene() = 0;
		/** End forward scene */
		virtual void		FLYCALL		Gfx_EndForwardScene() = 0;
		/** Begin deferred scene */
		virtual void		FLYCALL		Gfx_BeginDeferredScene() = 0;
		/** End deferred scene */
		virtual void		FLYCALL		Gfx_EndDeferredScene() = 0;
		/** Resizes the render targets */
		virtual void		FLYCALL		Gfx_Resize(int width, int height) = 0;
		/** Sets active camera */
		virtual void		FLYCALL		Gfx_SetCamera(FlyCamera* cam) = 0;
		
		virtual IShader*	FLYCALL		Gfx_GetShader(int id) = 0;
		virtual void 		FLYCALL		Gfx_GetShader(std::vector<IShader*>& shaders) = 0;
		virtual FlyCamera*	FLYCALL		Gfx_GetCamera() = 0;
	


		/** Creates a new 3D object 
		*	New Reference */
		virtual bool		FLYCALL Geometry_Create(FlyEngineGeometry type, SmartPtrStd<FlyMesh> object) = 0;
		/** Loads a single resource
		*	New Reference */
		virtual bool		FLYCALL Geometry_Load(const wchar_t* loadData, vector<SmartPtrStd<FlyMesh>>& objects) = 0;
		/** Loads a list of resources 
		*	Returns the nr of imported resources
		*	New Reference */
		virtual bool		FLYCALL Geometry_Load(vector<const wchar_t*> loadData, vector<SmartPtrStd<FlyMesh>>& objects) = 0;



		/** Initializes raw input */
		virtual bool		FLYCALL Input_Initialize() = 0;
		/** Removes device input from application */
		virtual bool		FLYCALL Input_Shutdown() = 0;
		/** Activates raw input */
		virtual void		FLYCALL Input_Activate() = 0;
		/** Deavctivates raw input */
		virtual void		FLYCALL Input_Deactivate() = 0;
};


extern "C" 
{
	FLY_EXPORT FlyEngine* FLYCALL FlyEngineCreate();
}


#endif
