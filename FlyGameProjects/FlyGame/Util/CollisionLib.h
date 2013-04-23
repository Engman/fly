#ifndef COLLISIONLIB_H
#define COLLISIONLIB_H

#include <D3DX11\D3DX10math.h>
#include "BoundingVolumes.h"

// Function prototypes
bool FrustumVSPoint(D3DXPLANE planes[6], D3DXVECTOR3& point);
bool FrustumVSBox(ViewFrustum frustum, BoundingBox box);
bool FrustumVSSphere(ViewFrustum frustum, BoundingSphere sphere);
bool FrustumVSRay(D3DXPLANE planes[6], D3DXVECTOR3& ray);

bool RayVSPoint(D3DXVECTOR3& ray, D3DXVECTOR3& point);
bool RayVSBox(D3DXVECTOR3& ray, D3DXVECTOR3& minPoint, D3DXVECTOR3& maxPoint);
bool RayVSSphere(D3DXVECTOR3& rayDirection, D3DXVECTOR3 rayOrigin, D3DXVECTOR3& center, float& radius);

bool PlaneVSPoint(D3DXPLANE& plane, D3DXVECTOR3& point);

bool BoxVSBox(BoundingBox box1, BoundingBox box2);
bool BoxVSSphere(BoundingBox box, BoundingSphere sphere);

#endif

