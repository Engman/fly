#include "GBufferShader.h"

GBufferShader::GBufferShader()
{

}

void GBufferShader::draw(SHADER_PARAMETER_DATA& wMatrixData)
{
	int indexC = 0;
	
	this->shader->Render();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		cBufferMatrix* cb = (cBufferMatrix*)wMatrixData.cMatrixBuffer->Map();
		if(cb)
		{
			cb->world = *this->drawData[i].worldMatrix; // add the world matrix of the object
			wMatrixData.cMatrixBuffer->Unmap();
		}

		wMatrixData.cMatrixBuffer->setBuffer();
		for(int k = 0; k <(int)this->drawData[i].buffers.size(); k++)
		{
			this->drawData[i].buffers[k]->setBuffer();

			if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_INDEX_BUFFER)
				indexC = this->drawData[i].buffers[k]->getNrOfElements();
		}

		this->shader->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->shader->GetDeviceContext()->DrawIndexed(indexC, 0, 0);
	}
	this->clearData();
}