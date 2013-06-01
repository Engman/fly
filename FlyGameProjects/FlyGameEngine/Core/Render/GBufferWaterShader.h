#ifndef GBUFFERWATERSHADER_H
#define GBUFFERWATERSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"

class GBufferWaterShader : public IShader
{
	public:

		GBufferWaterShader();
		void draw(PER_FRAME_DATA& frameData);
		int getType() const;

	private:
		
};

#endif