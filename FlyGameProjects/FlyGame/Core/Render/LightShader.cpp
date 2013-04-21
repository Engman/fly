#include "LightShader.h"

LightShader::LightShader()
{


}

void LightShader::draw(SHADER_PARAMETER_DATA& wMatrixData)
{
	int indexC = 0;

	this->setSRVBuffer();

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
			D3DXMatrixOrthoLH(&cb->projection, 800.0f, 600.0f, 0.1f, 100.0f);
			cb->worldInvTranspose = cb->world;

			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

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
void LightShader::setSRVBuffer()
{
	// gets the srv with the normal, depth and specular data
	int nr = D3DShell::self()->getNrOfSRV();
	ID3D11ShaderResourceView** srv; 
	srv = D3DShell::self()->getDefferedSRV();
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0, nr, srv);
}

