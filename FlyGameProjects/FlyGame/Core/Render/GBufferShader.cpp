#include "GBufferShader.h"

GBufferShader::GBufferShader()
{
	

	
}

void GBufferShader::draw(SHADER_PARAMETER_DATA& wMatrixData)
{	
	int indexC = 0;
	int vertexC = 0;
	
	this->shader->Render();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		cBufferMatrix* cb = (cBufferMatrix*)wMatrixData.cMatrixBuffer->Map();
		if(cb)
		{
			cb->world = *this->drawData[i].worldMatrix; // add the world matrix of the object
			D3DXMatrixLookAtLH(&cb->view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixPerspectiveFovLH(&cb->projection,(float)D3DX_PI * 0.45f, 800/600, 0.1f, 100.0f);
			cb->worldInvTranspose = cb->world;
			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

			wMatrixData.cMatrixBuffer->Unmap();
		}

		wMatrixData.cMatrixBuffer->setBuffer(); // set wvp matrix
		

		for(int k = 0; k <(int)this->drawData[i].buffers.size(); k++)	// set vertex and index buffers
		{
			this->drawData[i].buffers[k]->setBuffer();

			if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_INDEX_BUFFER)
				indexC = this->drawData[i].buffers[k]->getNrOfElements();
			else if(this->drawData[i].buffers[k]->getType() == BUFFER_FLAG::TYPE_VERTEX_BUFFER)
				vertexC = this->drawData[i].buffers[k]->getNrOfElements();
		}

		if(this->drawData[i].textures ) //if there is any textures
		{
			std::vector<ID3D11ShaderResourceView*> srv;
			int nr = (int)this->drawData[i].textures->size();
			for(int k= 0; k<(int)this->drawData[i].textures->size(); k++)
			{
				std::vector<ID3D11ShaderResourceView*>* tex = this->drawData[i].textures->at(i).getTextures();
				if(tex)
					for (int l = 0; l < (int)tex->size(); l++)
					{
						srv.push_back((*tex)[l]);
					}
			}
			D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nr, &srv[0]);
		}
		if(this->drawData[i].material)
		{
			//drawData[i].material->get
		}

		this->shader->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		if(indexC)
			this->shader->GetDeviceContext()->DrawIndexed(indexC, 0, 0);
		else if(vertexC)
			this->shader->GetDeviceContext()->Draw(vertexC, 0);
	}
	this->clearData();
}