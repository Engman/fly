#ifndef I_STATIC_OBJECT_H
#define I_STATIC_OBJECT_H

#include "../Entity.h"
#include "RawObjectData.h"
//#include "BoundingBox"


/** 
*	This class is the base class handling the drawable mesh data.
*	-----
*	It's possible to override function [void Update()] and [void Render()]
*	to update and render a different way than default.
*/
class Object		:public Entity
{
	protected:
		//BoundingBox boundingBox;
		//Mesh

	public:
		Object();
		virtual~Object();

		/** Override to update logic diffrent than default */
		virtual void Update();
		/** Override to render with diffrent than default */
		virtual void Render();

		/** Builds a Object than is ready for rendering */
		bool Initialize(ImportedObjectData modelRawData);
};


#endif

