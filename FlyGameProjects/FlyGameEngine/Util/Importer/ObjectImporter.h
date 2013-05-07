#ifndef OBJECT_IMPORTER_H
#define OBJECT_IMPORTER_H

#include <string>
#include <vector>

#include "..\vertex.h"
#include "..\SmartPtrs.h"

#pragma region FORWARD DECLARATIONS

	struct ID3D11Device;
	struct ID3D11DeviceContext;
	class Entity;


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

#pragma endregion


class ObjectImporter
{
	public:
		ObjectImporter() {  }
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

