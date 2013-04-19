#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\Light\LightHolder.h"

class ColorShader : public IShader
{
public:
	ColorShader();

	void draw(SHADER_PARAMETER_DATA& drawData);
	void setSRVBuffer();

private:

	

};
#endif