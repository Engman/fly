#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>

#include "..\stdafx.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\BaseBuffer.h"
#include "..\IShader.h"
#include "..\..\Util\vertex.h"



class FullScreenQuad
{
private:
	SmartPtrStd<BaseBuffer>	m_VertexBuffer;
	SmartPtrStd<BaseBuffer> m_IndexBuffer;

	IShader*				m_shader;
	
	
public:
	FullScreenQuad();
	~FullScreenQuad();
	void			Initialize(ID3D11Device* g_Device, ID3D11DeviceContext* g_DeviceContext, IShader* shader);
	
	void            Update(); 
	
	void			Render( ID3D11DeviceContext* g_DeviceContext);

	void			SetShader(IShader* shader);
};
#endif