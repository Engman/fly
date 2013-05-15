#include "FlyEngine_Core.h"

#include "..\Core\WindowShell.h"
#include "..\Core\Input.h"
#include "..\Core\Render\GBufferShader.h"
#include "..\Core\Render\GBufferAnimationShader.h"
#include "..\Core\Render\FinalShader.h"
#include "..\Core\Render\LightShader.h"
#include "..\Core\Render\ShadowMapShader.h"
#include "..\Core\Render\BlurShader.h"
#include "..\Util\Camera.h"



SmartPtrStd<FlyEngine> pFlyEngine;

static LRESULT CALLBACK FlyEngineCoreWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}


FlyEngine* FLYCALL FlyEngineCreate()
{
	FlyEngine_Core::_Interface();

	return pFlyEngine;
}



void FlyEngine_Core::_Interface()
{
	if(!pFlyEngine.IsValid())
		pFlyEngine = new FlyEngine_Core();

}



FlyEngine_Core::FlyEngine_Core()
{
	D3DShell::self();
	WindowShell::self();
	Input::self();

	this->gbufferShader			= new GBufferShader();
	this->gBufferNoDepthShader	= new GBufferShader();
	this->gBufferAnimationShader = new GBufferAnimationShader();
	this->finalShader			= new FinalShader();
	this->dirLightShader		= new LightShader();
	this->shadowMapShader		= new ShadowMapShader();
	this->blurHorizontShader	= new BlurShader();
	this->blurVerticalShader	= new BlurShader();

	this->cameraBuffer			= new BaseBuffer();
	this->fsq					= new FullScreenQuad();
	this->defaultCam			= new Camera();
	this->activeCamera			= NULL;
	this->deferredRenderFunc	= 0;
	this->deferredUpdateFunc	= 0;
	this->forwardRenderFunc		= 0;
	this->forwardUpdateFunc		= 0;
	this->splash				= 0;
	this->orthographicCamera	= 0;
}



bool FLYCALL FlyEngine_Core::Core_Run()
{
	if(!WindowShell::self()->getHWND())
		return false;

	if(this->splash)
		this->_ShowSplash();
	
	MSG	msg;
	while (true)
	{

		if(!WindowShell::self()->getParent())
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{ 
				if (msg.message == WM_QUIT)	
					break;
				DispatchMessage(&msg);
				continue;
			}
		}

		
		//Deferred rendering pass
		if(this->deferredUpdateFunc) this->deferredUpdateFunc();
		if(this->deferredRenderFunc) 
		{
			//Gfx_BeginDeferredScene();
			// add all the objects to the draw data 
			this->deferredRenderFunc();
			Gfx_DrawSkyBox();
			Gfx_DrawGbuffer();
			Gfx_DrawBlur();
			Gfx_DrawShadows();
			Gfx_DrawLighting();
			Gfx_DrawFinalPicture();

			//Gfx_EndDeferredScene();
		}

		//Forward rendering pass
		if(this->forwardUpdateFunc) this->forwardUpdateFunc();
		if(this->forwardRenderFunc) 
		{
			Gfx_BeginForwardScene();
			this->forwardRenderFunc();
			Gfx_EndForwardScene();
		}


	}
	return true;
}

bool FLYCALL FlyEngine_Core::Core_Initialize(FLY_ENGINE_INIT_DESC& desc)
{
	this->deferredRenderFunc	= desc.deferredRenderFunc;
	this->deferredUpdateFunc	= desc.deferredUpdateFunc;
	this->forwardRenderFunc		= desc.forwardRenderFunc;
	this->forwardUpdateFunc		= desc.forwardUpdateFunc;
	this->splash				= desc.showSplash;

	if(!this->_InitWin(desc))			return false;
	if(!this->_InitGfx(desc))			return false;

	if(!this->_InitGBufferShader())			return false;
	if(!this->_InitAnimationShader())	return false;
	if(!this->_InitFinalShader())		return false;
	if(!this->_InitDirLightShader())	return false;
	if(!this->_InitShadowMapShader())	return false;
	if(!this->_InitBlurShaders())		return false;
	
	if(!this->_InitCameraBuffer())		return false;

	if(!this->fsq->Initialize(D3DShell::self()->getDevice(), this->finalShader))
		return false;

	this->_InitCam();

	return true;
}

void FLYCALL FlyEngine_Core::Core_Shutdown()
{
	D3DShell::self()->destroy();
	WindowShell::self()->destroy();
	Input::self()->destroy();

	this->gbufferShader.Destroy();
	this->gBufferNoDepthShader.Destroy();
	this->gBufferAnimationShader.Destroy();			
	this->finalShader.Destroy();			
	this->dirLightShader.Destroy();			
	this->shadowMapShader.Destroy();
	this->blurHorizontShader.Destroy();
	this->blurVerticalShader.Destroy();

	this->cameraBuffer.Destroy();			
	this->fsq.Destroy();
	this->defaultCam.Destroy();

	pFlyEngine.Destroy();
}




LRESULT CALLBACK FlyEngineCoreWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cc = 0;
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		
		case WM_INPUT:
			Input::self()->proccessRawDeviceData(lParam);
		break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;

		case WM_MOUSEMOVE:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;
		case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
		break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}



//################################//
//########### LOCAL DATA #########//
//####################################################################//
bool FlyEngine_Core::_InitWin(FLY_ENGINE_INIT_DESC& desc)
{
	WindowShell::INIT_DESC_WINDOW descWindow;
	if(desc.handle)
		descWindow.hInstance		= desc.handle;
	else
		descWindow.hInstance		= GetModuleHandle(0);

	descWindow.windowName			= desc.windowName;
	descWindow.windowClassName		= L"FlyEngineWindowClass";
	descWindow.parent				= desc.parent;
	descWindow.windowPosition		= Point2D(desc.winPosX, desc.winPosY);
	descWindow.windowProcCallback	= FlyEngineCoreWndProc;
	descWindow.windowSize			= Point2D(desc.winWidth, desc.winHeight);

	if(!WindowShell::self()->createWin(descWindow))
		return false;

	return true;
}
void FlyEngine_Core::_ShowSplash()
{
	Texture2D texture;
	if(!texture.loadTexture(D3DShell::self()->getDevice(), L"..\\Resources\\Textures\\flyEngineSplash.png"))
		return;

	this->Gfx_BeginForwardScene();
	IShader::DRAW_DATA d;
	this->finalShader->getShader()->Render();
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0, 1, &(*texture.getTextures())[0]);
	this->Gfx_EndForwardScene();

	this->splash = false;
}
//####################################################################//
