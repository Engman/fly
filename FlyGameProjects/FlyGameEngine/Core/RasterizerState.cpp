#include "RasterizerState.h"
using namespace ShaderStates;

namespace
{
	static D3D11_RASTERIZER_DESC rasterizerStateBackCullNoMSDesc()
	{
        D3D11_RASTERIZER_DESC rd;
        ZeroMemory(&rd, sizeof(rd));
        rd.AntialiasedLineEnable		= false;
        rd.CullMode						= D3D11_CULL_BACK;
        rd.DepthBias					= 0;
        rd.DepthBiasClamp				= 0.0f;
        rd.DepthClipEnable				= true;
        rd.FillMode						= D3D11_FILL_SOLID;
        rd.FrontCounterClockwise		= false;
        rd.MultisampleEnable			= false;
        rd.ScissorEnable				= false;
        rd.SlopeScaledDepthBias			= 0.0f;

        return rd;
	}
	static D3D11_RASTERIZER_DESC rasterizerStateFrontCullNoMSDesc()
	{
        D3D11_RASTERIZER_DESC rd;
        ZeroMemory(&rd, sizeof(rd));
        rd.AntialiasedLineEnable		= false;
        rd.CullMode						= D3D11_CULL_FRONT;
        rd.DepthBias					= 0;
        rd.DepthBiasClamp				= 0.0f;
        rd.DepthClipEnable				= true;
        rd.FillMode						= D3D11_FILL_SOLID;
        rd.FrontCounterClockwise		= false;
        rd.MultisampleEnable			= false;
        rd.ScissorEnable				= false;
        rd.SlopeScaledDepthBias			= 0.0f;

        return rd;
	}
	static D3D11_RASTERIZER_DESC rasterizerStateNoCullNoMsDesc()
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.AntialiasedLineEnable		= false;
		rd.CullMode						= D3D11_CULL_NONE;
		rd.DepthBias					= 0;
		rd.DepthBiasClamp				= 0.0f;
		rd.DepthClipEnable				= true;
		rd.FillMode						= D3D11_FILL_SOLID;
		rd.FrontCounterClockwise		= false;
		rd.MultisampleEnable			= false;
		rd.ScissorEnable				= false;
		rd.SlopeScaledDepthBias			= 0.0f;
	
		return rd;
	}
	static D3D11_RASTERIZER_DESC rasterizerStateNoCullNoMSWFDesc()
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.AntialiasedLineEnable		= false;
		rd.CullMode						= D3D11_CULL_NONE;
		rd.DepthBias					= 0;
		rd.DepthBiasClamp				= 0.0f;
		rd.DepthClipEnable				= true;
		rd.FillMode						= D3D11_FILL_WIREFRAME;
		rd.FrontCounterClockwise		= false;
		rd.MultisampleEnable			= false;
		rd.ScissorEnable				= false;
		rd.SlopeScaledDepthBias			= 0.0f;
	
		return rd;
	}

	static ID3D11RasterizerState* rasterizerStateBackCull_NoMS = NULL;
	static ID3D11RasterizerState* rasterizerStateFrontCull_NoMS = NULL;
	static ID3D11RasterizerState* rasterizerStateNoCull_NoMs = NULL;
	static ID3D11RasterizerState* rasterizerStateNoCull_NoMS_WF = NULL;
	static int RasterizerStateUseCount = 0;	//Count number if users for safe delete
}
struct RasterizerState::_Data
{		
	ID3D11RasterizerState* custom;

	_Data()
		:custom(0)
	{ RasterizerStateUseCount++; }
	~_Data()
	{
		if(RasterizerStateUseCount == 1)	//Release if last
		{
			if (rasterizerStateBackCull_NoMS)
				rasterizerStateBackCull_NoMS->Release();

			if (rasterizerStateFrontCull_NoMS)
				rasterizerStateFrontCull_NoMS->Release();

			if(rasterizerStateNoCull_NoMs)
				rasterizerStateNoCull_NoMs->Release();

			if (rasterizerStateNoCull_NoMS_WF)
				rasterizerStateNoCull_NoMS_WF->Release();
		}
		RasterizerStateUseCount--;

        if (this->custom)
                this->custom->Release();
	}
};


RasterizerState::RasterizerState()
{
	this->_data = new _Data();
}
RasterizerState::~RasterizerState()
{
	delete this->_data;
}


bool RasterizerState::init(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc)
{
	if(desc)
		if(!this->_data->custom)
			if ( FAILED ( device->CreateRasterizerState(desc, &this->_data->custom) ) )
				return false;
			

	if ( !rasterizerStateBackCull_NoMS && FAILED ( device->CreateRasterizerState(&rasterizerStateBackCullNoMSDesc(), &rasterizerStateBackCull_NoMS) ) )
		return false;

	if ( !rasterizerStateFrontCull_NoMS && FAILED ( device->CreateRasterizerState(&rasterizerStateFrontCullNoMSDesc(), &rasterizerStateFrontCull_NoMS) ) )
		return false;

	if( !rasterizerStateNoCull_NoMs && FAILED ( device->CreateRasterizerState(&rasterizerStateNoCullNoMsDesc(), &rasterizerStateNoCull_NoMs) ) )
		return false;

	if ( !rasterizerStateNoCull_NoMS_WF && FAILED ( device->CreateRasterizerState(&rasterizerStateNoCullNoMSWFDesc(), &rasterizerStateNoCull_NoMS_WF) ) )
		return false;

    return true;
}


ID3D11RasterizerState* RasterizerState::getBackCullNoMS() 
{
	return rasterizerStateBackCull_NoMS;
}
ID3D11RasterizerState* RasterizerState::getFrontCullNoMS() 
{
	return rasterizerStateFrontCull_NoMS;
}
ID3D11RasterizerState* RasterizerState::getNoCullNoMs() 
{
	return rasterizerStateNoCull_NoMs;
}
ID3D11RasterizerState* RasterizerState::getNoCullNoMSWF() 
{
	return rasterizerStateNoCull_NoMS_WF;
}
ID3D11RasterizerState* RasterizerState::getCustom() 
{
	return this->_data->custom;
}
ID3D11RasterizerState* RasterizerState::getState(FLAGS::STATE_RASTERIZER s) 
{
	
	switch (s)
	{
	case FLAGS::RASTERIZER_BackCullNoMS:
		return rasterizerStateBackCull_NoMS;
		break;
	case FLAGS::RASTERIZER_FrontCullNoMS:
		return rasterizerStateFrontCull_NoMS;
		break;
	case FLAGS::RASTERIZER_NoCullNoMs:
		return rasterizerStateNoCull_NoMs;
		break;
	case FLAGS::RASTERIZER_NoCullNoMSWF:
		return rasterizerStateNoCull_NoMS_WF;
		break;
	case FLAGS::RASTERIZER_Custom:
		return this->_data->custom;
		break;
	}
	return NULL;
}

