#include "LightHolder.h"

LightHolder::LightHolder()
{
}
LightHolder::~LightHolder()
{
}

bool LightHolder::Initialize()
{
	this->lightBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC lightBufferDesc;
	lightBufferDesc.dc = D3DShell::self()->getDeviceContext();
	lightBufferDesc.device = D3DShell::self()->getDevice();
	lightBufferDesc.elementSize = sizeof(DirectionalLightProxy);
	lightBufferDesc.nrOfElements = 1;
	lightBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	lightBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->lightBuffer->Initialize(lightBufferDesc)))
		return false;

	return true;
}
void LightHolder::addLight(DirectionalLightProxy lightProxy)
{
	DirectionLight light; 
	light.Initialize(lightProxy);
	dirLights.push_back(light);
	
}
BaseBuffer* LightHolder::getDirLights()
{
	
	return this->lightBuffer;
}
