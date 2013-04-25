#include "EngineEditorWrapper.h"


#ifdef DLL_USAGE

	#include "..\FlyEngine.h"

	EngineEditorWrapper::EngineEditorWrapper()
	{
		this->flyEngine = new FlyEngine();
	}
	EngineEditorWrapper::~EngineEditorWrapper()
	{
		delete this->flyEngine;
	}

	HRESULT EngineEditorWrapper::Init(HWND hWnd, int width, int height)
	{
		if(!this->flyEngine->Initialize(hWnd, width, height))
			return E_FAIL;
		return S_OK;
	}
	HRESULT EngineEditorWrapper::Shutdown()
	{
		this->flyEngine->Shutdown();
		return S_OK;
	}

	int EngineEditorWrapper::LoadResource(std::wstring path)
	{
		return this->flyEngine->LoadResources(path);
	}

	HRESULT EngineEditorWrapper::Update()
	{
		this->flyEngine->Update();
		return S_OK;
	}

	HRESULT EngineEditorWrapper::Render()
	{
		if(!this->flyEngine->Render())
			return E_FAIL;
		return S_OK;
	}

	HRESULT EngineEditorWrapper::OnResize		(int width, int height)
	{
		this->flyEngine->Resize(width, height);
		return S_OK;
	}
	HRESULT EngineEditorWrapper::OnMouseDown	(int x, int y)
	{
		return S_OK;
	}
	HRESULT EngineEditorWrapper::OnMouseUp		(int x, int y)
	{
		return S_OK;
	}

#endif