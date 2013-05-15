#include "DirectionLight.h"
DirectionLight::DirectionLight(int type)
	: Entity(type)
{

}
DirectionLight::~DirectionLight()
{

}

void DirectionLight::Initialize(DirectionalLightProxy data, IShader* shader, bool castShadow)
{
	D3DXVECTOR3 lookAt(1,-3,-1);
	D3DXVec3Normalize(&lookAt,&lookAt);

	this->camera.SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->camera.SetPosition(D3DXVECTOR3(0.0f, 50.0f, -100.0f));
	this->camera.SetRotation(0,0,0);
	
	this->dirLight = data;

	this->dirLightData = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC dirLightDesc;
	dirLightDesc.dc = D3DShell::self()->getDeviceContext();
	dirLightDesc.device = D3DShell::self()->getDevice();
	dirLightDesc.elementSize = sizeof(DirectionalLightProxy);
	dirLightDesc.nrOfElements = 1;
	dirLightDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	dirLightDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(dirLightData->Initialize(dirLightDesc)))
	{
		//return false;
	}
	
	DirectionalLightProxy* dirLightBuffer = (DirectionalLightProxy*) dirLightData->Map();
	dirLightBuffer->ambient =	data.ambient;
	dirLightBuffer->diffuse =	data.diffuse;
	dirLightBuffer->specular =	data.specular;
	dirLightBuffer->direction = data.direction;
	dirLightData->Unmap();


	this->dirLightViewProj = new BaseBuffer();
	LightViewProj lightViewProj; 
	lightViewProj.lView = camera.GetViewMatrix();
	lightViewProj.lProj = camera.GetProjectionMatrix();

	BaseBuffer::BUFFER_INIT_DESC viewProjBufferDesc;
	viewProjBufferDesc.dc = D3DShell::self()->getDeviceContext();
	viewProjBufferDesc.device = D3DShell::self()->getDevice();
	viewProjBufferDesc.elementSize = sizeof(LightViewProj);
	viewProjBufferDesc.nrOfElements = 1;
	viewProjBufferDesc.data = &lightViewProj;
	viewProjBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	viewProjBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->dirLightViewProj->Initialize(viewProjBufferDesc)))
	{

	}

	this->fullScreenLight.Initialize(D3DShell::self()->getDevice(), shader);
	this->castShadows = castShadow;
}
void DirectionLight::Render(ViewFrustum& frustum)
{
	fullScreenLight.Render(dirLightData, dirLightViewProj);
}

BaseBuffer* DirectionLight::getLight()
{
	return this->dirLightData;
}

D3DXMATRIX DirectionLight::getView()
{
	return this->camera.GetViewMatrix();
}
D3DXMATRIX DirectionLight::getProjection()
{
	return this->camera.GetProjectionMatrix();
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