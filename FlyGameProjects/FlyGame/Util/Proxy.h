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
	D3DXVECTOR4 attenuate;	//Vector4(Vector3 attenuate, 0.0f)

	SpotLightProxy()	{ ZeroMemory(this, sizeof(this)); }
};
/** Describes how the point light is used on gpu */
struct PointLightProxy
{
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 posRange;	//Vcetor4(Vector3 position, float range)
	D3DXVECTOR4 attenuate;	//Vector4(Vector3 Attenuate, 0.0f)

	PointLightProxy()	{ ZeroMemory(this, sizeof(this)); }
};
/** Describes how the directional light is used on gpu */
struct DirectionalLightProxy
{
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 direction;	//Vector4(Vector3 direction, 0.0f)

	DirectionalLightProxy()	{ ZeroMemory(this, sizeof(this)); }
};

/** Describes how the lightning cbuffer is used on gpu */
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

struct ViewFrustum
{
	D3DXPLANE planes[6];

	D3DXVECTOR3 spherePoint;
	float radius;
};

#endif

