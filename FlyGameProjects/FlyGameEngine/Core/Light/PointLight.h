#ifndef POINTLIGHT_H
#define POINTLIGHT_H


#include "..\BaseBuffer.h"
#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\Proxy.h"
#include "..\D3DShell.h"
#include "..\Entity.h"
#include "..\Mesh\Sphere.h"


class PointLight : public Entity
{
private:
	PointLightProxy pointLight; 
	SmartPtrStd<BaseBuffer> pointLightData; 
	Sphere lightSphere; 

public:
	PointLight(int type);
	~PointLight();

	bool Initialize(PointLightProxy data, IShader* shader);
	void Render(ViewFrustum& frustum);


	BaseBuffer* getLight();
	float getRadie();

};
#endif