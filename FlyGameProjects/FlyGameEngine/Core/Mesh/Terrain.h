#ifndef TERRAIN_H
#define TERRAIN_H

#include "FlyMesh.h"
#include "../../Util/OctTree.h"

class Terrain : public FlyMesh
{
	private:
		OctTree octTree;
		SmartPtrStd<std::vector<VERTEX::VertexPNT>>	vertices;

	public:
		Terrain();
		~Terrain();

		bool Initialize(OBJECT_DESC& data);

		void Update();
		void Render(ViewFrustum& frustum);

		vector<VERTEX::VertexPNT> GetCollidedBoxes(BoundingBox box);

		void Release();
};

#endif