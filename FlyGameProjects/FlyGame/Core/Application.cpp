#include "Application.h"
#include "..\Util\vertex.h"
#include "..\Util\Importer\ResourceImporter.h"
#include "Mesh\MaterialHandler.h"



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

bool Application::Initialize(HINSTANCE hInst)
{
	ID3D11Device* dev = D3DShell::self()->getDevice();
	Point2D size(800, 600);
	if(!InitWindow(hInst, size))	return false;
	if(!InitD3D(size))				return false;
	if(!InitInput())				return false;
	if(!InitGBuffers())				return false;
	if(!InitColorShader())			return false;
	if(!InitMatrixBuffer())			return false;
	if(!LoadResources())			return false;


	this->mainCamera.SetProjectionMatrix((float)D3DX_PI/2.0f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->mainCamera.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 1, 1000);
	this->mainCamera.SetPosition(0.0f, 0.0f, 0.0f);
	this->mainCamera.SetRotation(0.0f, 0.0f, 0.0f);

	initTestData();


	this->mainCamera.GetViewFrustum();

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
	static int count = 0;
	count ++;

	if(k == Key::K_Escape)
		PostQuitMessage(0);
	//bool removed = Input::self()->unsubscribeKeyDown<Application>		(&Application::KeyPressEvent);
	//removed = Input::self()->unsubscribeKeyUp<Application>		(&Application::KeyPressEvent);
	//removed = Input::self()->unsubscribeMouseBtnDown<Application>	(&Application::KeyPressEvent);
	//removed = Input::self()->unsubscribeMouseBtnUp<Application>	(&Application::KeyPressEvent);
	//removed = Input::self()->unsubscribeMouseMove<Application>	(&Application::MouseMoveEvent);
}
void Application::MouseMoveEvent(Input::MouseMoveData d)
{
	static int idCounter = 0;
	idCounter ++;
}
void Application::Update()
{
	g_cube->Update();
}
bool Application::Render()
{

	DeferedRendering();

	return true;
}


bool Application::InitD3D(Point2D size)
{
	D3DShell::D3D_INIT_DESC desc;

	desc.height = size.y;
	desc.width = size.x;
	desc.hwnd = WindowShell::self()->getHWND();
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
	d.deviceFlag = Input::Flags::DAFAULT;
	d.deviceType = Input::Flags::keyboard;

	if(!Input::self()->registerInputDevice(d))
		return false;


	d.deviceType = Input::Flags::mouse;
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
	gBufferDesc.VSFilename = L"../Resources/Shaders/deferredShaderTextVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/deferredShaderTextPS.ps";
	//gBufferDesc.VSFilename = L"../Resources/Shaders/colorVS.vs";
	//gBufferDesc.PSFilename = L"../Resources/Shaders/colorPS.ps";
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
	lightShaderDesc.VSFilename = L"../Resources/Shaders/LightVS.vs";
	lightShaderDesc.PSFilename = L"../Resources/Shaders/LightPS.ps";
	lightShaderDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	lightShaderDesc.polygonLayout = VERTEX::VertexPNC3_InputElementDesc;
	lightShaderDesc.nrOfElements = 3;

	if(!this->g_lightShader.init(lightShaderDesc))	
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
	colorShaderDesc.polygonLayout = VERTEX::VertexPNC3_InputElementDesc;
	colorShaderDesc.nrOfElements = 3;

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

IShader::SHADER_PARAMETER_DATA Application::getWVPBuffer()
{
	IShader::SHADER_PARAMETER_DATA gBufferDrawData;
	gBufferDrawData.lights = g_lightHolder->getDirLights();
	cBufferMatrix* dataPtr = (cBufferMatrix*)(this->pMatrixBuffer->Map());
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world); //game world
	dataPtr->world = world;

	D3DXMatrixLookAtLH(&dataPtr->view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&dataPtr->projection,(float)D3DX_PI * 0.45f, 800/600, 0.1f, 100.0f);

	D3DXMatrixTranspose(&dataPtr->world, &dataPtr->world);
	D3DXMatrixTranspose(&dataPtr->view,&dataPtr->view);
	D3DXMatrixTranspose(&dataPtr->projection,&dataPtr->projection);
	dataPtr->worldInvTranspose = world;


	this->pMatrixBuffer->Unmap();
	gBufferDrawData.cMatrixBuffer = this->pMatrixBuffer;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	return gBufferDrawData;
}
void Application::initTestData()
{
	//test values -----------------
	D3DXMATRIX world; 
	D3DXMatrixIdentity(&world);

	g_plane = new Plane();
	g_plane->Initialize(world, 2, 2, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

	g_FullscreenQuad = new FullScreenQuad();
	g_FullscreenQuad->Initialize( D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &g_colorShader);

	g_cube = new Cube();
	D3DXMatrixTranslation(&world, 2,2,0);
	g_cube->Initialize(world, 2, 2, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

	g_lightHolder = new LightHolder();
	g_lightHolder->Initialize();
	DirectionalLightProxy dirLight;
	dirLight.ambient = D3DXVECTOR4(1,1,1,1);
	dirLight.diffuse= D3DXVECTOR4(1,1,1,1);
	dirLight.specular= D3DXVECTOR4(0.2,0.2,0.2, 0);
	D3DXVECTOR4 dir(0,-1,-1,0);
	D3DXVec4Normalize(&dir, &dir); 
	dirLight.direction= dir;
	g_lightHolder->addLight(dirLight);

	//-----------------------------------

}

bool Application::LoadResources()
{
	//Load mesh objects
	SmartPtrStd<ImportedObjectData> raw;
	if(!ResourceImporter::ImportObject(L"../Resources/Models/simplePlane.obj", raw))
		return false;

	
	//MaterialHandler::GetMaterial(raw->objects[0].material);

	return true;
}

void Application::DeferedRendering()
{

	FLAGS::STATE_SAMPLING samp[1] =  { FLAGS::SAMPLER_Linear };
	D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0,1);

	IShader::SHADER_PARAMETER_DATA gBufferDrawData;
	gBufferDrawData = getWVPBuffer();

	//--------G-buffers---------//
	//render plane
	D3DShell::self()->BeginGBufferRenderTargets();
	g_plane->Render(D3DShell::self()->getDeviceContext());
	this->gBufferShader.draw(gBufferDrawData);

	//reset the world matrix
	gBufferDrawData  = getWVPBuffer();

	//render cube to g-buffer
	g_cube->Render(D3DShell::self()->getDeviceContext());
	this->gBufferShader.draw(gBufferDrawData);

	//reset the world matrix
	gBufferDrawData  = getWVPBuffer();

	//----------Final pass add light and color together------//
	//second render stage
	//sampling from g-buffers
	D3DShell::self()->setRenderTarget();
	D3DShell::self()->beginScene();
	//dir light
	this->g_FullscreenQuad->Render(D3DShell::self()->getDeviceContext());
	this->g_colorShader.draw(gBufferDrawData);
	
	D3DShell::self()->endScene();
	D3DShell::self()->releaseSRV();
}
