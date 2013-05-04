#ifndef FLY_ENGINE_H
#define FLY_ENGINE_H


#include "FlyEngine.h"
#include "..\Util\SmartPtrs.h"
#include "..\Core\Mesh\FullScreenQuad.h"


class BaseShader;
class BaseBuffer;



class FlyEngine_Core	:public FlyEngine
{
	private:
		FlyEngine_Core();

	private:
		ForwardUpdateFunc						forwardUpdateFunc;
		ForwardRenderFunc						forwardRenderFunc;
		DeferredRenderFunc						deferredUpdateFunc;
		DeferredRenderFunc						deferredRenderFunc;

		SmartPtrStd<IShader>					gbufferShader;
		SmartPtrStd<IShader>					colorShader; 
		SmartPtrStd<IShader>					lightShader;

		SmartPtrStd<BaseBuffer>					matrixBuffer;

		SmartPtrStd<FullScreenQuad>				fsq;

		SmartPtrStd<FlyCamera>					defaultCam;
		FlyCamera*								activeCamera;

		bool									splash;

	private:
		bool		_InitGBuffers							();
		bool		_InitColorShader						();
		bool		_InitLightShader						();
		bool		_InitMatrixBuffer						();
		bool		_InitGfx								(FLY_ENGINE_INIT_DESC& desc);
		bool		_InitWin								(FLY_ENGINE_INIT_DESC& desc);
		void		_InitCam								();
		void		_ShowSplash								();

	public:
		bool		FLYCALL		Core_Run					();
		bool		FLYCALL		Core_Initialize				(FLY_ENGINE_INIT_DESC& initDesc);
		void		FLYCALL		Core_Shutdown				();

		void		FLYCALL		Gfx_Update					();
		void		FLYCALL		Gfx_BeginForwardScene		();
		void		FLYCALL		Gfx_EndForwardScene			();
		void		FLYCALL		Gfx_BeginDeferredScene		();
		void		FLYCALL		Gfx_EndDeferredScene		();
		void		FLYCALL		Gfx_Resize					(int width, int height);
		void		FLYCALL		Gfx_SetCamera				(FlyCamera* cam);
		FlyCamera*	FLYCALL		Gfx_GetCamera				();
		IShader*	FLYCALL		Gfx_GetShader				(int id);
		void 		FLYCALL		Gfx_GetShader				(vector<IShader*>& shaders);
		
		

		bool		FLYCALL		Geometry_Create				(FlyEngineGeometry type, SmartPtrStd<FlyMesh> object);
		bool		FLYCALL		Geometry_Load				(const wchar_t* loadData, vector<SmartPtrStd<FlyMesh>>& objects);
		bool		FLYCALL		Geometry_Load				(vector<const wchar_t*> loadData, vector<SmartPtrStd<FlyMesh>>& objects);


		bool		FLYCALL		Input_Initialize			();
		bool		FLYCALL		Input_Shutdown				();
		void		FLYCALL		Input_Activate				();
		void		FLYCALL		Input_Deactivate			();

		static void  _Interface();
};

extern SmartPtrStd<FlyEngine> pFlyEngine;

#endif

