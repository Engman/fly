#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H


#include "..\BaseBuffer.h"
#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\Proxy.h"
#include "..\D3DShell.h"
#include "..\Entity.h"
#include "..\Mesh\FullScreenQuad.h"
#include "..\..\Util\Camera.h"

class DirectionLight : public Entity
{
public:
	struct DIRLIGHT_DESC
	{
		DirectionalLightProxy data;
		LightViewProj viewProj; 
		IShader* shader;
	
	};
private:
	SmartPtrStd<BaseBuffer> dirLightData; 
	SmartPtrStd<BaseBuffer> dirLightViewProj;
	FullScreenQuad fullScreenLight;
	bool castShadows;


	void initLightDataBuffer(DirectionalLightProxy data);
	void initLightViewProjBuffer(LightViewProj viewProj);

public:
	DirectionLight(int type);
	virtual ~DirectionLight();

	void Initialize(DIRLIGHT_DESC lightData);
	void Render(ViewFrustum& frustum);
	
	
	
	bool castShadow();
	BaseBuffer* getLightViewProj();
	BaseBuffer* getLight();
};
#endif