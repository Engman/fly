#ifndef MESH_H
#define MESH_H

#include "Object.h"

class Mesh
{
	private:
		Object mesh;

	public:
		Mesh();
		virtual ~Mesh();

		bool Initialize(/*Model* modelRawData*/);

		void Update();
		void Render();
};

#endif