#include "SamplerState.h"
using namespace ShaderStates;

namespace
{
	static D3D11_SAMPLER_DESC samplerStatePointDesc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter           = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sd.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 1;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD           = 0.0f;
		sd.MaxLOD           = D3D11_FLOAT32_MAX;
		
		return sd;
	}
	static D3D11_SAMPLER_DESC samplerStateLinearDesc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 1;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD			= 0.0f;
		sd.MaxLOD			= D3D11_FLOAT32_MAX;
		
		return sd;
	}
	static D3D11_SAMPLER_DESC samplerStateAnisotropic2Desc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter			= D3D11_FILTER_ANISOTROPIC;
		sd.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 2;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD			= 0.0f;
		sd.MaxLOD			= D3D11_FLOAT32_MAX;
	
		return sd;
	}
	static D3D11_SAMPLER_DESC samplerStateAnisotropic4Desc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter           = D3D11_FILTER_ANISOTROPIC;
		sd.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 4;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD           = 0.0f;
		sd.MaxLOD           = D3D11_FLOAT32_MAX;
	
		return sd;
	}
	static D3D11_SAMPLER_DESC samplerStateAnisotropic8Desc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter			= D3D11_FILTER_ANISOTROPIC;
		sd.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 8;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD			= 0.0f;
		sd.MaxLOD			= D3D11_FLOAT32_MAX;

		return sd;
	}
	static D3D11_SAMPLER_DESC samplerStateAnisotropic16Desc()
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter			= D3D11_FILTER_ANISOTROPIC;
		sd.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 16;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD			= 0.0f;
		sd.MaxLOD			= D3D11_FLOAT32_MAX;

		return sd;
	}

	static D3D11_SAMPLER_DESC samplerStateLinearClampDesc()
	{

		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.MipLODBias		= 0.0f;
		sd.MaxAnisotropy	= 16;
		sd.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0]	= 0.0f;
		sd.BorderColor[1]	= 0.0f;
		sd.BorderColor[2]	= 0.0f;
		sd.BorderColor[3]	= 0.0f;
		sd.MinLOD			= 0.0f;
		sd.MaxLOD			= D3D11_FLOAT32_MAX;
			
		return sd;
	}


    static ID3D11SamplerState* samplerStatePoint;
    static ID3D11SamplerState* samplerStateLinear;
    static ID3D11SamplerState* samplerStateAnisotropic2;
    static ID3D11SamplerState* samplerStateAnisotropic4;
    static ID3D11SamplerState* samplerStateAnisotropic8;
    static ID3D11SamplerState* samplerStateAnisotropic16;
	static ID3D11SamplerState* samplerStateLinearClamp;
	static int SamplerStateUseCount = 0;
}
struct SamplerState::_Data
{
	ID3D11SamplerState* custom;

	_Data()
		:custom(0)
	{ SamplerStateUseCount++; }
	~_Data()
	{
		if(SamplerStateUseCount == 1)	//Release if last
		{
			if (samplerStatePoint)
				samplerStatePoint->Release();
	
			if (samplerStateLinear)
				samplerStateLinear->Release();
			
			if (samplerStateAnisotropic2)
				samplerStateAnisotropic2->Release();
			
			if (samplerStateAnisotropic4)
				samplerStateAnisotropic4->Release();
			
			if (samplerStateAnisotropic8)
				samplerStateAnisotropic8->Release();
			
			if (samplerStateAnisotropic16)
				samplerStateAnisotropic16->Release();
			if(samplerStateLinearClamp)
				samplerStateLinearClamp->Release();
		}
		SamplerStateUseCount--;

		if(this->custom)
			this->custom->Release();
	}
};


SamplerState::SamplerState()
{
	this->_data = new _Data();
}
SamplerState::~SamplerState()
{
	delete this->_data;
}


bool SamplerState::init(ID3D11Device* device, D3D11_SAMPLER_DESC* desc)
{
	if(desc)
		if(!this->_data->custom)
			if( FAILED( device->CreateSamplerState(desc, &this->_data->custom) ) )
				return false;

    if( !samplerStatePoint && FAILED( device->CreateSamplerState(&samplerStatePointDesc(), &samplerStatePoint) ) )
		return false;
    if( !samplerStateLinear && FAILED( device->CreateSamplerState(&samplerStateLinearDesc(), &samplerStateLinear) ) )
		return false;
    if( !samplerStateAnisotropic2 && FAILED( device->CreateSamplerState(&samplerStateAnisotropic2Desc(), &samplerStateAnisotropic2) ) )
		return false;
    if( !samplerStateAnisotropic4 && FAILED( device->CreateSamplerState(&samplerStateAnisotropic4Desc(), &samplerStateAnisotropic4) ) )
		return false;
    if( !samplerStateAnisotropic8 && FAILED( device->CreateSamplerState(&samplerStateAnisotropic8Desc(), &samplerStateAnisotropic8) ) )
		return false;
    if( !samplerStateAnisotropic16 && FAILED( device->CreateSamplerState(&samplerStateAnisotropic16Desc(), &samplerStateAnisotropic16) ) )
		return false;
	if( !samplerStateLinearClamp && FAILED( device->CreateSamplerState(&samplerStateLinearClampDesc(), &samplerStateLinearClamp) ) )
		return false;

        return true;
}


ID3D11SamplerState* SamplerState::getPoint() 
{
	return samplerStatePoint;
}
ID3D11SamplerState* SamplerState::getLinear() 
{
	return samplerStateLinear;
}
ID3D11SamplerState* SamplerState::getAnisotropic2() 
{
	return samplerStateAnisotropic2;
}
ID3D11SamplerState* SamplerState::getAnisotropic4() 
{
	return samplerStateAnisotropic4;
}
ID3D11SamplerState* SamplerState::getAnisotropic8() 
{
	return samplerStateAnisotropic8;
}
ID3D11SamplerState* SamplerState::getAnisotropic16() 
{
	return samplerStateAnisotropic16;
}
ID3D11SamplerState* SamplerState::getLinearClamp() 
{
	return samplerStateLinearClamp;
}
ID3D11SamplerState* SamplerState::getCustom() 
{
	return this->_data->custom;
}
ID3D11SamplerState* SamplerState::getState(FLAGS::STATE_SAMPLING s) 
{
	switch (s)
	{
	case FLAGS::SAMPLER_Point:
		return samplerStatePoint;
		break;
	case FLAGS::SAMPLER_Linear:
		return samplerStateLinear;
		break;
	case FLAGS::SAMPLER_Anisotropic2:
		return samplerStateAnisotropic2;
		break;
	case FLAGS::SAMPLER_Anisotropic4:
		return samplerStateAnisotropic4;
		break;
	case FLAGS::SAMPLER_Anisotropic8:
		return samplerStateAnisotropic8;
		break;
	case FLAGS::SAMPLER_Anisotropic16:
		return samplerStateAnisotropic16;
		break;
	case FLAGS::SAMPLER_LinearClamp:
		return samplerStateLinearClamp;
		break;
	case FLAGS::SAMPLER_Custom:
		return this->_data->custom;
		break;
	}

	return NULL;
}

