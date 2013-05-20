#include "PointLight.h"
PointLight::PointLight(int type)
	: Entity(type)
{

}
PointLight::~PointLight()
{

}

void PointLight::Initialize(PointLightProxy data)
{
	this->pointLight = data;

	this->light = new BaseBuffer();


	BaseBuffer::BUFFER_INIT_DESC dirLightDesc;
	dirLightDesc.dc = D3DShell::self()->getDeviceContext();
	dirLightDesc.device = D3DShell::self()->getDevice();
	dirLightDesc.elementSize = sizeof(PointLightProxy);
	dirLightDesc.nrOfElements = 1;
	dirLightDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	dirLightDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(light->Initialize(dirLightDesc)))
	{
		//return false;
	}
	
	PointLightProxy* pLightBuffer = (PointLightProxy*) light->Map();
	pLightBuffer->ambient =	data.ambient;
	pLightBuffer->diffuse =	data.diffuse;
	pLightBuffer->specular =	data.specular;
	pLightBuffer->attenuate = data.attenuate;
	pLightBuffer->posRange = data.posRange;
	light->Unmap();
	
}

BaseBuffer* PointLight::getLight()
{
	return this->light;
}