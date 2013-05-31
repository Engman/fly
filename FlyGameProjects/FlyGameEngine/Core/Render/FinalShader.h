#ifndef FINALSHADER_H
#define FINALSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\Light\LightHolder.h"

class FinalShader : public IShader
{
	public:
		FinalShader();
		void draw(PER_FRAME_DATA& drawData);

	private:

};
#endif