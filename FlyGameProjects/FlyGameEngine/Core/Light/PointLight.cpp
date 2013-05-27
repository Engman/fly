#include "PointLight.h"
PointLight::PointLight(int type)
	: Entity(type)
{

}
PointLight::~PointLight()
{

}

void PointLight::Initialize(PointLightProxy data, IShader* shader)
{
	this->pointLight = data;

	this->pointLightData = new BaseBuffer();


	BaseBuffer::BUFFER_INIT_DESC dirLightDesc;
	dirLightDesc.dc = D3DShell::self()->getDeviceContext();
	dirLightDesc.device = D3DShell::self()->getDevice();
	dirLightDesc.elementSize = sizeof(PointLightProxy);
	dirLightDesc.nrOfElements = 1;
	dirLightDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	dirLightDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(pointLightData->Initialize(dirLightDesc)))
	{
		//return false;
	}
	
	PointLightProxy* pLightBuffer = (PointLightProxy*) pointLightData->Map();
	pLightBuffer->ambient =	data.ambient;
	pLightBuffer->diffuse =	data.diffuse;
	pLightBuffer->specular =	data.specular;
	pLightBuffer->attenuate = data.attenuate;
	pLightBuffer->posRange = data.posRange;
	pointLightData->Unmap();

	D3DXMATRIX translate; 
	D3DXMatrixTranslation(&translate, data.posRange.x, data.posRange.y, data.posRange.z);
	lightSphere.Initiate(data.posRange.w, 10,10, D3DShell::self()->getDevice(), shader, translate);
	
}
void PointLight::Render(ViewFrustum& frustum)
{
	
	lightSphere.Render(pointLightData, this->world);

}

BaseBuffer* PointLight::getLight()
{
	return this->pointLightData;
}