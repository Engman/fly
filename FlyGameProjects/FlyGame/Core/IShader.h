#ifndef I_SHADER_H
#define I_SHADER_H


#include "BaseBuffer.h"
#include "BaseShader.h"
#include "Texture2D.h"
#include "..\Util\GID.h"
#include "..\Util\Proxy.h"
#include "..\Util\SmartPtrs.h"
#include "TextureArray.h"

/** Inherit to create shaders */
class IShader
{
	public:
		struct SHADER_PARAMETER_DATA
		{
			ID3D11DeviceContext* dc;
			BaseBuffer* cMatrixBuffer;
			BaseBuffer* lights;
		};
		/** Used to set data for a draw call */
		struct DRAW_DATA
		{
			D3DXMATRIX* worldMatrix;
			std::vector<BaseBuffer*> buffers;
			//TextureArray* textures;
			std::vector<Texture2D>* textures; 
			DRAW_DATA()
				:worldMatrix(0)
			{}
		};

	private:
		GID id;

	protected:
		SmartPtrStd<BaseShader> shader;
		std::vector<DRAW_DATA> drawData;

	public:
		IShader() 
		{ 
			this->shader = NULL; 
			this->drawData = std::vector<DRAW_DATA>(); 
		}

		virtual void draw(SHADER_PARAMETER_DATA&) = 0;
		/** Use this each frame to clear old content */
		void clearData()
		{
			this->drawData.clear();
			this->drawData.shrink_to_fit();
		}

		bool init(BaseShader::BASE_SHADER_DESC& desc)
		{
			this->shader = new BaseShader();
			if( FAILED (this->shader->Initialize(desc) ) )
				return false;
			return true;
		}
		void addDrawData(DRAW_DATA data)
		{
			this->drawData.push_back(data);
		}
		
		BaseShader* getShader()
		{
			return this->shader;
		}
		int getId()
		{
			return this->id.get();
		}

};


#endif

