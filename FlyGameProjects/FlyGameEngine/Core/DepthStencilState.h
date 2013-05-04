#ifndef DEPTH_STENCIL_STATES_H
#define DEPTH_STENCIL_STATES_H

#include <D3D11.h>
#include "ShaderFlagStates.h"

namespace ShaderStates
{
	class DepthStencilState
	{
		private:
	        struct _Data;
			_Data *_data;
	
		public:
	        DepthStencilState();
	        virtual ~DepthStencilState();
	
			/** The device to initiare the states with.
			*	Set D3D11_DEPTH_STENCIL_DESC to create custom DepthStencilState, or leave bland for none */
	        bool init(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc = NULL);
	
	        ID3D11DepthStencilState* getDisabledDepth()						;
	        ID3D11DepthStencilState* getEnabledDepth()						;
			ID3D11DepthStencilState* getCustom()							;
			ID3D11DepthStencilState* getState(FLAGS::STATE_DEPTH_STENCIL)	;
	};
}

#endif