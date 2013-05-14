#ifndef OCTTREE_H
#define OCTTREE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX10math.h>
#include <stdio.h>
#include "../Core/BaseBuffer.h"
#include <vector>
#include "vertex.h"
#include "../Core/D3DShell.h"
#include "../Core/IShader.h"

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

	private:	
		struct Node
		{
			BoundingBox box;

			Node* children;

			SmartPtrStd<BaseBuffer> pVertexBuffer;
			SmartPtrStd<std::vector<D3DXVECTOR3>> nodeVertexList;
			SmartPtrStd<vector<VERTEX::VertexPNT>> bigVertexList;

			unsigned long indexCount;
		};

		Node* head;

		SmartPtrStd<std::vector<VERTEX::VertexPNT>> pVertexList;
		unsigned long vertexCount;
		unsigned long indexCount;

		void NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int iterations);
		void CalculateBoxSize();

		bool IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);

		vector<BaseBuffer*> RenderNode(ID3D11DeviceContext* dc, Node* parent, ViewFrustum frustum, IShader* shader, IShader::DRAW_DATA data);

		vector<vector<D3DXVECTOR3>*> OctTree::GetCollidedBoxNode(Node* parent, BoundingSphere sphere);

		void ReleaseChild(Node* parent);

		void PutVerticesInBox(Node* parent, int index);

		void InitNodeBuffers(Node* parent);

	public:
		OctTree();
		virtual ~OctTree();

		/** The vertex list sent to Initialize will be handled by the QuadTree class, i.e. the data pointed to will be deleted in this method*/
		void Initialize(SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertexList, int vertexCount, int iterations);
		void Release();

		/** The returned value contains a list of buffer structures which are prepared to be rendered*/
		vector<BaseBuffer*> Render(ViewFrustum frustum, IShader* shader, IShader::DRAW_DATA data);

		vector<vector<D3DXVECTOR3>*> GetCollidedBoxes(BoundingSphere sphere);
};

#endif