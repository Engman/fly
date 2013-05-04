#ifndef GBUFFERSHADER_H
#define GBUFFERSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"

class GBufferShader : public IShader
{
	public:
		GBufferShader();

		void draw(PER_FRAME_DATA& frameData);

	private:
		
		
};

#endif