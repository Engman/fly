#include "BaseBuffer.h"
#include "..\Util\SmartPtrs.h"


BaseBuffer::BaseBuffer()
	:dc(0), nrOfElements(0), elementsSize(0), id(), buffer()
{
	BUFFER_FLAG::TYPE bufferType = BUFFER_FLAG::TYPE_NONE;
	BUFFER_FLAG::USAGE bufferUsage = BUFFER_FLAG::USAGE_DEFAULT;
}
BaseBuffer::BaseBuffer(const BaseBuffer& d)
		:dc(d.dc), nrOfElements(d.nrOfElements), elementsSize(d.elementsSize), buffer(d.buffer)
{
	BUFFER_FLAG::TYPE bufferType = d.bufferType;
	BUFFER_FLAG::USAGE bufferUsage = d.bufferUsage;
	this->id = d.id;
}
BaseBuffer::~BaseBuffer()
{ }


HRESULT BaseBuffer::Initialize(BaseBuffer::BUFFER_INIT_DESC& iDesc)
{
	this->dc			= iDesc.dc;
	this->bufferType	= iDesc.type;
	this->bufferUsage	= iDesc.usage;
	this->elementsSize	= iDesc.elementSize;
	this->nrOfElements	= iDesc.nrOfElements;
	D3D11_BUFFER_DESC	  desc;

	/** 
	*	Build the Buffer description to initiate our buffer 
	*/

	/* Get the buffer type */
	switch (this->bufferType)
	{
		case BUFFER_FLAG::TYPE_CONSTANT_HS_BUFFER:
		case BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER:
		case BUFFER_FLAG::TYPE_CONSTANT_GS_BUFFER:
		case BUFFER_FLAG::TYPE_CONSTANT_DS_BUFFER:
		case BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER:
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;
		
		break;
		case BUFFER_FLAG::TYPE_VERTEX_BUFFER:		
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
		case BUFFER_FLAG::TYPE_INDEX_BUFFER:	
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
		case BUFFER_FLAG::TYPE_GS_STREAM_OUTPUT:
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		break;
	}

	/* Get the buffer usage */
	switch (this->bufferUsage)
	{
		case BUFFER_FLAG::USAGE_DEFAULT:		   
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;
		break;
		case BUFFER_FLAG::USAGE_IMMUTABLE:		   
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE:
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD:
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_READ:  
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_READ_WRITE:
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		break;
	}

	desc.MiscFlags = 0;
	desc.ByteWidth = iDesc.elementSize * this->nrOfElements;
	desc.StructureByteStride = 0;

	/* Get subrescource data */
	if(iDesc.data)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = iDesc.data;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		if( FAILED( iDesc.device->CreateBuffer(&desc, &data, this->buffer)))
			return E_FAIL;
	}
	else
		if( FAILED( iDesc.device->CreateBuffer(&desc, NULL, this->buffer)))
			return E_FAIL;


	return S_OK;
}


HRESULT BaseBuffer::setBuffer(UINT32 startSlot, UINT32 offset)
{
	switch (this->bufferType)
	{
		case BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER:
			this->dc->VSSetConstantBuffers(startSlot, 1, this->buffer);
		break;
		
		case BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER:
			this->dc->PSSetConstantBuffers(startSlot, 1, this->buffer);
		break;
		
		case BUFFER_FLAG::TYPE_CONSTANT_GS_BUFFER:
			this->dc->GSSetConstantBuffers(startSlot, 1, this->buffer);
		break;

		case BUFFER_FLAG::TYPE_CONSTANT_HS_BUFFER:
			this->dc->HSSetConstantBuffers(startSlot, 1, this->buffer);
		break;

		case BUFFER_FLAG::TYPE_CONSTANT_DS_BUFFER:
			this->dc->DSSetConstantBuffers(startSlot, 1, this->buffer);
		break;

		case BUFFER_FLAG::TYPE_GS_STREAM_OUTPUT:
			this->dc->SOSetTargets(1, this->buffer, &offset);
		break;

		case BUFFER_FLAG::TYPE_INDEX_BUFFER:
			this->dc->IASetIndexBuffer(this->buffer, DXGI_FORMAT_R32_UINT, offset);
		break;

		case BUFFER_FLAG::TYPE_VERTEX_BUFFER:
			this->dc->IASetVertexBuffers(startSlot, 1, this->buffer, &this->elementsSize, &offset);
		break;

		default:
			return E_FAIL;
		break;
	}
	return S_OK;
}

void* BaseBuffer::Map()
{
	if(!this->dc)
		return NULL;

	UINT32 mapType = 0;
	switch (this->bufferUsage)
	{
		case BUFFER_FLAG::USAGE_DEFAULT:
		case BUFFER_FLAG::USAGE_IMMUTABLE:					 
			mapType = -1;
		break;

		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE:			 
			mapType = D3D11_MAP_WRITE;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD:	 
			mapType = D3D11_MAP_WRITE_DISCARD;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_READ:			 
			mapType = D3D11_MAP_READ;
		break;
		case BUFFER_FLAG::USAGE_DYNAMIC_CPU_READ_WRITE:		 
			mapType = D3D11_MAP_READ_WRITE;
		break;
		default:
			mapType = D3D11_MAP_WRITE_DISCARD;
		break;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	void* dataPtr;

	if(FAILED(this->dc->Map(this->buffer, 0, (D3D11_MAP)mapType, 0, &mappedResource)))
		return NULL;
	
	dataPtr = (void*)mappedResource.pData;

	return dataPtr;
}
void BaseBuffer::Unmap()
{
	this->dc->Unmap(this->buffer, 0);
}

BUFFER_FLAG::TYPE BaseBuffer::getType() const
{
	return this->bufferType;
}
BUFFER_FLAG::USAGE BaseBuffer::getUsage()	 const	 
{
	return this->bufferUsage;
}
int BaseBuffer::getNrOfElements()		 const		 
{
	return this->nrOfElements;
}
ID3D11Buffer* BaseBuffer::getBuffer()				 
{
	return this->buffer;
}
int BaseBuffer::getID()  const
{
	return this->id.get();
}