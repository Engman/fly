#ifndef RESOURCE_IMPORT_H
#define RESOURCE_IMPORT_H

#include <vector>

using namespace std;

struct ID3D11Device;
struct ID3D11DeviceContext;
class FlyMesh;
template<typename T> class SmartPtrStd;

class ResourceImporter
{
	enum ImportType
	{
		Material,
	};
	public:
		/** Imports an drawable object.
		*	The data will be stored in the objData param */
		static bool ImportObject(const wchar_t* filename, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<SmartPtrStd<FlyMesh>>& object);
		/**
		*	Impoerts a list of drawable objects.
		*	The data will be stored in the objData param */
		static bool ImportObject(vector<const wchar_t*>& filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<SmartPtrStd<FlyMesh>>& objData);
		/**
		*	Imports a material file.
		*	The material identifiers is stored in the id param.
		*	The material data is stored in the global MaterialHandler */
		static bool ImportMaterial(const wchar_t* filename, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<int>& id);
		/**
		*	Imports a serveral external material files.
		*	The material identifiers is stored in the id param.
		*	The material data is stored in the global MaterialHandler */
		static bool ImportMaterial(vector<const wchar_t*>& filenames, ID3D11Device* device, ID3D11DeviceContext* _dc, vector<int>& id);
};


#endif

