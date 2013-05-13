#include "GBufferAnimationShader.h"

GBufferAnimationShader::GBufferAnimationShader()
{
	time =0 ;
}


void GBufferAnimationShader::draw(PER_FRAME_DATA& frameData)
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
				cb->worldInvTranspose = temp;
			}
			
			D3DXMatrixTranspose(&cb->world, &cb->world);
			D3DXMatrixTranspose(&cb->view,&cb->view);
			D3DXMatrixTranspose(&cb->projection,&cb->projection);

			this->matrixBuffer->Unmap();
		}
		this->matrixBuffer->setBuffer(0);

		time+=0.1f;
		if (time>1)
		{
			time=0;
		}
		aminationWeight weightCB;
		weightCB.weight = time;

		BaseBuffer::BUFFER_INIT_DESC bufferDesc;
		bufferDesc.dc = D3DShell::self()->getDeviceContext();
		bufferDesc.device = D3DShell::self()->getDevice();
		bufferDesc.elementSize = sizeof(aminationWeight);
		bufferDesc.data = &weightCB;
		bufferDesc.nrOfElements = 1;
		bufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER;
		bufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

		BaseBuffer m_weightBuffer;
		if(FAILED(m_weightBuffer.Initialize(bufferDesc)))
		{
			MessageBox(0, L"Could not initialize weightbuffer! Plane.cpp - Initialize", L"Error", MB_OK);
		}

		m_weightBuffer.setBuffer(1);

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