#include "BaseShader.h"
#include "..\Util\GID.h"
#include "..\Util\vertex.h"

/** This struct contains private data to BaseShader */
struct BaseShader::_Data
{
	ID3D11DeviceContext		*dc;
	ID3D11VertexShader		*pVertexShader;
	ID3D11PixelShader		*pPixelShader;
	ID3D11GeometryShader	*pGeometryShader;
	ID3D11HullShader		*pHullShader;
	ID3D11DomainShader		*pDomainShader;
	ID3D11InputLayout		*pLayout;
	GID						id;

	struct DefaultShaderPtr
	{
		ID3D11VertexShader*		pDefaultVertexShader;
		ID3D11PixelShader*		pDefaultPixelShader;
		ID3D11GeometryShader*	pDefaultGeometryShader;
		ID3D11HullShader*		pDefaultHullShader;
		ID3D11DomainShader*		pDefaultDomainShader;
		ID3D11InputLayout*		pDefaultLayout;
		DefaultShaderPtr()
			:pDefaultVertexShader(0), pDefaultPixelShader(0), pDefaultGeometryShader(0), pDefaultHullShader(0), pDefaultDomainShader(0), pDefaultLayout(0)
		{}
	} defaultShaderPtr;

	_Data()
		:dc(0), pVertexShader(0), pPixelShader(0), pGeometryShader(0), pHullShader(0), pDomainShader(0), pLayout(0)
	{}
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
	HRESULT initVShader(BASE_SHADER_DESC& desc);
	HRESULT initPShader(BASE_SHADER_DESC& desc);
	HRESULT initGShader(BASE_SHADER_DESC& desc);
	HRESULT initHShader(BASE_SHADER_DESC& desc);
	HRESULT initDShader(BASE_SHADER_DESC& desc);
};

BaseShader::BaseShader()
{
	this->_data = new _Data();
}

BaseShader::~BaseShader()
{
	Release();
	delete this->_data;
}

void BaseShader::Release()
{
	if(this->_data->pLayout)
	{
		this->_data->pLayout->Release();
		this->_data->pLayout = 0;
	}

	if(this->_data->pPixelShader)
	{
		this->_data->pPixelShader->Release();
		this->_data->pPixelShader = 0;
	}

	if(this->_data->pVertexShader)
	{
		this->_data->pVertexShader->Release();
		this->_data->pVertexShader = 0;
	}

	if(this->_data->pGeometryShader)
	{
		this->_data->pGeometryShader->Release();
		this->_data->pGeometryShader = 0;
	}

	if(this->_data->pHullShader)
	{
		this->_data->pHullShader->Release();
		this->_data->pHullShader = 0;
	}

	if(this->_data->pDomainShader)
	{
		this->_data->pDomainShader->Release();
		this->_data->pDomainShader = 0;
	}
}

HRESULT BaseShader::Initialize(BASE_SHADER_DESC& desc)
{
	this->_data->dc	= desc.dc;
	
	if( this->_data->initVShader(desc) )
		return E_FAIL;
	if( this->_data->initPShader(desc) )
		return E_FAIL;
	if( this->_data->initGShader(desc) )
		return E_FAIL;
	if( this->_data->initHShader(desc) )
		return E_FAIL;
	if( this->_data->initDShader(desc) )
		return E_FAIL;

	return S_OK;
}

void BaseShader::Render()
{
	this->_data->dc->IASetInputLayout(this->_data->pLayout);

	this->_data->dc->VSSetShader(this->_data->pVertexShader, NULL, 0);
	this->_data->dc->PSSetShader(this->_data->pPixelShader, NULL, 0);
	this->_data->dc->GSSetShader(this->_data->pGeometryShader, NULL, 0);
	this->_data->dc->HSSetShader(this->_data->pHullShader, NULL, 0);
	this->_data->dc->DSSetShader(this->_data->pDomainShader, NULL, 0);
}

ID3D11VertexShader*		BaseShader::GetVertexShader()
{
	return this->_data->pVertexShader;
}
ID3D11PixelShader*		BaseShader::GetPixelShader()
{
	return this->_data->pPixelShader;
}
ID3D11GeometryShader*	BaseShader::GetGeometryShader()
{
	return this->_data->pGeometryShader;
}
ID3D11HullShader*		BaseShader::GetHullShader()
{
	return this->_data->pHullShader;
}
ID3D11DomainShader*		BaseShader::GetDomainShader()
{
	return this->_data->pDomainShader;
}
ID3D11InputLayout*		BaseShader::GetInputLayout()
{
	return this->_data->pLayout;
}
ID3D11DeviceContext*	BaseShader::GetDeviceContext()
{
	return this->_data->dc;
}

void BaseShader::SetVShader(ID3D11VertexShader* shader)
{				 
	this->_data->pVertexShader = shader;	 
}				 			 
void BaseShader::SetPShader(ID3D11PixelShader* shader)
{				
	this->_data->pPixelShader = shader;	 
}							
void BaseShader::SetGShader(ID3D11GeometryShader* shader)
{				 
	this->_data->pGeometryShader = shader;	 
}				 			 
void BaseShader::SetHShader(ID3D11HullShader* shader)
{				 
	this->_data->pHullShader = shader;	 
}				 			 
void BaseShader::SetDShader(ID3D11DomainShader* shader)
{
	this->_data->pDomainShader = shader;	 
}
void BaseShader::SetInputLayout(ID3D11InputLayout* layout)
{
	this->_data->pLayout = layout;
}

void BaseShader::Restore()
{
	this->_data->pVertexShader		= this->_data->defaultShaderPtr.pDefaultVertexShader;
	this->_data->pPixelShader		= this->_data->defaultShaderPtr.pDefaultPixelShader;
	this->_data->pGeometryShader	= this->_data->defaultShaderPtr.pDefaultGeometryShader;
	this->_data->pHullShader		= this->_data->defaultShaderPtr.pDefaultHullShader;
	this->_data->pDomainShader		= this->_data->defaultShaderPtr.pDefaultDomainShader;
	this->_data->pLayout			= this->_data->defaultShaderPtr.pDefaultLayout;
}
void BaseShader::RestoreVS()
{
	this->_data->pVertexShader = this->_data->defaultShaderPtr.pDefaultVertexShader;
}
void BaseShader::RestorePS()
{
	this->_data->pPixelShader = this->_data->defaultShaderPtr.pDefaultPixelShader;
}
void BaseShader::RestoreGS()
{
	this->_data->pGeometryShader = this->_data->defaultShaderPtr.pDefaultGeometryShader;
}
void BaseShader::RestoreHS()
{
	this->_data->pHullShader = this->_data->defaultShaderPtr.pDefaultHullShader;
}
void BaseShader::RestoreDS()
{
	this->_data->pDomainShader = this->_data->defaultShaderPtr.pDefaultDomainShader;
}
void BaseShader::RestoreInputLayout()
{
	this->_data->pLayout = this->_data->defaultShaderPtr.pDefaultLayout;
}




/****************************************************
*					PRIVATE FUNCTIONS				*
*****************************************************/

void BaseShader::_Data::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	size_t bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	//MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}
HRESULT BaseShader::_Data::initVShader(BASE_SHADER_DESC& desc)
{
	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* errorMessage;

	wchar_t p[MAX_PATH];
	_wgetcwd(p, MAX_PATH);

	// Compile the vertex shader code.
	if(desc.VSFilename != NULL)
	{
		if( FAILED( D3DX11CompileFromFile(desc.VSFilename, NULL, NULL, "FVertexShader", 
										 ("vs_"+desc.shaderVersion).c_str(), 
										 D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
										 &vertexShaderBuffer, &errorMessage, NULL) ) )
		{
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage, desc.VSFilename);

			return E_FAIL;
		}
	}
	// Create the vertex shader from the buffer.
	if(vertexShaderBuffer && FAILED(desc.device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->pVertexShader)))
		return E_FAIL;


	//Copy pointer to default shaders for easy switching between compatible effects
	this->defaultShaderPtr.pDefaultVertexShader = this->pVertexShader;
	

	if(desc.polygonLayout != NULL)
	{
		// Create the vertex input layout.
		if(FAILED(desc.device->CreateInputLayout(desc.polygonLayout, desc.nrOfElements, vertexShaderBuffer->GetBufferPointer(), 
												 vertexShaderBuffer->GetBufferSize(), &this->pLayout)))
		{
			return E_FAIL;
		}
	}

	// Release the buffers since they are no longer needed.
	if(vertexShaderBuffer)
	{
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;
	}
	return S_OK;
}
HRESULT BaseShader::_Data::initPShader(BASE_SHADER_DESC& desc)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer = 0;


	// Compile the pixel shader code.
	if(desc.PSFilename != NULL)
	{
		if( FAILED( D3DX11CompileFromFile(desc.PSFilename, NULL, NULL, "FPixelShader", 
										 ("ps_"+desc.shaderVersion).c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 
										 0, NULL, &pixelShaderBuffer, &errorMessage, NULL) ) )
		{
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage, desc.PSFilename);
		
			return E_FAIL;
		}
	}
	// Create the pixel shader from the buffer.
	if(pixelShaderBuffer && (desc.device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->pPixelShader)))
		return E_FAIL;


	//Copy pointer to default shaders for easy switching between compatible effects
	this->defaultShaderPtr.pDefaultPixelShader = this->pPixelShader;


	if(pixelShaderBuffer)
	{
		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;
	}
	return S_OK;
}
HRESULT BaseShader::_Data::initGShader(BASE_SHADER_DESC& desc)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* geometryShaderBuffer = 0;
	
	
	// Compile the geometry shader code.
	if(desc.GSFilename != NULL)
	{
		if(FAILED(D3DX11CompileFromFile	(desc.GSFilename, NULL, NULL, "FGeometryShader", 
										("gs_"+desc.shaderVersion).c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 
										0, NULL, &geometryShaderBuffer, &errorMessage, NULL)))
		{
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage, desc.GSFilename);

			return E_FAIL;
		}
	}

	// Create the geometry shader from the buffer.
	if(geometryShaderBuffer && FAILED(desc.device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &this->pGeometryShader)))
		return E_FAIL;

	//Copy pointer to default shaders for easy switching between compatible effects
	this->defaultShaderPtr.pDefaultGeometryShader = this->pGeometryShader;
	

	if(geometryShaderBuffer)
	{
		geometryShaderBuffer->Release();
		geometryShaderBuffer = 0;
	}
	
	return S_OK;
}
HRESULT BaseShader::_Data::initHShader(BASE_SHADER_DESC& desc)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* hullShaderBuffer = 0;


	// Compile the hull shader code.
	if(desc.HSFilename != NULL)
	{
		if(FAILED(D3DX11CompileFromFile	(desc.HSFilename, NULL, NULL, "FHullShader", 
										("hs_"+desc.shaderVersion).c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 
										0, NULL,  &hullShaderBuffer, &errorMessage, NULL)))
		{
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage, desc.HSFilename);

			return E_FAIL;
		}
	}
	// Create the hull shader from the buffer.
	if(hullShaderBuffer && FAILED(desc.device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &this->pHullShader)))
		return E_FAIL;


	//Copy pointer to default shaders for easy switching between compatible effects
	this->defaultShaderPtr.pDefaultHullShader = this->pHullShader;


	if(hullShaderBuffer)
	{
		hullShaderBuffer->Release();
		hullShaderBuffer = 0;
	}

	return S_OK;
}
HRESULT BaseShader::_Data::initDShader(BASE_SHADER_DESC& desc)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* domainShaderBuffer = 0;


	// Compile the domain shader code.
	if(desc.DSFilename != NULL)
	{
		if(FAILED(D3DX11CompileFromFile	(desc.DSFilename, NULL, NULL, "FDomainShader", 
										("ds_"+desc.shaderVersion).c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 
										0, NULL, &domainShaderBuffer, &errorMessage, NULL)))
		{
			if(errorMessage)
				OutputShaderErrorMessage(errorMessage, desc.DSFilename);

			return E_FAIL;
		}
	}
	// Create the domain shader from the buffer.
	if(domainShaderBuffer && (desc.device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &this->pDomainShader)))
		return E_FAIL;


	//Copy pointer to default shaders for easy switching between compatible effects
	this->defaultShaderPtr.pDefaultDomainShader = this->pDomainShader;


	if(domainShaderBuffer)
	{
		domainShaderBuffer->Release();
		domainShaderBuffer = 0;
	}

	return S_OK;
}

int BaseShader::getID() const
{
	return this->_data->id.get();
}