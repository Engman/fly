#include "Cube.h"

Cube::Cube()
{
	//m_VertexBuffer = NULL;
}
Cube::~Cube()
{
	//DELETE_PTR(m_VertexBuffer);
	//SAFE_DELETE(g_VertexBuffer);
}
void Cube::Initialize(D3DXMATRIX world,  float height, float width, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader )
{	
	VERTEX::VertexPNC3 mesh[] =
	{
		{D3DXVECTOR4(-1, 1,-1	,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0 ,1, 0 , 0)},
		{D3DXVECTOR4(1,1,-1		,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0 ,1, 0 , 0)},
		{D3DXVECTOR4(-1,-1,-1	,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0 ,1, 0 , 0)},

		{D3DXVECTOR4(-1, -1,-1	,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0, 1, 0 , 0)},
		{D3DXVECTOR4(1,1,-1		,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0, 1, 0 , 0)},
		{D3DXVECTOR4(1,-1, -1	,1)	,D3DXVECTOR3(0,0,-1), D3DXVECTOR4(0, 1, 0 , 0)},


		{D3DXVECTOR4(1, 1, -1	,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},
		{D3DXVECTOR4(1,1,1		,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},
		{D3DXVECTOR4(1,-1,-1	,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},

		{D3DXVECTOR4(1,-1, -1	,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},
		{D3DXVECTOR4(1,1,1		,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},
		{D3DXVECTOR4(1,-1, 1	,1)	,D3DXVECTOR3(1,0,0), D3DXVECTOR4(1,0 , 0 , 0)},

		{D3DXVECTOR4(1, 1, 1	,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},
		{D3DXVECTOR4(-1,1,1		,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},
		{D3DXVECTOR4(1,-1,1		,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},
												
		{D3DXVECTOR4(1,-1, 1	,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},
		{D3DXVECTOR4(-1,1,1		,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},
		{D3DXVECTOR4(-1,-1, 1	,1)	,D3DXVECTOR3(0,0,1), D3DXVECTOR4(0,0 , 1 , 0)},

		{D3DXVECTOR4(-1, 1, 1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},
		{D3DXVECTOR4(-1,1,-1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},
		{D3DXVECTOR4(-1,-1,1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},

		{D3DXVECTOR4(-1,-1, 1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},
		{D3DXVECTOR4(-1,1,-1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},
		{D3DXVECTOR4(-1,-1, -1	,1)	,D3DXVECTOR3(-1,0,0), D3DXVECTOR4(0.5,0 , 0.5 , 0)},

		{D3DXVECTOR4(-1, 1, 1	,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},
		{D3DXVECTOR4(1,1,1		,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},
		{D3DXVECTOR4(-1,1,-1	,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},
																
		{D3DXVECTOR4(-1,1, -1	,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},
		{D3DXVECTOR4(1,1,1		,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},
		{D3DXVECTOR4(1,1, -1	,1)	,D3DXVECTOR3(0,1,0), D3DXVECTOR4(0.5,0.5 , 0 , 0)},

		{D3DXVECTOR4(-1, -1, -1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)},
		{D3DXVECTOR4(1,-1,-1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)},
		{D3DXVECTOR4(-1,-1,1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)},

		{D3DXVECTOR4(-1,-1, 1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)},
		{D3DXVECTOR4(1,-1,-1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)},
		{D3DXVECTOR4(1,-1, 1	,1)	,D3DXVECTOR3(0,-1,0), D3DXVECTOR4(0,0.5 , 0.5 , 0)}
	};


	BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(VERTEX::VertexPNC3);
	bufferDesc.data = mesh;
	bufferDesc.nrOfElements = 36;
	bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	this->m_VertexBuffer = new BaseBuffer();
	if(FAILED(m_VertexBuffer->Initialize(bufferDesc)))
	{
		MessageBox(0, L"Could not initialize planeVertexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	}

	int index []= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, 24,25,26,27,28,29,30,31,32,33,34,35};
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(int);
	bufferDesc.data = index;
	bufferDesc.nrOfElements = 36; 
	bufferDesc.type = BUFFER_FLAG::TYPE_INDEX_BUFFER;
	bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	m_IndexBuffer = new BaseBuffer();
	if(FAILED(m_IndexBuffer->Initialize(bufferDesc)))
	{
		MessageBox(0, L"Could not initialize planeIndexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	}
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixIdentity(&m_rotation);
	D3DXMatrixIdentity(&m_scale);
	m_tranlate = world;
	m_shader = shader;
}


void Cube::Update()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationX(&rot, 0.003);
	m_rotation *= rot;
	m_world = m_rotation * m_tranlate;
	//m_world *= m_tranlate;
}


void Cube::setWorld(D3DXMATRIX world)
{
	m_world= world;
}
D3DXMATRIX Cube::getWorld()
{
	return m_world;
}

void Cube::Render( ID3D11DeviceContext* g_DeviceContext)
{
	IShader::DRAW_DATA draw_data;
	draw_data.buffers.push_back(m_VertexBuffer);
	draw_data.buffers.push_back(m_IndexBuffer);
	draw_data.worldMatrix = &m_world;
	m_shader->addDrawData(draw_data);
}
void Cube::SetShader(IShader* shader)
{
	m_shader = shader;
}
D3DXVECTOR3 Cube::getNormal(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{
	D3DXVECTOR3 v1= p1-p0;
	D3DXVECTOR3 v2= p2-p0;

	D3DXVECTOR3 normal, n;

	D3DXVec3Cross(&normal, &v1, &v2);
	D3DXVec3Normalize(&n, &normal);

	return n;
} 