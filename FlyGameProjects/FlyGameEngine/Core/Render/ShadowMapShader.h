#ifndef SHADOWMAPSHADER_H
#define SHADOWMAPSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"

class ShadowMapShader : public IShader
{
public:

	ShadowMapShader();
	~ShadowMapShader();
	bool init(BaseShader::BASE_SHADER_DESC& desc);
	void draw(PER_FRAME_DATA& drawData);

private:

	SmartPtrStd<BaseBuffer> lightViewBuffer; 

};
#endif