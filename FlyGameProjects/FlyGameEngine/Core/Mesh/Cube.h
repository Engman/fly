#ifndef CUBE_H
#define CUBE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>

#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\BaseBuffer.h"
#include "..\IShader.h"
#include "..\..\Util\vertex.h"
#include "..\Texture2D.h"
#include "ObjectMaterial.h"

class Cube
{
private:
	struct WeightCB
	{
		float weight1;
		D3DXVECTOR3 padd;
	};

	SmartPtrStd<BaseBuffer>	m_VertexBuffer;
	SmartPtrStd<BaseBuffer>	m_VertexBuffer2;
	SmartPtrStd<BaseBuffer>	m_AnimationBuffer;
	SmartPtrStd<BaseBuffer>	m_weightBuffer;
	SmartPtrStd<BaseBuffer> m_IndexBuffer;
	D3DXMATRIX				m_world;
	D3DXMATRIX				m_scale;
	D3DXMATRIX				m_rotation;
	D3DXMATRIX				m_tranlate;

	std::vector<Texture2D>	m_texture;
	IShader*				m_shader;
	ObjectMaterial			m_material;
	float time; 

	D3DXVECTOR3 getNormal(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2);
	
public:
	Cube();
	~Cube();
	void			Initialize(D3DXMATRIX world,  float height, float widht, float depth,  ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader);
	
	void            Update(); 
	void			setWorld(D3DXMATRIX world);
	
	void			Render( ID3D11DeviceContext* g_DeviceContext);
	D3DXMATRIX		getWorld();
	void			SetShader(IShader* shader);
	void			addTextures(std::vector<WCHAR*> textNames);
	void			createAnimationBuffer();
};
#endif