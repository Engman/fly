#include "OctTree.h"
#include "CollisionLib.h"

OctTree::OctTree()
{
	this->head = NULL;
}

OctTree::~OctTree()
{

}

void OctTree::Initialize(SmartPtrStd<std::vector<VERTEX::VertexPNT>> vertexList, int vertexCount, int renderIterations, int collisionIterations)
{
	this->pVertexList = vertexList;
	this->vertexCount = vertexCount;
	this->collIter = collisionIterations;
	this->renderIter = renderIterations;
	this->head = new Node();

	CalculateBoxSize();

	NewChild(this->head, this->head->box.minPoint,this->head->box.maxPoint, renderIterations - 1, collisionIterations - 1);

	for(unsigned long i = 0; i < this->vertexCount; i+=3)
	{
		PutVerticesInBox(this->head, i, renderIterations - 1, collisionIterations - 1);
	}

	InitNodeBuffers(this->head, renderIterations - 1);
}

void OctTree::NewChild(Node* parent, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, int renderIterations, int collisionIterations)
{
	maxPoint = D3DXVECTOR3(maxPoint.x, maxPoint.y, maxPoint.z);
	minPoint = D3DXVECTOR3(minPoint.x, minPoint.y, minPoint.z);

	parent->box.minPoint = minPoint;
	parent->box.maxPoint = maxPoint;

	if(collisionIterations > 0)
	{
		parent->children = new Node[8];

		D3DXVECTOR3 middle = D3DXVECTOR3(maxPoint.x*0.5f + minPoint.x*0.5f, maxPoint.y, maxPoint.z*0.5f + minPoint.z*0.5f);

		NewChild(&parent->children[0], minPoint, middle, renderIterations - 1, collisionIterations - 1);
		NewChild(&parent->children[1], D3DXVECTOR3(middle.x, minPoint.y, minPoint.z), D3DXVECTOR3(maxPoint.x, maxPoint.y, middle.z), renderIterations - 1, collisionIterations - 1);
		NewChild(&parent->children[2], D3DXVECTOR3(middle.x, minPoint.y, middle.z), D3DXVECTOR3(maxPoint.x, maxPoint.y, maxPoint.z), renderIterations - 1, collisionIterations - 1);
		NewChild(&parent->children[3], D3DXVECTOR3(minPoint.x, minPoint.y, middle.z), D3DXVECTOR3(middle.x, maxPoint.y, maxPoint.z), renderIterations - 1, collisionIterations - 1);

		NewChild(&parent->children[4], D3DXVECTOR3(minPoint.x, middle.y, minPoint.z), D3DXVECTOR3(middle.x, maxPoint.y, middle.z), renderIterations - 1, collisionIterations - 1);
		NewChild(&parent->children[5], D3DXVECTOR3(middle.x, middle.y, minPoint.z), D3DXVECTOR3(maxPoint.x, maxPoint.y, middle.z), renderIterations - 1, collisionIterations - 1);
		NewChild(&parent->children[6], middle, maxPoint, renderIterations - 1, collisionIterations - 1);														   
		NewChild(&parent->children[7], D3DXVECTOR3(minPoint.x, middle.y, middle.z), D3DXVECTOR3(middle.x, maxPoint.y, maxPoint.z), renderIterations - 1, collisionIterations - 1);
	}
	else
	{
		parent->nodeVertexList = new vector<D3DXVECTOR3>;
		parent->indexCount = 0;
		parent->children = NULL;		
	}

	if(renderIterations == 0)
	{
		parent->bigVertexList = new vector<VERTEX::VertexPNT>;
		parent->indexCount = 0;
	}
}

void OctTree::PutVerticesInBox(Node* parent, int index, int renderIterations, int collisionIterations)
{
	if(IsPointContained(index, parent->box.minPoint, parent->box.maxPoint))
	{
		if(parent->children != NULL)
		{
			PutVerticesInBox(&parent->children[0], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[1], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[2], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[3], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[4], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[5], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[6], index, renderIterations - 1, collisionIterations - 1);
			PutVerticesInBox(&parent->children[7], index, renderIterations - 1, collisionIterations - 1);
		}
		if(renderIterations == 0)
		{
			parent->bigVertexList->push_back(this->pVertexList->at(index));
			parent->bigVertexList->push_back(this->pVertexList->at(index+1));
			parent->bigVertexList->push_back(this->pVertexList->at(index+2));		
			parent->indexCount+=3;
		}
		if(collisionIterations == 0)
		{
			parent->nodeVertexList->push_back(D3DXVECTOR3(this->pVertexList->at(index).position.x,this->pVertexList->at(index).position.y,this->pVertexList->at(index).position.z));
			parent->nodeVertexList->push_back(D3DXVECTOR3(this->pVertexList->at(index+1).position.x,this->pVertexList->at(index+1).position.y,this->pVertexList->at(index+1).position.z));
			parent->nodeVertexList->push_back(D3DXVECTOR3(this->pVertexList->at(index+2).position.x,this->pVertexList->at(index+2).position.y,this->pVertexList->at(index+2).position.z));
		}
	}
}

void OctTree::InitNodeBuffers(Node* parent, int renderIterations)
{
	if(renderIterations > 0)
	{
		InitNodeBuffers(&parent->children[0], renderIterations - 1);
		InitNodeBuffers(&parent->children[1], renderIterations - 1);
		InitNodeBuffers(&parent->children[2], renderIterations - 1);
		InitNodeBuffers(&parent->children[3], renderIterations - 1);
		InitNodeBuffers(&parent->children[4], renderIterations - 1);
		InitNodeBuffers(&parent->children[5], renderIterations - 1);
		InitNodeBuffers(&parent->children[6], renderIterations - 1);
		InitNodeBuffers(&parent->children[7], renderIterations - 1);
	}
	else
	{
		BaseBuffer::BUFFER_INIT_DESC bufferDesc;
		bufferDesc.dc = D3DShell::self()->getDeviceContext();
		bufferDesc.device = D3DShell::self()->getDevice();
		bufferDesc.elementSize = sizeof(VERTEX::VertexPNT);
		bufferDesc.nrOfElements = (int)parent->indexCount;
		bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
		bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;
		if(parent->bigVertexList->size() != 0)
		{
			bufferDesc.data = &(*parent->bigVertexList)[0];
		}
		else
		{
			parent->indexCount = 0;
			bufferDesc.data = NULL;
		}

		parent->pVertexBuffer = new BaseBuffer();

		parent->pVertexBuffer->Initialize(bufferDesc);
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

	/*this->head->box.maxPoint.x += 5.0f;
	this->head->box.maxPoint.y += 5.0f;
	this->head->box.maxPoint.z += 5.0f;
	this->head->box.minPoint.x -= 5.0f;
	this->head->box.minPoint.y -= 5.0f;
	this->head->box.minPoint.z -= 5.0f;*/
}

bool OctTree::IsPointContained(int index, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	BoundingBox tempBox;
	tempBox.minPoint = minPoint;
	tempBox.maxPoint = maxPoint;
	D3DXVECTOR3 tempPoint[3];
	tempPoint[0].x = this->pVertexList->at(index).position.x;
	tempPoint[0].y = this->pVertexList->at(index).position.y;
	tempPoint[0].z = this->pVertexList->at(index).position.z;
	tempPoint[1].x = this->pVertexList->at(index+1).position.x;
	tempPoint[1].y = this->pVertexList->at(index+1).position.y;
	tempPoint[1].z = this->pVertexList->at(index+1).position.z;
	tempPoint[2].x = this->pVertexList->at(index+2).position.x;
	tempPoint[2].y = this->pVertexList->at(index+2).position.y;
	tempPoint[2].z = this->pVertexList->at(index+2).position.z;

	return BoxVSTriangle(tempBox, tempPoint);
}

vector<BaseBuffer*> OctTree::Render(ViewFrustum frustum, IShader* shader, IShader::DRAW_DATA data)
{
	vector<BaseBuffer*> bufferList;
	int verticesToRender = 0;

	if(FrustumVSBox(frustum, this->head->box))
	{
		bufferList = RenderNode(D3DShell::self()->getDeviceContext(), this->head, frustum, shader, data);
	}


	return bufferList;
}

vector<BaseBuffer*> OctTree::RenderNode(ID3D11DeviceContext* dc, Node* parent, ViewFrustum frustum, IShader* shader, IShader::DRAW_DATA data)
{
	vector<BaseBuffer*> bufferList;

	if(parent->indexCount != 0 && FrustumVSBox(frustum, parent->box))
	{
		if(parent->indexCount > 0)
		{
			data.buffers.push_back(parent->pVertexBuffer);
			shader->addDrawData(data);
		}
		else
		{
			vector<BaseBuffer*> tempList;

			RenderNode(dc, &parent->children[0], frustum, shader, data);
			RenderNode(dc, &parent->children[1], frustum, shader, data);
			RenderNode(dc, &parent->children[2], frustum, shader, data);
			RenderNode(dc, &parent->children[3], frustum, shader, data);
			RenderNode(dc, &parent->children[4], frustum, shader, data);
			RenderNode(dc, &parent->children[5], frustum, shader, data);
			RenderNode(dc, &parent->children[6], frustum, shader, data);
			RenderNode(dc, &parent->children[7], frustum, shader, data);
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


vector<vector<D3DXVECTOR3>*> OctTree::GetCollidedBoxes(BoundingSphere sphere)
{
	vector<vector<D3DXVECTOR3>*> bufferList;
	int verticesToRender = 0;

	if(BoxVSSphere(this->head->box, sphere))
	{
		bufferList = GetCollidedBoxNode(this->head, sphere);
	}

	return bufferList;
}

vector<vector<D3DXVECTOR3>*> OctTree::GetCollidedBoxNode(Node* parent, BoundingSphere sphere)
{
	vector<vector<D3DXVECTOR3>*> vertexList;

	if(BoxVSSphere(parent->box, sphere))
	{
		if(parent->children == NULL)
		{
			vertexList.push_back(parent->nodeVertexList);
		}
		else
		{
			vector<D3DXVECTOR3>* tempList;
			vector<vector<D3DXVECTOR3>*> tempPointerList;

			tempPointerList = GetCollidedBoxNode(&parent->children[0], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList.at(i);
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}
			
			tempPointerList = GetCollidedBoxNode(&parent->children[1], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

			tempPointerList = GetCollidedBoxNode(&parent->children[2], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

			tempPointerList = GetCollidedBoxNode(&parent->children[3], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

			tempPointerList = GetCollidedBoxNode(&parent->children[4], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList.at(i);
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}
			
			tempPointerList = GetCollidedBoxNode(&parent->children[5], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

			tempPointerList = GetCollidedBoxNode(&parent->children[6], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

			tempPointerList = GetCollidedBoxNode(&parent->children[7], sphere);
			if(tempPointerList.size() > 0)
			{
				for(unsigned int i = 0; i < tempPointerList.size(); i++)
				{
					tempList = tempPointerList[i];
					if(tempList && tempList->size() != 0)
						vertexList.push_back(tempList);
				}
			}

		}
	}

	return vertexList;
}

BoundingBox OctTree::GetTopBox() const
{
	return this->head->box;
}

void OctTree::TransformBoxes(Matrix transform)
{
	TransformChildBox(this->head, transform);
}

void OctTree::TransformChildBox(Node* parent, Matrix transform)
{
	vec4 vectorFour;
	parent->box.minPoint = (vec3)*D3DXVec3Transform(&vectorFour, &parent->box.minPoint, &transform);
	parent->box.maxPoint = (vec3)*D3DXVec3Transform(&vectorFour, &parent->box.maxPoint, &transform);

	if(parent->children != NULL)
	{
		TransformChildBox(&parent->children[0], transform);
		TransformChildBox(&parent->children[1], transform);
		TransformChildBox(&parent->children[2], transform);
		TransformChildBox(&parent->children[3], transform);
		TransformChildBox(&parent->children[4], transform);
		TransformChildBox(&parent->children[5], transform);
		TransformChildBox(&parent->children[6], transform);
		TransformChildBox(&parent->children[7], transform);
	}
}