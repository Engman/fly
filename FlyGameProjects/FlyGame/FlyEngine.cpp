#include "FlyEngine.h"



FlyEngine::FlyEngine()
{
	D3DShell::self();
	this->fsq = new	FullScreenQuad();
}
FlyEngine::~FlyEngine()
{
}


bool FlyEngine::Initialize(HWND hWnd, int width, int height, bool multisampling, bool vSync, bool fullScreen)
{
	if(!InitD3D(hWnd, width, height, multisampling, vSync, fullScreen))	
	{
		DisplayText("Failed to initiate 3D");
		return false;
	}
	if(!InitGBuffers())		
	{			
		DisplayText("Failed to initiate Geometry buffer");
		return false;
	}
	if(!InitColorShader())	
	{			
		DisplayText("Failed to initiate Color shader");
		return false;
	}
	if(!InitMatrixBuffer())	
	{			
		DisplayText("Failed to initiate Matrix buffer");
		return false;
	}

	this->fsq->Initialize(D3DShell::self()->getDevice(), this->colorShader);

	this->mainCamera.setLookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
	this->mainCamera.setPerspective((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);


	return true;
}
void FlyEngine::Shutdown()
{
	D3DShell::self()->destroy();
}


void FlyEngine::Update()
{
	this->mainCamera.updateView();
}
bool FlyEngine::Render()
{
	DeferedRendering();

	return true;
}
void FlyEngine::DeferedRendering()
{
	D3DShell::self()->BeginGBufferRenderTargets();

	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->mainCamera.getView();
	gBufferDrawData.projection = this->mainCamera.getProj();



//#################################//
//########## G-buffers ############//
//#################################//

	for (int i = 0; i <(int)this->objects.size(); i++)
	{
		this->objects[i]->Render();
	}
	this->gbufferShader->draw(gBufferDrawData);

	
//##################################################################//
//------------- Final pass, add light and color together ----------//
//##################################################################//
	//second render stage
	//sampling from g-buffers
	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();

	this->fsq->Render(D3DShell::self()->getDeviceContext());
	this->colorShader->draw(gBufferDrawData);
	
	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}




bool FlyEngine::InitD3D(HWND hWnd, int w, int h, bool multisampling, bool vSync, bool fullScreen)
{
	D3DShell::D3D_INIT_DESC desc;

	desc.height = h;
	desc.width = w;
	desc.hwnd = hWnd;
	desc.MSAA = multisampling;
	desc.MSAASampleCount = 4;
	desc.vSync = vSync;
	desc.fullScreen = fullScreen;

	if(!D3DShell::self()->init(desc))
		return false;

	return true;

}
bool FlyEngine::InitGBuffers()
{
	BaseShader::BASE_SHADER_DESC gBufferDesc;

	gBufferDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferDesc.device = D3DShell::self()->getDevice();
	gBufferDesc.VSFilename = L"../Resources/Shaders/deferredShaderTextVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/deferredShaderTextPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNT_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	this->gbufferShader = new GBufferShader();
	if(!this->gbufferShader->init(gBufferDesc))	
		return false;

	return true;
}
bool FlyEngine::InitLightShader()
{
	BaseShader::BASE_SHADER_DESC lightShaderDesc;

	lightShaderDesc.dc = D3DShell::self()->getDeviceContext();
	lightShaderDesc.device = D3DShell::self()->getDevice();
	lightShaderDesc.VSFilename = L"../Resources/Shaders/LightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/LightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexPNC_InputElementDesc;
	lightShaderDesc.nrOfElements = 3;

	this->lightShader = new LightShader();
	if(!this->lightShader->init(lightShaderDesc))	
		return false;

	return true;
}
bool FlyEngine::InitColorShader()
{
	BaseShader::BASE_SHADER_DESC colorShaderDesc;

	colorShaderDesc.dc = D3DShell::self()->getDeviceContext();
	colorShaderDesc.device = D3DShell::self()->getDevice();
	colorShaderDesc.VSFilename = L"../Resources/Shaders/TextLightVS.vs";
	colorShaderDesc.PSFilename = L"../Resources/Shaders/TextLightPS.ps";
	colorShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	colorShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	colorShaderDesc.nrOfElements = 2;

	this->colorShader = new ColorShader();
	if(!this->colorShader->init(colorShaderDesc))
		return false;

	return true;
}
bool FlyEngine::InitMatrixBuffer()
{
	this->pMatrixBuffer = new BaseBuffer();

	BaseBuffer::BUFFER_INIT_DESC matrixBufferDesc;
	matrixBufferDesc.dc = D3DShell::self()->getDeviceContext();
	matrixBufferDesc.device = D3DShell::self()->getDevice();
	matrixBufferDesc.elementSize = sizeof(cBufferMatrix);
	matrixBufferDesc.nrOfElements = 1;
	matrixBufferDesc.type = BUFFER_FLAG::TYPE_CONSTANT_VS_BUFFER;
	matrixBufferDesc.usage = BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;

	if(FAILED(this->pMatrixBuffer->Initialize(matrixBufferDesc)))
		return false;
	
	return true;
}

int FlyEngine::LoadResources(std::wstring folder)
{
	int count = 0;
	WIN32_FIND_DATA FindFileData;
	std::wstring p = L"";
	p.append(folder);
	p.append(L"\\*fgm");
	HANDLE hFind = FindFirstFile(p.c_str(), &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		std::wstring msg = L"";
		msg.append(L"No valid files in:\n");
		msg.append(folder);
		DisplayText(msg);
		return false;
	}
	else
	{
		folder.append(L"\\");
		vector<std::wstring> models;
		std::wstring tempFirst = folder; 
		tempFirst.append(FindFileData.cFileName);
		models.push_back(tempFirst);

		while ( FindNextFile(hFind, &FindFileData) )
		{
			std::wstring file = folder; 
			file.append(FindFileData.cFileName);
			models.push_back(file);
		}

		for (int i = 0; i < (int)models.size(); i++)
		{
			//Load mesh objects
			SmartPtrStd<ImportedObjectData> raw;
			if(!ResourceImporter::ImportObject(models[i].c_str(), D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), raw))
				return 0;
			count ++;
			//Create the object for rendering
			Object* model = new Object();
			Object::OBJECT_DESC desc;
			desc.device = D3DShell::self()->getDevice();
			desc.deviceContext = D3DShell::self()->getDeviceContext();
			desc.material_id = raw->objects[0].material;
			desc.shader = this->gbufferShader;
			desc.vertecies = raw->objects[0].vertex;
			desc.vCount = (int)raw->objects[0].vertex->size();
			if(!model->Initialize(desc))
				return 0;

			this->objects.push_back(model);
		
		}
	}
	return count;
}
int FlyEngine::LoadResources(std::vector<std::wstring> resources)
{
	int count = 0;

	for (int i = 0; i < (int)resources.size(); i++)
	{
		//Load mesh objects
		SmartPtrStd<ImportedObjectData> raw;
		if(!ResourceImporter::ImportObject(resources[i].c_str(), D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), raw))
			return 0;
		count ++;
		//Create the object for rendering
		Object* model = new Object();
		Object::OBJECT_DESC desc;
		desc.device = D3DShell::self()->getDevice();
		desc.deviceContext = D3DShell::self()->getDeviceContext();
		desc.material_id = raw->objects[0].material;
		desc.shader = this->gbufferShader;
		desc.vertecies = raw->objects[0].vertex;
		desc.vCount = (int)raw->objects[0].vertex->size();
		if(!model->Initialize(desc))
			return 0;

		this->objects.push_back(model);
	}
	
	return count;
}


void FlyEngine::Resize(int width, int height)
{
	D3DShell::self()->resizeViewport((UINT)width, (UINT)height);
}
SmartPtrStd<std::vector<SmartPtrStd<Entity>>> FlyEngine::GetEntityList()
{
	SmartPtrStd<std::vector<SmartPtrStd<Entity>>> ret = &this->objects;
	
	return ret;
}
Entity* FlyEngine::PickObject(int x, int y)
{

	return NULL;
}


