#include "ObjectMaterial.h"

ObjectMaterial::ObjectMaterial()
{
	this->_name					= L"";
	this->_buffer				= new BaseBuffer();
	this->_ambientTex			= new Texture2D();
	this->_diffuseTex			= new Texture2D();
	this->_specularTex			= new Texture2D();
	this->_glowTex				= new Texture2D();
	this->_normalTex			= new Texture2D();
}
ObjectMaterial::~ObjectMaterial()
{}

bool ObjectMaterial::CreateMaterial(OBJECT_MATERIAL_DESC& desc)
{
	this->_name					= desc.name;
	this->_data.ambient			= desc.ambient;
	this->_data.diffuse			= desc.diffuse;
	this->_data.specular		= desc.specular;
	this->_data.specularPower	= desc.specualarPow;

	if(desc.ambientTexture.size())
		if(!this->_ambientTex->loadTexture(desc.device, desc.ambientTexture))
			return false;
	if(desc.diffuseTexture.size())
		if(!this->_diffuseTex->loadTexture(desc.device, desc.diffuseTexture))
			return false;
	if(desc.specularTexture.size())
		if(!this->_specularTex->loadTexture(desc.device, desc.specularTexture))
			return false;
	if(desc.normalTexture.size())
		if(!this->_normalTex->loadTexture(desc.device, desc.normalTexture))
			return false;
	if(desc.glowTexture.size())
		if(!this->_glowTex->loadTexture(desc.device, desc.glowTexture))
			return false;


	BaseBuffer::BUFFER_INIT_DESC bDesc;
	bDesc.device = desc.device;
	bDesc.elementSize = sizeof(ObjectMaterialProxy);
	bDesc.nrOfElements = 1;
	bDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	bDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;
	bDesc.data = &this->_data;
	if( FAILED( this->_buffer->Initialize(bDesc) ) )
		return false;

	return true;
}
ObjectMaterialProxy* ObjectMaterial::GetProxy()
{
	return &this->_data;
}
std::wstring ObjectMaterial::GetName() const
{
	return this->_name;
}
int ObjectMaterial::GetID() const
{
	return this->_id.get();
}

ID3D11ShaderResourceView*	ObjectMaterial::GetAmbientTexture	()
{
	if(this->_ambientTex->getTextures()->size())
		return (*this->_ambientTex->getTextures())[0];
	return NULL;
}
ID3D11ShaderResourceView*	ObjectMaterial::GetDiffuseTexture	()
{
	if(this->_diffuseTex->getTextures()->size())
		return (*this->_diffuseTex->getTextures())[0];
	return NULL;
}
ID3D11ShaderResourceView*	ObjectMaterial::GetSpecularTexture	()
{
	if(this->_specularTex->getTextures()->size())
		return (*this->_specularTex->getTextures())[0];
	return NULL;
}
ID3D11ShaderResourceView*	ObjectMaterial::GetGlowTexture		()
{
	if(this->_glowTex->getTextures()->size())
		return (*this->_glowTex->getTextures())[0];
	return NULL;
}
ID3D11ShaderResourceView*	ObjectMaterial::GetNormalTexture	()
{
	if(this->_normalTex->getTextures()->size())
		return (*this->_normalTex->getTextures())[0];
	return NULL;
}
BaseBuffer*					ObjectMaterial::GetBuffer			()
{
	return this->_buffer;
}