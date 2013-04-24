#ifndef RESOURCE_IMPORT_H
#define RESOURCE_IMPORT_H

#include "ObjectImporter.h"

class ResourceImporter
{
	enum ImportType
	{
		Material,
	};
	public:
		/** Imports an drawable object.
		*	The data will be stored in the objData param */
		static bool ImportObject(std::wstring  filename, ID3D11Device* device, ID3D11DeviceContext* _dc, __out SmartPtrStd<ImportedObjectData>& objData);
		/**
		*	Impoerts a list of drawable objects.
		*	The data will be stored in the objData param */
		static bool ImportObject(std::vector<std::wstring>  filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, __out std::vector<SmartPtrStd<ImportedObjectData>>& objData);
		/**
		*	Imports a material file.
		*	The material identifiers is stored in the id param.
		*	The material data is stored in the global MaterialHandler */
		static bool ImportMaterial(std::wstring filename, ID3D11Device* device, ID3D11DeviceContext* _dc, __out std::vector<int>& id);
		/**
		*	Imports a serveral external material files.
		*	The material identifiers is stored in the id param.
		*	The material data is stored in the global MaterialHandler */
		static bool ImportMaterial(std::vector<std::wstring> filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, __out std::vector<int>& id);
};


#endif

