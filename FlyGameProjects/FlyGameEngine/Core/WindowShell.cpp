#include "WindowShell.h"
#include <vector>


struct ChildWin;
struct _PrSt;

#pragma region Declarations

	namespace
	{
		//Private data
		static WindowShell* instance = NULL;
		int childIdCounter = 0;
		_PrSt *pData = NULL;
		const wchar_t *WINDOW_CLASS_NAME = 0;
	}

	struct ChildWin
	{
		int id;
		HWND hWnd;

		ChildWin()
		{
			hWnd = NULL;
			childIdCounter++;
			id = childIdCounter;
		}
		int ID() const { return id; }
	};
	struct _PrSt
	{
		HINSTANCE							hIns;
		HWND								hWnd;
		HWND								parent;
		bool								windowed;
		std::vector<ChildWin>				childWindows;

		_PrSt()
		{
			hIns = NULL;
			hWnd = NULL;
			parent = NULL;
			windowed = false;
		}
	};

#pragma endregion



WindowShell::WindowShell()
{
	pData = new _PrSt();
}
WindowShell::~WindowShell()
{
	delete pData;
}



bool WindowShell::createWin(INIT_DESC_WINDOW &desc)
{
	if(pData->hWnd)
	{
		MessageBox(0, L"There is already a window registered\nPlease use child windows to create more windows!" ,L"Error", 0);
		return false;
	}
	if(!desc.windowProcCallback)
	{
		MessageBox(0, L"No callback function for window messages was found!" ,L"Error", 0);
		return false;
	}
	if(!desc.hInstance)
	{
		MessageBox(0, L"No HINSTANCE was specified!" ,L"Error", 0);
		return false;
	}
	if(desc.windowSize < 0)
	{
		MessageBox(0, L"Size specified for window is invalid!" ,L"Error", 0);
	}
	if(!desc.windowClassName.size())
		MessageBox(0, L"Window class name not found!" ,L"Error", 0);
	
	pData->parent	= desc.parent;
	pData->hIns = desc.hInstance;
	WINDOW_CLASS_NAME = desc.windowClassName.c_str();

		#pragma region Register
	
	WNDCLASSEX wc;
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.hIconSm		= NULL;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = desc.windowProcCallback; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = pData->hIns;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = desc.windowClassName.c_str();

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(0, L"Failed to register class", L"Initialization error", 0);
		return false;
	}
		#pragma endregion
	

		#pragma region Create window


	RECT rectW;
	int width;
	int height;
	DWORD style = WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION;
	bool windowed = false;

	width = desc.windowSize.x + GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	height = desc.windowSize.y + GetSystemMetrics(SM_CYFIXEDFRAME)*2 + GetSystemMetrics(SM_CYCAPTION);

	rectW.left=(GetSystemMetrics(SM_CXSCREEN)-width)/2;
	rectW.top=(GetSystemMetrics(SM_CYSCREEN)-height)/2;
	rectW.right=rectW.left+width;
	rectW.bottom=rectW.top+height;
	
	
	if(pData->parent)
	{
		rectW.left		= 0;
		rectW.top		= 0;
		rectW.right		= desc.windowSize.x;
		rectW.bottom	= desc.windowSize.y;
		style			= WS_CHILD | WS_VISIBLE; 
		windowed		= true;
	}

	if(windowed)
		pData->hWnd = CreateWindowEx(
									0,
									WINDOW_CLASS_NAME , 
									desc.windowName.c_str(),
									style, 
									rectW.left, 
									rectW.top, 
									rectW.right - rectW.left, 
									rectW.bottom - rectW.top, 
									pData->parent, 
									NULL, 
									pData->hIns, 
									NULL
								  ); 
	else
		pData->hWnd = CreateWindowEx(	
									0,
									WINDOW_CLASS_NAME , 
									desc.windowName.c_str(),
									style, 
									desc.windowPosition.x, 
									desc.windowPosition.y, 
									desc.windowSize.x, 
									desc.windowSize.y, 
									0, 
									0, 
									pData->hIns, 
									0
								  ); 
	
	if( !pData->hWnd )
	{
		MessageBox(0, L"Failed to create window", L"Error!", 0);
		return false;
	}

		#pragma endregion


	//Show and update window
	ShowWindow(pData->hWnd, SW_SHOW);
	//UpdateWindow(pData->hWnd);
	
	return true;
}
int WindowShell::createChildWin(INIT_DESC_CHILD_WINDOW &desc)
{
	ChildWin win;


	char idStr[3];
	_itoa_s(win.id, idStr, 10);
	std::string next = idStr;
	std::wstring str = std::wstring(next.begin(), next.end());
	std::wstring childClassName = L"ChildWindow_";
	childClassName += str;

	WNDCLASSEX wcex;
	wcex.cbSize	= sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW;
	wcex.lpfnWndProc = desc.windowProcCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = pData->hIns;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = childClassName.c_str();
	wcex.hIconSm = NULL;

	if(!RegisterClassEx(&wcex))
	{
		MessageBox(0, L"", 0, 0);
	}

	if(!desc.style)
		desc.style = WS_EX_CLIENTEDGE;

	win.hWnd =	CreateWindowEx
				(
					desc.style,
					childClassName.c_str(), 
					desc.name.c_str(), 
					WS_CAPTION | WS_SYSMENU ,
					desc.topLeftPos.x,  desc.topLeftPos.y, 
					desc.windowSize.x, desc.windowSize.y, 
					pData->hWnd,
					NULL,
					pData->hIns, 
					NULL
				);


	if (win.hWnd)
	{
		pData->childWindows.push_back(win);
		ShowWindow(win.hWnd, 5);
		UpdateWindow(win.hWnd);
	}
	else
	{
		DWORD err = GetLastError();
		MessageBox(0, L"Failed to create child window", L"Error!", MB_OK);
		return false;
	}

	return win.id;
}
bool WindowShell::removeChild(int id)
{
	for (int i = 0; i < (int)pData->childWindows.size(); i++)
	{
		if(id == pData->childWindows[i].id)
		{
			pData->childWindows.erase(pData->childWindows.begin() + i);
			return true;
		}
	}

	return false;
}
bool WindowShell::removeChild(HWND hwnd)
{
	for (int i = 0; i < (int)pData->childWindows.size(); i++)
	{
		if(hwnd == pData->childWindows[i].hWnd)
		{
			pData->childWindows.erase(pData->childWindows.begin() + i);
			return true;
		}
	}

	return false;
}



const HINSTANCE WindowShell::getHINSTANCE() const
{
	return pData->hIns;
}
const HWND WindowShell::getHWND() const
{
	return pData->hWnd;
}
const HWND WindowShell::getParent() const
{
	return pData->parent;
}
const HWND WindowShell::getChildHWND(int id) const
{
	for(int i = 0; i<(int)pData->childWindows.size(); i++)
	{
		if(id == pData->childWindows[i].id)
			return pData->childWindows[i].hWnd;
	}

	return NULL;
}
const int WindowShell::getChildID(HWND hwnd) const
{
	for(int i = 0; i<(int)pData->childWindows.size(); i++)
	{
		if(hwnd == pData->childWindows[i].hWnd)
			return pData->childWindows[i].id;
	}

	return -1;
}

WindowShell* WindowShell::self()
{
	if(!instance)
		instance = new WindowShell();

	return instance;
}
void WindowShell::destroy()
{
	for (int i = 0; i < (int)pData->childWindows.size(); i++)
	{
		if(pData->childWindows[i].hWnd)
			DestroyWindow(pData->childWindows[i].hWnd);
	}

	if(pData->hWnd)
		DestroyWindow(pData->hWnd);

	if(pData->hIns) UnregisterClass(WINDOW_CLASS_NAME, pData->hIns);

	delete instance;
	instance = NULL;
}