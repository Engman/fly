#include "FinalShader.h"

FinalShader::FinalShader()
	:IShader()
{

}

void FinalShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;

	D3DShell::self()->setLightSRV();

	this->shader->Render();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	FLAGS::STATE_SAMPLING samp[1] = {FLAGS::SAMPLER_LinearClamp};
	D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0, 1);
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		//set camdata, same for all lights
		frameData.camForLight->setBuffer(0);

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


