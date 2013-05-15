#ifndef POINTLIGHT_H
#define POINTLIGHT_H


#include "..\BaseBuffer.h"
#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\Proxy.h"
#include "..\D3DShell.h"
#include "..\Entity.h"
#include "..\Mesh\Cube.h"

class PointLight : public Entity
{
private:
	PointLightProxy pointLight; 
	SmartPtrStd<BaseBuffer> light; 

public:
	PointLight(int type);
	~PointLight();

	void Initialize(PointLightProxy data);
	BaseBuffer* getLight();

};
#endif