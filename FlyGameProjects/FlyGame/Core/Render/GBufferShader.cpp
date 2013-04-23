#include "GBufferShader.h"

GBufferShader::GBufferShader()
{
	

	
}

void GBufferShader::draw(PER_FRAME_DATA& wMatrixData)
{	

	int indexC = 0;
	
	this->shader->Render();
	
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		cBufferMatrix* cb = (cBufferMatrix*)this->matrixBuffer->Map();
		if(cb)
		{
			cb->world = *this->drawData[i].worldMatrix; // add the world matrix of the object
			D3DXMatrixLookAtLH(&cb->view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixPerspectiveFovLH(&cb->projection,(float)D3DX_PI * 0.45f, 800/600, 0.1f, 100.0f);
			cb->worldInvTranspose = cb->world;
			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

			this->matrixBuffer->Unmap();
		}

		this->matrixBuffer->setBuffer(); // set wvp matrix
		

		for(int k = 0; k <(int)this->drawData[i].buffers.size(); k++)	// set vertex and index buffers
		{
			this->drawData[i].buffers[k]->setBuffer();

			if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_INDEX_BUFFER)
				indexC = this->drawData[i].buffers[k]->getNrOfElements();
		}

		if(this->drawData[i].material ) //if there is any textures
		{
			ID3D11ShaderResourceView* srv[3];
			this->drawData[i].material->GetAmbientTexture();
			//int nr = this->drawData[i].textures->size();
			//for(int k= 0; k<(int)this->drawData[i].textures->size(); k++)
			//{
			//srv[i] = this->drawData[i].textures->at(i).getSRV();
			//}
			srv[0] = this->drawData[i].material->GetAmbientTexture();
			srv[1] = this->drawData[i].material->GetDiffuseTexture();
			D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,2, srv);

			this->drawData[i].material->GetBuffer();
		}
		//set material cBuffer
		
	


		this->shader->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->shader->GetDeviceContext()->DrawIndexed(indexC, 0, 0);

	}
	this->clearData();
}