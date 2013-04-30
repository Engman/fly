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