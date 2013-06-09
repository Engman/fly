#ifndef STDAFX_H
#define STDAFX_H



/** Use to find memoryleaks in your code */
#if defined(DEBUG) || defined (_DEBUG)
#define FIND_MEMORY_LEAKS _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
#define FIND_MEMORY_LEAKS
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <map>
#include <vector>



#define DLL_USAGE
#define DLL_IMPORT __declspec(dllimport)
#define DLL_EXPORT __declspec(dllexport)



#endif