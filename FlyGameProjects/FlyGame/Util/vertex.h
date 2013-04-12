#ifndef VERTEX_H
#define VERTEX_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX10math.h>


namespace VERTEX
{

	/* 
	*	@P - Vector4 Position
	*/
	struct VertexP
	{
		D3DXVECTOR4 position;
		VertexP()
			:position(0.0f, 0.0f, 0.0f, 1.0f)
		{}
	};
	static D3D11_INPUT_ELEMENT_DESC VertexP_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
	};



	/* 
	*	@P - Vector4 Position 
	*	@C - Vector4 Color
	*/
	struct VertexPC
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 color;
		VertexPC()
			:position(0.0f, 0.0f, 0.0f, 1.0f), color(0.0f, 0.0f, 0.0f, 1.0f)
		{}
	};
	static D3D11_INPUT_ELEMENT_DESC VertexPC_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};



	/* 
	*	@P - Vector4 Position 
	*	@T - Vector2 TexxtureCoord
	*/
	struct VertexPT
	{
		D3DXVECTOR4 position;
		D3DXVECTOR2 texcoord;
		VertexPT()
			:position(0.0f, 0.0f, 0.0f, 1.0f), texcoord(0.0f, 0.0f)
		{}

	};
	static D3D11_INPUT_ELEMENT_DESC VertexPT_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};



	/* 
	*	@P - Vector4 Position 
	*	@N - Vector4 Normal
	*/
	struct VertexPN
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 normal;
		VertexPN()
			:position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f)
		{}
	};
	static D3D11_INPUT_ELEMENT_DESC VertexPN_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};



	/* 
	*	@P - Vector4 Position 
	*	@N - Vector4 Normal
	*	@T - Vector2 TextureCoord
	*/
	struct VertexPNT
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 normal;
		D3DXVECTOR2 texcoord;
		VertexPNT()
			:position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 1.0f), texcoord(0.0f, 0.0f)
		{}
	};
	static D3D11_INPUT_ELEMENT_DESC VertexPNT_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	/* 
	*	@P - Vector4 Position 
	*	@N - Vector4 Normal
	*	@C - Vector4 Colour
	*/
	struct VertexPNC
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 normal;
		D3DXVECTOR4 color;
		VertexPNC()
			:position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 1.0f), color(0.0f, 0.0f, 0.0f, 1.0f)
		{}
	};
	static D3D11_INPUT_ELEMENT_DESC VertexPNC_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT   , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//struct made by linda
	struct VertexPNC3
	{
		D3DXVECTOR4 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
		/*VertexPNC3()
		:position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 1.0f)
		{}
		VertexPNC3(D3DXVECTOR4 position, D3DXVECTOR3 normal, D3DXVECTOR4 color)
		:position(position), normal(normal), color(color)
		{}*/
	};
	static D3D11_INPUT_ELEMENT_DESC VertexPNC3_InputElementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT   , 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}

#endif