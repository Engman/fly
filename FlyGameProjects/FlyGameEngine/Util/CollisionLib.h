#ifndef COLLISIONLIB_H
#define COLLISIONLIB_H

#include <D3DX11\D3DX10math.h>
#include "BoundingVolumes.h"

// Function prototypes
bool FrustumVSPoint(D3DXPLANE planes[6], D3DXVECTOR3& point);
bool FrustumVSBox(ViewFrustum& frustum, BoundingBox& box);
bool FrustumVSSphere(ViewFrustum& frustum, BoundingSphere& sphere);
bool FrustumVSRay(D3DXPLANE planes[6], D3DXVECTOR3& ray);

bool RayVSPoint(D3DXVECTOR3& ray, D3DXVECTOR3& point);
bool RayVSBox(D3DXVECTOR3& ray, D3DXVECTOR3& minPoint, D3DXVECTOR3& maxPoint);
bool RayVSSphere(D3DXVECTOR3& rayDirection, D3DXVECTOR3& rayOrigin, D3DXVECTOR3& center, float& radius);
float RayVSSphereLength(D3DXVECTOR3& rayDirection, D3DXVECTOR3& rayOrigin, D3DXVECTOR3& center, float& radius);

bool PlaneVSPoint(D3DXPLANE& plane, D3DXVECTOR3& point);

bool BoxVSBox(BoundingBox& box1, BoundingBox& box2);
bool BoxVSSphere(BoundingBox& box, BoundingSphere& sphere);
bool BoxVSPoint(BoundingBox& box, D3DXVECTOR3& point);
bool BoxVSTriangle(BoundingBox& box, D3DXVECTOR3 triangle[3]);

bool SphereVSSphere(BoundingSphere& sphere, BoundingSphere& sphere2);

bool TriangleVSTriangle(D3DXVECTOR3 triangle1[3], D3DXVECTOR3 triangle2[3]);

D3DXVECTOR3  closestPointOnTriangle(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 p);

D3DXVECTOR3 closestPointOnLine(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 p);

float intersect(D3DXVECTOR3 pOrigin, D3DXVECTOR3 pNormal, D3DXVECTOR3 rOrigin, D3DXVECTOR3 rVector);
float intersectSphere(D3DXVECTOR3 rO, D3DXVECTOR3 rV, D3DXVECTOR3 sO, float sR);

bool CheckPointInTriangle(D3DXVECTOR3 point, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);

#endif

