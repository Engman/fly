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
		SmartPtrStd<IShader>					gBufferNoDepthShader;
		SmartPtrStd<IShader>					gBufferAnimationShader;
		SmartPtrStd<IShader>					finalShader; 
		SmartPtrStd<IShader>					finalColorShader; 
		SmartPtrStd<IShader>					dirLightShader;
		SmartPtrStd<IShader>					shadowMapShader;
		SmartPtrStd<IShader>					blurHorizontShader;
		SmartPtrStd<IShader>					blurVerticalShader;

		SmartPtrStd<BaseBuffer>					cameraBuffer;

		SmartPtrStd<FullScreenQuad>				fsq;

		SmartPtrStd<Camera>						defaultCam;
		Camera*									activeCamera;

		bool									splash;
		bool									orthographicCamera;

	private:
		bool		_InitGBufferShader							();
		bool		_InitFinalShader						();
		bool		_InitDirLightShader						();
		bool		_InitShadowMapShader					();
		bool		_InitBlurShaders						();
		bool		_InitAnimationShader					();
		bool		_InitCameraBuffer						();
		bool		_InitGfx								(FLY_ENGINE_INIT_DESC& desc);
		bool		_InitWin								(FLY_ENGINE_INIT_DESC& desc);
		void		_InitCam								();
		void		_ShowSplash								();

	public:
		bool		FLYCALL		Core_Run					();
		bool		FLYCALL		Core_Initialize				(FLY_ENGINE_INIT_DESC& initDesc);
		void		FLYCALL		Core_Shutdown				();
		bool		FLYCALL		Core_Message				();

		void		FLYCALL		Gfx_Update					();
		void		FLYCALL		Gfx_BeginForwardScene		();
		void		FLYCALL		Gfx_EndForwardScene			();
		void		FLYCALL		Gfx_BeginDeferredScene		();

		void		FLYCALL		Gfx_DrawSkyBox				();
		void		FLYCALL		Gfx_DrawGbuffer				();
		void		FLYCALL		Gfx_DrawShadows				(Camera theCamera);
		void		FLYCALL		Gfx_DrawLighting			();
		void		FLYCALL		Gfx_DrawBlur				();
		void		FLYCALL		Gfx_DrawGbufferOrtho		();
		void		FLYCALL		Gfx_DrawFinalPicture		(Camera theCamera);
		
		void		FLYCALL PlaySoundTrack(const wchar_t* path);
		void		FLYCALL		Gfx_EndDeferredScene		();
		void		FLYCALL		Gfx_EndDeferredSceneOrtho	();
		void		FLYCALL		Gfx_Resize					(int width, int height);
		void		FLYCALL		Gfx_SetCamera				(Camera* cam, bool isOrthographic = false);
		Camera*		FLYCALL		Gfx_GetCamera				();
		Camera*		FLYCALL		Gfx_GetDefaultCamera		();
		IShader*	FLYCALL		Gfx_GetShader				(FlyEngineShaders shader);
		void 		FLYCALL		Gfx_GetShader				(vector<IShader*>& shaders);
		
	

		bool		FLYCALL		Geometry_Create				(FlyEngineGeometry type, Entity* object);
		bool		FLYCALL		Geometry_Load				(const wchar_t* loadData, vector<Entity*>* objects);
		bool		FLYCALL		Geometry_Load				(vector<const wchar_t*> loadData, vector<Entity*>* objects);
		bool		FLYCALL		Geometry_Load				(const wchar_t* path, vector<Entity*>* object, FlyEngineGeometry special, int renderBoxes, int collisionBoxes);
		Entity*		FLYCALL		Geometry_Pick				(const vector<Entity*>& objectToTest, int posX, int posY);



		bool		FLYCALL		Input_Initialize			();
		bool		FLYCALL		Input_Shutdown				();
		void		FLYCALL		Input_Activate				();
		void		FLYCALL		Input_Deactivate			();
		
		
		void		FLYCALL		Audio_Initialize			();
		void		FLYCALL		Audio_PlaySound				(FlyEngineSounds sound);
		void		FLYCALL		Audio_Shutdown				();	

		static void  _Interface();
};

extern SmartPtrStd<FlyEngine> pFlyEngine;

#endif

