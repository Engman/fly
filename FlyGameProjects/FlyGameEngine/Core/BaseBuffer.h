#ifndef BASEBUFFER_H
#define BASEBUFFER_H


#include <D3DX11\D3D11.h>
#include "..\Util\SmartPtrs.h"
#include "..\Util\GID.h"

namespace BUFFER_FLAG
{
	enum TYPE
	{
		TYPE_CONSTANT_VS_BUFFER,
		TYPE_CONSTANT_PS_BUFFER,
		TYPE_CONSTANT_GS_BUFFER,
		TYPE_CONSTANT_HS_BUFFER,
		TYPE_CONSTANT_DS_BUFFER,
		TYPE_VERTEX_BUFFER,
		TYPE_INDEX_BUFFER,
		TYPE_GS_STREAM_OUTPUT,
		TYPE_NONE,
	};
	enum USAGE
	{
		USAGE_DEFAULT, 
		USAGE_IMMUTABLE,
		USAGE_DYNAMIC_CPU_WRITE,  
		USAGE_DYNAMIC_CPU_WRITE_DISCARD,
		USAGE_DYNAMIC_CPU_READ,
		USAGE_DYNAMIC_CPU_READ_WRITE,
		USAGE_NONE,
	};
};

class BaseBuffer
{
	public:
		struct BUFFER_INIT_DESC
		{
			ID3D11Device* device;
			ID3D11DeviceContext* dc;

			BUFFER_FLAG::TYPE type;
			BUFFER_FLAG::USAGE usage;
			void* data;
			int nrOfElements;
			int elementSize;

			BUFFER_INIT_DESC()
				:device(0), dc(0), type(BUFFER_FLAG::TYPE_NONE), usage(BUFFER_FLAG::USAGE_NONE), data(0), nrOfElements(0), elementSize(0)
			{ }
		};

		
	private:
		ID3D11DeviceContext* dc;
		SmartPtrCom<ID3D11Buffer> buffer;

		BUFFER_FLAG::TYPE bufferType;
		BUFFER_FLAG::USAGE bufferUsage;
		int nrOfElements;
		UINT elementsSize;
		GID id;


	public:
		BaseBuffer();
		BaseBuffer(const BaseBuffer& original);
		virtual ~BaseBuffer();

		/**The Initialize function wants a BUFFER_TYPE::FLAG which is accesible from the BaseBuffer class */
		HRESULT Initialize(BUFFER_INIT_DESC& desc);

		//void Release();

		/** Sets the buffer to the pipeline with given slot */
		HRESULT setBuffer(UINT32 slot = 0, UINT32 offset = 0);

		BUFFER_FLAG::TYPE	getType()			const;
		BUFFER_FLAG::USAGE	getUsage()			const;
		int					getNrOfElements()	const;
		int					getID()				const;
		ID3D11Buffer*		getBuffer();
		

		/**	The "Map" function will map the buffer subresource data to a void pointer and return it */
		void* Map();
		/** Unmap the buffer using the devicecontext */
		void Unmap();
};

#endif