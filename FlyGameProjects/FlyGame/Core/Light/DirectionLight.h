#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H


#include "..\BaseBuffer.h"
#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\Proxy.h"
#include "..\D3DShell.h"
#include "..\Entity.h"
#include "..\Mesh\FullScreenQuad.h"

class DirectionLight : public Entity
{
private:
	DirectionalLightProxy dirLight; 
	SmartPtrStd<BaseBuffer> light; 

public:
	DirectionLight(int type);
	~DirectionLight();

	void Initialize(DirectionalLightProxy data);
	BaseBuffer* getLight();

};
#endif