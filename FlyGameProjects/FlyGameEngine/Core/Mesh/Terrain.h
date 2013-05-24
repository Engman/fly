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

		vector<vector<D3DXVECTOR3>*> GetCollidedBoxes(BoundingSphere sphere);
		BoundingBox GetBoundingBox() const;

		void TransformBoxes();

		void Release() override;
};

#endif