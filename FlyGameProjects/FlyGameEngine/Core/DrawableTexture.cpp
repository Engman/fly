#include"DrawableTexture.h"
#include <algorithm>
#include <string>
#include <vector>
#include "..\Util\GID.h"
#include "..\Util\SmartPtrs.h"





DrawableTexture::DrawableTexture()
	: mWidth(0), mHeight(0), 
	 mColorMapSRV(0), mColorMapRTV(0), mDepthMapSRV(0), mDepthMapDSV(0)
{

}
DrawableTexture::DrawableTexture(const DrawableTexture& obj)
{
	this->mColorMapSRV = obj.mColorMapSRV;
	this->mColorMapRTV = obj.mColorMapRTV;

	this->mDepthMapSRV = obj.mDepthMapSRV;
	this->mDepthMapDSV = obj.mDepthMapDSV;
}
DrawableTexture& DrawableTexture::operator=(const DrawableTexture& obj)
{
	this->mColorMapSRV = obj.mColorMapSRV;
	this->mColorMapRTV = obj.mColorMapRTV;

	this->mDepthMapSRV = obj.mDepthMapSRV;
	this->mDepthMapDSV = obj.mDepthMapDSV;
	return *this;
}
DrawableTexture::~DrawableTexture()
{
	if(this->mColorMapSRV)
		mColorMapSRV->Release();
	this->mColorMapSRV = NULL;

	if(this->mColorMapRTV)
		mColorMapRTV->Release();
	this->mColorMapRTV = NULL;

	if(this->mDepthMapSRV)
		mDepthMapSRV->Release();
	this->mDepthMapSRV = NULL;

	if(this->mDepthMapDSV)
		mDepthMapDSV->Release();
	this->mDepthMapDSV = NULL;
}

bool DrawableTexture::init( UINT width, UINT height, bool hasColorMap,
	DXGI_FORMAT colorFormat, bool hasDepthMap)
{
	mWidth  = width;
	mHeight = height;

	if(hasDepthMap)
		buildDSV();

	if( hasColorMap )
		buildSRV(colorFormat);
	return true;
}
ID3D11ShaderResourceView* DrawableTexture::getColorSRV()
{
	return mColorMapSRV;
}

ID3D11ShaderResourceView* DrawableTexture::getDepthSRV()
{
	return mDepthMapSRV;
}
ID3D11RenderTargetView* DrawableTexture::getRenderTargetView()
{
	return mColorMapRTV;
}

ID3D11DepthStencilView* DrawableTexture::getDepthStencilView()
{
	return mDepthMapDSV;
}
bool DrawableTexture::buildDSV()
{
	ID3D11Texture2D* depthMap = 0;
	HRESULT result;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory( &texDesc, sizeof(texDesc) );
	texDesc.Width     = mWidth;
	texDesc.Height    = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count   = 1;  
	texDesc.SampleDesc.Quality = 0;  
	texDesc.Usage          = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0; 
	texDesc.MiscFlags      = 0;

	result = D3DShell::self()->getDevice()->CreateTexture2D(&texDesc, 0, &depthMap);
	if(FAILED(result))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory( &dsvDesc, sizeof(dsvDesc) );
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	result = D3DShell::self()->getDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthMapDSV);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	result = D3DShell::self()->getDevice()->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV);
	if(FAILED(result))
	{
		return false;
	}
	// View saves a reference to the texture so we can release our reference.
	depthMap->Release();
	depthMap= NULL;
	return true; 
}

bool DrawableTexture::buildSRV(DXGI_FORMAT colorFormat)
{
	ID3D11Texture2D* colorMap = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	HRESULT result;

	// Initialize the render target texture description.
	ZeroMemory(&texDesc, sizeof(texDesc));

	// Setup the render target texture description.
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = colorFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the render target texture.
	result = D3DShell::self()->getDevice()->CreateTexture2D(&texDesc, NULL, &colorMap);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = texDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = D3DShell::self()->getDevice()->CreateRenderTargetView(colorMap, &renderTargetViewDesc, &mColorMapRTV);
	if(FAILED(result))
	{
		return false;
	}

	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = texDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = D3DShell::self()->getDevice()->CreateShaderResourceView(colorMap, &shaderResourceViewDesc, &mColorMapSRV);
	if(FAILED(result))
	{
		return false;
	}
	colorMap->Release();
	colorMap = NULL;
	return true;
}