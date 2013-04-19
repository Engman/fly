#include "DirectionLight.h"
DirectionLight::DirectionLight()
{

}
DirectionLight::~DirectionLight()
{

}

void DirectionLight::Initialize(DirectionalLightProxy data)
{
	this->dirLight = data;
}