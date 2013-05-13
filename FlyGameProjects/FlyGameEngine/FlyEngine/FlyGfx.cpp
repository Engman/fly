#include "FlyEngine_Core.h"

#include "..\Core\WindowShell.h"
#include "..\Core\Render\GBufferShader.h"
#include "..\Core\Render\ColorShader.h"
#include "..\Core\Render\LightShader.h"
#include "..\Util\Camera.h"




void FLYCALL FlyEngine_Core::Gfx_Update()
{
	if(this->activeCamera)
		this->activeCamera->Render();
}

void FLYCALL FlyEngine_Core::Gfx_BeginForwardScene()
{
	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();
}

void FLYCALL FlyEngine_Core::Gfx_EndForwardScene()
{
	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	gBufferDrawData.projection = this->activeCamera->GetProjectionMatrix();
	this->colorShader->draw(gBufferDrawData);
	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}

void FLYCALL FlyEngine_Core::Gfx_BeginDeferredScene()
{
	D3DShell::self()->BeginGBufferRenderTargets();
}

void FLYCALL FlyEngine_Core::Gfx_EndDeferredScene()
{
	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	if(this->orthographicCamera)
		gBufferDrawData.projection = this->activeCamera->GetOrthogonalMatrix();
	else
		gBufferDrawData.projection = this->activeCamera->GetProjectionMatrix();
	this->gbufferShader->draw(gBufferDrawData);


	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();

	this->fsq->Render(D3DShell::self()->getDeviceContext());
	this->colorShader->draw(gBufferDrawData);
	

	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}

void FLYCALL FlyEngine_Core::Gfx_Resize(int width, int height)
{
	D3DShell::self()->resizeViewport((UINT)width, (UINT)height);
}

void FLYCALL FlyEngine_Core::Gfx_SetCamera(Camera* cam, bool isOrthographic)
{
	if(!cam)
		this->activeCamera = this->defaultCam;
	else
		this->activeCamera = cam;

	this->orthographicCamera = isOrthographic;
}

IShader* FLYCALL FlyEngine_Core::Gfx_GetShader(FlyEngineShaders shader)
{
	switch (shader)
	{
		case FlyShader_Default:
			return this->gbufferShader;
		break;

		case FlyShader_Color:
			return this->colorShader;
		break;

		case FlyShader_Light:
			return this->lightShader;
		break;
	}
	return NULL;
}

void FLYCALL FlyEngine_Core::Gfx_GetShader(vector<IShader*>& shaders)
{
	shaders.push_back(this->colorShader);
	shaders.push_back(this->gbufferShader);
	shaders.push_back(this->lightShader);
}

Camera* FLYCALL FlyEngine_Core::Gfx_GetCamera()
{
	return this->activeCamera;
}

Camera*	FLYCALL	FlyEngine_Core::Gfx_GetDefaultCamera()
{
	return this->defaultCam;
}




//################################//
//########### LOCAL DATA #########//
//####################################################################//
bool FlyEngine_Core::_InitGfx(FLY_ENGINE_INIT_DESC& _desc)
{
	D3DShell::D3D_INIT_DESC desc;

	desc.height				= _desc.winHeight;
	desc.width				= _desc.winWidth;
	desc.hwnd				= WindowShell::self()->getHWND();
	desc.MSAA				= _desc.multisampling;
	desc.MSAASampleCount	= 4;
	desc.vSync				= _desc.vSync;
	desc.fullScreen			= _desc.fullscreen;

	if(!D3DShell::self()->init(desc))
		return false;

	return true;

}
bool FlyEngine_Core::_InitGBuffers()
{
	BaseShader::BASE_SHADER_DESC gBufferDesc;

	gBufferDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferDesc.device = D3DShell::self()->getDevice();
	gBufferDesc.VSFilename = L"../Resources/Shaders/deferredShaderTextVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/deferredShaderTextPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNT_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	this->gbufferShader = new GBufferShader();
	if(!this->gbufferShader->init(gBufferDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitLightShader()
{
	BaseShader::BASE_SHADER_DESC lightShaderDesc;

	lightShaderDesc.dc = D3DShell::self()->getDeviceContext();
	lightShaderDesc.device = D3DShell::self()->getDevice();
	lightShaderDesc.VSFilename = L"../Resources/Shaders/LightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/LightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexPNC_InputElementDesc;
	lightShaderDesc.nrOfElements = 3;

	this->lightShader = new LightShader();
	if(!this->lightShader->init(lightShaderDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitColorShader()
{
	BaseShader::BASE_SHADER_DESC colorShaderDesc;

	colorShaderDesc.dc = D3DShell::self()->getDeviceContext();
	colorShaderDesc.device = D3DShell::self()->getDevice();
	colorShaderDesc.VSFilename = L"../Resources/Shaders/TextLightVS.vs";
	colorShaderDesc.PSFilename = L"../Resources/Shaders/TextLightPS.ps";
	colorShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	colorShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	colorShaderDesc.nrOfElements = 2;

	this->colorShader = new ColorShader();
	if(!this->colorShader->init(colorShaderDesc))
		return false;

	return true;
}
bool FlyEngine_Core::_InitMatrixBuffer()
{
	this->matrixBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC matrixBufferDesc;
	matrixBufferDesc.dc = D3DShell::self()->getDeviceContext();
	matrixBufferDesc.device = D3DShell::self()->getDevice();
	matrixBufferDesc.elementSize = sizeof(cBufferMatrix);
	matrixBufferDesc.nrOfElements = 1;
	matrixBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER;
	matrixBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->matrixBuffer->Initialize(matrixBufferDesc)))
		return false;
	
	return true;
}
void FlyEngine_Core::_InitCam()
{
	this->defaultCam->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	this->defaultCam->SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);
	this->activeCamera = this->defaultCam;
}
//####################################################################//
