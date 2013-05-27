#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Core\WindowShell.h"
#include "..\Core\Mesh\Terrain.h"
#include "..\Core\Mesh\FlyMeshAnimated.h"
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

bool FLYCALL FlyEngine_Core::Geometry_Load(const wchar_t* path, vector<Entity*>* objects, FlyEngineGeometry special, int renderBoxes, int collisionBoxes)
{
	ImportedObjectData raw;
	if(!ResourceImporter::ImportObject(path, &raw))
		return false;

	if(special == FlyGeometry_Terrain)
	{
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
			d.shader = 0;
			d.vCount = (int)raw.objects[i].vertex->size();
			d.vertecies = raw.objects[i].vertex;
			d.boundingSphere = bs;

			Terrain *obj = new Terrain();
			if(!obj->Initialize(d, renderBoxes, collisionBoxes))
				return false;
			objects->push_back(obj);
		}
	}
	else if(special == FlyGeometry_AnimatedMesh)
	{

		ImportedObjectData raw;
		if(!ResourceImporter::ImportObject(path, &raw))
			return false;

		SmartPtrStd<std::vector<BoundingSphere>> boundingSpheres; 
		boundingSpheres = new vector<BoundingSphere>; 
		SmartPtrStd<std::vector<std::vector<FrameData>>> frames; 
		frames  = new vector<vector<FrameData>>; 
		SmartPtrStd<std::vector<std::vector<VERTEX::VertexPNT>>> vert; 
		vert = new vector<vector<VERTEX::VertexPNT>>;

		for (int i = 0; i < (int)raw.objects.size(); i++)
		{
			BoundingBox bb;
			bb.minPoint = vec3((*raw.objects[i].vertex)[0].position.x, (*raw.objects[i].vertex)[0].position.y, (*raw.objects[i].vertex)[0].position.z);
			bb.maxPoint = bb.minPoint;
			for (int k = 0; k < (int)raw.objects[i].vertex->size(); k++)
			{
				GetMinMax(bb, (*raw.objects[i].vertex)[k].position);
			}
			BoundingSphere bs;
			bs.radius = D3DXVec3Length(&(bb.maxPoint - bb.minPoint)) * 0.5f;
			bs.center = (bb.maxPoint + bb.minPoint) * 0.5f;
			(*boundingSpheres).push_back(bs);
			
		
			std::vector<VERTEX::VertexPNT> tempVector;
			tempVector.resize(raw.objects[i].vertex->size());

			for(int k =0;k<(int)raw.objects[i].vertex->size(); k++)
			{
				tempVector.at(k) = raw.objects[i].vertex->at(k);
			}
			(*vert).push_back(tempVector);
			
		}
		(*frames).resize(raw.animations.size());
		for(int i = 0; i<(int)raw.animations.size(); i++)
		{
			for(int k = 0; k<(int)raw.animations.at(i).frames.size(); k++)
			{
				(*frames).at(i).push_back(raw.animations.at(i).frames.at(k));
			}
		}

		
		FlyMeshAnimated::ANIM_OBJECT_DESC d;
		d.device = D3DShell::self()->getDevice();
		d.deviceContext = D3DShell::self()->getDeviceContext();
		d.material_id = raw.objects[0].material;
		d.name = raw.name;
		d.filename = raw.name;
		d.shader = 0;
		d.vCount = (int)raw.objects[0].vertex->size();
		d.vertecies = vert;
		d.boundingSphere = boundingSpheres;
		d.frames =  frames;

		FlyMeshAnimated *obj = new FlyMeshAnimated();
		if(!obj->Initialize(d))
			return false;
		objects->push_back(obj);

		(*boundingSpheres).clear();
		(*frames).clear(); 
		(*vert).clear(); 
	}

	return true;
}

Entity* FLYCALL FlyEngine_Core::Geometry_Pick(const vector<Entity*>& obj, bool orto)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(WindowShell::self()->getHWND(), &p);

	float farZ = this->activeCamera->GetFarZ();
	Entity* retVal = 0;
	float pointX, pointY;
	D3DXMATRIX inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)p.x) / (float)D3DShell::self()->getWidth()) - 1.0f;
	pointY = (((2.0f * (float)p.x) / (float)D3DShell::self()->getHeight()) - 1.0f) * -1.0f;
		
	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	
	if(orto)
	{
		pointX = pointX / this->activeCamera->GetOrthogonalMatrix()._11;
		pointY = pointY / this->activeCamera->GetOrthogonalMatrix()._22;
	}
	else
	{
		pointX = pointX / this->activeCamera->GetProjectionMatrix()._11;
		pointY = pointY / this->activeCamera->GetProjectionMatrix()._22;	
	}


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
			FlyMesh* temp = (FlyMesh*)obj[i];
			vector<vec3> *tris = temp->GetTriangles();
			if(tris)
			{
				for (int i = 0; i < (int)tris->size(); i+=3)
				{
					vec3 tri[3] = 
					{
						(*tris)[i],
						(*tris)[i + 1],
						(*tris)[i + 2]
					};
					if(RayVSTriangle(rayOrigin, rayDirection,tri))
					{
						retVal = obj[i];
					}
				}
			}
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

