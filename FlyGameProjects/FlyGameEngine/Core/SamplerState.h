#ifndef SAMPLER_STATE_H
#define SAMPLER_STATE_H

#include <D3D11.h>
#include "ShaderFlagStates.h"

namespace ShaderStates
{
	class SamplerState
	{
		private:
            struct _Data;
			_Data *_data;

		public:
            SamplerState();
            virtual ~SamplerState();

			/** The device to initiare the states with.
			*	Set D3D11_SAMPLER_DESC to create custom SamplerState, or leave bland for none */
            bool init(ID3D11Device* device, D3D11_SAMPLER_DESC* desc = NULL);

            ID3D11SamplerState* getPoint()						;
            ID3D11SamplerState* getLinear()						;
            ID3D11SamplerState* getAnisotropic2()				;
            ID3D11SamplerState* getAnisotropic4()				;
            ID3D11SamplerState* getAnisotropic8()				;
            ID3D11SamplerState* getAnisotropic16()				;
			ID3D11SamplerState* getLinearClamp()				;
            ID3D11SamplerState* getCustom()						;
            ID3D11SamplerState* getState(FLAGS::STATE_SAMPLING)	;


	};

}

#endif

