#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"

class LightShader : public IShader
{
public:

	LightShader();
	~LightShader();
	bool init(BaseShader::BASE_SHADER_DESC& desc);
	void draw(PER_FRAME_DATA& drawData);

private:
	SmartPtrStd<BaseBuffer> cameraBuffer; 
};
#endif