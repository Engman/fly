#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <vector>


class Texture2D
{
	private:
		struct _Data;
		_Data* _data;

	public:
		Texture2D();
		Texture2D(const Texture2D& obj);
		Texture2D& operator=(const Texture2D& obj);
		virtual~Texture2D();

		/** Returns a vector with the collection of shader resource views
		*	Returns NULL if no textures*/
		std::vector<ID3D11ShaderResourceView*>* getTextures();

		/** Returns the first texture in collection
		*	Returns NULL if no texture*/
		ID3D11ShaderResourceView* getTexture();
		
		/** Loads a texture from file as shader resource view */
		bool loadTexture(ID3D11Device *device, std::wstring file);
};

#endif