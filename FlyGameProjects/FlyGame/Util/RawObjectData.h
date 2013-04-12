#ifndef RAW_OBJECT_DATA_H
#define RAW_OBJECT_DATA_H

#include "misc.h"
#include "SmartPtrs.h"
#include "vertex.h"

class RawObjectData
{
	public:
		SmartPtrArr<int>	index;
		SmartPtrArr<vec4>	position;
		SmartPtrArr<vec4>	normal;
		SmartPtrArr<vec2>	textCoord;

		SmartPtrArr<char*> textures;

		//SmartPtrArr<Material> material;
};


#endif

