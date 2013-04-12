#ifndef RASTERIZER_STATE_H
#define RASTERIZER_STATE_H

#include <D3D11.h>
#include "ShaderFlagStates.h"

namespace ShaderStates
{
	class RasterizerState
	{
		private:
			struct _Data;
			_Data* _data;
	
		public:
			RasterizerState();
			virtual~RasterizerState();
	
			/** The device to initiare the states with.
			*	Set D3D11_RASTERIZER_DESC to create custom RasterizerState, or leave bland for none */
			bool init(ID3D11Device* a_device, D3D11_RASTERIZER_DESC* custom = NULL);
	
			ID3D11RasterizerState* getBackCullNoMS()				;
			ID3D11RasterizerState* getFrontCullNoMS()				;
			ID3D11RasterizerState* getNoCullNoMs()					;
			ID3D11RasterizerState* getNoCullNoMSWF()				;
			ID3D11RasterizerState* getCustom()						;
			ID3D11RasterizerState* getState(FLAGS::STATE_RASTERIZER) ;
	};
}


#endif