#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"

class ColorShader : public IShader
{
public:
	ColorShader();

	void draw(SHADER_PARAMETER_DATA& drawData);
	void setSRVBuffer();

private:
	ID3D11ShaderResourceView *m_colorSRV; 
	BaseBuffer SRVBuffer;
	

};
#endif