#ifndef RAW_OBJECT_DATA_H
#define RAW_OBJECT_DATA_H

#include "..\..\Util\vertex.h"
#include "..\..\Util\SmartPtrs.h"
#include <vector>

struct RawVertIndexData
{
	int v;
	int vt;
	int vn;
	RawVertIndexData()
		:v(0), vt(0), vn(0)
	{}
	RawVertIndexData(int _v, int _vt, int _vn)
		:v(_v), vt(_vt), vn(_vn)
	{}
};
struct RawVertecieFaceData
{
	RawVertIndexData f[3];

	RawVertecieFaceData()
	{}
};
struct RawVertecieData
{
	std::vector<RawVertecieFaceData>		index;
	std::vector<D3DXVECTOR4>				position;
	std::vector<D3DXVECTOR4>				normal;
	std::vector<D3DXVECTOR2>				textCoord;
};
struct RawMaterialData
{
	std::wstring							name;
	D3DXVECTOR4								ambient;
	D3DXVECTOR4								diffuse;
	D3DXVECTOR4								specular;
	int										specualarPow;
	std::wstring							ambientTexture;
	std::wstring							diffuseTexture;
	std::wstring							specularTexture;
	std::wstring							glowTexture;
	std::wstring							normalTexture;
};

struct VertexObjectData
{
	std::vector<VERTEX::VertexPNT>	vertex;
	std::vector<UINT>				indecies;
};

/** Wraps imported data, contains data from ONE file */
struct ImportedObjectData
{
	/** The name of the object collection */
	std::wstring									name;
	/** Contains n loaded objects */
	std::vector<VertexObjectData>		MeshData;
	/** The material loaded with the obeject */
	//std::vector<RawMaterialData>		materialData;
};


#endif

