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

		ID3D11ShaderResourceView* getSRV();
		bool loadTexture(ID3D11Device *device, WCHAR* filename);
		bool loadTexture(ID3D11Device *device, ID3D11DeviceContext* dc, std::vector<WCHAR*>& filename);

		int getID() const;
};

#endif