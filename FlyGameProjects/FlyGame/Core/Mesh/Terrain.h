#ifndef TERRAIN_H
#define TERRAIN_H

#include "Object.h"
#include "../../Util/OctTree.h"

class Terrain : public Object
{
	private:
		OctTree octTree;

		SmartPtrStd<std::vector<VERTEX::VertexPNT>>	vertices;

	public:
		Terrain();

		bool Initialize(OBJECT_DESC& data);

		void Update();
		void Render();
};

#endif