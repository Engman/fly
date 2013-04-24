#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include "..\..\Core\Mesh\RawObjectData.h"
#include "..\misc.h"




class ObjectImporter
{
	public:
		ObjectImporter() {}
		/**
		*	Imports a object file
		*/
		bool Import(std::wstring file, ID3D11Device* device, ID3D11DeviceContext* _dc, __out SmartPtrStd<ImportedObjectData>& objectData);
		/**
		*	Imports a material file
		*/
		bool Import(std::wstring file, ID3D11Device* device, ID3D11DeviceContext* _dc, __out std::vector<int>& identifiers);
};


#endif

