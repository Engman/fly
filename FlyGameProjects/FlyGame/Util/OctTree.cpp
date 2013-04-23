#include "OctTree.h"
#include "CollisionLib.h"

OctTree::OctTree()
{
	this->head = NULL;
}

OctTree::~OctTree()
{

}

void OctTree::Initialize(VertexType* vertexList, int vertexCount, int iterations, ID3D11Device* device)
{
	this->pVertexList = vertexList;
	this->vertexCount = vertexCount;

	this->head = new Node();

	CalculateBoxSize();

	NewChild(this->head, this->head->box.minPoint,this->head->box.maxPoint, iterations - 1, device);

	delete[]this->pVertexList;
	this->pVertexList = NULL;
}

void OctTree::NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int iterations, ID3D11Device* device)
{
	maxPoint = D3DXVECTOR3(maxPoint.x, maxPoint.y, maxPoint.z);
	minPoint = D3DXVECTOR3(minPoint.x, minPoint.y, minPoint.z);

	parent->box.minPoint = minPoint;
	parent->box.maxPoint = maxPoint;

	if(iterations > 0)
	{
		parent->children = new Node[8];

		D3DXVECTOR3 middle = D3DXVECTOR3(maxPoint.x*0.5f + minPoint.x*0.5f, maxPoint.y*0.5f + minPoint.y*0.5f, maxPoint.z*0.5f + minPoint.z*0.5f);

		NewChild(&parent->children[0], minPoint, middle, iterations - 1, device);
		NewChild(&parent->children[1], D3DXVECTOR3(middle.x, minPoint.y, minPoint.z), D3DXVECTOR3(maxPoint.x, middle.y, middle.z), iterations - 1, device);
		NewChild(&parent->children[2], D3DXVECTOR3(middle.x, minPoint.y, middle.z), D3DXVECTOR3(maxPoint.x, middle.y, maxPoint.z), iterations - 1, device);
		NewChild(&parent->children[3], D3DXVECTOR3(minPoint.x, minPoint.y, middle.z), D3DXVECTOR3(middle.x, middle.y, maxPoint.z), iterations - 1, device);
		
		NewChild(&parent->children[4], D3DXVECTOR3(minPoint.x, middle.y, minPoint.z), D3DXVECTOR3(middle.x, maxPoint.y, middle.z), iterations - 1, device);
		NewChild(&parent->children[5], D3DXVECTOR3(middle.x, middle.y, minPoint.z), D3DXVECTOR3(maxPoint.x, maxPoint.y, middle.z), iterations - 1, device);
		NewChild(&parent->children[6], middle, maxPoint, iterations - 1, device);
		NewChild(&parent->children[7], D3DXVECTOR3(minPoint.x, middle.y, middle.z), D3DXVECTOR3(middle.x, maxPoint.y, maxPoint.z), iterations - 1, device);
	
	}
	else
	{
		VertexType* vertices = NULL;
		unsigned long* indices = NULL;
		unsigned long verticesInNode = 0;
		unsigned long nodeVertexIndex = 0;

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		for(unsigned long i = 0; i < this->vertexCount; i+=3)
		{
			if(IsPointContained(i, minPoint, maxPoint) || IsPointContained(i+1, minPoint, maxPoint) || IsPointContained(i+2, minPoint, maxPoint))
			{
				verticesInNode+=3;
			}
		}

		parent->indexCount = verticesInNode;
		indices = new unsigned long[verticesInNode];
		vertices = new VertexType[verticesInNode];
		
		

		for(unsigned long i = 0; i < this->vertexCount; i+=3)
		{
			if(IsPointContained(i, minPoint, maxPoint) || IsPointContained(i+1, minPoint, maxPoint) || IsPointContained(i+2, minPoint, maxPoint))
			{
				vertices[nodeVertexIndex].position = this->pVertexList[i].position;
				vertices[nodeVertexIndex].texture = this->pVertexList[i].texture;
				vertices[nodeVertexIndex].normal = this->pVertexList[i].normal;
				indices[nodeVertexIndex] = nodeVertexIndex;
				nodeVertexIndex++;

				vertices[nodeVertexIndex].position = this->pVertexList[i+1].position;
				vertices[nodeVertexIndex].texture = this->pVertexList[i+1].texture;
				vertices[nodeVertexIndex].normal = this->pVertexList[i+1].normal;
				indices[nodeVertexIndex] = nodeVertexIndex;
				nodeVertexIndex++;

				vertices[nodeVertexIndex].position = this->pVertexList[i+2].position;
				vertices[nodeVertexIndex].texture = this->pVertexList[i+2].texture;
				vertices[nodeVertexIndex].normal = this->pVertexList[i+2].normal;
				indices[nodeVertexIndex] = nodeVertexIndex;
				nodeVertexIndex++;
			}
		}

  		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType)*verticesInNode;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now finally create the vertex buffer.
		device->CreateBuffer(&vertexBufferDesc, &vertexData, &parent->pVertexBuffer);

		// Set up the description of the index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long)*nodeVertexIndex;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		device->CreateBuffer(&indexBufferDesc, &indexData, &parent->pIndexBuffer);

		// Release the vertex and index arrays now that the data is stored in the buffers in the node.
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;

		parent->children = NULL;
	}

}

void OctTree::CalculateBoxSize()
{
	this->head->box.minPoint = this->pVertexList[0].position;
	this->head->box.maxPoint = this->pVertexList[0].position;

	for(unsigned long i = 0; i < this->vertexCount; i++)
	{
		if(this->pVertexList[i].position.x < this->head->box.minPoint.x)
		{
			this->head->box.minPoint.x = this->pVertexList[i].position.x;
		}
		if(this->pVertexList[i].position.y < this->head->box.minPoint.y)
		{
			this->head->box.minPoint.y = this->pVertexList[i].position.y;
		}
		if(this->pVertexList[i].position.z < this->head->box.minPoint.z)
		{
			this->head->box.minPoint.z = this->pVertexList[i].position.z;
		}

		if(this->pVertexList[i].position.x > this->head->box.maxPoint.x)
		{
			this->head->box.maxPoint.x = this->pVertexList[i].position.x;
		}
		if(this->pVertexList[i].position.y > this->head->box.maxPoint.y)
		{
			this->head->box.maxPoint.y = this->pVertexList[i].position.y;
		}
		if(this->pVertexList[i].position.z > this->head->box.maxPoint.z)
		{
			this->head->box.maxPoint.z = this->pVertexList[i].position.z;
		}
	}
}

bool OctTree::IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	if(!(minPoint.x <= this->pVertexList[index].position.x &&  this->pVertexList[index].position.x <= maxPoint.x))
	{
		return false;
	}
	if(!(minPoint.y <= this->pVertexList[index].position.y  &&  this->pVertexList[index].position.y <= maxPoint.y))
	{
		return false;
	}
	if(!(minPoint.z <= this->pVertexList[index].position.z  &&  this->pVertexList[index].position.z <= maxPoint.z))
	{
		return false;
	}

	return true;
}

vector<OctTree::RenderBufferType> OctTree::Render(ID3D11DeviceContext* dc, ViewFrustum frustum)
{
	vector<RenderBufferType> bufferList;
	int verticesToRender = 0;

	if(FrustumVSBox(frustum, this->head->box))
	{
		bufferList = RenderNode(dc, this->head, frustum);
	}


	return bufferList;
}

vector<OctTree::RenderBufferType> OctTree::RenderNode(ID3D11DeviceContext* dc, Node* parent, ViewFrustum frustum)
{
	vector<RenderBufferType> bufferList;
	RenderBufferType buffers;

	if(FrustumVSBox(frustum, parent->box))
	{
		if(parent->children == NULL)
		{
			buffers.vertexBuffer = parent->pVertexBuffer;
			buffers.indexBuffer = parent->pIndexBuffer;

			buffers.verticesToRender = parent->indexCount;

			bufferList.push_back(buffers);
		}
		else
		{
			vector<RenderBufferType> tempList;

			tempList = RenderNode(dc, &parent->children[0], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[1], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[2], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[3], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[4], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[5], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[6], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
			tempList = RenderNode(dc, &parent->children[7], frustum);
			bufferList.insert(bufferList.end(), tempList.begin(), tempList.end());
		}
	}

	return bufferList;
}


void OctTree::Release()
{
	if(this->head != NULL)
	{
		ReleaseChild(this->head);
	}

	delete head;
}

void OctTree::ReleaseChild(Node* parent)
{
	if(parent->children != NULL)
	{
		ReleaseChild(&parent->children[0]);
		ReleaseChild(&parent->children[1]);
		ReleaseChild(&parent->children[2]);
		ReleaseChild(&parent->children[3]);
		ReleaseChild(&parent->children[4]);
		ReleaseChild(&parent->children[5]);
		ReleaseChild(&parent->children[6]);
		ReleaseChild(&parent->children[7]);

		delete[]parent->children;
		parent->children = 0;
	}
	else if(parent->indexCount > 0)
	{
		parent->pVertexBuffer->Release();
		parent->pVertexBuffer = 0;
		parent->pIndexBuffer->Release();
		parent->pIndexBuffer = 0;
	}
}