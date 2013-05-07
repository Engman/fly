#include "BlendState.h"
using namespace ShaderStates;

namespace
{
	static D3D11_BLEND_DESC blendStateAlphaBlendDesc()
	{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
			desc.AlphaToCoverageEnable = false;
			desc.IndependentBlendEnable = false;
			for (unsigned int i = 0; i < 8; ++i)
			{
				desc.RenderTarget[i].BlendEnable			= true;
				desc.RenderTarget[i].BlendOp				= D3D11_BLEND_OP_ADD;
				desc.RenderTarget[i].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				desc.RenderTarget[i].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[i].DestBlendAlpha			= D3D11_BLEND_ONE;
				desc.RenderTarget[i].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
				desc.RenderTarget[i].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].SrcBlendAlpha			= D3D11_BLEND_ONE;
			}

			return desc;
	}
	static D3D11_BLEND_DESC blendStateDisabledBlendDesc()
	{
			D3D11_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = false;
			bd.IndependentBlendEnable = false;
			for (unsigned int i = 0; i < 8; ++i)
			{
				bd.RenderTarget[i].BlendEnable                  = false;
				bd.RenderTarget[i].BlendOp						= D3D11_BLEND_OP_ADD;
				bd.RenderTarget[i].BlendOpAlpha					= D3D11_BLEND_OP_ADD;
				bd.RenderTarget[i].DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
				bd.RenderTarget[i].DestBlendAlpha				= D3D11_BLEND_ONE;
				bd.RenderTarget[i].RenderTargetWriteMask		= D3D11_COLOR_WRITE_ENABLE_ALL;
				bd.RenderTarget[i].SrcBlend                     = D3D11_BLEND_SRC_ALPHA;
				bd.RenderTarget[i].SrcBlendAlpha                = D3D11_BLEND_ONE;
			}

			return bd;
	}
		
	static ID3D11BlendState* blendStateAlphaBlend = NULL;
	static ID3D11BlendState* blendStateDisabledBlend = NULL;
	static int BlendStateUseCount = 0;
}
struct BlendStates::_Data
{
	ID3D11BlendState* custom;
	
	_Data()
		:custom(0)
	{ BlendStateUseCount++; }
	~_Data()
	{
		if(BlendStateUseCount == 1)	//Release if last
		{
			if (blendStateAlphaBlend)
				blendStateAlphaBlend->Release();

			if (blendStateDisabledBlend)
				blendStateDisabledBlend->Release();
		}
		BlendStateUseCount--;

        if (custom)
			custom->Release();
	}
};


BlendStates::BlendStates()
{
	this->_data = new _Data();
}
BlendStates::~BlendStates()
{
	delete this->_data;
}


bool BlendStates::init(ID3D11Device* device, D3D11_BLEND_DESC* desc)
{
	if(desc)
		if(!this->_data->custom)
			if (FAILED ( device->CreateBlendState(desc, &this->_data->custom) ) )
				return false;

	if (!blendStateAlphaBlend && FAILED ( device->CreateBlendState(&blendStateAlphaBlendDesc(), &blendStateAlphaBlend) ) )
		return false;

	if (!blendStateDisabledBlend && FAILED ( device->CreateBlendState(&blendStateDisabledBlendDesc(), &blendStateDisabledBlend) ) )
		return false;

	return true;
}


ID3D11BlendState* BlendStates::getAlphaBlend() 
{
	return blendStateAlphaBlend;
}
ID3D11BlendState* BlendStates::getDisabledBlend() 
{
	return blendStateDisabledBlend;
}
ID3D11BlendState* BlendStates::getCustom() 
{
	return this->_data->custom;
}
ID3D11BlendState* BlendStates::getState(FLAGS::STATE_BLEND_MODE s) 
{
	switch (s)
	{
		case FLAGS::BLEND_MODE_AlphaBlend:
			return blendStateAlphaBlend;
		break;

		case FLAGS::BLEND_MODE_DisabledBlend:
			return blendStateDisabledBlend;
		break;

		case FLAGS::BLEND_MODE_Custom:
			return this->_data->custom;
		break;
	}

	return NULL;
}