#include "EngineWrapper.h"
#include "..\Core\Application.h"

EngineWrapper::EngineWrapper()
{
	this->flyGameEngine = new Application();
}
EngineWrapper::~EngineWrapper()
{
	delete this->flyGameEngine;
	this->flyGameEngine = NULL;
}

HRESULT EngineWrapper::Init(HWND hWnd, int width, int height)
{
	if(!this->flyGameEngine->Initialize((HINSTANCE)hWnd, width, height))
		return E_FAIL;

	return S_OK;
}
HRESULT EngineWrapper::Shutdown()
{
	this->flyGameEngine->Shutdown();

	return S_OK;
}

HRESULT EngineWrapper::ProcessFrame()
{
	this->flyGameEngine->Frame();
	return S_OK;
}
HRESULT EngineWrapper::OnResize(int width, int height)
{
	return S_OK;
}
