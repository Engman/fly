#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Core\Mesh\FlyMesh.h"
#include "..\Util\Importer\ResourceImporter.h"


bool FLYCALL FlyEngine_Core::Geometry_Create(FlyEngineGeometry type, SmartPtrStd<FlyMesh> object)
{
	switch (type)
	{
		case Sphere:
		break;

		case Cube:
		break;

		case Plane:
		break;

		default:
			return false;
	}

	return true;
}

bool FLYCALL FlyEngine_Core::Geometry_Load(const wchar_t* resourcesToLoad, vector<SmartPtrStd<FlyMesh>>& objects)
{
	return ResourceImporter::ImportObject(resourcesToLoad , D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), objects);
}

bool FLYCALL FlyEngine_Core::Geometry_Load(vector<const wchar_t*> resourcesToLoad, vector<SmartPtrStd<FlyMesh>>& objects)
{
	return ResourceImporter::ImportObject(resourcesToLoad , D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), objects);
}



