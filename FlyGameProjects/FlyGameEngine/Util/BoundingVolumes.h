#ifndef BOUNDINGVOLUMES_H
#define BOUNDINGVOLUMES_H

#include <D3DX11\d3dx10math.h>

struct BoundingBox
{
	D3DXVECTOR3 maxPoint;
	D3DXVECTOR3 minPoint;
};

struct BoundingSphere
{
	D3DXVECTOR3 center;
	float radius;
};

struct ViewFrustum
{
	D3DXPLANE planes[6];
	BoundingSphere sphere;
};

struct BoundingEllipse
{
	D3DXVECTOR3 center;
	D3DXVECTOR3 radiusVector;
};

#endif