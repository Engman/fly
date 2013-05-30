#ifndef FLYMESHANIMATED_H
#define FLYMESHANIMATED_H

#include "..\Entity.h"
#include "..\..\Util\vertex.h"
#include "..\..\Util\Importer\ObjectImporter.h"
//#include "BoundingVolumes"




/** 
*	This class is the base class handling the drawable mesh data.
*	-----
*	It's possible to override function [void Update()] and [void Render()]
*	to update and render a different way than default.
*/
class FlyMeshAnimated		:public Entity
{
	public:

		struct ANIM_OBJECT_DESC
		{
			/** The ID3D11Device to use for initialization */
			ID3D11Device					*device;
			/** The ID3D11DeviceContext to use for initialization */
			ID3D11DeviceContext				*deviceContext;
			/** The vertex list to use in the buffer (VertexPNT) */

			SmartPtrStd<std::vector<std::vector<VERTEX::VertexPNT>>>		vertecies;

			SmartPtrStd<std::vector<std::vector<FrameData >>> frames;
			/** Vertex count */
			UINT							vCount;
			/** The material identifier */
			int								material_id;
			/** (Optional) */
			IShader*						shader;
			/** Name (Optional) */
			std::wstring					name;
			/** The bounding spehere to the object (optional) */
			SmartPtrStd<std::vector<BoundingSphere>>		boundingSphere;
			ANIM_OBJECT_DESC()
				:device(0), deviceContext(0), vertecies(0), vCount(0), material_id(-1), shader(0), name(L"")
			{}
		};
	private:
		std::vector<std::vector<vec3>> vertexList;
		std::vector<int> currFrame; 
		std::vector<float> time;
		int vertexCount; 
		int animationCount; 
		SmartPtrStd<std::vector<vec3>> triangleList;
		SmartPtrStd<std::vector<BoundingSphere>> boundingSpheres;
		SmartPtrStd<std::vector<std::vector<FrameData>>> frameData; 
		SmartPtrStd<std::vector<std::vector<VERTEX::VertexPNT>>> keyFramesVertices; 
		

		void MorphAmination();
		void MorphAmination2();
	public:
		FlyMeshAnimated();
		virtual~FlyMeshAnimated();

		/** Override to update logic diffrent than default */
		virtual void Update() override;

		void UpdateAnimation(int nr, float deltaTime);
		void StopAnimation(int nr);
		/** Override to render with diffrent than default */
		virtual void Render(ViewFrustum& frustum) override;

		/** Builds a FlyMesh than is ready for rendering */
		virtual bool Initialize(ANIM_OBJECT_DESC& data);

		virtual void Release() override { }

		/** Get triangles for mesh */
		virtual vector<vec3>* GetTriangles();
};


#endif

