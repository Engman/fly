#include "Sphere.h"
Sphere::Sphere()
{
	vertexBuffer= NULL;
	indexBuffer = NULL;
}
Sphere::~Sphere()
{
	/*if(vertexBuffer)
		vertexBuffer
		vertexBuffer->Release();
	if(indexBuffer)
		indexBuffer->Release();*/
}

void Sphere::Initiate(float radius, UINT sliceCount, UINT stackCount, ID3D11Device *device, IShader* shader, D3DXMATRIX translate)
{
	CreateSphere(radius, sliceCount, stackCount, sphereObj);
	InitBuffers(device);
	m_shader = shader;
	D3DXMatrixIdentity(&this->world);
	this->world *= translate;
}

void Sphere::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	VertexSphere topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	VertexSphere bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back( topVertex );

	float phiStep   = PI/stackCount;
	float thetaStep = 2.0f*PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			VertexSphere v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			D3DXVec3Normalize(&v.TangentU, &v.TangentU);
			//D3DXVECTOR3 T = v.TangentU;
			//XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			D3DXVec3Normalize(&v.Normal, &v.Position);
			//XMVECTOR p = XMLoadFloat3(&v.Position);
			//XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / (PI*2);
			v.TexC.y = phi / PI;

			meshData.Vertices.push_back( v );
		}
	}

	meshData.Vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i+1);
		meshData.Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.Vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for(UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex+i);
		meshData.Indices.push_back(baseIndex+i+1);
	}
}

void Sphere::InitBuffers(ID3D11Device *device)
{

	std::vector<VERTEX::VertexP> vertices(sphereObj.Indices.size());

	for(size_t i = 0; i < sphereObj.Indices.size(); ++i)
	{
	

		vertices[i].position = D3DXVECTOR4(sphereObj.Vertices[sphereObj.Indices.at(i)].Position, 1);
		//vertices[i].normal = D3DXVECTOR4(sphereObj.Vertices[sphereObj.Indices.at(i)].Normal,0);
		//vertices[i].texcoord = sphereObj.Vertices[sphereObj.Indices.at(i)].TexC;
	}

	BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(VERTEX::VertexP);
	bufferDesc.data = &vertices[0];
	bufferDesc.nrOfElements = (int)sphereObj.Indices.size();
	bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	this->vertexBuffer = new BaseBuffer();
	if(FAILED(vertexBuffer->Initialize(bufferDesc)))
	{
		MessageBox(0, L"Could not initialize sphereVertexBuffer! Sphere.cpp - Initialize", L"Error", MB_OK);
	}

	//mIndexCount = sphereObj.Indices.size();
	//std::vector<int> indices16;
	//indices16.assign(sphereObj.Indices.begin(), sphereObj.Indices.end());

	//
	//bufferDesc.dc = D3DShell::self()->getDeviceContext();
	//bufferDesc.device = D3DShell::self()->getDevice();
	//bufferDesc.elementSize = sizeof(int);
	//bufferDesc.data = &indices16[0];
	//bufferDesc.nrOfElements = mIndexCount;
	//bufferDesc.type = BUFFER_FLAG::TYPE_INDEX_BUFFER;
	//bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	//this->indexBuffer = new BaseBuffer();
	//if(FAILED(indexBuffer->Initialize(bufferDesc)))
	//{
	//	MessageBox(0, L"Could not initialize sphereVertexBuffer! Sphere.cpp - Initialize", L"Error", MB_OK);
	//	
	//}
	//float width = 20; 
	//float height = 20; 

	//VERTEX::VertexPNT mesh[] =
	//{
	//	{D3DXVECTOR4(-width, height,0, 1 ), D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0 ,0)},
	//	{D3DXVECTOR4(width,height,0,1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1 ,0)},
	//	{D3DXVECTOR4(-width,-height,0,1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0 ,1)},

	//	{D3DXVECTOR4(-width, -height,0, 1)	,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0, 1)},
	//	{D3DXVECTOR4(width,height,0, 1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1, 0)},
	//	{D3DXVECTOR4(width,-height, 0,1)	,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1, 1)}

	//};

	//BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	//bufferDesc.dc = D3DShell::self()->getDeviceContext();
	//bufferDesc.device = D3DShell::self()->getDevice();
	//bufferDesc.elementSize = sizeof(VERTEX::VertexPNT);
	//bufferDesc.data = mesh;
	//bufferDesc.nrOfElements = 6;
	//bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	//bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	//this->vertexBuffer = new BaseBuffer();
	//if(FAILED(vertexBuffer->Initialize(bufferDesc)))
	//{
	//	MessageBox(0, L"Could not initialize planeVertexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	//}

	//int index []= {0,1,2,3,4,5};
	//bufferDesc.dc = D3DShell::self()->getDeviceContext();
	//bufferDesc.device = D3DShell::self()->getDevice();
	//bufferDesc.elementSize = sizeof(int);
	//bufferDesc.data = index;
	//bufferDesc.nrOfElements = 6; 
	//bufferDesc.type = BUFFER_FLAG::TYPE_INDEX_BUFFER;
	//bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	//indexBuffer = new BaseBuffer();
	//if(FAILED(indexBuffer->Initialize(bufferDesc)))
	//{
	//	MessageBox(0, L"Could not initialize planeIndexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	//}
}

void Sphere::Render(BaseBuffer* light, D3DXMATRIX world)
{
	IShader::DRAW_DATA draw_data;
	draw_data.buffers.push_back(vertexBuffer);
	//draw_data.buffers.push_back(indexBuffer);

	draw_data.lightBuffers.push_back(light);

	draw_data.material = NULL;

	draw_data.worldMatrix = &this->world;

	m_shader->addDrawData(draw_data);
}
Sphere::MeshData Sphere::GetSphere()
{
	return sphereObj; 
}