#include "DepthStencilState.h"
using namespace ShaderStates;

namespace
{
	static D3D11_DEPTH_STENCIL_DESC depthStencilStateDisabledDepthDesc()
	{
			D3D11_DEPTH_STENCIL_DESC ds;
			ZeroMemory(&ds, sizeof(D3D11_DEPTH_STENCIL_DESC));

			ds.DepthEnable							= false;
			ds.DepthFunc							= D3D11_COMPARISON_LESS_EQUAL;
			ds.DepthWriteMask						= D3D11_DEPTH_WRITE_MASK_ZERO;
			ds.StencilEnable						= false;
			ds.StencilReadMask						= D3D11_DEFAULT_STENCIL_READ_MASK;
			ds.StencilWriteMask						= D3D11_DEFAULT_STENCIL_WRITE_MASK;
			ds.FrontFace.StencilDepthFailOp			= D3D11_STENCIL_OP_KEEP;
			ds.FrontFace.StencilFailOp				= D3D11_STENCIL_OP_KEEP;
			ds.FrontFace.StencilPassOp				= D3D11_STENCIL_OP_REPLACE;
			ds.FrontFace.StencilFunc				= D3D11_COMPARISON_ALWAYS;
			ds.BackFace								= ds.FrontFace;

			return ds;
	}
	static D3D11_DEPTH_STENCIL_DESC depthStencilStateEnabledDepthDesc()
	{
			D3D11_DEPTH_STENCIL_DESC ds;
			ZeroMemory(&ds, sizeof(D3D11_DEPTH_STENCIL_DESC));

			ds.DepthEnable                          = true;
			ds.DepthFunc                            = D3D11_COMPARISON_LESS_EQUAL;
			ds.DepthWriteMask                       = D3D11_DEPTH_WRITE_MASK_ALL;
			ds.StencilEnable                        = false;
			ds.StencilReadMask                      = D3D11_DEFAULT_STENCIL_READ_MASK;
			ds.StencilWriteMask						= D3D11_DEFAULT_STENCIL_WRITE_MASK;
			ds.FrontFace.StencilDepthFailOp			= D3D11_STENCIL_OP_KEEP;
			ds.FrontFace.StencilFailOp				= D3D11_STENCIL_OP_KEEP;
			ds.FrontFace.StencilPassOp				= D3D11_STENCIL_OP_REPLACE;
			ds.FrontFace.StencilFunc				= D3D11_COMPARISON_ALWAYS;
			ds.BackFace                             = ds.FrontFace;

			return ds;
	}

	static ID3D11DepthStencilState* depthStencilStateDisabledDepth = NULL;
	static ID3D11DepthStencilState* depthStencilStateEnabledDepth = NULL;
	static int DepthStencilStateUseCount = 0;
}
struct DepthStencilState::_Data
{
	ID3D11DepthStencilState* custom;

	_Data()
		:custom(0)
	{ DepthStencilStateUseCount++; }
	~_Data()
	{
		if(DepthStencilStateUseCount == 1)	//Relase if last
		{
			if (depthStencilStateDisabledDepth)
					depthStencilStateDisabledDepth->Release();

			if (depthStencilStateEnabledDepth)
					depthStencilStateEnabledDepth->Release();
		}
		DepthStencilStateUseCount--;

		if(this->custom)
			this->custom->Release();
	}
};


DepthStencilState::DepthStencilState()
{
	this->_data = new _Data();
}
DepthStencilState::~DepthStencilState()
{
	delete this->_data;
}


bool DepthStencilState::init(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc)
{
	if(desc)
		if(!this->_data->custom)
			if( FAILED( device->CreateDepthStencilState(desc, &this->_data->custom) ) )
				return false;

	if( !depthStencilStateDisabledDepth && FAILED( device->CreateDepthStencilState(&depthStencilStateDisabledDepthDesc(), &depthStencilStateDisabledDepth) ) )
		return false;

	if( !depthStencilStateEnabledDepth && FAILED( device->CreateDepthStencilState(&depthStencilStateEnabledDepthDesc(), &depthStencilStateEnabledDepth) ) )
		return false;

	return true;
}


ID3D11DepthStencilState* DepthStencilState::getDisabledDepth() 
{
	return depthStencilStateDisabledDepth;
}
ID3D11DepthStencilState* DepthStencilState::getEnabledDepth() 
{
	return depthStencilStateEnabledDepth;
}
ID3D11DepthStencilState* DepthStencilState::getCustom() 
{
	return this->_data->custom;
}
ID3D11DepthStencilState* DepthStencilState::getState(FLAGS::STATE_DEPTH_STENCIL s) 
{
	
	switch (s)
	{
	case FLAGS::DEPTH_STENCIL_DisabledDepth:
		return depthStencilStateDisabledDepth;
		break;
	case FLAGS::DEPTH_STENCIL_EnabledDepth:
		return depthStencilStateEnabledDepth;
		break;
	case FLAGS::DEPTH_STENCIL_Custom:
		return this->_data->custom;
		break;
	}

	return NULL;
}
