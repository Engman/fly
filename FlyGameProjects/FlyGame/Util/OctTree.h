#ifndef OCTTREE_H
#define OCTTREE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX10math.h>
#include <stdio.h>
#include <vector>

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
			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;

			int verticesToRender;
		};
	private:	
		struct Node
		{
			BoundingBox box;

			Node* children;

			ID3D11Buffer* pVertexBuffer;
			ID3D11Buffer* pIndexBuffer;

			unsigned long indexCount;
		};

		Node* head;

		VertexType* pVertexList;
		unsigned long vertexCount;
		unsigned long indexCount;

		void NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int iterations, ID3D11Device* device);
		void CalculateBoxSize();

		bool IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);

		vector<RenderBufferType> RenderNode(ID3D11DeviceContext* dc, Node* parent, ViewFrustum frustum);

		void ReleaseChild(Node* parent);

	public:
		OctTree();
		virtual ~OctTree();

		/** The vertex list sent to Initialize will be handled by the QuadTree class, i.e. the data pointed to will be deleted in this method*/
		void Initialize(VertexType* vertexList, int vertexCount, int iterations, ID3D11Device* device);
		void Release();

		/** The returned value contains a list of buffer structures which are prepared to be rendered*/
		vector<RenderBufferType> Render(ID3D11DeviceContext* dc, ViewFrustum frustum);
};

#endif