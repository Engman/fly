#include "OctTree.h"
#include "CollisionLib.h"

OctTree::OctTree()
{
	this->head = NULL;
}

OctTree::~OctTree()
{

}

void OctTree::Initialize(SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertexList, int vertexCount, int iterations)
{
	this->pVertexList = vertexList;
	this->vertexCount = vertexCount;

	this->head = new Node();

	CalculateBoxSize();

	NewChild(this->head, this->head->box.minPoint,this->head->box.maxPoint, iterations - 1);
}

void OctTree::NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int iterations)
{
	maxPoint = D3DXVECTOR3(maxPoint.x, maxPoint.y, maxPoint.z);
	minPoint = D3DXVECTOR3(minPoint.x, minPoint.y, minPoint.z);

	parent->box.minPoint = minPoint;
	parent->box.maxPoint = maxPoint;

	if(iterations > 0)
	{
		parent->children = new Node[8];

		D3DXVECTOR3 middle = D3DXVECTOR3(maxPoint.x*0.5f + minPoint.x*0.5f, maxPoint.y*0.5f + minPoint.y*0.5f, maxPoint.z*0.5f + minPoint.z*0.5f);

		NewChild(&parent->children[0], minPoint, middle, iterations - 1);
		NewChild(&parent->children[1], D3DXVECTOR3(middle.x, minPoint.y, minPoint.z), D3DXVECTOR3(maxPoint.x, middle.y, middle.z), iterations - 1);
		NewChild(&parent->children[2], D3DXVECTOR3(middle.x, minPoint.y, middle.z), D3DXVECTOR3(maxPoint.x, middle.y, maxPoint.z), iterations - 1);
		NewChild(&parent->children[3], D3DXVECTOR3(minPoint.x, minPoint.y, middle.z), D3DXVECTOR3(middle.x, middle.y, maxPoint.z), iterations - 1);
		
		NewChild(&parent->children[4], D3DXVECTOR3(minPoint.x, middle.y, minPoint.z), D3DXVECTOR3(middle.x, maxPoint.y, middle.z), iterations - 1);
		NewChild(&parent->children[5], D3DXVECTOR3(middle.x, middle.y, minPoint.z), D3DXVECTOR3(maxPoint.x, maxPoint.y, middle.z), iterations - 1);
		NewChild(&parent->children[6], middle, maxPoint, iterations - 1);
		NewChild(&parent->children[7], D3DXVECTOR3(minPoint.x, middle.y, middle.z), D3DXVECTOR3(middle.x, maxPoint.y, maxPoint.z), iterations - 1);
	
	}
	else
	{
		SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertices = NULL;
		unsigned long verticesInNode = 0;
		unsigned long nodeVertexIndex = 0;


		for(unsigned long i = 0; i < this->vertexCount; i+=3)
		{
			if(IsPointContained(i, minPoint, maxPoint) || IsPointContained(i+1, minPoint, maxPoint) || IsPointContained(i+2, minPoint, maxPoint))
			{
				verticesInNode+=3;
			}
		}

		parent->indexCount = verticesInNode;
		vertices = new vector<VERTEX::VertexPNT>;
		
		

		for(unsigned long i = 0; i < this->vertexCount; i+=3)
		{
			if(IsPointContained(i, minPoint, maxPoint) || IsPointContained(i+1, minPoint, maxPoint) || IsPointContained(i+2, minPoint, maxPoint))
			{
				vertices->push_back(VERTEX::VertexPNT());

				vertices->at(nodeVertexIndex).position = this->pVertexList->at(i).position;
				vertices->at(nodeVertexIndex).texcoord = this->pVertexList->at(i).texcoord;
				vertices->at(nodeVertexIndex).normal = this->pVertexList->at(i).normal;
				nodeVertexIndex++;
				

				vertices->push_back(VERTEX::VertexPNT());

				vertices->at(nodeVertexIndex).position = this->pVertexList->at(i+1).position;
				vertices->at(nodeVertexIndex).texcoord = this->pVertexList->at(i+1).texcoord;
				vertices->at(nodeVertexIndex).normal = this->pVertexList->at(i+1).normal;
				nodeVertexIndex++;

				vertices->push_back(VERTEX::VertexPNT());

				vertices->at(nodeVertexIndex).position = this->pVertexList->at(i+2).position;
				vertices->at(nodeVertexIndex).texcoord = this->pVertexList->at(i+2).texcoord;
				vertices->at(nodeVertexIndex).normal = this->pVertexList->at(i+2).normal;
				nodeVertexIndex++;
			}
		}

		parent->nodeVertexList = vertices;

  		SmartPtrStd<BaseBuffer> vertexBuffer = new BaseBuffer();

		BaseBuffer::BUFFER_INIT_DESC bufferDesc;
		bufferDesc.dc = D3DShell::self()->getDeviceContext();
		bufferDesc.device = D3DShell::self()->getDevice();
		bufferDesc.elementSize = sizeof(VERTEX::VertexPNT);
		bufferDesc.nrOfElements = (int)verticesInNode;
		bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
		bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;
		bufferDesc.data = &(*vertices)[0];

		parent->pVertexBuffer = new BaseBuffer();

		parent->pVertexBuffer->Initialize(bufferDesc);
			//return false;



		// Release the vertex and index arrays now that the data is stored in the buffers in the node.

		parent->children = NULL;
	}

}

void OctTree::CalculateBoxSize()
{
	this->head->box.minPoint = D3DXVECTOR3(this->pVertexList->at(0).position.x, this->pVertexList->at(0).position.y, this->pVertexList->at(0).position.z);
	this->head->box.maxPoint = D3DXVECTOR3(this->pVertexList->at(0).position.x, this->pVertexList->at(0).position.y, this->pVertexList->at(0).position.z);

	for(unsigned long i = 0; i < this->vertexCount; i++)
	{

		if(this->pVertexList->at(i).position.x < this->head->box.minPoint.x)
		{
			this->head->box.minPoint.x = this->pVertexList->at(i).position.x;
		}
		if(this->pVertexList->at(i).position.y < this->head->box.minPoint.y)
		{
			this->head->box.minPoint.y = this->pVertexList->at(i).position.y;
		}
		if(this->pVertexList->at(i).position.z < this->head->box.minPoint.z)
		{
			this->head->box.minPoint.z = this->pVertexList->at(i).position.z;
		}

		if(this->pVertexList->at(i).position.x > this->head->box.maxPoint.x)
		{
			this->head->box.maxPoint.x = this->pVertexList->at(i).position.x;
		}
		if(this->pVertexList->at(i).position.y > this->head->box.maxPoint.y)
		{
			this->head->box.maxPoint.y = this->pVertexList->at(i).position.y;
		}
		if(this->pVertexList->at(i).position.z > this->head->box.maxPoint.z)
		{
			this->head->box.maxPoint.z = this->pVertexList->at(i).position.z;
		}
	}
}

bool OctTree::IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	if(!(minPoint.x <= this->pVertexList->at(index).position.x && this->pVertexList->at(index).position.x <= maxPoint.x))
	{
		return false;
	}
	if(!(minPoint.y <= this->pVertexList->at(index).position.y && this->pVertexList->at(index).position.y <= maxPoint.y))
	{
		return false;
	}
	if(!(minPoint.z <= this->pVertexList->at(index).position.z && this->pVertexList->at(index).position.z <= maxPoint.z))
	{
		return false;
	}

	return true;
}

vector<OctTree::RenderBufferType> OctTree::Render(ViewFrustum frustum)
{
	vector<RenderBufferType> bufferList;
	int verticesToRender = 0;

	if(FrustumVSBox(frustum, this->head->box))
	{
		bufferList = RenderNode(D3DShell::self()->getDeviceContext(), this->head, frustum);
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
}


vector<VERTEX::VertexPNT> OctTree::GetCollidedBoxes(BoundingBox box)
{
	vector<VERTEX::VertexPNT> bufferList;
	int verticesToRender = 0;

	if(BoxVSBox(box, this->head->box))
	{
		bufferList =  GetCollidedBoxNode(this->head, box);
	}

	return bufferList;
}

vector<VERTEX::VertexPNT> OctTree::GetCollidedBoxNode(Node* parent, BoundingBox box)
{
	vector<VERTEX::VertexPNT> vertexList;

	if(BoxVSBox(box, parent->box))
	{
		if(parent->children == NULL)
		{
			vertexList = *parent->nodeVertexList;
		}
		else
		{
			vector<VERTEX::VertexPNT> tempList;

			tempList = GetCollidedBoxNode(&parent->children[0], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[1], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[2], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[3], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[4], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[5], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[6], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
			tempList = GetCollidedBoxNode(&parent->children[7], box);
			vertexList.insert(vertexList.end(), tempList.begin(), tempList.end());
		}
	}

	return vertexList;
}