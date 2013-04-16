#include "ResourceImporter.h"

#pragma region IMPORTERS

	static ObjectImporter objectImporter = ObjectImporter();

#pragma endregion

bool ResourceImporter::Import(wchar_t*  filename, __out SmartPtrStd<ImportedObjectData>& rawData)
{
	return objectImporter.Import(filename, rawData);
}