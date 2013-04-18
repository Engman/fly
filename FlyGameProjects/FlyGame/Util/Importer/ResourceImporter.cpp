#include "ResourceImporter.h"

#pragma region IMPORTERS

	static ObjectImporter objectImporter = ObjectImporter();

#pragma endregion

bool ResourceImporter::ImportObject(wchar_t*  filename, __out SmartPtrStd<ImportedObjectData>& objData)
{
	return objectImporter.Import(filename, objData);
}
bool ResourceImporter::ImportObject(std::vector<wchar_t*>  filenames, __out std::vector<SmartPtrStd<ImportedObjectData>>& objData)
{
	if(!filenames.size())
		return false;

	if(filenames.size() != objData.size())
		objData.resize(filenames.size());

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], objData[i]))
			return false;
	}

	return true;
}

bool ResourceImporter::ImportMaterial(wchar_t* filename, __out std::vector<int>& id)
{
	return objectImporter.Import(filename, id);
}
bool ResourceImporter::ImportMaterial(std::vector<wchar_t*> filenames, __out std::vector<int>& id)
{
	if(!filenames.size())
		return false;

	std::vector<int> temp;

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(filenames[i], temp))
			return false;

		id.insert(id.end(), temp.begin(), temp.end());
	}

	return true;
}

