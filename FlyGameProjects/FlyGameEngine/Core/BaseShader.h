#ifndef BASESHADER_H
#define BASESHADER_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX10math.h>
#include <D3DX11\D3DX11async.h>

#include <fstream>
#include <vector>
#include <string>

using namespace std;


/** 
*	The BaseShader class handles various shaders (VS, PS, GS, HS, DS)
*	You can use this class to easily switch between various shaders.
*/
class BaseShader
{
	public:
		/** The filenames in this struct work like flags where NULL means it will not be included  */
		struct BASE_SHADER_DESC
		{
			WCHAR						*VSFilename;
			WCHAR						*PSFilename;
			WCHAR						*GSFilename;
			WCHAR						*HSFilename;
			WCHAR						*DSFilename;
			ID3D11Device				*device;
			ID3D11DeviceContext			*dc;
			D3D11_INPUT_ELEMENT_DESC	*polygonLayout;
			int							nrOfElements;
			string						shaderVersion;

			BASE_SHADER_DESC() 
				:VSFilename(0), PSFilename(0), GSFilename(0), HSFilename(0), DSFilename(0), device(0), dc(0), polygonLayout(0), nrOfElements(0), shaderVersion("")
			{ }
		};


	private:
		/** struct defined elsewhere that contains private data */
		struct _Data;
		_Data* _data;


	public:
		BaseShader();
		virtual ~BaseShader();

		/** For the shaders, function point of entry must be FVertexShader, FPixexlShader, FGeometryShader, FHullShader, FDomainShader.
			polygonLayout must match the layout found in the vertex shader.	
			shaderVersion is a string with the layout "x_y" where x and y are whole numbers.
		*/
		HRESULT Initialize(BASE_SHADER_DESC& shaderDescription);

		/** Releases com objects and destroys initialized pointers */
		void Release();

		/** Render relies on buffer paramters already set */
		void Render();

		ID3D11VertexShader*		GetVertexShader();
		ID3D11PixelShader*		GetPixelShader();
		ID3D11GeometryShader*	GetGeometryShader();
		ID3D11HullShader*		GetHullShader();
		ID3D11DomainShader*		GetDomainShader();  
		ID3D11InputLayout*		GetInputLayout();
		ID3D11DeviceContext*	GetDeviceContext();

		void SetVShader(ID3D11VertexShader* shader);
		void SetPShader(ID3D11PixelShader* shader);
		void SetGShader(ID3D11GeometryShader* shader);
		void SetHShader(ID3D11HullShader* shader);
		void SetDShader(ID3D11DomainShader* shader);
		void SetInputLayout(ID3D11InputLayout* layout);
		
		void Restore();
		void RestoreVS();
		void RestorePS();
		void RestoreGS();
		void RestoreHS();
		void RestoreDS();
		void RestoreInputLayout();

		int getID() const;
};

#endif