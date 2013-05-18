#ifndef BLURSHADER_H
#define BLURSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\..\Util\SmartPtrs.h"

class BlurShader : public IShader
{
public:

	BlurShader();
	~BlurShader();
	void draw(PER_FRAME_DATA& drawData);
	bool init(BaseShader::BASE_SHADER_DESC& desc);

private:
	struct blurBuffer
	{
		float textureHeight;
		float textureWidth;
		float blurFactor;
		float padd;
	};

	SmartPtrStd<BaseBuffer> blurCB; 

};
#endif