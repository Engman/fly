#include "LightHolder.h"

LightHolder::LightHolder()
{
}
LightHolder::~LightHolder()
{
}

bool LightHolder::Initialize()
{
	this->camView = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC lightBufferDesc;
	lightBufferDesc.dc = D3DShell::self()->getDeviceContext();
	lightBufferDesc.device = D3DShell::self()->getDevice();
	lightBufferDesc.elementSize = sizeof(CameraView);
	lightBufferDesc.nrOfElements = 1;
	lightBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_PS_BUFFER;
	lightBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->camView->Initialize(lightBufferDesc)))
		return false;

	return true;
}
void LightHolder::addLight(DirectionalLightProxy lightProxy, IShader* shader)
{
	/*DirectionLight light(Type::LIGHT); 
	light.Initialize(lightProxy);

	dirLights.push_back(light);*/
	this->dLight = new DirectionLight(Type::LIGHT);
	//this->dLight->Initialize(lightProxy, shader, true);
}

void LightHolder::addLight(PointLightProxy lightProxy, IShader* shader)
{
	PointLight light(Type::LIGHT); 
	light.Initialize(lightProxy);
	pointLights.push_back(light);
}
BaseBuffer* LightHolder::getDirLight(int nr)
{
	if((int)this->dirLights.size()>nr)	
		return this->dirLights[nr].getLight();
	else 
		return NULL;
}
BaseBuffer* LightHolder::getPointLight(int nr)
{
	if((int)this->pointLights.size()>nr)	
		return this->pointLights[nr].getLight();
	else 
		return NULL;
}	
D3DXMATRIX LightHolder::getDirLightView(int nr)
{
	return this->dirLights[nr].getView();
}
D3DXMATRIX LightHolder::getDirLightProjection(int nr)
{
	return this->dirLights[nr].getProjection();
}
void LightHolder::setCamViewBuffer(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 camPos)
{
	CameraView* cam = (CameraView*)camView->Map();
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);
	cam->mInvViewProj = projection;
	cam->mInvView = view;
	cam->cameraPos = camPos; 
	cam->padd = D3DShell::self()->getWidth();
	camView->Unmap();
	
}
BaseBuffer* LightHolder::getCamViewBuffer()
{
	return this->camView;
}
int LightHolder::getNrOfDirLight()
{
	return (int)this->dirLights.size();
}

void LightHolder::RenderDirLight(ViewFrustum viewFustrum, IShader* lightShader, IShader* shadowShader)
{

	for(int i=0; i<(int)this->dirLights.size(); i++)
	{
		this->dirLights[i].Render(viewFustrum);
	}
	this->dLight->setShader(lightShader);
	this->dLight->Render(viewFustrum);

	if(this->dLight->castShadow())
	{
		this->dLight->setShader(lightShader);
		this->dLight->Render(viewFustrum);
	}
}