#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Core\WindowShell.h"
#include "..\Core\Mesh\Terrain.h"
#include "..\Util\Importer\ObjectImporter.h"
#include "..\Util\Importer\ResourceImporter.h"
#include "..\Util\CollisionLib.h"
#include "..\Util\Camera.h"

void GetMinMax(BoundingBox& bb, const vec4& vertex);


bool FLYCALL FlyEngine_Core::Geometry_Create(FlyEngineGeometry type, Entity* object)
{
	switch (type)
	{
		case FlyGeometry_Sphere:
		break;

		case FlyGeometry_Cube:
		break;

		case FlyGeometry_Plane:
		break;
	}

	return true;
}

bool FLYCALL FlyEngine_Core::Geometry_Load(const wchar_t* resourcesToLoad, vector<Entity*>* objects)
{
	ImportedObjectData raw;
	if(!ResourceImporter::ImportObject(resourcesToLoad, &raw))
		return false;
	
	for (int i = 0; i < (int)raw.objects.size(); i++)
	{
		BoundingBox bb;
		bb.minPoint = vec3((*raw.objects[i].vertex)[0].position.x, (*raw.objects[i].vertex)[0].position.y, (*raw.objects[i].vertex)[0].position.z);
		bb.maxPoint = bb.minPoint;
		for (int k = 0; k < (int)raw.objects[i].vertex->size(); k++)
		{
			GetMinMax(bb, (*raw.objects[i].vertex)[k].position);
		}
		BoundingSphere* bs = new BoundingSphere();
		bs->radius = D3DXVec3Length(&(bb.maxPoint - bb.minPoint)) * 0.5f;
		bs->center = (bb.maxPoint + bb.minPoint) * 0.5f;


		FlyMesh::OBJECT_DESC d;
		d.device = D3DShell::self()->getDevice();
		d.deviceContext = D3DShell::self()->getDeviceContext();
		d.material_id = raw.objects[i].material;
		d.name = raw.name;
		d.filename = raw.name;
		d.shader = 0;
		d.vCount = (int)raw.objects[i].vertex->size();
		d.vertecies = raw.objects[i].vertex;
		d.boundingSphere = bs;

		FlyMesh *obj = new FlyMesh();
		if(!obj->Initialize(d))
			return false;
		objects->push_back(obj);
	}
	return true;
}

bool FLYCALL FlyEngine_Core::Geometry_Load(vector<const wchar_t*> resourcesToLoad, vector<Entity*>* objects)
{
	std::vector<ImportedObjectData> raw;
	if(!ResourceImporter::ImportObject(resourcesToLoad , &raw))
		return false;
	
	for (int i = 0; i < (int)raw.size(); i++)
	{
		for (int k = 0; k < (int)raw[i].objects.size(); k++)
		{
			BoundingBox bb;
			bb.minPoint = vec3((*raw[i].objects[k].vertex)[0].position.x, (*raw[i].objects[k].vertex)[0].position.y, (*raw[i].objects[k].vertex)[0].position.z);
			bb.maxPoint = bb.minPoint;
			for (int j = 0; j < (int)raw[i].objects[k].vertex->size(); j++)
			{
				GetMinMax(bb, (*raw[i].objects[k].vertex)[j].position);
			}
			BoundingSphere* bs = new BoundingSphere();
			bs->radius = D3DXVec3Length(&(bb.maxPoint - bb.minPoint)) * 0.5f;
			bs->center = (bb.maxPoint + bb.minPoint) * 0.5f;

			FlyMesh::OBJECT_DESC d;
			d.device = D3DShell::self()->getDevice();
			d.deviceContext = D3DShell::self()->getDeviceContext();
			d.material_id = raw[i].objects[k].material;
			d.name = raw[i].name;
			d.filename = raw[i].name;
			d.shader = 0;
			d.vCount = (int)raw[i].objects[k].vertex->size();
			d.vertecies = raw[i].objects[k].vertex;
			d.boundingSphere = bs;

			FlyMesh *obj = new FlyMesh();
			if(!obj->Initialize(d))
				return false;
			objects->push_back(obj);
		}
	}

	return true;;
}

bool FLYCALL FlyEngine_Core::Geometry_Load(const wchar_t* path, vector<Entity*>* objects, FlyEngineGeometry special)
{
	if(special == FlyGeometry_Terrain)
	{
		ImportedObjectData raw;
		if(!ResourceImporter::ImportObject(path, &raw))
			return false;


		for (int i = 0; i < (int)raw.objects.size(); i++)
		{
			BoundingBox bb;
			bb.minPoint = vec3((*raw.objects[i].vertex)[0].position.x, (*raw.objects[i].vertex)[0].position.y, (*raw.objects[i].vertex)[0].position.z);
			bb.maxPoint = bb.minPoint;
			for (int k = 0; k < (int)raw.objects[i].vertex->size(); k++)
			{
				GetMinMax(bb, (*raw.objects[i].vertex)[k].position);
			}
			BoundingSphere* bs = new BoundingSphere();
			bs->radius = D3DXVec3Length(&(bb.maxPoint - bb.minPoint)) * 0.5f;
			bs->center = (bb.maxPoint + bb.minPoint) * 0.5f;


			FlyMesh::OBJECT_DESC d;
			d.device = D3DShell::self()->getDevice();
			d.deviceContext = D3DShell::self()->getDeviceContext();
			d.material_id = raw.objects[i].material;
			d.name = raw.name;
			d.filename = raw.name;
			d.shader = 0;
			d.vCount = (int)raw.objects[i].vertex->size();
			d.vertecies = raw.objects[i].vertex;
			d.boundingSphere = bs;

			Terrain *obj = new Terrain();
			if(!obj->Initialize(d))
				return false;
			objects->push_back(obj);
		}
	}
	return true;
}

/*
Entity* FLYCALL FlyEngine_Core::Geometry_Pick(const vector<Entity*>& obj, int posX, int posY)
{
	return TestIntersection(obj);

	D3DXMATRIX InvWorld;
	D3DXMATRIX InvView;
	D3DXVECTOR3 origin = this->activeCamera->GetPosition();
	D3DXVECTOR3 direction;
	D3DXVECTOR3 rayOrigin;
	D3DXVECTOR3 rayDirection;
	float length = this->activeCamera->GetFarZ();
	Entity* picked = NULL;
	

	// Move the mouse coordinates into the -1 to +1 range.
	float pointX =  (((2.0f * (float)posX) / D3DShell::self()->getWidth())  - 1.0f) / this->activeCamera->GetProjectionMatrix()._11;
	float pointY = -(((2.0f * (float)posY) / D3DShell::self()->getHeight()) - 1.0f) / this->activeCamera->GetProjectionMatrix()._22;

	D3DXMatrixInverse(&InvView, 0, &this->activeCamera->GetViewMatrix());

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * InvView._11) + (pointY * InvView._21) + InvView._31;
	direction.y = (pointX * InvView._12) + (pointY * InvView._22) + InvView._32;
	direction.z = (pointX * InvView._13) + (pointY * InvView._23) + InvView._33;

	for (int i = 0; i < (int)obj.size(); i++)
	{
		//// Get the inverse of the translated world matrix.
		//D3DXMATRIX InvWorldView = obj[i]->getWorld() * this->activeCamera->GetViewMatrix();
		//D3DXMatrixInverse(&InvWorldView, NULL, &InvWorldView);
		D3DXMatrixInverse(&InvWorld, NULL, &obj[i]->getWorld());
		
		//// Transform the ray origin and the ray direction from view space to world space.
		D3DXVec3TransformCoord(&rayOrigin, &origin, &InvWorld);
		//rayOrigin = origin;
		D3DXVec3TransformNormal(&rayDirection, &direction, &InvWorld);


		// Normalize the ray direction.
		D3DXVec3Normalize(&rayDirection, &rayDirection);
		
		float len = RayVSSphereLength(rayDirection, rayOrigin, obj[i]->getBoundingSphere()->center, obj[i]->getBoundingSphere()->radius);
		
		//wchar_t b1[10];
		//wchar_t b2[10];
		//wchar_t b3[10];
		//wchar_t b4[10];
		//wchar_t b5[10];
		//wchar_t b6[10];
		//_itow(rayDirection.x * 100, b1, 10);
		//_itow(rayDirection.y * 100, b2, 10);
		//_itow(rayDirection.z * 100, b3, 10);
		//_itow(rayOrigin.x	 * 100, b4, 10);
		//_itow(rayOrigin.y	 * 100, b5, 10);
		//_itow(rayOrigin.z	 * 100, b6, 10);
		//wstring text3 = L"RayDirection(";
		//text3.append(b1);
		//text3.append(L", ");
		//text3.append(b2);
		//text3.append(L", ");
		//text3.append(b3);
		//text3.append(L")\n");
		//text3.append(L"RayOrigin(");
		//text3.append(b4);
		//text3.append(L", ");
		//text3.append(b5);
		//text3.append(L", ");
		//text3.append(b6);
		//text3.append(L")\n");
		//MessageBox(WindowShell::self()->getHWND(), text3.c_str(), L"", 0);
		//
		//wchar_t buffX[10];
		//wchar_t buffY[10];
		//wchar_t buffZ[10];
		//wchar_t buffI1[10];
		//wchar_t buffI2[10];
		//_itow(obj[i]->getBoundingSphere()->center.x * 100, buffX, 10);
		//_itow(obj[i]->getBoundingSphere()->center.y * 100, buffY, 10);
		//_itow(obj[i]->getBoundingSphere()->center.z * 100, buffZ, 10);
		//_itow(obj[i]->getBoundingSphere()->radius * 100, buffI1, 10);
		//_itow(len * 100, buffI2, 10);
		//wstring text2 = L"SphereCenter (";
		//text2.append(buffX);
		//text2.append(L", ");
		//text2.append(buffY);
		//text2.append(L", ");
		//text2.append(buffZ);
		//text2.append(L")\n");
		//text2.append(L"SphereRadius: ");
		//text2.append(buffI1);
		//text2.append(L"\nLength: ");
		//text2.append(buffI2);
		//MessageBox(WindowShell::self()->getHWND(), text2.c_str(), L"", 0);

		// Now perform the ray-sphere intersection test.
		if(len >= 0 && len <= length)
		{
			D3DXMatrixInverse(&InvWorld, NULL, &InvWorld);
			//InvWorld = obj[i]->getWorld();

			vector<vec3>* vert = dynamic_cast<FlyMesh*>(obj[i])->GetTriangles();
			for (int k = 0; k < (int)vert->size(); k += 3)
			{
				D3DXVECTOR4 rd4;
				D3DXVECTOR4 ro4;
				D3DXVECTOR3 rd3;
				D3DXVECTOR3 ro3;

				D3DXVec3Transform(&rd4, &rayDirection, &InvWorld);
				D3DXVec3Transform(&ro4, &rayOrigin, &InvWorld);
				ro3.x = ro4.x; ro3.y = ro4.y; ro3.z = ro4.z;
				rd3.x = rd4.x; rd3.y = rd4.y; ro3.z = rd4.z;

				vec3 tri[3] = 
				{
					(*vert)[k],
					(*vert)[k + 1],
					(*vert)[k + 2]
				};
				
				//Check against triangles
				if(RayVSTriangle(ro3, rd3, tri))
				{
					length = len;
					picked = obj[i];
				}
			}
		}
	}

	return picked;
}
*/

Entity* FLYCALL FlyEngine_Core::Geometry_Pick(const vector<Entity*>& obj)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(WindowShell::self()->getHWND(), &p);

	float farZ = this->activeCamera->GetFarZ();
	Entity* retVal;
	float pointX, pointY;
	D3DXMATRIX inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)p.x) / (float)D3DShell::self()->getWidth()) - 1.0f;
	pointY = (((2.0f * (float)p.x) / (float)D3DShell::self()->getHeight()) - 1.0f) * -1.0f;
		
	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	
	pointX = pointX / this->activeCamera->GetProjectionMatrix()._11;
	pointY = pointY / this->activeCamera->GetProjectionMatrix()._22;

	// Get the inverse of the view matrix.
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &this->activeCamera->GetViewMatrix());

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = this->activeCamera->GetPosition();


	for(int i = 0; i < (int)obj.size(); i++)
	{
		// Get the world matrix and translate to the location of the sphere.
	
		D3DXMatrixTranslation(&translateMatrix, -5.0f, 1.0f, 5.0f);
		D3DXMatrixMultiply(&worldMatrix, &obj[i]->getWorld(), &translateMatrix); 

		// Now get the inverse of the translated world matrix.
		D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

		// Now transform the ray origin and the ray direction from view space to world space.
		D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

		// Normalize the ray direction.
		D3DXVec3Normalize(&rayDirection, &rayDirection);

		// Now perform the ray-sphere intersection test.
		float length = RayVSSphereLength(rayOrigin, rayDirection, obj[i]->getBoundingSphere()->center, obj[i]->getBoundingSphere()->radius);
	
		if(length >= 0 && length < farZ)
		{
			retVal = obj[i];
		}
	}

	return retVal;
}


void GetMinMax(BoundingBox& bb, const vec4& vertex)
{
	if(vertex.x < bb.minPoint.x)		bb.minPoint.x = vertex.x;
	if(vertex.y < bb.minPoint.y)		bb.minPoint.y = vertex.y;
	if(vertex.z < bb.minPoint.z)		bb.minPoint.z = vertex.z;

	if(vertex.x > bb.maxPoint.x)		bb.maxPoint.x = vertex.x;
	if(vertex.y > bb.maxPoint.y)		bb.maxPoint.y = vertex.y;
	if(vertex.z > bb.maxPoint.z)		bb.maxPoint.z = vertex.z;
 }
