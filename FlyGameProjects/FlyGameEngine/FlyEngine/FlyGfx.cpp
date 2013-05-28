#include "FlyEngine_Core.h"

#include "..\Core\WindowShell.h"
#include "..\Core\Render\GBufferShader.h"
#include "..\Core\Render\GBufferAnimationShader.h"
#include "..\Core\Render\FinalShader.h"
#include "..\Core\Render\LightShader.h"
#include "..\Core\Render\ShadowMapShader.h"
#include "..\Core\Render\BlurShader.h"
#include "..\Util\Camera.h"
#include "..\Core\AudioClass.h"



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
	this->finalShader->draw(gBufferDrawData);
	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}

void FLYCALL FlyEngine_Core::Gfx_BeginDeferredScene()
{
	D3DShell::self()->BeginGBufferRenderTargets(false);
	D3DShell::self()->BeginGBufferRenderTargets(true);
}
void FLYCALL FlyEngine_Core::Gfx_DrawSkyBox()
{
	//draw without depth
	D3DShell::self()->BeginGBufferRenderTargets(false);
	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	gBufferDrawData.projection = this->activeCamera->GetProjectionMatrix();
	
	this->gBufferNoDepthShader->draw(gBufferDrawData);

}

void FLYCALL FlyEngine_Core::Gfx_DrawGbuffer()
{
	D3DShell::self()->BeginGBufferRenderTargets(true);

	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	gBufferDrawData.projection = this->activeCamera->GetProjectionMatrix();
	
	this->gbufferShader->draw(gBufferDrawData);

	this->gbufferBumpShader->draw(gBufferDrawData);
}
void FLYCALL FlyEngine_Core::Gfx_DrawGbufferOrtho()
{
	float blend[4] = {1.0f,1.0f,1.0f,1.0f};

	D3DShell::self()->setBlendModeState(FLAGS::BLEND_MODE_AlphaBlend, blend,  0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_DisabledDepth,1); 

	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	gBufferDrawData.projection = this->activeCamera->GetOrthogonalMatrix();

	this->gbufferShader->draw(gBufferDrawData);

	//reset the blend state to normal
	D3DShell::self()->getDeviceContext()->OMSetBlendState(0,0,0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_EnabledDepth,1);
}
void FLYCALL FlyEngine_Core::Gfx_DrawShadows(vector<LightViewProj*> *shadowViews)
{
	IShader::PER_FRAME_DATA shadowsDrawData;
	shadowsDrawData.dc = D3DShell::self()->getDeviceContext(); 

	for(int i= 0; i<(int)shadowViews->size(); i++)
	{
		D3DShell::self()->BeginShadowRenderTarget(i);

		shadowsDrawData.view = shadowViews->at(0)->lView; 
		shadowsDrawData.projection = shadowViews->at(0)->lProj; 
		
		this->shadowMapShader->draw(shadowsDrawData);
	}
}
void FLYCALL FlyEngine_Core::Gfx_DrawLighting()
{
	//set light render target and give it the albedo, normal and specular textures
	D3DShell::self()->BeginLightRenderTarget();

	float blend[4] = {0.75f,0.75f,0.75f,1.0f};

	D3DShell::self()->setBlendModeState(FLAGS::BLEND_MODE_Custom, blend,  0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_DisabledDepth,1); 

	IShader::PER_FRAME_DATA lightDrawData;
	lightDrawData.dc = D3DShell::self()->getDeviceContext();
	lightDrawData.view = this->activeCamera->GetViewMatrix();
	lightDrawData.projection = this->activeCamera->GetProjectionMatrix();

	D3DXMATRIX invView, invViewProj, viewProj;

	viewProj =  lightDrawData.view * lightDrawData.projection;
	float det = D3DXMatrixDeterminant(&lightDrawData.view);
	D3DXMatrixInverse(&invView, &det, &lightDrawData.view);
	det= D3DXMatrixDeterminant(&viewProj);
	D3DXMatrixInverse(&invViewProj, &det, &viewProj);

	D3DXMatrixTranspose(&invView, &invView);
	D3DXMatrixTranspose(&invViewProj, &invViewProj);
	
	CameraView* cb = (CameraView*)this->cameraBuffer->Map();
	cb->cameraPos =  this->activeCamera->GetPosition();
	cb->mInvView = invView;
	cb->mInvViewProj  = invViewProj;
	cameraBuffer->Unmap();
	lightDrawData.camForLight = this->cameraBuffer; 

	//this->dirLightShader->draw(lightDrawData);

	this->pointLightShader->draw(lightDrawData);

	//reset the blend state to normal
	D3DShell::self()->getDeviceContext()->OMSetBlendState(0,0,0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_EnabledDepth,1); 


}
void FLYCALL FlyEngine_Core::Gfx_DrawBlur()
{
	//horizontal blur
	D3DShell::self()->BeginBlurRenderTarget();
	D3DShell::self()->setBlurSRV();

	IShader::PER_FRAME_DATA blurDrawData;
	blurDrawData.dc = D3DShell::self()->getDeviceContext();
	//blurDrawData.view = this->activeCamera->GetViewMatrix();
	//blurDrawData.projection = this->activeCamera->GetProjectionMatrix();

	this->fsq->SetShader(blurHorizontShader);
	this->fsq->Render();

	this->blurHorizontShader->draw(blurDrawData);

	//vertical blur
	D3DShell::self()->BeginBlur2RenderTarget();
	D3DShell::self()->setBlur2SRV();
	this->fsq->SetShader(blurVerticalShader);
	this->fsq->Render();

	this->blurVerticalShader->draw(blurDrawData);
}

void FLYCALL FlyEngine_Core::Gfx_DrawFinalPicture(vector<LightViewProj*> *shadowViews)
{
	D3DShell::self()->setRenderTarget();

	D3DShell::self()->beginScene();

	IShader::PER_FRAME_DATA finalPictureDrawData;
	finalPictureDrawData.dc = D3DShell::self()->getDeviceContext();
	finalPictureDrawData.view = this->activeCamera->GetViewMatrix();
	finalPictureDrawData.projection = this->activeCamera->GetProjectionMatrix();


	D3DXMATRIX invCameraViewProj; 
	invCameraViewProj =  finalPictureDrawData.view *  finalPictureDrawData.projection;
	float det = D3DXMatrixDeterminant(&invCameraViewProj);
	D3DXMatrixInverse(&invCameraViewProj, &det, &invCameraViewProj);
	
	D3DXMATRIX lightViewProj;

	
	D3DXMATRIX lView = shadowViews->at(0)->lView;
	D3DXMATRIX lProj = shadowViews->at(0)->lProj; 
	

	D3DXMATRIX lViewProj = lView * lProj;
	D3DXMatrixTranspose(&lViewProj, &lViewProj);
	D3DXMatrixTranspose(&invCameraViewProj, &invCameraViewProj);

	CameraView* camView = (CameraView*)cameraBuffer->Map();
	camView->mInvViewProj = invCameraViewProj;
	camView->mInvView	  = lViewProj; 
	camView->cameraPos	= this->activeCamera->GetPosition();
	camView->padd		=600;
	cameraBuffer->Unmap();

	finalPictureDrawData.camForLight = cameraBuffer;

	this->fsq->SetShader(this->finalShader);
	this->fsq->Render();
	this->finalShader->draw(finalPictureDrawData);

	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();

}


void FLYCALL FlyEngine_Core::Gfx_EndDeferredScene()
{
	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	gBufferDrawData.projection = this->activeCamera->GetProjectionMatrix();
	
	this->gbufferShader->draw(gBufferDrawData);

	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();

	this->fsq->SetShader(this->finalShader);
	this->fsq->Render();
	this->finalShader->draw(gBufferDrawData);
	

	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}

void FLYCALL FlyEngine_Core::Gfx_EndDeferredSceneOrtho()
{

	 float blend[4] = {1.0f,1.0f,1.0f,1.0f};

	 D3DShell::self()->setBlendModeState(FLAGS::BLEND_MODE_AlphaBlend, blend,  0xffffffff);


	 IShader::PER_FRAME_DATA gBufferDrawData;
	 gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	 gBufferDrawData.view = this->activeCamera->GetViewMatrix();
	 gBufferDrawData.projection = this->activeCamera->GetOrthogonalMatrix();
 
	 this->gbufferShader->draw(gBufferDrawData);

	 //reset the blend state to normal
	 D3DShell::self()->getDeviceContext()->OMSetBlendState(0,0,0xffffffff);

	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();


	this->fsq->SetShader(this->finalColorShader);
	this->fsq->Render();

	gBufferDrawData.camForLight = new BaseBuffer(); //finalBuffer;//
	this->finalColorShader->draw(gBufferDrawData);
	delete gBufferDrawData.camForLight; 

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
		case FlyShader_gBufferDefault:
			return this->gbufferShader;
		break;

		case  FlyShader_gBufferNoDepth:
			return this->gBufferNoDepthShader; 
		break;
		case FlyShader_gBufferBump:
			return this->gbufferBumpShader;
		break;
		case FlyShader_gBufferAnimated:
			return this->gBufferAnimationShader;
		break;

		case FlyShader_Final:
			return this->finalShader;
		break;

		case FlyShader_DirLight:
			return this->dirLightShader;
		break;
		case  FlyShader_PointLight:
			return this->pointLightShader;
		break;

		case FlyShader_Shadow:
			return this->shadowMapShader;
		break;

		case FlyShader_BlurHorizont:
			return this->blurHorizontShader;
		break;

		case  FlyShader_BlurVertical:
			return this->blurVerticalShader;
		break;

	}
	return NULL;
}

void FLYCALL FlyEngine_Core::Gfx_GetShader(vector<IShader*>& shaders)
{
	shaders.push_back(this->gbufferShader);
	shaders.push_back(this->gBufferNoDepthShader);
	shaders.push_back(this->gbufferBumpShader);
	shaders.push_back(this->gBufferAnimationShader);
	shaders.push_back(this->finalShader);
	shaders.push_back(this->dirLightShader);
	shaders.push_back(this->pointLightShader);
	shaders.push_back(this->shadowMapShader);
	shaders.push_back(this->blurHorizontShader);
	shaders.push_back(this->blurVerticalShader);
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
	desc.fullScreen			= !_desc.fullscreen;

	if(!D3DShell::self()->init(desc))
		return false;


	return true;

}
bool FlyEngine_Core::_InitGBufferShader()
{
	BaseShader::BASE_SHADER_DESC gBufferDesc;

	gBufferDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferDesc.device = D3DShell::self()->getDevice();
	gBufferDesc.VSFilename = L"../Resources/Shaders/g-BufferVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/g-BufferPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNT_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	if(!this->gbufferShader->init(gBufferDesc))	
		return false;

	if(!this->gBufferNoDepthShader->init(gBufferDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitBumpShader()
{
	BaseShader::BASE_SHADER_DESC gBufferBumpDesc;

	gBufferBumpDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferBumpDesc.device = D3DShell::self()->getDevice();
	gBufferBumpDesc.VSFilename = L"../Resources/Shaders/g-bufferBumpVS.vs";
	gBufferBumpDesc.PSFilename = L"../Resources/Shaders/g-BufferBumpPS.ps";
	gBufferBumpDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferBumpDesc.polygonLayout = VERTEX::VertexPNTBT_InputElementDesc;
	gBufferBumpDesc.nrOfElements = 5;

	if(!this->gbufferBumpShader->init(gBufferBumpDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitAnimationShader()
{
	BaseShader::BASE_SHADER_DESC gBufferAnimationDesc;

	gBufferAnimationDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferAnimationDesc.device = D3DShell::self()->getDevice();
	gBufferAnimationDesc.VSFilename = L"../Resources/Shaders/g-bufferAnimationVS.vs";
	gBufferAnimationDesc.PSFilename = L"../Resources/Shaders/g-BufferPS.ps";
	gBufferAnimationDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferAnimationDesc.polygonLayout = VERTEX::Animation_2Frame_InputElementDesc;
	gBufferAnimationDesc.nrOfElements = 6;

	if(!this->gBufferAnimationShader->init(gBufferAnimationDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitFinalShader()
{
	BaseShader::BASE_SHADER_DESC finalShaderDesc;

	finalShaderDesc.dc = D3DShell::self()->getDeviceContext();
	finalShaderDesc.device = D3DShell::self()->getDevice();
	finalShaderDesc.VSFilename = L"../Resources/Shaders/FullScreenQuadVS.vs";
	finalShaderDesc.PSFilename = L"../Resources/Shaders/FinalPS.ps";
	finalShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	finalShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	finalShaderDesc.nrOfElements = 2;


	if(!this->finalShader->init(finalShaderDesc))
		return false;
	
	finalShaderDesc.PSFilename = L"../Resources/Shaders/colorPS.ps";

	if(!this->finalColorShader->init(finalShaderDesc))
		return false;

	return true;
}
bool FlyEngine_Core::_InitDirLightShader()
{
	BaseShader::BASE_SHADER_DESC lightShaderDesc;

	lightShaderDesc.dc = D3DShell::self()->getDeviceContext();
	lightShaderDesc.device = D3DShell::self()->getDevice();
	lightShaderDesc.VSFilename = L"../Resources/Shaders/dirLightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/dirLightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	lightShaderDesc.nrOfElements = 2;

	if(!this->dirLightShader->init(lightShaderDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitPointLightShader()
{
	BaseShader::BASE_SHADER_DESC lightShaderDesc;

	lightShaderDesc.dc = D3DShell::self()->getDeviceContext();
	lightShaderDesc.device = D3DShell::self()->getDevice();
	lightShaderDesc.VSFilename = L"../Resources/Shaders/pointLightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/pointLightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexP_InputElementDesc;
	lightShaderDesc.nrOfElements = 1;

	if(!this->pointLightShader->init(lightShaderDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitShadowMapShader()
{
	BaseShader::BASE_SHADER_DESC shadowShaderDesc;

	shadowShaderDesc.dc = D3DShell::self()->getDeviceContext();
	shadowShaderDesc.device = D3DShell::self()->getDevice();
	shadowShaderDesc.VSFilename = L"../Resources/Shaders/ShadowMapVS.vs";
	shadowShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	shadowShaderDesc.polygonLayout = VERTEX::VertexPNT_InputElementDesc;
	shadowShaderDesc.nrOfElements = 3;

	if(!this->shadowMapShader->init(shadowShaderDesc))	
		return false;

	return true;
}
bool FlyEngine_Core::_InitBlurShaders()
{
	BaseShader::BASE_SHADER_DESC blurShaderDesc;

	blurShaderDesc.dc = D3DShell::self()->getDeviceContext();
	blurShaderDesc.device = D3DShell::self()->getDevice();
	blurShaderDesc.VSFilename = L"../Resources/Shaders/FullScreenQuadVS.vs";
	blurShaderDesc.PSFilename = L"../Resources/Shaders/BlurHorizontPS.ps";
	blurShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	blurShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	blurShaderDesc.nrOfElements = 2;

	if(!this->blurHorizontShader->init(blurShaderDesc))	
		return false;


	blurShaderDesc.PSFilename = L"../Resources/Shaders/BlurVerticalPS.ps";

	if(!this->blurVerticalShader->init(blurShaderDesc))	
		return false;

	return true;
}

bool FlyEngine_Core::_InitCameraBuffer()
{
	this->cameraBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC cameraBufferDesc;
	cameraBufferDesc.dc = D3DShell::self()->getDeviceContext();
	cameraBufferDesc.device = D3DShell::self()->getDevice();
	cameraBufferDesc.elementSize = sizeof(CameraView);
	cameraBufferDesc.nrOfElements = 1;
	cameraBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	cameraBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->cameraBuffer->Initialize(cameraBufferDesc)))
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
