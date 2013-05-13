#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader()
	:IShader()
{

}

void ShadowMapShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;
	int vertexC = 0;

	D3DShell::self()->setShadowSRV();
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

