#include "CollisionLib.h"

// Function bodies
bool FrustumVSBox(ViewFrustum frustum, BoundingBox box)
{
	if(!BoxVSSphere(box, frustum.sphere))
	{
		return false;
	}

	// Check if any one point of the cube is in the view frustum.
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
bool BoxVSPoint(BoundingBox box, D3DXVECTOR3 point)
{
	return true;
}
bool BoxVSTriangle(BoundingBox box, D3DXVECTOR3 triangle[3])
{
	D3DXVECTOR3 boxCenter;
	D3DXVECTOR3 boxHalfSize;
	D3DXVECTOR3 v0,v1,v2;
	float min,max,p0,p1,p2,rad,fex,fey,fez;
	D3DXVECTOR3 normal,e0,e1,e2;

	boxCenter[0] = (box.minPoint.x + box.maxPoint.x)*0.5f;
	boxCenter[1] = (box.minPoint.y + box.maxPoint.y)*0.5f;
	boxCenter[2] = (box.minPoint.z + box.maxPoint.z)*0.5f;

	boxHalfSize[0] = (box.maxPoint.x - box.minPoint.x)*0.5f;
	boxHalfSize[1] = (box.maxPoint.y - box.minPoint.y)*0.5f;
	boxHalfSize[2] = (box.maxPoint.z - box.minPoint.z)*0.5f;


	/* move everything so that the boxcenter is in (0,0,0) */
	v0[0] = triangle[0][0] - boxCenter[0]; 
	v0[1] = triangle[0][1] - boxCenter[1]; 
	v0[2] = triangle[0][2] - boxCenter[2]; 

	v1[0] = triangle[1][0] - boxCenter[0]; 
	v1[1] = triangle[1][1] - boxCenter[1]; 
	v1[2] = triangle[1][2] - boxCenter[2];

	v2[0] = triangle[2][0] - boxCenter[0]; 
	v2[1] = triangle[2][1] - boxCenter[1]; 
	v2[2] = triangle[2][2] - boxCenter[2];

	/* compute triangle edges */
	e0[0] = v1[0] - v0[0]; 
	e0[1] = v1[1] - v0[1]; 
	e0[2] = v1[2] - v0[2];

	e1[0] = v2[0] - v1[0]; 
	e1[1] = v2[1] - v1[1]; 
	e1[2] = v2[2] - v1[2];

	e2[0] = v0[0] - v2[0]; 
	e2[1] = v0[1] - v2[1]; 
	e2[2] = v0[2] - v2[2];

	//Check edges
	fex = fabsf(e0[0]);
	fey = fabsf(e0[1]);
	fez = fabsf(e0[2]);
	//X
	p0 = e0[2]*v0[1] - e0[1]*v0[2]; 
	p2 = e0[2]*v2[1] - e0[1]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fex*boxHalfSize[1] + fey*boxHalfSize[2]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e0[2]*v0[0] + e0[0]*v0[2];	
	p2 = -e0[2]*v2[0] + e0[0]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p1 = e0[1]*v1[0] - e0[0]*v1[1]; 
	p2 = e0[1]*v2[0] - e0[0]*v2[1]; 
	if(p2<p1) {min=p2; max=p1;} 
	else {min=p1; max=p2;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;

	fex = fabsf(e1[0]);
	fey = fabsf(e1[1]);
	fez = fabsf(e1[2]);
	//X
	p0 = e1[2]*v0[1] - e1[1]*v0[2]; 
	p2 = e1[2]*v2[1] - e1[1]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fex*boxHalfSize[1] + fey*boxHalfSize[2]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e1[2]*v0[0] + e1[0]*v0[2];	
	p2 = -e1[2]*v2[0] + e1[0]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p0 = e1[1]*v0[0] - e1[0]*v0[1]; 
	p1 = e1[1]*v1[0] - e1[0]*v1[1]; 
	if(p0<p1) {min=p0; max=p1;} 
	else {min=p1; max=p0;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;

	fex = fabsf(e2[0]);
	fey = fabsf(e2[1]);
	fez = fabsf(e2[2]);
	//X
	p0 = e2[2]*v0[1] - e2[1]*v0[2]; 
	p1 = e2[2]*v1[1] - e2[1]*v1[2]; 
	if(p0<p1) {min=p0; max=p1;} 
	else {min=p1; max=p0;} 
	rad = fez * boxHalfSize[1] + fey * boxHalfSize[2]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e2[2]*v0[0] + e2[0]*v0[2]; 
	p1 = -e2[2]*v1[0] + e2[0]*v1[2];	
	if(p0<p1) {min=p0; max=p1;}
	else {min=p1; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2];
	if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p1 = e2[1]*v1[0] - e2[0]*v1[1]; 
	p2 = e2[1]*v2[0] - e2[0]*v2[1]; 
	if(p2<p1) {min=p2; max=p1;} 
	else {min=p1; max=p2;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	if((min>rad || max<-rad) && rad != 0.0f) return false;

	//Test box against minimal bounding box for triangle
	/* test in X-direction */
	min = max = v0[0]; 
	if(v1[0]<min) min=v1[0]; 
	if(v1[0]>max) max=v1[0]; 
	if(v2[0]<min) min=v2[0]; 
	if(v2[0]>max) max=v2[0];
	if(min>boxHalfSize[0] || max<-boxHalfSize[0]) return false;
	/* test in Y-direction */
	min = max = v0[1]; 
	if(v1[1]<min) min=v1[1]; 
	if(v1[1]>max) max=v1[1]; 
	if(v2[1]<min) min=v2[1]; 
	if(v2[1]>max) max=v2[1];
	if(min>boxHalfSize[1] || max<-boxHalfSize[1]) return false;
	/* test in Z-direction */
	min = max = v0[2]; 
	if(v1[2]<min) min=v1[2]; 
	if(v1[2]>max) max=v1[2]; 
	if(v2[2]<min) min=v2[2]; 
	if(v2[2]>max) max=v2[2];
	if(min>boxHalfSize[2] || max<-boxHalfSize[2]) return false;

	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	D3DXVec3Cross(&normal, &e0, &e1);

	int q;
	D3DXVECTOR3 vmin,vmax;
	float v;

	for(q=0;q<=2;q++)
	{
		v=v0[q];	

		if(normal[q]>0.0f)
		{
			vmin[q]=-boxHalfSize[q] - v;
			vmax[q]= boxHalfSize[q] - v;
		}

		else
		{
			vmin[q]= boxHalfSize[q] - v;
			vmax[q]=-boxHalfSize[q] - v;
		}
	}

	if(D3DXVec3Dot(&normal, &vmin) > 0.0f) return false;
	if(D3DXVec3Dot(&normal, &vmax) >= 0.0f) return true;

	return false;
}

bool TriangleVSTriangle(D3DXVECTOR3 triangle1[3], D3DXVECTOR3 triangle2[3])
{
	//Make two planes with the help of points and check if either triangles points lie in the others plane
	D3DXVECTOR3 normal2;
	D3DXVec3Cross(&normal2, &D3DXVECTOR3(triangle2[1] - triangle2[0]), &D3DXVECTOR3(triangle2[2] - triangle2[0]));

	D3DXVec3Normalize(&normal2, &normal2);
	
	float dotVN2 = D3DXVec3Dot(&-normal2, &triangle2[0]);
	float dV1[3];

	for(int i = 0; i < 3; i++)
	{
		dV1[i] = D3DXVec3Dot(&normal2, &triangle1[i]) + dotVN2;
	}

	if(dV1[0] < 0 && dV1[1] < 0 && dV1[2] < 0)
	{
		return false;
	}
	else if(dV1[0] > 0 && dV1[1] > 0 && dV1[2] > 0)
	{
		return false;
	}

	D3DXVECTOR3 normal1;
	D3DXVec3Cross(&normal1, &D3DXVECTOR3(triangle1[1] - triangle1[0]), &D3DXVECTOR3(triangle1[2] - triangle1[0]));
	D3DXVec3Normalize(&normal1, &normal1);

	float dotVN1 = D3DXVec3Dot(&-normal1, &triangle1[0]);
	float dV2[3];

	for(int i = 0; i < 3; i++)
	{
		dV2[i] = D3DXVec3Dot(&normal1, &triangle2[i]) + dotVN1;
	}

	if(dV2[0] < 0 && dV2[1] < 0 && dV2[2] < 0)
	{
		return false;
	}
	else if(dV2[0] > 0 && dV2[1] > 0 && dV2[2] > 0)
	{
		return false;
	}

	//Planes intersected, find line for intersection
	D3DXVECTOR3 lineDirection;
	D3DXVec3Cross(&lineDirection, &normal1, &normal2);

	D3DXVec3Normalize(&lineDirection, &lineDirection);

	//Triangle 1 interval
	float lineInterval1[3];

	for(int i = 0; i < 3; i++)
	{
		lineInterval1[i] = D3DXVec3Dot(&lineDirection, &triangle1[i]);
	}

	float T1t1;
	float T1t2;

	if(dV1[0] <= 0 && dV1[2] <= 0 || dV1[0] > 0 && dV1[2] > 0)
	{
		T1t1 = lineInterval1[0] + (lineInterval1[1] - lineInterval1[0])*(dV1[0]/(dV1[0] - dV1[1]));
		T1t2 = lineInterval1[2] + (lineInterval1[1] - lineInterval1[2])*(dV1[2]/(dV1[2] - dV1[1]));
	}
	else if(dV1[1] <= 0 && dV1[2] <= 0 || dV1[1] > 0 && dV1[2] > 0)
	{
		T1t1 = lineInterval1[1] + (lineInterval1[0] - lineInterval1[1])*(dV1[1]/(dV1[1] - dV1[0]));
		T1t2 = lineInterval1[2] + (lineInterval1[0] - lineInterval1[2])*(dV1[2]/(dV1[2] - dV1[0]));
	}
	else if(dV1[0] <= 0 && dV1[1] <= 0 || dV1[0] > 0 && dV1[1] > 0)
	{
		T1t1 = lineInterval1[0] + (lineInterval1[2] - lineInterval1[0])*(dV1[0]/(dV1[0] - dV1[2]));
		T1t2 = lineInterval1[1] + (lineInterval1[2] - lineInterval1[1])*(dV1[1]/(dV1[1] - dV1[2]));
	}

	//Triangle 2 interval
	float lineInterval2[3];

	for(int i = 0; i < 3; i++)
	{
		lineInterval2[i] = D3DXVec3Dot(&lineDirection, &triangle2[i]);
	}

	float T2t1;
	float T2t2;

	if(dV2[0] < 0 && dV2[2] < 0 || dV2[0] > 0 && dV2[2] > 0)
	{
		T2t1 = lineInterval2[0] + (lineInterval2[1] - lineInterval2[0])*(dV2[0]/(dV2[0] - dV2[1]));
		T2t2 = lineInterval2[2] + (lineInterval2[1] - lineInterval2[2])*(dV2[2]/(dV2[2] - dV2[1]));
	}
	else if(dV2[1] < 0 && dV2[2] < 0 || dV2[1] > 0 && dV2[2] > 0)
	{
		T2t1 = lineInterval2[1] + (lineInterval2[0] - lineInterval2[1])*(dV2[1]/(dV2[1] - dV2[0]));
		T2t2 = lineInterval2[2] + (lineInterval2[0] - lineInterval2[2])*(dV2[2]/(dV2[2] - dV2[0]));
	}
	else if(dV2[0] < 0 && dV2[1] < 0 || dV2[0] > 0 && dV2[1] > 0)
	{
		T2t1 = lineInterval2[0] + (lineInterval2[2] - lineInterval2[0])*(dV2[0]/(dV2[0] - dV2[2]));
		T2t2 = lineInterval2[1] + (lineInterval2[2] - lineInterval2[1])*(dV2[1]/(dV2[1] - dV2[2]));
	}

	if(T1t1 <= T2t1 && T2t1 <= T1t2 || T1t1 <= T2t2 && T2t2 <= T1t2)
	{
		return true;
	}
	if(T1t1 >= T2t1 && T2t1 >= T1t2 || T1t1 >= T2t2 && T2t2 >= T1t2)
	{
		return true;
	}

	return false;
}
bool SphereVSSphere(BoundingSphere sphere1, BoundingSphere sphere2)
{


	return false;
}