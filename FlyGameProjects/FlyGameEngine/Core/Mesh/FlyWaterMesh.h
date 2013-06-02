#ifndef FLYWATERMESH_H
#define FLYWATERMESH_H

#include "..\Entity.h"
#include "..\..\Util\vertex.h"
//#include "BoundingVolumes"




/** 
*	This class is the base class handling the drawable mesh data.
*	-----
*	It's possible to override function [void Update()] and [void Render()]
*	to update and render a different way than default.
*/
class FlyWaterMesh		:public Entity
{
	public:
		struct OBJECT_DESC
		{
			/** The ID3D11Device to use for initialization */
			ID3D11Device					*device;
			/** The ID3D11DeviceContext to use for initialization */
			ID3D11DeviceContext				*deviceContext;
			/** The vertex list to use in the buffer (VertexPNT) */

			SmartPtrStd<std::vector<VERTEX::VertexPNT>>		vertecies;

			/** Vertex count */
			UINT							vCount;
			/** The material identifier */
			int								material_id;
			/** (Optional) */
			IShader*						shader;
			/** Name (Optional) */
			std::wstring					name;
			/** The bounding spehere to the object (optional) */
			SmartPtrStd<BoundingSphere>		boundingSphere;
			OBJECT_DESC()
				:device(0), deviceContext(0), vertecies(0), vCount(0), material_id(-1), shader(0), name(L""), boundingSphere(0)
			{}
		};
	private:
		SmartPtrStd<std::vector<vec3>> vertexList;
		SmartPtrStd<BaseBuffer> waterBuffer; 
		D3DXVECTOR2 waterTranslation; 

		void CalculateTangentBinormal(VERTEX::VertexPNT vertex1,VERTEX::VertexPNT vertex2,VERTEX::VertexPNT vertex3,D3DXVECTOR4& tangent,D3DXVECTOR4& binormal);

	public:
		FlyWaterMesh();
		virtual~FlyWaterMesh();

		/** Override to update logic different than default */
		virtual void Update() override;
		/** Override to render with different than default */
		virtual void Render(ViewFrustum& frustum) override;

		/** Builds a FlyMesh than is ready for rendering */
		virtual bool Initialize(OBJECT_DESC& data);

		virtual void Release() override { }

		/** Get triangles for mesh */
		virtual vector<vec3>* GetTriangles();

		BaseBuffer* getWaterBuffer(); 
		void UpdateWater(float deltaTime); 
};


#endif

