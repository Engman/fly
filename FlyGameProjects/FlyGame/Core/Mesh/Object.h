#ifndef I_STATIC_OBJECT_H
#define I_STATIC_OBJECT_H

#include "../Entity.h"
#include "RawObjectData.h"
//#include "BoundingBox"

class Object		:public Entity
{
	protected:
		//BoundingBox boundingBox;
		//Mesh

	public:
		Object();
		virtual~Object();

		virtual void Update();
		virtual void Render();

		virtual bool Initialize(RawObjectData modelRawData);
};


#endif

