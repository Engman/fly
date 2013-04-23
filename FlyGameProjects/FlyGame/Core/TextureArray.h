#ifndef TEXTUREARRAY_H
#define TEXTUREARRAY_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <vector>
#include "..\Util\misc.h"


class TextureArray
{
private:
	ID3D11ShaderResourceView** m_textArray;
	int nrOfTextures; 
public:
	TextureArray();
	~TextureArray();

	bool Initilize(std::vector<WCHAR*>& textNames, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int getNrOfTextures();
	ID3D11ShaderResourceView** GetTextureArray();

};




#endif