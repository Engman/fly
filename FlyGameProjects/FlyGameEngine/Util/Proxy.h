#ifndef PROXY_H
#define PROXY_H

#include <D3DX11\D3DX10math.h>
#include <vector>

/** Describes how the spot light is used on gpu */
struct SpotLightProxy
{
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 posRange;	//Vector4(Vector3 position, float range)
	D3DXVECTOR4 dirRad;		//Vector4(Vector3 direction, float spotRadius)

	SpotLightProxy()
		:ambient(0.0f, 0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f, 0.0f), 
		specular(0.0f, 0.0f, 0.0f, 0.0f), posRange(0.0f, 0.0f, 0.0f, 0.0f),
		dirRad(0.0f, 0.0f, 0.0f, 0.0f)
	{  }
};
/** Describes how the point light is used on gpu */
struct PointLightProxy
{
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 posRange;	//Vcetor4(Vector3 position, float range)

	PointLightProxy()	
		:ambient(0.0f, 0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f, 0.0f), 
		specular(0.0f, 0.0f, 0.0f, 0.0f), posRange(0.0f, 0.0f, 0.0f, 0.0f)
	{  }
};
/** Describes how the directional light is used on gpu */
struct DirectionalLightProxy
{
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 direction;	//Vector4(Vector3 direction, 0.0f)

	DirectionalLightProxy()	
		:ambient(0.0f, 0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f, 0.0f), 
		specular(0.0f, 0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 0.0f, 0.0f)
	{ }
};

/** Describes how the matrix cbuffer is structured on the gpu */
struct cBufferMatrix
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX worldInvTranspose;
};
/** Describes how the lightning cbuffer is used on gpu */
struct cBufferLights
{
	D3DXVECTOR3							lightCount; //Vector3(DirectionalCount, PointCount, SpotCount)
	std::vector<DirectionalLightProxy>	directionalLights;
	std::vector<PointLightProxy>		pointLights;
	std::vector<SpotLightProxy>			spotLights;
};


struct CameraView
{
	D3DXMATRIX mInvViewProj;
	D3DXMATRIX mInvView;
	D3DXVECTOR3 cameraPos;
	float padd;
};

struct LightViewProj
{
	D3DXMATRIX lView;
	D3DXMATRIX lProj;
};

struct WaterBuffer
{
	D3DXVECTOR2 waterTranslation; 
	D3DXVECTOR2 padd; 
};



#endif

