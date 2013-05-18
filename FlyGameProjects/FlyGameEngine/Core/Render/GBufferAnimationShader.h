#ifndef GBUFFERANIMATIONSHADER_H
#define GBUFFERANIMATIONSHADER_H

#include "..\IShader.h"
#include "..\D3DShell.h"

class GBufferAnimationShader : public IShader
{
	public:

		GBufferAnimationShader();
		void draw(PER_FRAME_DATA& frameData);

	private:
	struct aminationWeight
	{
		float weight;
		D3DXVECTOR3 padd;
	};
	float time;
};

#endif