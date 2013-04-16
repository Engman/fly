#ifndef RESOURCE_IMPORT_H
#define RESOURCE_IMPORT_H

#include "ObjectImporter.h"

class ResourceImporter
{
	public:
		/** Imports a mesh in raw format from given file 
		*	Data will be stored in rawData */
		static bool Import(wchar_t*  filename, __out SmartPtrStd<ImportedObjectData>& objData);
};


#endif

