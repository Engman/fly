#ifndef FGM_IMPORTER_H
#define FGM_IMPORTER_H

#include "FgmImportData.h"

#pragma region FORWARD DECLARATIONS

	struct ID3D11Device;
	struct ID3D11DeviceContext;
	class Entity;

#pragma endregion


class FGMImport
{
	public:
		FGMImport() {  }
		/**
		*	Imports a object file
		*/
		bool Import(std::wstring file, ImportedObjectData* object);
		/**
		*	Imports a material file
		*/
		bool Import(std::wstring file, std::vector<int>* identifiers);
};


#endif

