#include "LightShader.h"

LightShader::LightShader()
{
}

void LightShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;

	D3DShell::self()->setDefferedSRV();

	this->shader->Render(); // set vertex and pixel shader
	
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		
		//this->matrixBuffer->setBuffer();

		frameData.camForLight->setBuffer(0);
		frameData.lights->setBuffer(1);
		
	
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

