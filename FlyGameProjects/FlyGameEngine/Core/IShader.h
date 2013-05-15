#ifndef I_SHADER_H
#define I_SHADER_H


#include "BaseBuffer.h"
#include "BaseShader.h"
#include "..\Util\GID.h"
#include "..\Util\Proxy.h"
#include "..\Util\SmartPtrs.h"
#include "Mesh\ObjectMaterial.h"

/** Inherit to create shaders */
class IShader
{
	public:
		struct PER_FRAME_DATA
		{
			ID3D11DeviceContext* dc;
			//BaseBuffer* cMatrixBuffer;
			Matrix	view;
			Matrix	projection;
			BaseBuffer* lights;
		};
		/** Used to set data for a draw call */
		struct DRAW_DATA
		{
			D3DXMATRIX* worldMatrix;
			std::vector<BaseBuffer*> buffers;
			ObjectMaterial* material;
			//std::vector<Texture2D>* textures; 
			DRAW_DATA()
				:worldMatrix(0), material(0)
			{}
		};

	private:
		GID id;

	protected:
		BaseBuffer* matrixBuffer;
		SmartPtrStd<BaseShader> shader;
		std::vector<DRAW_DATA> drawData;
		int type;

	public:
		IShader() 
		{ 
			this->shader = NULL; 
			this->drawData = std::vector<DRAW_DATA>(); 
			this->type = -1;
		}
		virtual~IShader()
		{}
		virtual void draw(PER_FRAME_DATA&) = 0;
		/** Use this each frame to clear old content */
		void clearData()
		{
			this->drawData.clear();
			this->drawData.shrink_to_fit();
		}

		bool init(BaseShader::BASE_SHADER_DESC& desc, int type)
		{
			this->shader = new BaseShader();
			this->type = type;
			if( FAILED (this->shader->Initialize(desc) ) )
				return false;

			
			static SmartPtrStd<BaseBuffer> mb = new BaseBuffer();
			static bool initiated = false;

			if(!initiated)
			{
				BaseBuffer::BUFFER_INIT_DESC matrixBufferDesc;
				matrixBufferDesc.dc = desc.dc;
				matrixBufferDesc.device = desc.device;
				matrixBufferDesc.elementSize = sizeof(cBufferMatrix);
				matrixBufferDesc.nrOfElements = 1;
				matrixBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER;
				matrixBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;
				matrixBufferDesc.data = NULL;
				if( FAILED( mb->Initialize(matrixBufferDesc) ) )
					return false;
				
				initiated = true;
			}

			this->matrixBuffer = mb;
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
		virtual int getType() const = 0;
};


#endif

