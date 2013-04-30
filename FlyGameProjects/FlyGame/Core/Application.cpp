#include "Application.h"
#include "..\Util\vertex.h"
#include "..\Util\Importer\ResourceImporter.h"
#include "Mesh\MaterialHandler.h"
#include "Mesh\Object.h"



Application::Application()
{
	//Initiate singeltons
	WindowShell::self();
	D3DShell::self();
	Input::self();
}

Application::Application(const Application& other)
{
	
}

Application::~Application()
{

}

bool Application::Initialize(HINSTANCE hInst, int width, int height)
{
	ID3D11Device* dev = D3DShell::self()->getDevice();
	Point2D size(width, height);
	if(!InitWindow(hInst, size))	return false;
	if(!InitD3D(size, WindowShell::self()->getHWND()))		return false;
	if(!InitInput())				return false;
	if(!InitGBuffers())				return false;
	if(!InitLightShader())			return false;
	if(!InitColorShader())			return false;
	if(!InitMatrixBuffer())			return false;
	if(!LoadResources())			return false;


	this->mainCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->mainCamera.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 1, 1000);
	this->mainCamera.SetPosition(0.0f, 0.0f, 0.0f);
	this->mainCamera.SetRotation(0.0f, 0.0f, 0.0f);

	initTestData();

	this->flyCamera.setLookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
	this->flyCamera.setPerspective((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1.0f, 1000.0f);

	return true;
}
bool Application::Initialize(HWND hwnd, int width, int height)
{
	Point2D size(width, height);
	if(!InitD3D(size, hwnd))		return false;
	if(!InitInput())				return false;
	if(!InitGBuffers())				return false;
	if(!InitLightShader())			return false;
	if(!InitColorShader())			return false;
	if(!InitMatrixBuffer())			return false;
	if(!LoadResources())			return false;


	this->mainCamera.SetProjectionMatrix((float)D3DX_PI*0.2f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->mainCamera.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 1, 1000);
	this->mainCamera.SetPosition(-20.0f, 80.0f, -200.0f);
	this->mainCamera.SetRotation(0.0f, 0.0f, 0.0f);

	initTestData();

	return true;
}


void Application::Run()
{
	MSG msg = {0};

	while(WM_QUIT != msg.message)
	{
		 if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		  {
			   TranslateMessage( &msg );
			   DispatchMessage( &msg );
		  }
		 else
		 {
			Update();
			Render();
		 }

	}
}
bool Application::Frame()
{
	Update();
	Render();

	return true;
}

void Application::Shutdown()
{
	WindowShell::self()->destroy();
	D3DShell::self()->destroy();
	Input::self()->destroy();
}

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_INPUT:
			Input::self()->proccessRawDeviceData(lParam);
		break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
		break;

		default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



void Application::KeyPressEvent(Input::KeyCodes::Key k)
{
	if(k == Key::K_Escape)
		PostQuitMessage(0);
	
	else if(k == Key::K_S)
		this->flyCamera.Move(-1.0f);
	else if(k == Key::K_W)
		this->flyCamera.Move(1.0f);
	else if(k == Key::K_A)
		this->flyCamera.Strafe(-1.0f);
	else if(k == Key::K_D)
		this->flyCamera.Strafe(1.0f);
	else if(k == Key::K_Ctrl)
		this->flyCamera.Ascend(-1.0f);
	else if(k == Key::K_Space)

	this->flyCamera.Ascend(1.0f);
}
void Application::MouseMoveEvent(Input::MouseMoveData d)
{
	this->flyCamera.Yaw((float)D3DXToRadian(d.relativeX*0.3)); //* this->gtime->DeltaTime() * 150);
	this->flyCamera.Pitch((float)D3DXToRadian(d.relativeY*0.3)); //* this->gtime->DeltaTime() * 150);
}


void Application::Update()
{
	//g_cube->Update();

	D3DXVECTOR3 rot; 
	rot = this->objects[0]->getRotation();
	rot.x += 0.003f;
	this->objects[0]->setRotation(rot);

	this->flyCamera.updateView();

}
bool Application::Render()
{
	this->mainCamera.Render();
	DeferedRendering();

	return true;
}
void Application::DeferedRendering()
{
	D3DShell::self()->BeginGBufferRenderTargets();

	IShader::PER_FRAME_DATA gBufferDrawData;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	gBufferDrawData.view = this->flyCamera.getView();
	gBufferDrawData.projection = this->flyCamera.getProj();



//#################################//
//########## G-buffers ############//
//#################################//


	//g_plane->Render(D3DShell::self()->getDeviceContext());
	//g_cube->Render(D3DShell::self()->getDeviceContext());

	for (int i = 0; i <(int)this->objects.size(); i++)
	{
		this->objects[i]->Render();
	}
	this->gBufferShader.draw(gBufferDrawData);

	//set light render target and give it the albedo, normal and specular textures
	D3DShell::self()->BeginLightRenderTarget();
	float blend[4] = {0.75f,0.75f,0.75f,1.0f};
	//D3DShell::self()->setBlendModeState(FLAGS::BLEND_MODE_Custom,blend, NULL);


	D3DShell::self()->getDeviceContext()->OMSetBlendState(0,0,0xffffffff);
	
	ID3D11BlendState* Transparency;
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof(rtbd) );

	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend				 = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	D3DShell::self()->getDevice()->CreateBlendState(&blendDesc, &Transparency);

	D3DShell::self()->getDeviceContext()->OMSetBlendState(Transparency, blend, 0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_DisabledDepth,1); 

	//BaseBuffer * dirLights = g_lightHolder->getDirLights();
	g_lightHolder->setCamViewBuffer(this->mainCamera.GetViewMatrix(),this->mainCamera.GetProjectionMatrix(), this->mainCamera.GetPosition() );
	
	gBufferDrawData.camForLight = g_lightHolder->getCamViewBuffer();
	//for(int i=0; i<g_lightHolder->getNrOfDirLight();i++)
	//{
	//	gBufferDrawData.lights = g_lightHolder->getDirLight(i);
	//	//plane vertices, change so that lights hold geometry
	//	this->g_dirLight->Render(D3DShell::self()->getDeviceContext());
	//	this->g_dirLightShader.draw(gBufferDrawData);
	//}

	//gBufferDrawData.projection = this->mainCamera.GetOrthogonalMatrix();
	
	
	gBufferDrawData.lights = g_lightHolder->getPointLight(1);
	this->g_cube->Render(D3DShell::self()->getDeviceContext());
	this->g_pointLightShader.draw(gBufferDrawData);

	gBufferDrawData.lights = g_lightHolder->getPointLight(0);
	this->g_cube2->Render(D3DShell::self()->getDeviceContext());
	this->g_pointLightShader.draw(gBufferDrawData);

	//D3DShell::self()->setBlendModeState(FLAGS::BLEND_MODE_DisabledBlend, blend, NULL);

	D3DShell::self()->getDeviceContext()->OMSetBlendState(0,0,0xffffffff);
	D3DShell::self()->setDepthStencilState(FLAGS::DEPTH_STENCIL_EnabledDepth,1); 


//##################################################################//
//------------- Final pass, add light and color together ----------//
//##################################################################//
	//second render stage
	//sampling from g-buffers
	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();
	//dir light
	this->g_FullscreenQuad->Render(D3DShell::self()->getDeviceContext());
	this->g_colorShader.draw(gBufferDrawData);
	
	D3DShell::self()->releaseSRV();
	D3DShell::self()->endScene();
}



bool Application::LoadResources()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(L"..\\Resources\\Models\\*.fgm", &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		DisplayText("No files in [Model] directory.");
		return false;
	}
	else
	{
		vector<std::wstring> models;
		std::wstring tempFirst = L"..\\Resources\\Models\\"; 
		tempFirst.append(FindFileData.cFileName);
		models.push_back(tempFirst);

		while ( FindNextFile(hFind, &FindFileData) )
		{
			std::wstring file = L"..\\Resources\\Models\\"; 
			file.append(FindFileData.cFileName);
			models.push_back(file);
		}

		for (int i = 0; i < (int)models.size(); i++)
		{
			//Load mesh objects
			SmartPtrStd<ImportedObjectData> raw;
			if(!ResourceImporter::ImportObject(models[i].c_str(), D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), raw))
				return false;

			//Create the object for rendering
			Object *model = new Object();
			Object::OBJECT_DESC desc;
			desc.device = D3DShell::self()->getDevice();
			desc.deviceContext = D3DShell::self()->getDeviceContext();
			desc.material_id = raw->objects[0].material;
			desc.shader = &this->gBufferShader;
			desc.vertecies = raw->objects[0].vertex;
			desc.vCount = (int)raw->objects[0].vertex->size();
			if(!model->Initialize(desc))
				return false;

			this->objects.push_back(model);
		}
	}
	return true;
}



bool Application::InitD3D(Point2D size, HWND hWnd)
{
	D3DShell::D3D_INIT_DESC desc;

	desc.height = size.y;
	desc.width = size.x;
	desc.hwnd = hWnd;
	desc.MSAA = false;
	desc.MSAASampleCount = 4;
	desc.vSync = false;
	desc.fullScreen = false;

	if(!D3DShell::self()->init(desc))
		return false;

	return true;

}
bool Application::InitWindow(HINSTANCE& hinst, Point2D size)
{
	WindowShell::INIT_DESC_WINDOW descWindow;

	descWindow.hInstance = hinst;
	descWindow.windowName = L"Test";
	descWindow.windowPosition = Point2D(50, 50);
	descWindow.windowProcCallback = WndProc;
	descWindow.windowSize = size;

	if(!WindowShell::self()->createWin(descWindow))
		return false;

	return true;
}
bool Application::InitInput()
{
	Input::GLARE_INPUT_INIT_DESC d;

	d.target = WindowShell::self()->getHWND();
	d.deviceFlag = Input::Flags::NOLEGACY;
	d.deviceType = Input::Flags::keyboard;

	if(!Input::self()->registerInputDevice(d))
		return false;


	d.deviceType = Input::Flags::mouse;
	d.deviceFlag = Input::Flags::DAFAULT;
	if(!Input::self()->registerInputDevice(d))
		return false;

	Input::self()->subscribeKeyDown<Application>(this, &Application::KeyPressEvent);
	Input::self()->subscribeKeyUp<Application>(this, &Application::KeyPressEvent);
	Input::self()->subscribeMouseBtnDown<Application>(this, &Application::KeyPressEvent);
	Input::self()->subscribeMouseBtnUp<Application>(this, &Application::KeyPressEvent);
	Input::self()->subscribeMouseMove<Application>(this, &Application::MouseMoveEvent);

	return true;
}
bool Application::InitGBuffers()
{
	BaseShader::BASE_SHADER_DESC gBufferDesc;

	gBufferDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferDesc.device = D3DShell::self()->getDevice();
	gBufferDesc.VSFilename = L"../Resources/Shaders/g-BufferVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/g-BufferPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNT_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	if(!this->gBufferShader.init(gBufferDesc))	
		return false;

	return true;
}
bool Application::InitLightShader()
{
	BaseShader::BASE_SHADER_DESC lightShaderDesc;

	lightShaderDesc.dc = D3DShell::self()->getDeviceContext();
	lightShaderDesc.device = D3DShell::self()->getDevice();
	lightShaderDesc.VSFilename = L"../Resources/Shaders/dirLightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/dirLightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	lightShaderDesc.nrOfElements = 2;

	if(!this->g_dirLightShader.init(lightShaderDesc))	
		return false;


	lightShaderDesc.VSFilename = L"../Resources/Shaders/pointLightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/pointLightPS.ps";
	if(!this->g_pointLightShader.init(lightShaderDesc))	
		return false;

	return true;
}
bool Application::InitColorShader()
{
	BaseShader::BASE_SHADER_DESC colorShaderDesc;

	colorShaderDesc.dc = D3DShell::self()->getDeviceContext();
	colorShaderDesc.device = D3DShell::self()->getDevice();
	//colorShaderDesc.VSFilename = L"../Resources/Shaders/colorVS.vs";
	//colorShaderDesc.PSFilename = L"../Resources/Shaders/colorPS.ps";
	colorShaderDesc.VSFilename = L"../Resources/Shaders/TextLightVS.vs";
	colorShaderDesc.PSFilename = L"../Resources/Shaders/TextLightPS.ps";
	colorShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	colorShaderDesc.polygonLayout = VERTEX::VertexPT_InputElementDesc;
	colorShaderDesc.nrOfElements = 2;

	if(!this->g_colorShader.init(colorShaderDesc))	
		return false;

	return true;
}
bool Application::InitMatrixBuffer()
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


//IShader::PER_FRAME_DATA Application::getWVPBuffer()
//{
//	IShader::PER_FRAME_DATA gBufferDrawData;
//	gBufferDrawData.lights = g_lightHolder->getDirLights();
//	cBufferMatrix* dataPtr = (cBufferMatrix*)(this->pMatrixBuffer->Map());
//	D3DXMATRIX world;
//	D3DXMatrixIdentity(&world); //game world
//	dataPtr->world = world;
//
//	D3DXMatrixLookAtLH(&dataPtr->view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
//	D3DXMatrixPerspectiveFovLH(&dataPtr->projection,(float)D3DX_PI * 0.45f, 800/600, 0.1f, 100.0f);
//
//	D3DXMatrixTranspose(&dataPtr->world, &dataPtr->world);
//	D3DXMatrixTranspose(&dataPtr->view,&dataPtr->view);
//	D3DXMatrixTranspose(&dataPtr->projection,&dataPtr->projection);
//	dataPtr->worldInvTranspose = world;
//
//
//	this->pMatrixBuffer->Unmap();
//	gBufferDrawData.cMatrixBuffer = this->pMatrixBuffer;
//	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
//	return gBufferDrawData;
//}

void Application::initTestData()
{
	//test values -----------------
	D3DXMATRIX world; 
	D3DXMatrixIdentity(&world);

	//g_plane = new Plane();
	//g_plane->Initialize(world, 2.0f, 2.0f, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

	g_FullscreenQuad = new FullScreenQuad();
	g_FullscreenQuad->Initialize( D3DShell::self()->getDevice(), &g_colorShader);


	//draw a plane over the screen with the lightShader
	g_dirLight = new FullScreenQuad();
	g_dirLight->Initialize( D3DShell::self()->getDevice(), &g_dirLightShader);


	g_cube = new Cube();
	D3DXMatrixTranslation(&world, -20.0f, 2.0f, 50.0f);
	g_cube->Initialize(world, 50.0f, 50.0f, 0.0f,  D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &g_pointLightShader);


	g_cube2 = new Cube();
	D3DXMatrixTranslation(&world, 50.0f, 2.0f, 100.0f);
	g_cube2->Initialize(world, 50.0f, 50.0f, 0.0f,  D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &g_pointLightShader);


	g_lightHolder = new LightHolder();
	g_lightHolder->Initialize();
	DirectionalLightProxy dirLight;
	dirLight.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	dirLight.diffuse	= D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	dirLight.specular	= D3DXVECTOR4(0,0,1,1);
	D3DXVECTOR4 dir(1,-3,-1,0);
	D3DXVec4Normalize(&dir, &dir); 
	dirLight.direction= dir;
	g_lightHolder->addLight(dirLight);

	DirectionalLightProxy dirLight2;
	dirLight2.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	dirLight2.diffuse	= D3DXVECTOR4(1.0f,0.2f,0.2f,1);
	dirLight2.specular	= D3DXVECTOR4(0,0,1,1);
	dir = D3DXVECTOR4(1,3,1,0);
	D3DXVec4Normalize(&dir, &dir); 
	dirLight2.direction= dir;
	g_lightHolder->addLight(dirLight2);
	//-----------------------------------


	PointLightProxy pointLight;
	pointLight.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	pointLight.diffuse	= D3DXVECTOR4(0.2f,0.5f,0.7f,1);
	pointLight.specular	= D3DXVECTOR4(0,0,1,1);
	pointLight.posRange = D3DXVECTOR4( -20.0f, 2.0f, 50.0f, 100);
	pointLight.attenuate = D3DXVECTOR4(2,2,2,2);
	g_lightHolder->addLight(pointLight);

	pointLight.ambient	= D3DXVECTOR4(0.2f,0.2f,0.2f,1);
	pointLight.diffuse	= D3DXVECTOR4(1.0f,0.2f,0.2f,1);
	pointLight.specular	= D3DXVECTOR4(0,0,1,1);
	pointLight.posRange = D3DXVECTOR4( 50.0f, 2.0f, 50.0f, 100);
	pointLight.attenuate = D3DXVECTOR4(2,2,2,2);
	g_lightHolder->addLight(pointLight);


	this->objects[0]->setPosition(D3DXVECTOR3(-50,0,50));
	this->objects[1]->setPosition(D3DXVECTOR3( 20,0,50));
}



//void Application::DeferedRendering()
//{
//
//	FLAGS::STATE_SAMPLING samp[1] =  { FLAGS::SAMPLER_Linear };
//	D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0,1);
//
//	IShader::PER_FRAME_DATA gBufferDrawData;
//	gBufferDrawData = getWVPBuffer();
//
//	//--------G-buffers---------//
//	//render plane
//	D3DShell::self()->BeginGBufferRenderTargets();
//	g_plane->Render(D3DShell::self()->getDeviceContext());
//	this->gBufferShader.draw(gBufferDrawData);
//
//	//reset the world matrix
//	gBufferDrawData  = getWVPBuffer();
//
//	//render cube to g-buffer
//	g_cube->Render(D3DShell::self()->getDeviceContext());
//	this->gBufferShader.draw(gBufferDrawData);
//
//	//reset the world matrix
//	gBufferDrawData  = getWVPBuffer();
//
//
//
//	//----------Final pass add light and color together------//
//	//second render stage
//	//sampling from g-buffers
//	D3DShell::self()->setRenderTarget();
//	D3DShell::self()->beginScene();
//	//dir light
//	this->g_FullscreenQuad->Render(D3DShell::self()->getDeviceContext());
//	this->g_colorShader.draw(gBufferDrawData);
//	
//	D3DShell::self()->endScene();
//	D3DShell::self()->releaseSRV();
//}

