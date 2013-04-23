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

	
	//D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);

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
				cb->worldInvTranspose = *D3DXMatrixInverse(&temp, &det, this->drawData[i].worldMatrix);
			
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


		//if(this->drawData[i].material ) //if there is any textures
		//{
		//	ID3D11ShaderResourceView* srv[3];
		//	this->drawData[i].material->GetAmbientTexture();
		//	//int nr = this->drawData[i].textures->size();
		//	//for(int k= 0; k<(int)this->drawData[i].textures->size(); k++)
		//	//{
		//	//srv[i] = this->drawData[i].textures->at(i).getSRV();
		//	//}
		//	srv[0] = this->drawData[i].material->GetAmbientTexture();
		//	srv[1] = this->drawData[i].material->GetDiffuseTexture();
		//	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,2, srv);

		//	this->drawData[i].material->GetBuffer();
		//}
		//set material cBuffer
		
	
		if(this->drawData[i].material)
		{
			ID3D11ShaderResourceView* temp[1] = { drawData[i].material->GetDiffuseTexture() };
			frameData.dc->PSSetShaderResources(0, 1, temp);
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