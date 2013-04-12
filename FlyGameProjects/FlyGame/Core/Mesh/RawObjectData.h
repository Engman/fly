#ifndef RAW_OBJECT_DATA_H
#define RAW_OBJECT_DATA_H

#include "../../Util/misc.h"
#include "../../Util/SmartPtrs.h"
#include <vector>

/** Contains vertecie data read from a mesh file */
struct RawVertecieData
{
	std::vector<int>						index;
	std::vector<vec4>						position;
	std::vector<vec4>						normal;
	std::vector<vec2>						textCoord;
};

/** Contains material data read from a mesh material file */
struct RawMaterialData
{
	vec4									ambient;
	vec4									diffuse;
	vec4									specular;
	SmartPtrArr<wchar_t>					ambientTexture;
	SmartPtrArr<wchar_t>					diffuseTexture;
	SmartPtrArr<wchar_t>					specularTexture;
	float									specualrPow;
};

/** Wraps [RawVertecieData] and [RawMaterialData] as two [SmartPtrStd] */
class RawObjectData
{
	public:
		SmartPtrStd<RawVertecieData>		vertecieData;
		SmartPtrStd<RawMaterialData>		materialData;
};


#endif

