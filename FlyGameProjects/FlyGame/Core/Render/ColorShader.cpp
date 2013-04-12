#include "ColorShader.h"

ColorShader::ColorShader()
{


}

void ColorShader::draw(SHADER_PARAMETER_DATA& wMatrixData)
{
	int indexC = 0;

	this->setSRVBuffer();

	this->shader->Render();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	int count = this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		cBufferMatrix* cb = (cBufferMatrix*)wMatrixData.cMatrixBuffer->Map();
		if(cb)
		{
			cb->world = *this->drawData[i].worldMatrix; // add the world matrix of the object
			wMatrixData.cMatrixBuffer->Unmap();
		}

		wMatrixData.cMatrixBuffer->setBuffer();
		for(int k = 0; k <this->drawData[i].buffers.size(); k++)
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
void ColorShader::setSRVBuffer()
{
	ID3D11ShaderResourceView* srv [1]; 
	srv[0]= D3DShell::self()->getDefferedSRV();
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,1, srv);
	
	
}

