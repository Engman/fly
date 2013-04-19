#include"Texture2D.h"
#include <algorithm>
#include <string>
#include <vector>
#include "..\Util\GID.h"


struct TexUsers
{
	ID3D11ShaderResourceView* texture;
	std::vector<WCHAR*> filename;
	int userCount;
	int textureCount;
	GID id;

	TexUsers()
		:texture(0), userCount(0), textureCount(0)
	{}
	TexUsers(WCHAR* file)
		:texture(0), userCount(0), textureCount(0)
	{ this->filename.push_back(file); }
	TexUsers(const TexUsers& obj)
	{
		this->userCount = obj.userCount;
		this->filename = obj.filename;
		this->texture = obj.texture;
	}
	TexUsers& operator=(const TexUsers& obj)
	{
		this->userCount = obj.userCount;
		this->filename = obj.filename;
		this->texture = obj.texture;

		return *this;
	}
};

struct Texture2D::_Data
{
	static std::vector<TexUsers*> resources;	//List of all loaded textures
	TexUsers* texture;	//Pointer to the actual texture
	_Data()
		:texture(0)
	{}
	TexUsers* find(WCHAR* file)
	{
		for (int i = 0; i < (int)this->resources.size(); i++)
		{
			for (int k = 0; k < (int)this->resources[i]->filename.size(); k++)
			{
				if(this->resources[i]->filename[k] == file)
					return this->resources[i];
			}
		}
		return NULL;
	}
	void releaseTexture();
};

std::vector<TexUsers*> Texture2D::_Data::resources = std::vector<TexUsers*>();


Texture2D::Texture2D()
{
	this->_data = new _Data();
}

Texture2D::Texture2D(const Texture2D& obj)
{
	this->_data = new _Data();
	this->_data->texture = obj._data->texture;
	this->_data->texture->userCount++;
}

Texture2D& Texture2D::operator=(const Texture2D& obj)
{
	this->_data->texture = obj._data->texture;

	if(this->_data->texture)
		this->_data->texture->userCount++;

	return *this;
}

Texture2D::~Texture2D()
{
	this->_data->releaseTexture();

	delete this->_data;
}

ID3D11ShaderResourceView* Texture2D::getSRV()
{
	if(!this->_data->texture)
		return NULL;

	return this->_data->texture->texture;
}


bool Texture2D::loadTexture(ID3D11Device *device, WCHAR* file)
{
	//wchar_t *path=NULL;
	//size_t size;
	//path = _wgetcwd(path, OFS_MAXPATHNAME);

	if(file == L"")
		return false;

	TexUsers* newTex = this->_data->find(file);
	
	//Texture resource already exists
	if(newTex)
	{
		this->_data->texture = newTex;
		newTex->userCount++;

		return true;
	}
	else	//Load the new texture resource
	{
		newTex = new TexUsers();
	
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.MiscFlags = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		ID3D11Texture2D* tex = NULL;

		if( FAILED( D3DX11CreateTextureFromFile(device, file, NULL, NULL, (ID3D11Resource**)&tex, NULL)) )
		{
			return false;
		}
		else
		{
			D3D11_TEXTURE2D_DESC texDesc;
			tex->GetDesc(&texDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory( &desc, sizeof( desc ) );
			desc.Format = texDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = texDesc.MipLevels;
			desc.Texture2D.MostDetailedMip = 0;

			if(FAILED(device->CreateShaderResourceView( tex, &desc, &newTex->texture )))
				return false;

			if(tex)
				tex->Release();
		}


		newTex->userCount++;
		newTex->filename.push_back(file);
		
		this->_data->resources.push_back(newTex);
		this->_data->texture = newTex;
		

	}
	return true;
}
bool Texture2D::loadTexture(ID3D11Device *device, ID3D11DeviceContext* dc, std::vector<WCHAR*>& files)
{
	//wchar_t *path=NULL;
	//size_t size;
	//path = _wgetcwd(path, OFS_MAXPATHNAME);

	if(files.size() == 0)
		return false;
	
	
	TexUsers* newTex = new TexUsers();
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
	std::vector<ID3D11Texture2D*> tex(files.size());
	int loadedTex = 0;


	//Load all textures
	for(int i = 0; i<(int)tex.size(); i++)
	{
		if( FAILED( D3DX11CreateTextureFromFile( device, files[i], NULL, NULL, (ID3D11Resource**)&tex[i], NULL ) ) )
		{
			for (int k = 0; i < loadedTex; k++)
				tex[k]->Release();

			return false;
		}
		loadedTex++;
	}
	

	//Create texture array
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_TEXTURE2D_DESC texArrayDesc;
	ID3D11Texture2D* textureArray;
	
	tex[0]->GetDesc(&texDesc);
	texArrayDesc.ArraySize			= (UINT)tex.size();
	texArrayDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;
	texArrayDesc.Format				= texDesc.Format;
	texArrayDesc.Height				= texDesc.Height;
	texArrayDesc.Width				= texDesc.Width;
	texArrayDesc.MipLevels			= texDesc.MipLevels;
	texArrayDesc.MiscFlags			= 0;
	texArrayDesc.Usage				= D3D11_USAGE_DEFAULT;
	texArrayDesc.SampleDesc.Quality	= 0;
	texArrayDesc.SampleDesc.Count	= 1;

	if( FAILED( device->CreateTexture2D(&texArrayDesc, NULL, &textureArray) ) )
	{
		for (int i = 0; i < loadedTex; i++)
			tex[i]->Release();
		return false;
	}

	//Copy textures to array
	for (int i = 0; i < (int)tex.size(); i++)
	{
		for (int k = 0; k < (int)texDesc.MipLevels; k++)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			if( FAILED( dc->Map(tex[i], (UINT)k, D3D11_MAP_READ, 0, &resource) ) )
			{
				for (int i = 0; i < loadedTex; i++)
					tex[i]->Release();
				return false;
			}

			dc->UpdateSubresource	(textureArray, D3D11CalcSubresource((UINT)k, (UINT)i, texDesc.MipLevels),
									0, resource.pData, resource.RowPitch, resource.DepthPitch);

			dc->Unmap(tex[i], (UINT)k);
		}
	}

	//Create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Format							= texDesc.Format;
	desc.ViewDimension					= D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2DArray.MipLevels		= texDesc.MipLevels;
	desc.Texture2DArray.MostDetailedMip = 0;
	desc.Texture2DArray.FirstArraySlice = 0;
	desc.Texture2DArray.ArraySize		= (UINT)tex.size();
	

	if(FAILED(device->CreateShaderResourceView( textureArray, &desc, &newTex->texture ) ) )
		return false;

	if(	textureArray )
		textureArray->Release();
	
	for (int i = 0; i < (int)tex.size(); i++)
		tex[i]->Release();


	newTex->userCount++;
	this->_data->resources.push_back(newTex);
	this->_data->texture = newTex;

	return true;
}

void Texture2D::_Data::releaseTexture()
{
	//Release texture if im only owner
	if(this->texture)
		if(this->texture->userCount == 1)
		{
			this->texture->texture->Release();

			std::vector<TexUsers*>::iterator i = std::find(this->resources.begin(), this->resources.end(), this->texture);
			if(i != this->resources.end())
			{
				int k = (int)(i - this->resources.begin());
				delete this->resources[k];
				this->resources.erase(i);
			}
			this->texture = NULL;
		}
		else
		{
			this->texture->userCount--;
			this->texture = NULL;
		}
}

int Texture2D::getID() const
{
	if(this->_data->texture)
		return this->_data->texture->id.get();
	return -1;
}