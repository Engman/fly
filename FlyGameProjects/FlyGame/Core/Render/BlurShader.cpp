#include "BlurShader.h"

BlurShader::BlurShader()
	:IShader()
{
	
}
bool BlurShader::init(BaseShader::BASE_SHADER_DESC& desc)
{
	IShader::init(desc);
	this->blurCB = new BaseBuffer();

	blurBuffer blurBufferData;
	blurBufferData.textureHeight =	D3DShell::self()->getWidth();
	blurBufferData.textureWidth =	D3DShell::self()->getHeight();
	blurBufferData.blurFactor =	10.0f;
	blurBufferData.padd = 0.0f;

	BaseBuffer::BUFFER_INIT_DESC cbDesc;
	cbDesc.dc = D3DShell::self()->getDeviceContext();
	cbDesc.device = D3DShell::self()->getDevice();
	cbDesc.elementSize = sizeof(blurBuffer);
	cbDesc.nrOfElements = 1;
	cbDesc.data = &blurBufferData; 
	cbDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	cbDesc.usage = BUFFER_FLAG::USAGE_DEFAULT;

	if(FAILED(blurCB->Initialize(cbDesc)))
	{
		DisplayText("Could not initialize blur constant buffer!");
	}
	return true;
}


void BlurShader::draw(PER_FRAME_DATA& frameData)
{

	int indexC = 0;


	this->shader->Render();
	FLAGS::STATE_SAMPLING samp[1] =  { FLAGS::SAMPLER_LinearClamp };

	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);
	D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0, 1);

	blurCB->setBuffer();

	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
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

