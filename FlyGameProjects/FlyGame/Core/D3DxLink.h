#ifndef D3D_X_LINKING_H
#define D3D_X_LINKING_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <D3DX11\D3D10_1.h>
#include <D3DX11\D3D10.h>
#include <D3DX11\D3DX10math.h>
#include <D3DX11\d3dCompiler.h>



/*****************************************/
/*			Custom lib Linking			*/
/*****************************************/
#if defined(_DEBUG) | defined(DEBUG)
	//#pragma comment (lib, "EventModuleD.lib")
#else
	//#pragma comment (lib, "EventModule.lib")
#endif


/*****************************************/
/*			D3D11 Linking				*/
/*****************************************/
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment(lib, "dxgi.lib")

#if defined(_DEBUG) | defined(DEBUG)
	#pragma comment(lib, "d3dx11d.lib")
	#pragma comment(lib, "d3dx10d.lib")
#else
	#pragma comment(lib, "d3dx11.lib")
	#pragma comment(lib, "d3dx10.lib")
#endif



#endif

