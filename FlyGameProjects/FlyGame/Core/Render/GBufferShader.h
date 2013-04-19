#ifndef GBUFFERSHADER_H
#define GBUFFERSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"

class GBufferShader : public IShader
{
	public:
		GBufferShader();

		void draw(SHADER_PARAMETER_DATA& drawData);

	private:
		
		
};

#endif