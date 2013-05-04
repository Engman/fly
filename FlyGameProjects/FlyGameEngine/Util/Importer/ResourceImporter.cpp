#include "ResourceImporter.h"
#include "ObjectImporter.h"
#include "..\..\Core\Mesh\FlyMesh.h"

#pragma region IMPORTERS

	static ObjectImporter objectImporter = ObjectImporter();

#pragma endregion

bool ResourceImporter::ImportObject(const wchar_t* filename, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<SmartPtrStd<FlyMesh>>& object)
{
	return objectImporter.Import(filename, device, _dc, object);
}
bool ResourceImporter::ImportObject(std::vector<const wchar_t*>& filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<SmartPtrStd<FlyMesh>>& objData)
{
	if(!filenames.size())
		return false;

	if(filenames.size() != objData.size())
		objData.resize(filenames.size());

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], device, _dc, objData))
			return false;
	}

	return true;
}

bool ResourceImporter::ImportMaterial(const wchar_t* filename, ID3D11Device* device, ID3D11DeviceContext* _dc, std::vector<int>& id)
{
	return objectImporter.Import(filename, device, _dc, id);
}
bool ResourceImporter::ImportMaterial(std::vector<const wchar_t*>& filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, std::vector<int>& id)
{
	if(!filenames.size())
		return false;

	std::vector<int> temp;

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], device, _dc, temp))
			return false;

		id.insert(id.end(), temp.begin(), temp.end());
	}

	return true;
}

