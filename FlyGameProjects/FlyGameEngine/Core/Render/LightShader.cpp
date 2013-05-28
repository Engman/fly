#include "LightShader.h"

LightShader::LightShader()
{
}
LightShader::~LightShader()
{
	this->cameraBuffer.Destroy();
}
bool LightShader::init(BaseShader::BASE_SHADER_DESC& desc)
{
	IShader::init(desc);
	this->cameraBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC cameraBufferDesc;
	cameraBufferDesc.dc = D3DShell::self()->getDeviceContext();
	cameraBufferDesc.device = D3DShell::self()->getDevice();
	cameraBufferDesc.elementSize = sizeof(CameraView);
	cameraBufferDesc.nrOfElements = 1;
	cameraBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	cameraBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;
	
	if(FAILED(cameraBuffer->Initialize(cameraBufferDesc)))
	{
		DisplayText("Could not initialize lightShader camera constant buffer!");
	}
	return true;

}

void LightShader::draw(PER_FRAME_DATA& frameData)
{
	int indexC = 0;
	int vertexC = 0;

	D3DShell::self()->setDefferedSRV();
	D3DShell::self()->setRasterizerState(FLAGS::RASTERIZER_NoCullNoMs);

	this->shader->Render(); // set vertex and pixel shader
	
	int count = (int)this->drawData.size();
	for( int i = 0; i< count;i++)
	{
		if(this->drawData[i].worldMatrix)
		{

		
		cBufferMatrix* cb = (cBufferMatrix*)this->matrixBuffer->Map();
		if(cb)
		{
			D3DXMATRIX world, translate; 
			D3DXMatrixIdentity(&world);
			D3DXMatrixTranslation(&translate, 50, 0, 0);
			cb->world = *this->drawData[i].worldMatrix;
			cb->view = frameData.view;
			cb->projection = frameData.projection;

			Matrix temp;
			float det = D3DXMatrixDeterminant(this->drawData[i].worldMatrix);
			if(det)
			{
				D3DXMatrixInverse(&temp, &det, &world); //this->drawData[i].worldMatrix);
				//D3DXMatrixTranspose(&temp, &temp);
				cb->worldInvTranspose = temp;
			}

			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

			this->matrixBuffer->Unmap();
		}
		}
		this->matrixBuffer->setBuffer();

		
		//frameData.lights->setBuffer(1);

		int cbOffset = 0; 
		this->drawData[i].lightBuffers[0]->setBuffer(1);
		cbOffset++;

		frameData.camForLight->setBuffer(0);

		
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
