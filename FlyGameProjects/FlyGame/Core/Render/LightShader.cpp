#include "LightShader.h"

LightShader::LightShader()
{


}

void LightShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;

	this->setSRVBuffer();

	this->shader->Render(); // set vertex and pixel shader
	
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		if(this->drawData[i].worldMatrix) //if the object have a world matrix
		{
			//set WVP till VS
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
		
		//set camdata, same for all lights
		frameData.camForLight->setBuffer(0,0);
		
		//set directional light
		frameData.lights->setBuffer(1, sizeof(CameraView));
		
		//set objects vercise data
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
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nr, srv);
}

