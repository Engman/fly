#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>


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
	bool			Initialize(ID3D11Device* g_Device, IShader* shader);
	
	void			Render();
	void			Render(BaseBuffer* light, BaseBuffer* lightViewProj);

	void			SetShader(IShader* shader);
};
#endif