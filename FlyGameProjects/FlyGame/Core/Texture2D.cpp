#include"Texture2D.h"
#include <algorithm>
#include <string>
#include <vector>
#include "..\Util\GID.h"
#include "..\Util\SmartPtrs.h"


struct User
{
	SmartPtrCom<ID3D11ShaderResourceView> texture;
	std::wstring filename;
	GID id;
	
	User()
		:filename(L"")
	{}
	User(const User& obj)
	{
		this->filename = obj.filename;
		this->texture = obj.texture;
	}

};

struct Texture2D::_Data
{
	static std::vector<SmartPtrStd<User>> resources;	//List of all loaded textures
	std::vector<ID3D11ShaderResourceView*> texture;

	_Data()
		:texture(0)
	{}
	User* find(std::wstring file)
	{
		for (int i = 0; i < (int)this->resources.size(); i++)
		{
			if(this->resources[i]->filename == file)
				return this->resources[i];
		}
		return NULL;
	}
};

std::vector<SmartPtrStd<User>> Texture2D::_Data::resources = std::vector<SmartPtrStd<User>>();


Texture2D::Texture2D()
{
	this->_data = new _Data();
}
Texture2D::Texture2D(const Texture2D& obj)
{
	this->_data = new _Data();
	this->_data->texture = obj._data->texture;
}
Texture2D& Texture2D::operator=(const Texture2D& obj)
{
	this->_data->texture = obj._data->texture;

	return *this;
}
Texture2D::~Texture2D()
{
	delete this->_data;
}



bool Texture2D::loadTexture(ID3D11Device *device, std::wstring file)
{
	if(file == L"")
		return false;

	User* u = this->_data->find(file);
	
	//Texture resource already exists
	if(u)
	{
		this->_data->texture.push_back(u->texture);
		return true;
	}
	else
	{
		ID3D11Texture2D* tex = NULL;

		if( FAILED( D3DX11CreateTextureFromFile(device, file.c_str(), NULL, NULL, (ID3D11Resource**)&tex, NULL)) )
		{
			return false;
		}
		else
		{
			SmartPtrStd<User> user = new User();
			D3D11_TEXTURE2D_DESC texDesc;
			tex->GetDesc(&texDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory( &desc, sizeof( desc ) );
			desc.Format = texDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = texDesc.MipLevels;
			desc.Texture2D.MostDetailedMip = 0;

			if( FAILED( device->CreateShaderResourceView( tex, &desc, user->texture ) ) )
				return false;

			if(tex)
				tex->Release();

			user->filename = file;
		
			this->_data->resources.push_back(user);
			this->_data->texture.push_back(user->texture);
		}
		

	}
	return true;
}



std::vector<ID3D11ShaderResourceView*>* Texture2D::getTextures()
{
	if(!this->_data->texture.size())
		return NULL;

	return &this->_data->texture;
}
ID3D11ShaderResourceView* Texture2D::getTexture()
{
	if(this->_data->texture.size())
		return this->_data->texture[0];
	return NULL;
}
