#ifndef CUBE_H
#define CUBE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>

#include "..\stdafx.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\BaseBuffer.h"
#include "..\IShader.h"
#include "..\..\Util\vertex.h"

//struct Vertex
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 normal;
//	D3DXVECTOR4 color;
//};


class Cube
{
private:
	SmartPtrStd<BaseBuffer>	m_VertexBuffer;
	SmartPtrStd<BaseBuffer> m_IndexBuffer;
	D3DXMATRIX				m_world;
	D3DXMATRIX				m_scale;
	D3DXMATRIX				m_rotation;
	D3DXMATRIX				m_tranlate;

	IShader*				m_shader;
	
	D3DXVECTOR3 getNormal(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2);
	
public:
	Cube();
	~Cube();
	void			Initialize(D3DXMATRIX world,  float height, float widht, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader);
	
	void            Update(); 
	void			setWorld(D3DXMATRIX world);
	
	void			Render( ID3D11DeviceContext* g_DeviceContext);
	D3DXMATRIX		getWorld();
	void			SetShader(IShader* shader);
};
#endif