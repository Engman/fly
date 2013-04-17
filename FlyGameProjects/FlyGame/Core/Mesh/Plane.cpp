#include "Plane.h"

Plane::Plane()
{
	//m_VertexBuffer = NULL;
}
Plane::~Plane()
{
	//DELETE_PTR(m_VertexBuffer);
	//SAFE_DELETE(g_VertexBuffer);
}
void Plane::Initialize(D3DXMATRIX world,  float height, float width, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader )
{
	D3DXVECTOR3 n1=getNormal(D3DXVECTOR3(0,-1,0), D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,0,0));
	D3DXVECTOR3 n2=getNormal(D3DXVECTOR3(0,-1,0), D3DXVECTOR3(1,0,0), D3DXVECTOR3(1,-1,0));	

	VERTEX::VertexPNC3 mesh[] =
	{
		{D3DXVECTOR4(-width, -height,0, 1 ), n1, D3DXVECTOR4(0,0, 1 ,0)},
		{D3DXVECTOR4(-width,height,0,1)		, n1, D3DXVECTOR4(0,0, 1 ,0)},
		{D3DXVECTOR4(width,height,0,1)	, n1, D3DXVECTOR4(0,0, 1 ,0)},

		{D3DXVECTOR4(-width, -height,0, 1)	, n2, D3DXVECTOR4(0,0, 1 ,0)},
		{D3DXVECTOR4(width,height,0, 1)		, n2, D3DXVECTOR4(0,0, 1 ,0)},
		{D3DXVECTOR4(width,-height, 0,1)	, n2, D3DXVECTOR4(0,0, 1 ,0)}

	};


	BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(VERTEX::VertexPNC3);
	bufferDesc.data = mesh;
	bufferDesc.nrOfElements = 6;
	bufferDesc.type = BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	this->m_VertexBuffer = new BaseBuffer();
	if(FAILED(m_VertexBuffer->Initialize(bufferDesc)))
	{
		MessageBox(0, L"Could not initialize planeVertexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	}

	int index []= {0,1,2,3,4,5};
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(int);
	bufferDesc.data = index;
	bufferDesc.nrOfElements = 6; 
	bufferDesc.type = BUFFER_FLAG::TYPE_INDEX_BUFFER;
	bufferDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	m_IndexBuffer = new BaseBuffer();
	if(FAILED(m_IndexBuffer->Initialize(bufferDesc)))
	{
		MessageBox(0, L"Could not initialize planeIndexBuffer! Plane.cpp - Initialize", L"Error", MB_OK);
	}
	m_world= world;
	m_shader = shader;
}


void Plane::Update()
{

}


void Plane::setWorld(D3DXMATRIX world)
{
	m_world= world;
}
D3DXMATRIX Plane::getWorld()
{
	return m_world;
}

void Plane::Render( ID3D11DeviceContext* g_DeviceContext)
{
	IShader::DRAW_DATA draw_data;
	draw_data.buffers.push_back(m_VertexBuffer);
	draw_data.buffers.push_back(m_IndexBuffer);
	draw_data.worldMatrix = &m_world;
	m_shader->addDrawData(draw_data);

	//unsigned int size= sizeof(VERTEX::VertexPNC3);
	//unsigned int offset=0;

	//g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_VertexBuffer->setBuffer();


}
void Plane::SetShader(IShader* shader)
{
	m_shader = shader;
}
D3DXVECTOR3 Plane::getNormal(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{
	D3DXVECTOR3 v1= p1-p0;
	D3DXVECTOR3 v2= p2-p0;

	D3DXVECTOR3 normal, n;

	D3DXVec3Cross(&normal, &v1, &v2);
	D3DXVec3Normalize(&n, &normal);

	return n;
} 