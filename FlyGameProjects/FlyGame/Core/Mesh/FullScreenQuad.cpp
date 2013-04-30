#include "FullScreenQuad.h"

FullScreenQuad::FullScreenQuad()
{

}
FullScreenQuad::~FullScreenQuad()
{

}
void FullScreenQuad::Initialize( ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader )
{
	
	VERTEX::VertexPT mesh[] =
	{
		{D3DXVECTOR4(-1, 1,0, 1),	D3DXVECTOR2(0 ,0)},
		{D3DXVECTOR4(1,1,0,1)	,	D3DXVECTOR2(1 ,0)},
		{D3DXVECTOR4(-1,-1,0,1)	,	D3DXVECTOR2(0 ,1)},

		{D3DXVECTOR4(-1, -1,0, 1),	D3DXVECTOR2(0, 1)},
		{D3DXVECTOR4(1,1,0, 1)	,	D3DXVECTOR2(1, 0)},
		{D3DXVECTOR4(1,-1, 0,1)	,	D3DXVECTOR2(1, 1)}

	};


	BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(VERTEX::VertexPT);
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

	m_shader = shader;
}
void FullScreenQuad::Update()
{

}

void FullScreenQuad::Render( ID3D11DeviceContext* g_DeviceContext)
{
	IShader::DRAW_DATA draw_data;
	draw_data.buffers.push_back(m_VertexBuffer);
	draw_data.buffers.push_back(m_IndexBuffer);
	//draw_data.textures = NULL;

	draw_data.material = NULL;

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	draw_data.worldMatrix = NULL;
	m_shader->addDrawData(draw_data);
}
void FullScreenQuad::SetShader(IShader* shader)
{
	m_shader = shader;
}
