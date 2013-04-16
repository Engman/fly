#include "Application.h"
#include "..\Util\vertex.h"



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
	Point2D size(800, 600);
	if(!InitWindow(hInst, size))	return false;
	if(!InitD3D(size))				return false;
	if(!InitInput())				return false;
	if(!InitGBuffers())				return false;
	if(!InitColorShader())			return false;
	if(!InitMatrixBuffer())			return false;


	this->mainCamera.SetProjectionMatrix((float)D3DX_PI/2.0f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->mainCamera.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 1, 1000);
	this->mainCamera.SetPosition(0.0f, 0.0f, 0.0f);
	this->mainCamera.SetRotation(0.0f, 0.0f, 0.0f);

	D3DXMATRIX world; 
	D3DXMatrixIdentity(&world);

	g_plane = new Plane();
	g_plane->Initialize(world, 2, 2, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

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

bool Application::Render()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	
	IShader::SHADER_PARAMETER_DATA gBufferDrawData;

	cBufferMatrix* dataPtr = (cBufferMatrix*)(this->pMatrixBuffer->Map());
	dataPtr->world = world;
	D3DXMatrixLookAtLH(&dataPtr->view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixOrthoLH(&dataPtr->projection, 800, 600, 1.0f, 100.0f);

	D3DXMatrixTranspose(&dataPtr->world, &dataPtr->world);
	D3DXMatrixTranspose(&dataPtr->projection,& dataPtr->projection);
	D3DXMatrixTranspose(&dataPtr->view,&dataPtr->view);
	dataPtr->worldInvTranspose = world;
	this->pMatrixBuffer->Unmap();
	gBufferDrawData.cMatrixBuffer = this->pMatrixBuffer;
	gBufferDrawData.dc = D3DShell::self()->getDeviceContext();
	
	D3DShell::self()->BeginGBufferRenderTargets();
	g_plane->SetShader(&this->gBufferShader);
	g_plane->Render(D3DShell::self()->getDeviceContext());
	this->gBufferShader.draw(gBufferDrawData);

	 D3DShell::self()->setRenderTarget();
	 D3DShell::self()->beginScene();
	 FLAGS::STATE_SAMPLING samp[1] =  { FLAGS::SAMPLER_Linear };
	 D3DShell::self()->setSamplerState(samp, FLAGS::PS, 0,1);
	 this->g_plane->SetShader(&g_colorShader);
	 this->g_plane->Render(D3DShell::self()->getDeviceContext());
	 this->g_colorShader.draw(gBufferDrawData);

	 D3DShell::self()->endScene();

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
	gBufferDesc.VSFilename = L"../Resources/Shaders/deferredShaderVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/deferredShaderPS.ps";
	//gBufferDesc.VSFilename = L"../Resources/Shaders/colorVS.vs";
	//gBufferDesc.PSFilename = L"../Resources/Shaders/colorPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNC3_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	if(!this->gBufferShader.init(gBufferDesc))	
		return false;

	return true;
}
bool Application::InitColorShader()
{
	BaseShader::BASE_SHADER_DESC gBufferDesc;

	gBufferDesc.dc = D3DShell::self()->getDeviceContext();
	gBufferDesc.device = D3DShell::self()->getDevice();
	//gBufferDesc.VSFilename = L"../Resources/Shaders/deferredShaderVS.vs";
	//gBufferDesc.PSFilename = L"../Resources/Shaders/deferredShaderPS.ps";
	gBufferDesc.VSFilename = L"../Resources/Shaders/colorVS.vs";
	gBufferDesc.PSFilename = L"../Resources/Shaders/colorPS.ps";
	gBufferDesc.shaderVersion = D3DShell::self()->getSuportedShaderVersion();
	gBufferDesc.polygonLayout = VERTEX::VertexPNC3_InputElementDesc;
	gBufferDesc.nrOfElements = 3;

	if(!this->g_colorShader.init(gBufferDesc))	
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


