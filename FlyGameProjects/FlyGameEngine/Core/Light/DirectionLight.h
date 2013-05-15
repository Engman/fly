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
private:
	DirectionalLightProxy dirLight; 
	SmartPtrStd<BaseBuffer> dirLightData; 
	SmartPtrStd<BaseBuffer> dirLightViewProj;
	Camera camera;
	FullScreenQuad fullScreenLight;
	bool castShadows; 


public:
	DirectionLight(int type);
	~DirectionLight();

	void Initialize(DirectionalLightProxy data, IShader* shader, bool castShadow);
	BaseBuffer* getLight();
	D3DXMATRIX getView();
	D3DXMATRIX getProjection();
	void Render(ViewFrustum& frustum);
	bool castShadow();
	BaseBuffer* getLightViewProj();
};
#endif