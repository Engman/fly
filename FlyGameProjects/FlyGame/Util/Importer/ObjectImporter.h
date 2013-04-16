#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include "..\..\Core\Mesh\RawObjectData.h"
#include "..\misc.h"

class ObjectImporter
{
	private:
		void ParseUVs			(RawVertecieData& rawVert, std::wifstream& in, int frame);				
		void ParsePositions		(RawVertecieData& rawVert, std::wifstream& in, int frame);		
		void ParseNormals		(RawVertecieData& rawVert, std::wifstream& in, int frame);	
		void ParseFace			(RawVertecieData& rawVert, std::wifstream& in, int frame, ImportedObjectData* rawData);
		void ParseMaterial		(RawMaterialData& rawData, std::wifstream& in);
		std::wstring ParseText	(std::wifstream& in, bool trash = false);
		void ParseVector4		(std::wifstream& in, vec4&);
		int ParseMeshCount		(std::wifstream& in, ImportedObjectData* rawData);

	public:
		ObjectImporter() {}
		bool Import(wchar_t* file, SmartPtrStd<ImportedObjectData>& rawData);
};


#endif

