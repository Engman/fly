#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Util\Importer\ObjectImporter.h"
#include "..\Util\Importer\ResourceImporter.h"
#include "..\Core\Mesh\Terrain.h"


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
		FlyMesh::OBJECT_DESC d;
		d.device = D3DShell::self()->getDevice();
		d.deviceContext = D3DShell::self()->getDeviceContext();
		d.material_id = raw.objects[i].material;
		d.name = raw.name;
		d.shader = 0;
		d.vCount = (int)raw.objects[i].vertex->size();
		d.vertecies = raw.objects[i].vertex;

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
			FlyMesh::OBJECT_DESC d;
			d.device = D3DShell::self()->getDevice();
			d.deviceContext = D3DShell::self()->getDeviceContext();
			d.material_id = raw[i].objects[k].material;
			d.name = raw[i].name;
			d.shader = 0;
			d.vCount = (int)raw[i].objects[k].vertex->size();
			d.vertecies = raw[i].objects[k].vertex;

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
	ImportedObjectData raw;
	if(!ResourceImporter::ImportObject(path, &raw))
		return false;

	for (int i = 0; i < (int)raw.objects.size(); i++)
	{
		FlyMesh::OBJECT_DESC d;
		d.device = D3DShell::self()->getDevice();
		d.deviceContext = D3DShell::self()->getDeviceContext();
		d.material_id = raw.objects[i].material;
		d.name = raw.name;
		d.shader = 0;
		d.vCount = (int)raw.objects[i].vertex->size();
		d.vertecies = raw.objects[i].vertex;

		FlyMesh *obj = new FlyMesh();
		if(!obj->Initialize(d))
			return false;
		objects->push_back(obj);
	}
	return true;
}





