#include "DirectionLight.h"
DirectionLight::DirectionLight(int type)
	: Entity(type)
{

}
DirectionLight::~DirectionLight()
{

}

void DirectionLight::Initialize(DIRLIGHT_DESC lightData)
{

	initLightDataBuffer(lightData.data);
	initLightViewProjBuffer(lightData.viewProj);

	this->fullScreenLight.Initialize(D3DShell::self()->getDevice(), lightData.shader);
	this->castShadows = false;
}
void DirectionLight::Render(ViewFrustum& frustum)
{
	fullScreenLight.Render(dirLightData, dirLightViewProj);
}
BaseBuffer* DirectionLight::getLight()
{
	return this->dirLightData;
}
bool DirectionLight::castShadow()
{
	if(castShadows)
		return true;
	else
		return false;
}
BaseBuffer* DirectionLight::getLightViewProj()
{
	return this->dirLightViewProj; 
}
void DirectionLight::initLightDataBuffer(DirectionalLightProxy data)
{
	this->dirLightData = new BaseBuffer();

	DirectionalLightProxy dirLightBuffer;
	dirLightBuffer.ambient		=	data.ambient;
	dirLightBuffer.diffuse		=	data.diffuse;
	dirLightBuffer.specular		=	data.specular;
	dirLightBuffer.direction	=	data.direction;

	BaseBuffer::BUFFER_INIT_DESC dirLightDesc;
	dirLightDesc.dc		= D3DShell::self()->getDeviceContext();
	dirLightDesc.device = D3DShell::self()->getDevice();
	dirLightDesc.elementSize	= sizeof(DirectionalLightProxy);
	dirLightDesc.nrOfElements	= 1;
	dirLightDesc.data	= &dirLightBuffer; 
	dirLightDesc.type	= BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	dirLightDesc.usage	= BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(dirLightData->Initialize(dirLightDesc)))
	{
		MessageBox(0,L"Failed to initialize Dirlight DirectionalLightProxy Buffer", L"Error!", 0);
	}

}
void DirectionLight::initLightViewProjBuffer(LightViewProj viewProj)
{
	//this->camera.SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
	//this->camera.SetPosition(pos);
	//this->camera.SetRotation(0,0,0);

	//this->dirLight = data;

	//LightViewProj lightViewProj; 
	//	lightViewProj.lView = //camera.GetViewMatrix();
	//	lightViewProj.lProj = //camera.GetProjectionMatrix();

	this->dirLightViewProj = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC viewProjBufferDesc;
	viewProjBufferDesc.dc = D3DShell::self()->getDeviceContext();
	viewProjBufferDesc.device = D3DShell::self()->getDevice();
	viewProjBufferDesc.elementSize = sizeof(LightViewProj);
	viewProjBufferDesc.nrOfElements = 1;
	viewProjBufferDesc.data = &viewProj;
	viewProjBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	viewProjBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->dirLightViewProj->Initialize(viewProjBufferDesc)))
	{
		MessageBox(0,L"Failed to initialize Dirlight LightViewProj Buffer", L"Error!", 0);
	}
}