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

	//this->ambientTex->loadTexture(

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