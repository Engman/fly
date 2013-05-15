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
	DirectionLight* dLight; 

public:

	LightHolder();
	~LightHolder();

	
	bool Initialize();
	void addLight(DirectionalLightProxy lightProxy, IShader* shader);
	void addLight(PointLightProxy lightProxy, IShader* shader);
	BaseBuffer* getDirLight(int nr);
	BaseBuffer* getPointLight(int nr);
	void setCamViewBuffer(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 camPos);
	BaseBuffer* getCamViewBuffer();
	int getNrOfDirLight();
	D3DXMATRIX getDirLightView(int nr);
	D3DXMATRIX getDirLightProjection(int nr);
	void RenderDirLight(ViewFrustum viewFustrum, IShader* lightShader, IShader* shadowShader);
};
#endif