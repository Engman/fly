#ifndef OBJECT_MATERIAL_H
#define OBJECT_MATERIAL_H


#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\misc.h"
#include "..\..\Util\GID.h"
#include "..\Texture2D.h"
#include "..\BaseBuffer.h"



struct ObjectMaterialProxy
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	int specularPower;
	vec3 pad;
	ObjectMaterialProxy()
		:ambient(0.0, 0.0, 0.0, 0.0), diffuse(0.0, 0.0, 0.0, 0.0), 
		specular(0.0, 0.0, 0.0, 0.0), specularPower(0)
	{}
};

class ObjectMaterial
{
	public:
		struct OBJECT_MATERIAL_DESC
		{
			ID3D11Device		*device;
			ID3D11DeviceContext *dc;
			std::wstring		name;
			vec4				ambient;
			vec4				diffuse;
			vec4				specular;
			int					specualarPow;
			std::wstring		ambientTexture;
			std::wstring		diffuseTexture;
			std::wstring		specularTexture;
			std::wstring		glowTexture;
			std::wstring		normalTexture;
		};

	private:
		std::wstring			_name;
		GID						_id;

		SmartPtrStd<BaseBuffer> _buffer;

		SmartPtrStd<Texture2D> _ambientTex;
		SmartPtrStd<Texture2D> _diffuseTex;
		SmartPtrStd<Texture2D> _specularTex;
		SmartPtrStd<Texture2D> _glowTex;
		SmartPtrStd<Texture2D> _normalTex;

	public:
		ObjectMaterial();
		virtual~ObjectMaterial();

		/** Creates a material from given description */
		bool CreateMaterial(OBJECT_MATERIAL_DESC& desc);


		std::wstring GetName() const;
		int GetID() const;

		ID3D11ShaderResourceView*	GetAmbientTexture	();
		ID3D11ShaderResourceView*	GetDiffuseTexture	();
		ID3D11ShaderResourceView*	GetSpecularTexture	();
		ID3D11ShaderResourceView*	GetGlowTexture		();
		ID3D11ShaderResourceView*	GetNormalTexture	();
		BaseBuffer*					GetBuffer			();
};


#endif

