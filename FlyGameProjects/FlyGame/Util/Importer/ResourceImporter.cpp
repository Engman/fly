#include "ResourceImporter.h"

#pragma region IMPORTERS

	static ObjectImporter objectImporter = ObjectImporter();

#pragma endregion

bool ResourceImporter::ImportObject(std::wstring filename, ID3D11Device* device, __out SmartPtrStd<ImportedObjectData>& objData)
{
	return objectImporter.Import(filename, device, objData);
}
bool ResourceImporter::ImportObject(std::vector<std::wstring>  filenames, ID3D11Device* device, __out std::vector<SmartPtrStd<ImportedObjectData>>& objData)
{
	if(!filenames.size())
		return false;

	if(filenames.size() != objData.size())
		objData.resize(filenames.size());

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], device, objData[i]))
			return false;
	}

	return true;
}

bool ResourceImporter::ImportMaterial(std::wstring filename, ID3D11Device* device, __out std::vector<int>& id)
{
	return objectImporter.Import(filename, device, id);
}
bool ResourceImporter::ImportMaterial(std::vector<std::wstring> filenames, ID3D11Device* device, __out std::vector<int>& id)
{
	if(!filenames.size())
		return false;

	std::vector<int> temp;

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], device, temp))
			return false;

		id.insert(id.end(), temp.begin(), temp.end());
	}

	return true;
}

