#include "FlyWindEdit.h"



namespace System { namespace Windows { namespace Interop {



	void FlyWindEdit::Render()
	{

	}
	void FlyWindEdit::Update()
	{

	}



	FlyWindEdit::FlyWindEdit()
	{
		this->flyEngine = FlyEngineCreate();
		this->models = new std::vector<SmartPtrStd<FlyMesh>>();
	}
			
	FlyWindEdit::~FlyWindEdit()
	{
		delete this->models;
		delete this->flyEngine;
	}

	bool FlyWindEdit::Init(IntPtr _hWnd, int width, int height)
	{
		HWND hwnd = (HWND)(void*)_hWnd;
		FLY_ENGINE_INIT_DESC d;
		d.parent = hwnd;
		d.winWidth = width;
		d.winHeight = height;
		if( FAILED (this->flyEngine->Core_Initialize(d) ) )
			return false;


		return true;
	}

	HRESULT FlyWindEdit::Shutdown()
	{
		this->flyEngine->Core_Shutdown();
		return S_OK;
	}

	HRESULT FlyWindEdit::ProcessFrame()
	{
		this->flyEngine->Core_BeginDeferredScene();

		for (int i = 0; i < (int)this->models->size(); i++)
		{
			(*this->models)[i]->Render();
		}

		this->flyEngine->Core_EndDeferredScene();
		return 1;
	}

	void FlyWindEdit::LoadResources(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects)
	{
		if(resourcePath->Length == 0)
			return;
				
		std::vector<const wchar_t*> path((int)resourcePath->Length);
		for(int i = 0; i<resourcePath->Length; i++)
		{
			path[i] = (const wchar_t*)Marshal::StringToHGlobalUni(resourcePath[i]).ToPointer();
		}
				
		this->flyEngine->Geometry_Load(L"..\\Resources\\Models\\WindEditSphere.fgm", *this->models);
				
				
		//std::map<std::wstring, int>::const_iterator i = loaded.begin();
		//for (i; i != loaded.end(); i++)
		//{
		//	String^ key = gcnew String(i->first.c_str());
		//	loadedObjects->Add(key, i->second);
		//}
	}

	void FlyWindEdit::OnResize	(int width, int height)
	{
				
		//this->flyEngine->ResizeWindow(width, height);
		//return 1;
	}
			

	void FlyWindEdit::OnKeyDown			(int key)
	{
		//this->flyEngine->OnKeyDown(key);
	}
	void FlyWindEdit::OnKeyUp				(int key)
	{
		//this->flyEngine->OnKeyDown(key);
	}
	void FlyWindEdit::OnMouseClick		(bool left, int x, int y)
	{
		//this->flyEngine->OnMouseClick(left, x, y);
	}
	void FlyWindEdit::OnMouseMove			(int x, int y)
	{
		//this->flyEngine->OnMouseMove(x, y);
	}
}}}