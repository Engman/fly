#ifndef LIGHTHOLDER_H
#define LIGHTHOLDER_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <vector>
#include "..\..\Util\misc.h"
//#include "..\..\Util\SmartPtrs.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "..\..\Util\Proxy.h"
//#include "..\BaseBuffer.h"
#include "..\D3DShell.h"

class LightHolder
{
private:
	std::vector<DirectionLight> dirLights;
	std::vector<PointLight> pointLights;
	SmartPtrStd<BaseBuffer> camView;

public:

	LightHolder();
	~LightHolder();

	
	bool Initialize();
	void addLight(DirectionalLightProxy lightProxy);
	void addLight(PointLightProxy lightProxy);
	BaseBuffer* getDirLight(int nr);
	BaseBuffer* getPointLight(int nr);
	void setCamViewBuffer(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 camPos);
	BaseBuffer* getCamViewBuffer();
	int getNrOfDirLight();


};
#endif