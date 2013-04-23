#ifndef PLANE_H
#define PLANE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>


#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\BaseBuffer.h"
#include "..\IShader.h"
#include "..\..\Util\vertex.h"
#include "..\Texture2D.h"
#include <vector>
#include "ObjectMaterial.h"

class Plane
{
private:
	SmartPtrStd<BaseBuffer>	m_VertexBuffer;
	SmartPtrStd<BaseBuffer> m_IndexBuffer;
	D3DXMATRIX				m_world;
	IShader*				m_shader;
	std::vector<Texture2D>	m_texture;
	ObjectMaterial			m_material;
	
public:
	Plane();
	~Plane();
	void			Initialize(D3DXMATRIX world,  float height, float widht, ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader);
	
	void            Update(); 
	void			setWorld(D3DXMATRIX world);
	
	void			Render( ID3D11DeviceContext* g_DeviceContext);
	D3DXMATRIX		getWorld();
	void			SetShader(IShader* shader);
	void			addTextures(std::vector<WCHAR*> textNames);
};
#endif