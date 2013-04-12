#ifndef STDAFX_H
#define STDAFX_H

/* 
*	Contains linking and macros
*/

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include <D3DX11\D3D10_1.h>
#include <D3DX11\D3D10.h>
#include <D3DX11\D3DX10math.h>
#include <D3DX11\d3dCompiler.h>


/** Use to find memoryleaks in your code */
#define FIND_MEMORY_LEAKS _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)


#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif


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