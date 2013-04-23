#ifndef I_STATIC_OBJECT_H
#define I_STATIC_OBJECT_H


#include "MaterialHandler.h"
#include "..\Entity.h"
#include "..\..\Util\vertex.h"
//#include "BoundingVolumes"


/** 
*	This class is the base class handling the drawable mesh data.
*	-----
*	It's possible to override function [void Update()] and [void Render()]
*	to update and render a different way than default.
*/
class Object		:public Entity
{
	public:
		struct OBJECT_DESC
		{
			/** The ID3D11Device to use for initialization */
			ID3D11Device					*device;
			/** The ID3D11DeviceContext to use for initialization */
			ID3D11DeviceContext				*deviceContext;
			/** The vertex list to use in the buffer (VertexPNT) */
			SmartPtrStd<std::vector<VERTEX::VertexPNT>>							vertecies;
			/** Vertex count */
			UINT							vCount;
			/** The material identifier */
			int								material_id;
			/** (Optional) */
			IShader*						shader;
		};

	protected:
		//BoundingBox boundingBox;
		ObjectMaterial* material;

	public:
		Object();
		virtual~Object();

		/** Override to update logic diffrent than default */
		virtual void Update();
		/** Override to render with diffrent than default */
		virtual void Render();

		/** Builds a Object than is ready for rendering */
		virtual bool Initialize(OBJECT_DESC& data);
};


#endif

