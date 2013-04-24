#ifndef OCTTREE_H
#define OCTTREE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX10math.h>
#include <stdio.h>
#include "../Core/BaseBuffer.h"
#include <vector>
#include "vertex.h"
#include "../Core/D3DShell.h"

#include "BoundingVolumes.h"

using namespace std;

class OctTree
{
	public:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR3 normal;
			D3DXVECTOR2 texture;
		};
		struct RenderBufferType
		{
			SmartPtrStd<BaseBuffer> vertexBuffer;

			int verticesToRender;
		};
	private:	
		struct Node
		{
			BoundingBox box;

			Node* children;

			SmartPtrStd<BaseBuffer> pVertexBuffer;

			unsigned long indexCount;
		};

		Node* head;

		SmartPtrStd<std::vector<VERTEX::VertexPNT>> pVertexList;
		unsigned long vertexCount;
		unsigned long indexCount;

		void NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int iterations);
		void CalculateBoxSize();

		bool IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);

		vector<RenderBufferType> RenderNode(ID3D11DeviceContext* dc, Node* parent, ViewFrustum frustum);

		void ReleaseChild(Node* parent);

	public:
		OctTree();
		virtual ~OctTree();

		/** The vertex list sent to Initialize will be handled by the QuadTree class, i.e. the data pointed to will be deleted in this method*/
		void Initialize(SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertexList, int vertexCount, int iterations);
		void Release();

		/** The returned value contains a list of buffer structures which are prepared to be rendered*/
		vector<RenderBufferType> Render(ID3D11DeviceContext* dc, ViewFrustum frustum);
};

#endif