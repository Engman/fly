#include "ColorShader.h"

ColorShader::ColorShader()
{


}

void ColorShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;

	this->setSRVBuffer();

	this->shader->Render();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	
	
	
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
void ColorShader::setSRVBuffer()
{
	int nr = 2;//D3DShell::self()->getNrOfSRV();
	ID3D11ShaderResourceView** srv; 
	srv = D3DShell::self()->getLightSRV();
	ID3D11ShaderResourceView* color[2]; 
	color[0] = srv[0];
	color[1] = srv[1];
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nr, color);
}

