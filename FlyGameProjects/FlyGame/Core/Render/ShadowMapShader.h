#ifndef SHADOWMAPSHADER_H
#define SHADOWMAPSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"

class ShadowMapShader : public IShader
{
public:

	ShadowMapShader();
	void draw(PER_FRAME_DATA& drawData);

private:

	

};
#endif