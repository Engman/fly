#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader()
	:IShader()
{

}
ShadowMapShader::~ShadowMapShader()
{
	this->lightViewBuffer.Destroy();
}
bool ShadowMapShader::init(BaseShader::BASE_SHADER_DESC& desc)
{
	IShader::init(desc);
	this->lightViewBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC lightBufferDesc;
	lightBufferDesc.dc = D3DShell::self()->getDeviceContext();
	lightBufferDesc.device = D3DShell::self()->getDevice();
	lightBufferDesc.elementSize = sizeof(CameraView);
	lightBufferDesc.nrOfElements = 1;
	lightBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	lightBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(lightViewBuffer->Initialize(lightBufferDesc)))
	{
		DisplayText("Could not initialize lightShader camera constant buffer!");
	}
	return true;

}

void ShadowMapShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;
	int vertexC = 0;

	D3DShell::self()->setShadowSRV();
	this->shader->Render();

	
	
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		if(this->drawData[i].worldMatrix) //if the object have a world matrix
		{

			cBufferMatrix* cb = (cBufferMatrix*)this->matrixBuffer->Map();
			if(cb)
			{
				cb->world = *this->drawData[i].worldMatrix;
				cb->view = frameData.view;
				cb->projection = frameData.projection;

				Matrix temp;
				float det = D3DXMatrixDeterminant(this->drawData[i].worldMatrix);
				if(det)
					cb->worldInvTranspose = *D3DXMatrixInverse(&temp, &det, this->drawData[i].worldMatrix);

				D3DXMatrixTranspose(&cb->world, &cb->world);
				D3DXMatrixTranspose(&cb->view,&cb->view);
				D3DXMatrixTranspose(&cb->projection,&cb->projection);

				this->matrixBuffer->Unmap();
			}
		}
		this->matrixBuffer->setBuffer();


		for(int k = 0; k <(int)this->drawData[i].buffers.size(); k++)	// set vertex and index buffers
		{
			this->drawData[i].buffers[k]->setBuffer();

			if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_INDEX_BUFFER)
				indexC = this->drawData[i].buffers[k]->getNrOfElements();
			else if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_VERTEX_BUFFER)
				vertexC = this->drawData[i].buffers[k]->getNrOfElements();
		}
		
		this->shader->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		if(indexC)
			this->shader->GetDeviceContext()->DrawIndexed(indexC, 0, 0);
		else if(vertexC)
			this->shader->GetDeviceContext()->Draw(vertexC, 0);

		indexC = 0;
		vertexC = 0;
	}
	this->clearData();
}

