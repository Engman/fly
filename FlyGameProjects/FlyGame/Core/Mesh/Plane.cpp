#include "Plane.h"

Plane::Plane()
{
}
Plane::~Plane()
{
}
void Plane::Initialize(D3DXMATRIX world,  float height, float width, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader )
{
	
	VERTEX::VertexPNT mesh[] =
	{
		{D3DXVECTOR4(-width, height,0, 1 ), D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0 ,0)},
		{D3DXVECTOR4(width,height,0,1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1 ,0)},
		{D3DXVECTOR4(-width,-height,0,1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0 ,1)},

		{D3DXVECTOR4(-width, -height,0, 1)	,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(0, 1)},
		{D3DXVECTOR4(width,height,0, 1)		,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1, 0)},
		{D3DXVECTOR4(width,-height, 0,1)	,D3DXVECTOR4(0,0,-1, 0), D3DXVECTOR2(1, 1)}

	};

	BaseBuffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.dc = D3DShell::self()->getDeviceContext();
	bufferDesc.device = D3DShell::self()->getDevice();
	bufferDesc.elementSize = sizeof(VERTEX::VertexPNT);
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

	std::vector<WCHAR*> text;
	text.push_back(L"..\\Resources\\Textures\\Baron_Nashor.png");
	text.push_back(L"..\\Resources\\Textures\\Baron_NashorBump.png");
	text.push_back(L"..\\Resources\\Textures\\Baron_NashorSpec.png");
	addTextures(text);
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
	draw_data.textures = &m_texture;
	draw_data.worldMatrix = &m_world;
	m_shader->addDrawData(draw_data);
}
void Plane::SetShader(IShader* shader)
{
	m_shader = shader;
}

void Plane::addTextures(std::vector<WCHAR*> textNames)
{
	SmartPtrArr<Texture2D>  text;
	text = new Texture2D[textNames.size()];

	for(int i=0; i< (int)textNames.size(); i++)
	{		
		text[i].loadTexture(D3DShell::self()->getDevice(), textNames[i]);
		m_texture.push_back(text[i]);
	}
	
}
