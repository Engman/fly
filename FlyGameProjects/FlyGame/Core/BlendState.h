#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include <D3D11.h>
#include "ShaderFlagStates.h"

namespace ShaderStates
{
	class BlendStates
	{
		private:
			struct _Data;
			_Data *_data;

		public:
			BlendStates();
			virtual ~BlendStates();
		
			/** 
			*	The device to initiate the states with.
			*	Set D3D11_BLEND_DESC to create custom BlendModeState, or leave bland for none 
			*/
			bool init(ID3D11Device*, D3D11_BLEND_DESC* desc = NULL);
		
			ID3D11BlendState* getAlphaBlend()					;
			ID3D11BlendState* getDisabledBlend()				;
			ID3D11BlendState* getCustom()						;
			ID3D11BlendState* getState(FLAGS::STATE_BLEND_MODE)	;
	};
}

#endif