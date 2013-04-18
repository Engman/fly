#ifndef RAW_OBJECT_DATA_H
#define RAW_OBJECT_DATA_H


#include "..\..\Util\vertex.h"
#include "..\..\Util\SmartPtrs.h"

#include <vector>

struct ObjectData
{
	int material;
	std::vector<VERTEX::VertexPNT> vertex;
};
struct FrameData
{
	int objectIndex;
	int frameNumber;
	float frameTime;
};
struct AnimationData
{
	int id;
	std::vector<FrameData> frames;
};


/** Wraps imported data */
struct ImportedObjectData
{
	/** The name of the object collection */
	std::wstring					name;
	/** Contains n loaded objects */
	std::vector<ObjectData>			objects;
	/** Contains animation information */
	std::vector<AnimationData>		animations;
	/** The material loaded with the obeject */
	//std::vector<int>				materialData;
};

#endif

