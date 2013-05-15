#include "D3DShell.h"



using namespace ShaderStates;

/* Private data for D3DShell */
struct D3DShell::PrDat
{
	static D3DShell					*d3dShellInstance;

	ID3D11Device					*d3dDevice;
	ID3D11DeviceContext				*d3dDeviceContext;
	IDXGISwapChain					*swapChain;
	ID3D11RenderTargetView			*renderTargetView;
	ID3D11DepthStencilView			*depthStencilView;
	ID3D11Texture2D					*depthStencilBuffer;
	D3D11_VIEWPORT					screenViewport;
	D3D_DRIVER_TYPE					driverType;
	D3D_FEATURE_LEVEL				featurelevel;

	DrawableTexture g_buffTextures [DeferredRenderLayout::MRT_COUNT];
	DrawableTexture g_bufferDepthTexture;
	
	DrawableTexture lightTexture;
	DrawableTexture shadowTexture;
	DrawableTexture blurTempTexture;
	DrawableTexture blurTexture;

	//States
	RasterizerState					rasterizerState;
	DepthStencilState				depthStencilState;
	SamplerState					samplerState;
	BlendStates						blendModeState;

	UINT							width;
	UINT							heigth;

	bool vSync;
	int videoCardMemory;

	unsigned int MSAAQuality;
	int MSAASampleCount;
	bool MSAAEnabled;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX projectionMatrix;

	PrDat()
		:d3dDevice(0), d3dDeviceContext(0), swapChain(0), renderTargetView(0), depthStencilView(0), depthStencilBuffer(0), 
		featurelevel(D3D_FEATURE_LEVEL_11_0), vSync(0), videoCardMemory(0), MSAAQuality(0), MSAASampleCount(4), MSAAEnabled(0)/*,
		deffRTV(0), deffTex(0), deffDepthStencil(0), deffTexDepth(0), width(800), heigth(600), deffSRV(0)*/
	{

		this->driverType	= D3D_DRIVER_TYPE_HARDWARE;

		this->screenViewport.Width		= 800.0f;
		this->screenViewport.Height		= 600.0f;
		this->screenViewport.MaxDepth	= 1.0f;
		this->screenViewport.MinDepth	= 0.0f;
		this->screenViewport.TopLeftX	= 0.0f;
		this->screenViewport.TopLeftY	= 0.0f;

		D3DXMatrixIdentity(&this->worldMatrix);
		D3DXMatrixIdentity(&this->projectionMatrix);

	}
	~PrDat()
	{
		if(this->d3dDevice)
			this->d3dDevice->Release();
		if(this->d3dDeviceContext)
			this->d3dDeviceContext->Release();
		if(this->swapChain)
			this->swapChain->Release();
		if(this->renderTargetView)
			this->renderTargetView->Release();
		if(this->depthStencilView)
			this->depthStencilView->Release();
		if(this->depthStencilBuffer)
			this->depthStencilBuffer->Release();


		this->d3dDevice				= NULL;
		this->d3dDeviceContext		= NULL;
		this->swapChain				= NULL;
		this->renderTargetView		= NULL;
		this->depthStencilView		= NULL;
		this->depthStencilBuffer	= NULL;
	
	}

};

D3DShell* D3DShell::PrDat::d3dShellInstance = NULL;



D3DShell::D3DShell()
{
	srand((unsigned)time(0));
	this->_prDatPtr = new PrDat();
}
D3DShell::~D3DShell()
{
	delete this->_prDatPtr;
}

bool D3DShell::init(D3D_INIT_DESC& desc)
{
	if(!desc.hwnd)
	{
		MessageBox(0, L"Could not complete initialization, no HWND specified!", L"Error", 0);
		return false;
	}
	if(desc.width < 100 || desc.height < 200)
	{
		desc.width = 800;
		desc.height = 600;
	}
	this->_prDatPtr->width = desc.width;
	this->_prDatPtr->heigth = desc.height;
	this->_prDatPtr->wndHandle = desc.hwnd;

					#pragma region Declarations


	this->_prDatPtr->MSAAEnabled = desc.MSAA;
	this->_prDatPtr->MSAASampleCount = desc.MSAASampleCount;
	this->_prDatPtr->vSync = desc.vSync;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	unsigned int  numerator;
	unsigned int  denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	//Drivers to test
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		this->_prDatPtr->driverType,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	//Featurelevel to support
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

			//Set debug flag
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	UINT CountFeratureLevel = sizeof(featureLevels) / sizeof(featureLevels[0]);
	UINT CountDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	HRESULT hr = S_OK;

					#pragma endregion

					#pragma region Graphic card information


	/* Create a DirectX graphics interface factory. */
	if( FAILED( CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) ) )
		return false;
	// Use the factory to create an adapter for the primary graphics interface (video card).
	if( FAILED( factory->EnumAdapters(0, &adapter) ) )
		return false;

	// Enumerate the primary adapter output (monitor).
	if( FAILED( adapter->EnumOutputs(0, &adapterOutput) ) )
		return false;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	if( FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL) ) )
		return false;

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	if(numModes > 0)
		displayModeList = new DXGI_MODE_DESC[numModes];
	else
		return false;

	// Now fill the display mode list structures.
	if( FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList) ) )
		return false;

	// Go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(int i = 0; i<(int)numModes; i++)
		if(displayModeList[i].Width == (unsigned int)desc.width)
			if(displayModeList[i].Height == (unsigned int)desc.height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				i = numModes; //Break loop 
			}
	

	// Get the adapter (video card) description.
	if( FAILED(adapter->GetDesc(&adapterDesc) ) )
		return false;

	// Store the dedicated video card memory in megabytes.
	this->_prDatPtr->videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

#pragma endregion

					#pragma region Device

	bool created = false;
	//Create D3D11Device And D3D11DeviceContext
	for(UINT i = 0; !created && i < CountDriverTypes; i++)
	{
		this->_prDatPtr->driverType = driverTypes[i];
		
		hr = D3D11CreateDevice	(
								NULL,
								this->_prDatPtr->driverType,
								NULL,
								createDeviceFlags,
								featureLevels,
								CountFeratureLevel,
								D3D11_SDK_VERSION,
								&this->_prDatPtr->d3dDevice,
								&this->_prDatPtr->featurelevel,
								&this->_prDatPtr->d3dDeviceContext);

		if( SUCCEEDED( hr ) )
		{
			created = true;
		}
	}


	if( FAILED(hr))
	{
		MessageBox(0, L"Failed to create D3D11Device!", L"Error", MB_OK);
		return false;
	}
//############################## END #####################################

					#pragma endregion

					#pragma region SwapChain
//########################################################################
//							Create SwapChain
//########################################################################

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	if(this->_prDatPtr->vSync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	}


	if( this->_prDatPtr->MSAAEnabled && !FAILED( this->_prDatPtr->d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, this->_prDatPtr->MSAASampleCount, &this->_prDatPtr->MSAAQuality)))
	{
		if(!this->_prDatPtr->MSAAQuality)
		{
			this->_prDatPtr->MSAAQuality = 0;
			this->_prDatPtr->MSAASampleCount = 1;
		}
		else
			this->_prDatPtr->MSAAQuality -= 1;
	}
	else
	{
		this->_prDatPtr->MSAAQuality = 0;
		this->_prDatPtr->MSAASampleCount = 1;
	}
	
	swapChainDesc.BufferDesc.Width					= desc.width;
	swapChainDesc.BufferDesc.Height					= desc.height;
	swapChainDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling				= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count					= this->_prDatPtr->MSAASampleCount;
	swapChainDesc.SampleDesc.Quality				= this->_prDatPtr->MSAAQuality;
	swapChainDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount						= 1;
	swapChainDesc.OutputWindow						= desc.hwnd;
	swapChainDesc.Windowed							= desc.fullScreen;
	swapChainDesc.SwapEffect						= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags								= 0;

				#pragma region SwapChain FrankLuna
	//						By Frank Luna (C) 2011
	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice* dxgiDevice = 0;
	if( FAILED(this->_prDatPtr->d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
	{
		MessageBox(0, L"error:\nd3dDevice->QueryInterface", L"error", MB_OK);
		return false;
	}
	      
	IDXGIAdapter* dxgiAdapter = 0;
	if( FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)))
	{
		MessageBox(0, L"error:\ndxgiDevice->GetParent", L"error", MB_OK);
		return false;
	}

	IDXGIFactory* dxgiFactory = 0;
	if( FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
	{
		MessageBox(0, L"error:\ndxgiAdapter->GetParent", L"error", MB_OK);
		return false;
	}

	if( FAILED(dxgiFactory->CreateSwapChain(this->_prDatPtr->d3dDevice, &swapChainDesc, &this->_prDatPtr->swapChain)))
	{
		MessageBox(0, L"Failed to create swapchain", L"error", MB_OK);
		return false;
	}
	
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

					#pragma endregion 


					#pragma endregion
	
					#pragma region RenderTargetView

	//First we need to get the depthStencilBuffer from the swapchain (using our variable for the stencile buffer temporary)
	hr = this->_prDatPtr->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&this->_prDatPtr->depthStencilBuffer);
	if( FAILED(hr))
	{
		MessageBox(0, L"Failed to get swapchain buffer", L"Error", MB_OK);
		return false;
	}

	//Then we can create the renderTargetView
	hr = this->_prDatPtr->d3dDevice->CreateRenderTargetView(this->_prDatPtr->depthStencilBuffer, NULL, &this->_prDatPtr->renderTargetView);
	if(this->_prDatPtr->depthStencilBuffer)
		this->_prDatPtr->depthStencilBuffer->Release();	
	if( FAILED(hr))
	{
		MessageBox(0, L"Failed to create render target view", L"Error", MB_OK);
		return false;
	}


					#pragma endregion

					#pragma region DepthBuffer/StencileBuffer&View

	//Create a Depth-Stencil Resource
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width				= desc.width;
	depthStencilDesc.Height				= desc.height;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.SampleDesc.Count	= this->_prDatPtr->MSAASampleCount;
	depthStencilDesc.SampleDesc.Quality	= this->_prDatPtr->MSAAQuality;
	depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;


	//Create DepthStencileBuffer
	if( FAILED( this->_prDatPtr->d3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &this->_prDatPtr->depthStencilBuffer) ) )
	{
		MessageBox(0, L"Failed to create DepthStencileBuffer", L"Error", MB_OK);
		return false;
	}

	if( FAILED( this->_prDatPtr->d3dDevice->CreateDepthStencilView(this->_prDatPtr->depthStencilBuffer, NULL, &this->_prDatPtr->depthStencilView) ) )
	{
		MessageBox(0, L"Failed to create DepthStencileView", L"Error", MB_OK);
		return false;
	}

	//this->_prDatPtr->d3dDeviceContext->OMSetRenderTargets(1, &this->_prDatPtr->renderTargetView, this->_prDatPtr->depthStencilView);

					#pragma endregion

					#pragma region ViewPort

	this->_prDatPtr->screenViewport.Height = (float)desc.height;
	this->_prDatPtr->screenViewport.Width = (float)desc.width;
	this->_prDatPtr->screenViewport.TopLeftX = 0.0f;
	this->_prDatPtr->screenViewport.TopLeftY = 0.0f;
	this->_prDatPtr->screenViewport.MinDepth = (float)0.0f;
	this->_prDatPtr->screenViewport.MaxDepth = (float)1.0f;

	this->_prDatPtr->d3dDeviceContext->RSSetViewports(1, &this->_prDatPtr->screenViewport);

#pragma endregion
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

	if(!this->_prDatPtr->blendModeState.init(this->_prDatPtr->d3dDevice, &blendDesc))
		return false;
	if(!this->_prDatPtr->rasterizerState.init(this->_prDatPtr->d3dDevice))	  
		return false;
	if(!this->_prDatPtr->depthStencilState.init(this->_prDatPtr->d3dDevice)) 
		return false;
	if(!this->_prDatPtr->samplerState.init(this->_prDatPtr->d3dDevice))  
		return false;

	// init render targets
	for(int i=0 ; i<DeferredRenderLayout::MRT_COUNT; i++)
	{
		this->_prDatPtr->g_buffTextures[i].init(desc.width, desc.height, true, DXGI_FORMAT_R16G16B16A16_UNORM, false);
	}
	this->_prDatPtr->g_bufferDepthTexture.init(desc.width, desc.height, false, DXGI_FORMAT_R32_TYPELESS, true);

	this->_prDatPtr->lightTexture.init(desc.width, desc.height, true, DXGI_FORMAT_R16G16B16A16_UNORM, false );
	this->_prDatPtr->shadowTexture.init(desc.width, desc.height, false, DXGI_FORMAT_R32_TYPELESS, true);
	this->_prDatPtr->blurTexture.init(desc.width, desc.height, true, DXGI_FORMAT_R16G16B16A16_UNORM, false );
	this->_prDatPtr->blurTempTexture.init(desc.width, desc.height, true, DXGI_FORMAT_R16G16B16A16_UNORM, false );

	
	return true;
}

D3DShell* D3DShell::self()
{
	if(!D3DShell::PrDat::d3dShellInstance)
		D3DShell::PrDat::d3dShellInstance = new D3DShell();

	return D3DShell::PrDat::d3dShellInstance;
}
void  D3DShell::destroy()
{
	delete D3DShell::PrDat::d3dShellInstance;
	D3DShell::PrDat::d3dShellInstance = NULL;
}

ID3D11Device* D3DShell::getDevice() const
{
	return this->_prDatPtr->d3dDevice;
}
ID3D11DeviceContext* D3DShell::getDeviceContext() const
{
	return this->_prDatPtr->d3dDeviceContext;
}
IDXGISwapChain* D3DShell::getSwapChain() const
{
	return this->_prDatPtr->swapChain;
}
ID3D11RenderTargetView* D3DShell::getRenderTargetView() const
{
	return this->_prDatPtr->renderTargetView;
}
ID3D11DepthStencilView*	D3DShell::getDepthStencilView() const
{
	return this->_prDatPtr->depthStencilView;
}
D3D11_VIEWPORT*	D3DShell::getScreenViewport() const
{
	return &this->_prDatPtr->screenViewport;
}
D3D_DRIVER_TYPE* D3DShell::getDriverType() const
{
	return &this->_prDatPtr->driverType;
}
float D3DShell::getWidth() const
{
	return this->_prDatPtr->screenViewport.Width;
}
float D3DShell::getHeight() const
{
	return this->_prDatPtr->screenViewport.Height;
}
float D3DShell::getAspectRatio() const
{
	return (this->_prDatPtr->screenViewport.Width / this->_prDatPtr->screenViewport.Height);
}
D3DXMATRIX&	D3DShell::getProjectionMatrix() const
{
	return this->_prDatPtr->projectionMatrix;
}
int	D3DShell::getVideoCardMem() const
{
	return this->_prDatPtr->videoCardMemory;
}

HWND D3DShell::getWndHandle()const
{
	return this->_prDatPtr->wndHandle;
}

void D3DShell::resizeViewport(unsigned int width, unsigned int height)
{
	this->_prDatPtr->renderTargetView->Release();
	this->_prDatPtr->depthStencilView->Release();
	this->_prDatPtr->depthStencilBuffer->Release();


	// Resize the swap chain and recreate the render target view.
	if( FAILED ( this->_prDatPtr->swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
	{
		MessageBox(0, L"Failed to resize buffers", L"Error", 0);
		return;
	}
	
	ID3D11Texture2D* backBuffer;
	if( FAILED( this->_prDatPtr->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)) ) )
	{
		MessageBox(0, L"Failed to get back buffer from swachain", L"Error", 0);
		return;
	}
	if( FAILED( this->_prDatPtr->d3dDevice->CreateRenderTargetView(backBuffer, 0, &this->_prDatPtr->renderTargetView) ) )
	{
		MessageBox(0, L"Failed create render target view", L"Error", 0);
		return;
	}
	backBuffer->Release();

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = width;
	depthStencilDesc.Height    = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( this->_prDatPtr->MSAAEnabled )
	{
		depthStencilDesc.SampleDesc.Count   = this->_prDatPtr->MSAASampleCount;
		depthStencilDesc.SampleDesc.Quality = this->_prDatPtr->MSAAQuality;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	if( FAILED (this->_prDatPtr->d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &this->_prDatPtr->depthStencilBuffer) ) )
	{

	}
	if( FAILED (this->_prDatPtr->d3dDevice->CreateDepthStencilView(this->_prDatPtr->depthStencilBuffer, 0, &this->_prDatPtr->depthStencilView) ) )
	{

	}


	// Bind the render target view and depth/stencil view to the pipeline.
	this->_prDatPtr->d3dDeviceContext->OMSetRenderTargets(1, &this->_prDatPtr->renderTargetView, this->_prDatPtr->depthStencilView);
	

	// Set the viewport transform.

	this->_prDatPtr->screenViewport.TopLeftX = 0;
	this->_prDatPtr->screenViewport.TopLeftY = 0;
	this->_prDatPtr->screenViewport.Width    = static_cast<float>(width);
	this->_prDatPtr->screenViewport.Height   = static_cast<float>(height);
	this->_prDatPtr->screenViewport.MinDepth = 0.0f;
	this->_prDatPtr->screenViewport.MaxDepth = 1.0f;

	this->_prDatPtr->d3dDeviceContext->RSSetViewports(1, &this->_prDatPtr->screenViewport);
}

void D3DShell::beginScene()
{
	float cls[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// Clear the back buffer.
	this->_prDatPtr->d3dDeviceContext->ClearRenderTargetView(this->_prDatPtr->renderTargetView, cls);
    
	// Clear the depth buffer.
	this->_prDatPtr->d3dDeviceContext->ClearDepthStencilView(this->_prDatPtr->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return;
}
void D3DShell::beginScene(float r, float g, float b, float a)
{
	float cls[4] = { r, g, b, a};
	// Clear the back buffer.
	this->_prDatPtr->d3dDeviceContext->ClearRenderTargetView(this->_prDatPtr->renderTargetView, cls);
    
	// Clear the depth buffer.
	this->_prDatPtr->d3dDeviceContext->ClearDepthStencilView(this->_prDatPtr->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return;
}
void D3DShell::endScene()
{
	if(this->_prDatPtr->vSync)
	{
		// Lock to screen refresh rate.
		this->_prDatPtr->swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		this->_prDatPtr->swapChain->Present(0, 0);
	}
}

void D3DShell::setViewport(D3D11_VIEWPORT* v, UINT nrOfViewports)
{
	this->_prDatPtr->d3dDeviceContext->RSSetViewports(nrOfViewports, v);
}
void D3DShell::setViewport()
{
	this->_prDatPtr->d3dDeviceContext->RSSetViewports(1, &this->_prDatPtr->screenViewport);
}
void D3DShell::setRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, UINT nrOfViews)
{
	this->_prDatPtr->d3dDeviceContext->OMSetRenderTargets(nrOfViews, &rtv, dsv);
}
void D3DShell::setRenderTarget()
{
	this->_prDatPtr->d3dDeviceContext->OMSetRenderTargets(1, &this->_prDatPtr->renderTargetView, this->_prDatPtr->depthStencilView);
}

void D3DShell::setRasterizerState(FLAGS::STATE_RASTERIZER state)
{
	this->_prDatPtr->d3dDeviceContext->RSSetState(this->_prDatPtr->rasterizerState.getState(state));
}
void D3DShell::setBlendModeState(FLAGS::STATE_BLEND_MODE state, float blend[4], UINT SampleMask)		 
{
	
	this->_prDatPtr->d3dDeviceContext->OMSetBlendState(this->_prDatPtr->blendModeState.getState(state), blend, SampleMask );
	//this->_prDatPtr->d3dDeviceContext->OMSetBlendState(NULL, blend, SampleMask );
}
void D3DShell::setSamplerState(FLAGS::STATE_SAMPLING *state, FLAGS::SHADERS stage, UINT startSlot, UINT count )			 
{
	if(!count)
		return;

	std::vector<ID3D11SamplerState*> samplers(count);
	for (UINT i = 0; i < count; i++)
		samplers[i] = this->_prDatPtr->samplerState.getState(state[i]);

	switch (stage)
	{
	case FLAGS::VS:
		this->_prDatPtr->d3dDeviceContext->VSSetSamplers(startSlot, count, &samplers[0]);
		break;
	case FLAGS::PS:
		this->_prDatPtr->d3dDeviceContext->PSSetSamplers(startSlot, count, &samplers[0]);
		break;
	case FLAGS::HS:
		this->_prDatPtr->d3dDeviceContext->HSSetSamplers(startSlot, count, &samplers[0]);
		break;
	case FLAGS::GS:
		this->_prDatPtr->d3dDeviceContext->GSSetSamplers(startSlot, count, &samplers[0]);
		break;
	case FLAGS::DS:
		this->_prDatPtr->d3dDeviceContext->DSSetSamplers(startSlot, count, &samplers[0]);
		break;
	}
}
void D3DShell::setDepthStencilState(FLAGS::STATE_DEPTH_STENCIL state, UINT stencilRef)	 
{
	this->_prDatPtr->d3dDeviceContext->OMSetDepthStencilState(this->_prDatPtr->depthStencilState.getState(state), stencilRef);
}

char* D3DShell::getSuportedShaderVersion() const
{
	switch (this->_prDatPtr->d3dDevice->GetFeatureLevel())
	{
		case D3D_FEATURE_LEVEL_11_0:
			return "5_0";
		break;
		case D3D_FEATURE_LEVEL_10_1:
		case D3D_FEATURE_LEVEL_10_0:
			return "4_0";
		break;
		case D3D_FEATURE_LEVEL_9_3:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
			return "2_0";
		break;
	};

	return "";
}



void D3DShell::BeginGBufferRenderTargets(bool withDepth)
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	
	ID3D11RenderTargetView* rtv[DeferredRenderLayout::MRT_COUNT];
	for(int i =0 ; i< DeferredRenderLayout::MRT_COUNT; i++)
	{
		rtv[i] = this->_prDatPtr->g_buffTextures[i].getRenderTargetView();
	}

	if(withDepth)
		this->getDeviceContext()->OMSetRenderTargets(DeferredRenderLayout::MRT_COUNT, rtv, this->_prDatPtr->g_bufferDepthTexture.getDepthStencilView());
	else
	{
		for(int i = 0; i < DeferredRenderLayout::MRT_COUNT; i++)
		{
			this->getDeviceContext()->ClearRenderTargetView(rtv[i], clearColor);
		}
		this->getDeviceContext()->ClearDepthStencilView(this->_prDatPtr->g_bufferDepthTexture.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);

		this->getDeviceContext()->OMSetRenderTargets(DeferredRenderLayout::MRT_COUNT, rtv, NULL);
	}
}
void	D3DShell::setDefferedSRV()
{
	int nrOfsrv = DeferredRenderLayout::MRT_COUNT;
	ID3D11ShaderResourceView* srv [DeferredRenderLayout::MRT_COUNT];
	for(int i=0; i<DeferredRenderLayout::MRT_COUNT;i++)
	{
		srv[i]= this->_prDatPtr->g_buffTextures[i].getColorSRV();
	}
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nrOfsrv,srv);
}
int D3DShell::getNrOfSRV()
{
	return DeferredRenderLayout::MRT_COUNT;
}
void D3DShell::releaseSRV()
{
	ID3D11ShaderResourceView *const pSRV[5] = {NULL, NULL, NULL, NULL, NULL};
	getDeviceContext()->PSSetShaderResources(0, 5, pSRV);
}

//--------------------Light shader---------------------//
void D3DShell::BeginLightRenderTarget()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	ID3D11RenderTargetView* rtv[1] = {this->_prDatPtr->lightTexture.getRenderTargetView()};

	this->getDeviceContext()->ClearRenderTargetView(rtv[0], clearColor);
	
	//this->getDeviceContext()->ClearDepthStencilView(this->_prDatPtr->deffDepthStencil[0], D3D11_CLEAR_DEPTH, 1.0, 0);

	this->getDeviceContext()->OMSetRenderTargets(1, rtv, NULL);
}

void D3DShell::setLightSRV()
{
	int nrOfsrv = 5;
	ID3D11ShaderResourceView* srv [5];
	srv[0] = this->_prDatPtr->g_buffTextures[0].getColorSRV();	
	srv[1] = this->_prDatPtr->lightTexture.getColorSRV();		
	srv[2] = this->_prDatPtr->shadowTexture.getDepthSRV();		
	srv[3] = this->_prDatPtr->g_buffTextures[1].getColorSRV();	
	srv[4] = this->_prDatPtr->blurTexture.getColorSRV();			
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nrOfsrv,srv);
}

//-----------------------Shadow maps---------------------//
void D3DShell::BeginShadowRenderTarget()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	//ID3D11RenderTargetView* rtv[1] = {this->_prDatPtr->shadowTexture.getRenderTargetView()};

	this->getDeviceContext()->ClearDepthStencilView(this->_prDatPtr->shadowTexture.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);

	this->getDeviceContext()->OMSetRenderTargets(0, NULL, this->_prDatPtr->shadowTexture.getDepthStencilView());
}
void D3DShell::setShadowSRV()
{
	ID3D11ShaderResourceView* srv [DeferredRenderLayout::MRT_COUNT];
	for(int i = 0; i< DeferredRenderLayout::MRT_COUNT; i++)
	{
		srv[i] = this->_prDatPtr->g_buffTextures[i].getColorSRV();
	}
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,DeferredRenderLayout::MRT_COUNT,srv);
}
//-----------------------Blur maps---------------------//
void D3DShell::BeginBlurRenderTarget()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	ID3D11RenderTargetView* rtv[1] = {this->_prDatPtr->blurTempTexture.getRenderTargetView()};
	this->getDeviceContext()->ClearRenderTargetView(rtv[0], clearColor);
	if(this->_prDatPtr->blurTempTexture.getDepthStencilView())
		this->getDeviceContext()->ClearDepthStencilView(this->_prDatPtr->blurTempTexture.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);

	this->getDeviceContext()->OMSetRenderTargets(1, rtv, this->_prDatPtr->blurTempTexture.getDepthStencilView());
}
void D3DShell::setBlurSRV()
{
	int nrOfsrv = 2;
	ID3D11ShaderResourceView* srv [2];
	srv[0] =	this->_prDatPtr->g_buffTextures[1].getColorSRV(); // normal_depth
	srv[1] = this->_prDatPtr->g_buffTextures[4].getColorSRV();//deffSRV[4]; //right now specular map
	
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nrOfsrv,srv);
}
void D3DShell::BeginBlur2RenderTarget()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	ID3D11RenderTargetView* rtv[1] = {this->_prDatPtr->blurTexture.getRenderTargetView()};
	this->getDeviceContext()->ClearRenderTargetView(rtv[0], clearColor);

	if(this->_prDatPtr->blurTexture.getDepthStencilView())
		this->getDeviceContext()->ClearDepthStencilView(this->_prDatPtr->blurTexture.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);

	this->getDeviceContext()->OMSetRenderTargets(1, rtv, this->_prDatPtr->blurTexture.getDepthStencilView());
}
void D3DShell::setBlur2SRV()
{
	int nrOfsrv = 2;
	ID3D11ShaderResourceView* srv [2];
	srv[0] =this->_prDatPtr->g_buffTextures[1].getColorSRV();
	srv[1] = this->_prDatPtr->blurTempTexture.getColorSRV(); 
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0,nrOfsrv,srv);

}