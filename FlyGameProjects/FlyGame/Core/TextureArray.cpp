#include "TextureArray.h"

TextureArray::TextureArray()
{

}
TextureArray::~TextureArray()
{
	/*for(int i=0; i<nrOfTextures; i++)
	{
		DELETE_COM(m_textArray[i]);
	}
	DELETE_ARRAY(m_textArray);*/
}

bool TextureArray::Initilize(std::vector<WCHAR*>& textNames, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{	
	this->nrOfTextures = (int)textNames.size();
	m_textArray = new ID3D11ShaderResourceView* [this->nrOfTextures];

	D3DX11_IMAGE_LOAD_INFO info;
	HRESULT ret;
	for(int i=0; i<this->nrOfTextures; i++)
	{	
		D3DX11CreateShaderResourceViewFromFile(device, textNames[i] , &info, 0, &m_textArray[i], &ret);
		if ( FAILED( ret ) )
		{
			MessageBox(0, L"Could not load texture.", L"Error", MB_OK);
		}
	}


	return true;
}

int TextureArray::getNrOfTextures()
{
	return this->nrOfTextures;
}
ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return this->m_textArray;
}