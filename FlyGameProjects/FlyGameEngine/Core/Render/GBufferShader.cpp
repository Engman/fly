
#include "GBufferShader.h"

GBufferShader::GBufferShader()
{
	

	
}


void GBufferShader::draw(PER_FRAME_DATA& frameData)
{	
	int indexC = 0;
	int vertexC = 0;
	
	FLAGS::STATE_SAMPLING samp[1] =  { FLAGS::SAMPLER_Linear };
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0, 1);

	this->shader->Render();

	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
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
			{
				D3DXMatrixInverse(&temp, &det, this->drawData[i].worldMatrix);
				//D3DXMatrixTranspose(&temp, &temp);
				cb->worldInvTranspose = temp;
			}
			
			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

			this->matrixBuffer->Unmap();
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
	
		if(this->drawData[i].material)
		{

			ID3D11ShaderResourceView* temp[4] = { 
				drawData[i].material->GetDiffuseTexture() , 
				drawData[i].material->GetNormalTexture() , 
				drawData[i].material->GetSpecularTexture(), 
				drawData[i].material->GetGlowTexture()
			};

			frameData.dc->PSSetShaderResources(0, 4, temp);
		}

		BaseBuffer* mat= drawData[i].material->GetBuffer();
		mat->setBuffer();
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