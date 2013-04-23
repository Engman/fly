#include "CollisionLib.h"

// Function bodies
bool FrustumVSBox(ViewFrustum frustum, BoundingBox box)
{
	if(!BoxVSSphere(box, frustum.sphere))
	{
		return false;
	}

	for(int i=0; i<6; i++) 
	{
		if(D3DXPlaneDotCoord(&frustum.planes[i],& box.minPoint) >= 0.0f)
		{
			continue;
		}
		
		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.minPoint.x, box.minPoint.y, box.maxPoint.z)) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.minPoint.x, box.maxPoint.y, box.minPoint.z)) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.maxPoint.x, box.minPoint.y, box.minPoint.z)) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&frustum.planes[i], &box.maxPoint) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.minPoint.x, box.maxPoint.y, box.maxPoint.z)) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.maxPoint.x, box.minPoint.y, box.maxPoint.z)) >= 0.0f)
		{
			continue;
		}
		
		if(D3DXPlaneDotCoord(&frustum.planes[i], &D3DXVECTOR3(box.maxPoint.x, box.maxPoint.y, box.minPoint.z)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool RayVSSphere(D3DXVECTOR3& rayDirection, D3DXVECTOR3 rayOrigin, D3DXVECTOR3& center, float& radius)
{
	D3DXVECTOR3 l = center - rayOrigin;
	float l2 = D3DXVec3Dot(&l, &l);
	float h = D3DXVec3Dot(&l, &rayDirection);
	float r2 = radius*radius;

	if(h >= 0.0f)
	{
		float m2 = l2 - h*h;
		
		if(m2 <= r2)
		{
			return true;
		}
	}

	return false;
}

bool BoxVSBox(BoundingBox box1, BoundingBox box2)
{
	if(box1.maxPoint.x < box2.minPoint.x || box2.maxPoint.x < box1.minPoint.x) return false;

	if(box1.maxPoint.y < box2.minPoint.y || box2.maxPoint.y < box1.minPoint.y) return false;

	if(box1.maxPoint.z < box2.minPoint.z || box2.maxPoint.z < box1.minPoint.z) return false;

	return true;
}
bool BoxVSSphere(BoundingBox box, BoundingSphere sphere)
{
	D3DXVECTOR3 closestPoint;

	D3DXVECTOR3 boxMiddle = box.maxPoint*0.5f + box.minPoint*0.5f;
	D3DXVECTOR3 sphereAxis = sphere.center - boxMiddle;
	float dist = D3DXVec3Length(&sphereAxis);

	D3DXVec3Normalize(&sphereAxis, &sphereAxis);

	if( sphereAxis.x >= sphereAxis.y && sphereAxis.x >= sphereAxis.z )
        sphereAxis /= sphereAxis.x;
    else if( sphereAxis.y >= sphereAxis.x && sphereAxis.y >= sphereAxis.z )
        sphereAxis /= sphereAxis.y;
    else
        sphereAxis /= sphereAxis.z;

	D3DXVECTOR3 extents = (box.maxPoint - box.minPoint);
    sphereAxis.x *= extents.x / 2.0f;
    sphereAxis.y *= extents.y / 2.0f;
    sphereAxis.z *= extents.z / 2.0f;

	double distanceSquared = D3DXVec3LengthSq(&D3DXVECTOR3(closestPoint - sphere.center));
 
	return dist <= (sphere.radius + D3DXVec3Length(&sphereAxis));
}