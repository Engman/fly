#include "CollisionLib.h"

// Function bodies
bool FrustumVSBox(ViewFrustum& frustum, BoundingBox& box)
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
bool FrustumVSSphere(ViewFrustum& frustum, BoundingSphere& sphere)
{
	if(SphereVSSphere(frustum.sphere, sphere))
	{
		for(int i=0; i<6; i++) 
		{
			if(D3DXPlaneDotCoord(&frustum.planes[i], &sphere.center) < -sphere.radius)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool RayVSSphere(D3DXVECTOR3& rayDirection, D3DXVECTOR3& rayOrigin, D3DXVECTOR3& center, float& radius)
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
float RayVSSphereLength(D3DXVECTOR3& rayDirection, D3DXVECTOR3& rayOrigin, D3DXVECTOR3& center, float& radius)
{
	D3DXVECTOR3 l = center - rayOrigin;
	float l2 = D3DXVec3Dot(&l, &l);
	float h = D3DXVec3Dot(&l, &rayDirection);
	float r2 = radius*radius;
	if(h >= 0.0f)
	{
		float m2 = l2 - h*h;
		if(m2 <= r2)
			return h - sqrtf(fabsf(m2));
	}

	return -1.0f;
}
bool RayVSTriangle(D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection, D3DXVECTOR3 triangle[3])
{
	D3DXVECTOR3 e1 = triangle[1] - triangle[0];
	D3DXVECTOR3 e2 = triangle[2] - triangle[0];
	D3DXVECTOR3 q;	D3DXVec3Cross(&q, &rayDirection, &e2);
	float epsilon = 0.000000000000001f;
	float a = D3DXVec3Dot(&e1, &q); // Determinant

	if(a > -epsilon && a < epsilon)
	{
		return false;
	}

	float f = 1/a; // Value to be multiplied into the matrix

	D3DXVECTOR3 s = rayOrigin - triangle[0];

	float u = f * D3DXVec3Dot(&s, &q);

	if(u < 0.0f)
	{
		return false;
	}

	D3DXVECTOR3 h;
	D3DXVec3Cross(&h, &s, &e1);
	
	float v = f * D3DXVec3Dot(&h, &rayDirection);

	if(v < 0.0f || (u + v) > 1.0f )
	{
		return false;
	}

	return true;
}

bool BoxVSBox(BoundingBox& box1, BoundingBox& box2)
{
	if(box1.maxPoint.x < box2.minPoint.x || box2.maxPoint.x < box1.minPoint.x) return false;

	if(box1.maxPoint.y < box2.minPoint.y || box2.maxPoint.y < box1.minPoint.y) return false;

	if(box1.maxPoint.z < box2.minPoint.z || box2.maxPoint.z < box1.minPoint.z) return false;

	return true;
}
bool BoxVSSphere(BoundingBox& box, BoundingSphere& sphere)
{
    float s, d = 0.0f; 

    //find the square of the distance
    //from the sphere to the box
    for( long i=0 ; i<3 ; i++ )
    {
		if( sphere.center[i] < box.minPoint[i] )
        { 
			s = sphere.center[i] - box.minPoint[i];
            d += s*s; 

        }
        else if( sphere.center[i] > box.maxPoint[i] )
        {
            s = sphere.center[i] - box.maxPoint[i];
            d += s*s; 
        }
    }
    return d <= sphere.radius*sphere.radius;

}
bool BoxVSPoint(BoundingBox& box, D3DXVECTOR3& point)
{
	return true;
}
bool BoxVSTriangle(BoundingBox& box, D3DXVECTOR3 triangle[3])
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
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e0[2]*v0[0] + e0[0]*v0[2];	
	p2 = -e0[2]*v2[0] + e0[0]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p1 = e0[1]*v1[0] - e0[0]*v1[1]; 
	p2 = e0[1]*v2[0] - e0[0]*v2[1]; 
	if(p2<p1) {min=p2; max=p1;} 
	else {min=p1; max=p2;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;

	fex = fabsf(e1[0]);
	fey = fabsf(e1[1]);
	fez = fabsf(e1[2]);
	//X
	p0 = e1[2]*v0[1] - e1[1]*v0[2]; 
	p2 = e1[2]*v2[1] - e1[1]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fex*boxHalfSize[1] + fey*boxHalfSize[2]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e1[2]*v0[0] + e1[0]*v0[2];	
	p2 = -e1[2]*v2[0] + e1[0]*v2[2]; 
	if(p0<p2) {min=p0; max=p2;} 
	else {min=p2; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p0 = e1[1]*v0[0] - e1[0]*v0[1]; 
	p1 = e1[1]*v1[0] - e1[0]*v1[1]; 
	if(p0<p1) {min=p0; max=p1;} 
	else {min=p1; max=p0;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;

	fex = fabsf(e2[0]);
	fey = fabsf(e2[1]);
	fez = fabsf(e2[2]);
	//X
	p0 = e2[2]*v0[1] - e2[1]*v0[2]; 
	p1 = e2[2]*v1[1] - e2[1]*v1[2]; 
	if(p0<p1) {min=p0; max=p1;} 
	else {min=p1; max=p0;} 
	rad = fez * boxHalfSize[1] + fey * boxHalfSize[2]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Y
	p0 = -e2[2]*v0[0] + e2[0]*v0[2]; 
	p1 = -e2[2]*v1[0] + e2[0]*v1[2];	
	if(p0<p1) {min=p0; max=p1;}
	else {min=p1; max=p0;} 
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2];
	//if((min>rad || max<-rad) && rad != 0.0f) return false;
	//Z
	p1 = e2[1]*v1[0] - e2[0]*v1[1]; 
	p2 = e2[1]*v2[0] - e2[0]*v2[1]; 
	if(p2<p1) {min=p2; max=p1;} 
	else {min=p1; max=p2;} 
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1]; 
	//if((min>rad || max<-rad) && rad != 0.0f) return false;

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

bool SphereVSSphere(BoundingSphere& sphere, BoundingSphere& sphere2)
{
	if(D3DXVec3Length(&(sphere.center - sphere2.center)) <= sphere.radius+sphere2.radius)
	{
		return true;
	}

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

D3DXVECTOR3  closestPointOnTriangle(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 p)
{
	D3DXVECTOR3 Rab = closestPointOnLine(a, b, p);
	D3DXVECTOR3 Rbc = closestPointOnLine(b, c, p);
	D3DXVECTOR3 Rca = closestPointOnLine(c, a, p);
	D3DXVECTOR3 closest;

	float abL = D3DXVec3Length(&(p - Rab));
	float bcL = D3DXVec3Length(&(p - Rbc));
	float caL = D3DXVec3Length(&(p - Rca));
	
	float min = abL;
	closest = Rab;

	if(bcL < min)
	{
		min = bcL;
		closest = Rbc;
	}

	if(caL < min)
	{
		closest = Rca;
	}

	return closest;
}

D3DXVECTOR3 closestPointOnLine(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 p)
{
	// Determine t (the length of the vector from ‘a’ to ‘p’)

	D3DXVECTOR3 c = p - a;
	D3DXVECTOR3 V = b - a;
	float d = D3DXVec3Length(&V);
	D3DXVec3Normalize(&V, &V);
	float t = D3DXVec3Dot(&V, &c);

	// Check to see if ‘t’ is beyond the extents of the line segment

	if (t < 0.0f) return a;
	if (t > d) return b;
 
	// Return the point between ‘a’ and ‘b’

	V *= t;
	return a + V;
}


float intersect(D3DXVECTOR3 pOrigin, D3DXVECTOR3 pNormal, D3DXVECTOR3 rOrigin, D3DXVECTOR3 rVector)
{
	float d = -D3DXVec3Dot(&pNormal, &pOrigin);	
	float numer = D3DXVec3Dot(&pNormal, &rOrigin) + d;
	float denom = D3DXVec3Dot(&pNormal, &rVector);

	if(denom == 0.0f)
	{
		return -1.0f;
	}

	return -(numer / denom);
}

float intersectSphere(D3DXVECTOR3 rO, D3DXVECTOR3 rV, D3DXVECTOR3 sO, float sR)
{
   D3DXVECTOR3 Q = sO - rO;
   float c = D3DXVec3Length(&Q);
   float v = D3DXVec3Dot(&Q, &rV);
   float d = sR*sR -  (c*c - v*v);

   // If there was no intersection, return -1

   if (d < 0.0) return -1.0f;

   // Return the distance to the [first] intersecting point

   return v - sqrt(d);
}

bool CheckPointInTriangle(D3DXVECTOR3 point, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c) 
{
	float total_angles = 0.0f;
       
	// make the 3 vectors
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;
	D3DXVECTOR3 v3;

	D3DXVECTOR3 v1n = point-a;
	D3DXVECTOR3 v2n = point-b;
	D3DXVECTOR3 v3n = point-c;
  
	D3DXVec3Normalize(&v1, &v1n);
	D3DXVec3Normalize(&v2, &v2n);
	D3DXVec3Normalize(&v3, &v3n);

	total_angles += acos(D3DXVec3Dot(&v1,&v2));   
	total_angles += acos(D3DXVec3Dot(&v2,&v3));
	total_angles += acos(D3DXVec3Dot(&v3,&v1)); 
    
	// allow a small margin because of the limited precision of
	// floating point math.
	float temp = 2*D3DX_PI;
	temp = total_angles - temp;
	if (fabs(total_angles-2*D3DX_PI) <= 0.005)
		return true;
     
	return false;
}