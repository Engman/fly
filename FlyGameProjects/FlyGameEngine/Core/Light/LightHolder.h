#ifndef LIGHTHOLDER_H
#define LIGHTHOLDER_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <vector>
#include "..\..\Util\misc.h"
#include "..\..\Util\SmartPtrs.h"
#include "DirectionLight.h"
#include "..\..\Util\Proxy.h"
#include "..\BaseBuffer.h"
#include "..\D3DShell.h"

class LightHolder
{
private:
	std::vector<DirectionLight> dirLights;
	SmartPtrStd<BaseBuffer> lightBuffer;

public:
	LightHolder();
	~LightHolder();

	bool Initialize();
	void addLight(DirectionalLightProxy lightProxy);
	BaseBuffer* getDirLights();
	


};
#endif