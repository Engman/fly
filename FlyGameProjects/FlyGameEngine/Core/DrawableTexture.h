#ifndef DRAWABLETEXTURE_H
#define DRAWABLETEXTURE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <vector>
#include "..\Util\SmartPtrs.h"
#include "D3DShell.h"


class DrawableTexture
{
	private:
		ID3D11ShaderResourceView*	mColorMapSRV;
		ID3D11RenderTargetView*		mColorMapRTV;

		ID3D11ShaderResourceView*	mDepthMapSRV;
		ID3D11DepthStencilView*		mDepthMapDSV;

		UINT mWidth;
		UINT mHeight;

		bool buildDSV();
		bool buildSRV(DXGI_FORMAT colorFormat);

	public:
		DrawableTexture();
		DrawableTexture(const DrawableTexture& obj);
		DrawableTexture& operator=(const DrawableTexture& obj);
		virtual~DrawableTexture();

		bool init( UINT width, UINT height, bool hasColorMap,
			DXGI_FORMAT colorFormat, bool hasDepthMap);

		ID3D11ShaderResourceView* getColorSRV();

		ID3D11ShaderResourceView* getDepthSRV();

		ID3D11RenderTargetView* getRenderTargetView();

		ID3D11DepthStencilView* getDepthStencilView();
		
};

#endif