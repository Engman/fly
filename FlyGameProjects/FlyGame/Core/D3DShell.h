#ifndef D3D_SHELL_H
#define D3D_SHELL_H

#include <vector>
#include <ctime>

#include "D3DxLink.h"
#include "stdafx.h"

#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "ShaderFlagStates.h"
#include "..\Util\GID.h"
#include "..\Util\SmartPtrs.h"
#include "DeferredRenderingLayout.h"

/**
*	DirectX 11 singleton shell
*	Don't forget to explicitly call destroy()
*/
class D3DShell
{
	public:
		/** Used to initialize 3D window. 
		* Only HWND is required, the rest is optional */
		struct D3D_INIT_DESC
		{
			/** The handle to the window */
			HWND hwnd;
			/** Width of viewport */
			long unsigned int width;
			/** Height of viewport */
			long unsigned int height;
			/** Full screen mode Enabled/Disabled */
			bool fullScreen;
			/** Multisampling Enabled/Disabled */
			bool MSAA;
			/** Multisampling count Enabled/Disabled */
			int MSAASampleCount;
			/** Vertical sync Enabled/Disabled */
			bool vSync;

			D3D_INIT_DESC()
				:hwnd(0), width(800), height(600), fullScreen(0), MSAA(0), MSAASampleCount(4), vSync(0)
			{}
		};

	private:
		struct PrDat;
		PrDat *_prDatPtr;

	private:
		D3DShell();
		~D3DShell();
		D3DShell(const D3DShell&);
		void operator=(const D3DShell&);

	public:

		bool						init(D3D_INIT_DESC&);
		
		static D3DShell*			self();
		static void					destroy();

		ID3D11Device*				getDevice()				const;
		ID3D11DeviceContext*		getDeviceContext()		const;
		IDXGISwapChain*				getSwapChain()			const;
		ID3D11RenderTargetView*		getRenderTargetView()	const;
		ID3D11DepthStencilView*		getDepthStencilView()	const;
		D3D11_VIEWPORT*				getScreenViewport()		const;
		D3D_DRIVER_TYPE*			getDriverType()			const;
		float						getWidth()				const;
		float						getHeight()				const;
		float						getAspectRatio()		const;
		D3DXMATRIX&					getProjectionMatrix()	const;
		int							getVideoCardMem()		const;

		void						resizeViewport(unsigned int width, unsigned int height);

		void						beginScene();
		void						beginScene(float r, float g, float b, float a = 1.0f);
		void						endScene();

		/** Sets the viewport. */
		void						setViewport(D3D11_VIEWPORT*, UINT);
		/** Sets the default viewport. */
		void						setViewport();
		/** Sets the render target. */
		void						setRenderTarget(ID3D11RenderTargetView*, ID3D11DepthStencilView*, UINT);
		/** Sets the render target. Set param to NULL for default. */
		void						setRenderTarget();
		void						setRasterizerState(FLAGS::STATE_RASTERIZER);
		void						setBlendModeState(FLAGS::STATE_BLEND_MODE state, float blend[4], UINT SampleMask);
		void						setSamplerState(FLAGS::STATE_SAMPLING *state, FLAGS::SHADERS stage, UINT startSlot, UINT count );
		void						setDepthStencilState(FLAGS::STATE_DEPTH_STENCIL state, UINT stencilRef);

		char*						getSuportedShaderVersion() const;


		/** Clears and sets the G-buffers and depth stencil views as the render target */
		void						BeginGBufferRenderTargets();
		ID3D11ShaderResourceView**	getDefferedSRV();   /**returns the shader recourse view array*/
		int getNrOfSRV();	
		void releaseSRV();

		void BeginLightRenderTarget();
		ID3D11ShaderResourceView** getLightSRV();





};


#endif