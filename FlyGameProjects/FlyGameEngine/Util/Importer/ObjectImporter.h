#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include <string>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
class FlyMesh;
template<typename T> class SmartPtrStd;
template<typename T> class SmartPtrArr;


class ObjectImporter
{
	public:
		ObjectImporter() {  }
		/**
		*	Imports a object file
		*/
		bool Import(std::wstring file, ID3D11Device* device, ID3D11DeviceContext* _dc, std::vector<SmartPtrStd<FlyMesh>>& object);
		/**
		*	Imports a material file
		*/
		bool Import(std::wstring file, ID3D11Device* device, ID3D11DeviceContext* _dc, std::vector<int>& identifiers);
};


#endif

