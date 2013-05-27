#ifndef FGM_IMPORT_DATA_H
#define FGM_IMPORT_DATA_H

#include <vector>
#include <string>
#include "..\SmartPtrs.h"
#include "..\vertex.h"

struct ObjectData
	{
		int material;
		SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertex;
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
	};

#endif

