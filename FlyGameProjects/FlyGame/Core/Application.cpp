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
	if(!InitColorShader())			return false;
	if(!InitMatrixBuffer())			return false;
	if(!LoadResources())			return false;


	this->mainCamera.SetProjectionMatrix((float)D3DX_PI/2.0f, D3DShell::self()->getAspectRatio(), 1, 1000);
	this->mainCamera.SetOrthogonalMatrix(D3DShell::self()->getWidth(), D3DShell::self()->getHeight(), 1, 1000);
	this->mainCamera.SetPosition(0.0f, 0.0f, 0.0f);
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
	//g_cube->Update();
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
	gBufferDrawData.view = this->mainCamera.GetViewMatrix();
	gBufferDrawData.projection = this->mainCamera.GetProjectionMatrix();



//#################################//
//########## G-buffers ############//
//#################################//


	//g_plane->Render(D3DShell::self()->getDeviceContext());
	g_cube->Render(D3DShell::self()->getDeviceContext());

	for (int i = 0; i <(int)this->objects.size(); i++)
	{
		//this->objects[i]->Render();
	}
	this->gBufferShader.draw(gBufferDrawData);

	


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
	HANDLE hFind = FindFirstFile(L"..\\Resources\\Models\\*.obj", &FindFileData);
	
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
			if(!ResourceImporter::ImportObject(models[i].c_str(), D3DShell::self()->getDevice(), raw))
				return false;

			//Create the object for rendering
			Object *model = new Object();
			Object::OBJECT_DESC desc;
			desc.device = D3DShell::self()->getDevice();
			desc.deviceContext = D3DShell::self()->getDeviceContext();
			desc.material_id = MaterialHandler::GetMaterial(raw->objects[0].material)->GetID();
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
	lightShaderDesc.polygonLayout = VERTEX::VertexPNC_InputElementDesc;
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
	colorShaderDesc.polygonLayout = VERTEX::VertexPNC_InputElementDesc;
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
void Application::initTestData()
{
	//test values -----------------
	D3DXMATRIX world; 
	D3DXMatrixIdentity(&world);

	g_plane = new Plane();
	g_plane->Initialize(world, 2.0f, 2.0f, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

	g_FullscreenQuad = new FullScreenQuad();
	g_FullscreenQuad->Initialize( D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &g_colorShader);

	g_cube = new Cube();
	D3DXMatrixTranslation(&world, 2.0f, 2.0f, 0.0f);
	g_cube->Initialize(world, 2.0f, 2.0f, D3DShell::self()->getDevice(), D3DShell::self()->getDeviceContext(), &gBufferShader);

	g_lightHolder = new LightHolder();
	g_lightHolder->Initialize();
	DirectionalLightProxy dirLight;
	dirLight.ambient = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight.diffuse= D3DXVECTOR4(1.0f ,1.0f, 1.0f , 1.0f);
	dirLight.specular= D3DXVECTOR4(0.2f ,0.2f ,0.2f , 0.0f);
	D3DXVECTOR4 dir(0.0f, -1.0f, -1.0f, 0.0f);
	D3DXVec4Normalize(&dir, &dir); 
	dirLight.direction= dir;
	g_lightHolder->addLight(dirLight);



	g_terrain = new Terrain();
	Object::OBJECT_DESC terrainDesc;
	terrainDesc.vCount = 0;
	terrainDesc.vertecies = new vector<VERTEX::VertexPNT>;
	g_terrain->Initialize(terrainDesc);

	//-----------------------------------
}