#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"

class LightShader : public IShader
{
public:
	LightShader();

	void draw(SHADER_PARAMETER_DATA& drawData);
	void setSRVBuffer();

private:

};
#endif