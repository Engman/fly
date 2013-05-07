#include "ResourceImporter.h"
#include "ObjectImporter.h"
#include "..\..\Core\Entity.h"

#pragma region IMPORTERS

	static ObjectImporter objectImporter = ObjectImporter();

#pragma endregion

wstring getDir(const wchar_t* path)
{
	std::wstring file;
	
	//System root
		 if(path[0] == L'C' && path[1] == L':' && path[2] == L'/')	{ file = path; }
	else if(path[0] == L'C' && path[1] == L':' && path[2] == L'\\')	{ file = path; }
	else if(path[0] == L'c' && path[1] == L':' && path[2] == L'/')	{ file = path; }
	else if(path[0] == L'c' && path[1] == L':' && path[2] == L'\\')	{ file = path; }
	else if(path[0] == L'.' && path[1] == L'.' && path[2] == L'\\')	{ file = path; }
	else if(path[0] == L'.' && path[1] == L'.' && path[2] == L'/')	{ file = path; }
	else
	{
		file = L"..\\Resources\\Models\\";
		file.append(path);
	}
	
	return file;
}

bool ResourceImporter::ImportObject(const wchar_t* filename, ImportedObjectData* object)
{
	return objectImporter.Import(getDir(filename), object);
}
bool ResourceImporter::ImportObject(std::vector<const wchar_t*>& filenames, vector<ImportedObjectData>* objData)
{
	if(!filenames.size())
		return false;
	
	objData->clear();
	if(objData->size() !=  filenames.size())
		objData->resize(filenames.size());

	for (int i = 0; i < (int)filenames.size(); i++)
	{

		if(!objectImporter.Import(getDir(filenames[i]), &(*objData)[i]))
			return false;
	}

	return true;
}

bool ResourceImporter::ImportMaterial(const wchar_t* filename, std::vector<int>* id)
{
	return objectImporter.Import(getDir(filename), id);
}
bool ResourceImporter::ImportMaterial(std::vector<const wchar_t*>& filenames, std::vector<int>* id)
{
	if(!filenames.size())
		return false;

	std::vector<int> temp;

	for (int i = 0; i < (int)filenames.size(); i++)
	{
		if(!objectImporter.Import(getDir(filenames[i]), &temp))
			return false;

		id->insert(id->end(), temp.begin(), temp.end());
	}

	return true;
}

