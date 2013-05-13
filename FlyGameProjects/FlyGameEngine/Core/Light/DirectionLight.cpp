#include "DirectionLight.h"
DirectionLight::DirectionLight(int type)
	: Entity(type)
{

}
DirectionLight::~DirectionLight()
{

}

void DirectionLight::Initialize(DirectionalLightProxy data)
{
	D3DXVECTOR3 lookAt(1,-3,-1);
	D3DXVec3Normalize(&lookAt,&lookAt);

	this->camera.setLookAt(vec3(0.0f, 50.0f, -100.0f), lookAt, vec3(0.0f, 1.0f, 0.0f));
	this->camera.setPerspective((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->camera.updateView();
	
	this->dirLight = data;

	this->light = new BaseBuffer();


	BaseBuffer::BUFFER_INIT_DESC dirLightDesc;
	dirLightDesc.dc = D3DShell::self()->getDeviceContext();
	dirLightDesc.device = D3DShell::self()->getDevice();
	dirLightDesc.elementSize = sizeof(DirectionalLightProxy);
	dirLightDesc.nrOfElements = 1;
	dirLightDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	dirLightDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(light->Initialize(dirLightDesc)))
	{
		//return false;
	}
	
	DirectionalLightProxy* dirLightBuffer = (DirectionalLightProxy*) light->Map();
	dirLightBuffer->ambient =	data.ambient;
	dirLightBuffer->diffuse =	data.diffuse;
	dirLightBuffer->specular =	data.specular;
	dirLightBuffer->direction = data.direction;
	light->Unmap();
	
}

BaseBuffer* DirectionLight::getLight()
{
	return this->light;
}

D3DXMATRIX DirectionLight::getView()
{
	return this->camera.getView();
}
D3DXMATRIX DirectionLight::getProjection()
{
	return this->camera.getProj();
}