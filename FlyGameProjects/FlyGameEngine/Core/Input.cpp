#include "Input.h"

bool initialized = false;
Input* Input::single = NULL;

Input* Input::self()
{
	if(!single)
    {
        single = new Input();
        return single;
    }
    else
    {
        return single;
    }
}

Input::Input()
{
	m_pDirectInput = 0;
	m_pKeyboard = 0;
	m_pMouse = 0;
	this->m_screenHeight = 0;
	this->m_screenWidth = 0;
	this->hWnd = 0;
}

Input::Input(const Input& input)
{

}

Input::~Input()
{

}

HRESULT Input::Initialize(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight)
{
	if(initialized)
		return S_OK;

	if(this->hWnd)
		return S_OK;

	if(!hWnd)
		return E_FAIL;

	if(!IsWindowVisible(hWnd))
		Sleep(10);
	if(!IsWindowEnabled(hWnd))
		EnableWindow(hWnd, TRUE);

	this->m_screenWidth = screenWidth;
	this->m_screenHeight = screenHeight;

	this->m_mouseX = 0;
	this->m_mouseY = 0;

	this->hWnd = hWnd;

	if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->m_pDirectInput, NULL)))
	{
		return E_FAIL;
	}

	//Keyboard
	if(FAILED(this->m_pDirectInput->CreateDevice(GUID_SysKeyboard, &this->m_pKeyboard, NULL)))
	{
		return E_FAIL;
	}

	if(FAILED(this->m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	if(FAILED(this->m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		return E_FAIL;
	}


	//Keyboard done

	//Mouse
	if(FAILED(this->m_pDirectInput->CreateDevice(GUID_SysMouse, &this->m_pMouse, NULL)))
	{
		return E_FAIL;
	}

	if(FAILED(this->m_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	if(FAILED(this->m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		return E_FAIL;
	}
	//Mouse done
	
	if(FAILED(this->m_pMouse->Acquire()))
	{
		return E_FAIL;
	}

	if(FAILED(this->m_pKeyboard->Acquire()))
	{
		return E_FAIL;
	}


	initialized = true;

	return S_OK;
}
bool Input::isInitialized() const
{
	if(!initialized)
		return false;
	return true;
}

void Input::ReleaseInput()
{
	if(this->m_pMouse)
	{
		this->m_pMouse->Unacquire();
		this->m_pMouse->Release();
		this->m_pMouse = 0;
	}

	if(this->m_pKeyboard)
	{
		this->m_pKeyboard->Unacquire();
		this->m_pKeyboard->Release();
		this->m_pKeyboard = 0;
	}

	if(this->m_pDirectInput)
	{
		this->m_pDirectInput->Release();
		this->m_pDirectInput = 0;
	}
}

HRESULT Input::Frame()
{
	if(!initialized)
		return E_FAIL;

	if(FAILED(ReadKeyboard()))
	{
		return E_FAIL;
	}

	if(FAILED(ReadMouse()))
	{
		return E_FAIL;
	}
	
	ProcessInput();

	return S_OK;
}

HRESULT Input::ReadKeyboard()
{
	HRESULT result;

	result = this->m_pKeyboard->GetDeviceState(sizeof(this->m_keyboardState), (LPVOID) &this->m_keyboardState);

	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			this->m_pKeyboard->Acquire();
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT Input::ReadMouse()
{
	HRESULT result = E_FAIL;

	result = this->m_pMouse->GetDeviceState(sizeof(this->m_mouseState), (LPVOID) &this->m_mouseState);

	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			this->m_pMouse->Acquire();
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void Input::ProcessInput()
{
	this->m_mouseX += this->m_mouseState.lX;
	this->m_mouseY += this->m_mouseState.lY;

	if(this->m_mouseX < 0) this->m_mouseX = 0;
	if(this->m_mouseY < 0) this->m_mouseY = 0;

	if(this->m_screenWidth < this->m_mouseX) this->m_mouseX = this->m_screenWidth;
	
	if(this->m_screenHeight < this->m_mouseY) this->m_mouseY = this->m_screenHeight;
}

bool Input::IsButtonPressed(int key)
{
	if(this->m_keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsMouseButtonPressed(int key)
{	
	if(this->m_mouseState.rgbButtons[key] & 0x80)
	{
		return true;
	}

	return false;
}

void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = this->m_mouseX;
	mouseY = this->m_mouseY;
}

void Input::GetMouseRelative(int& mouseX, int& mouseY)
{
	mouseX = this->m_mouseState.lX;
	mouseY = this->m_mouseState.lY;
}

void Input::SetCoopExclusive()
{
	this->m_pMouse->Unacquire();

	this->m_pDirectInput->CreateDevice(GUID_SysMouse, &this->m_pMouse, NULL);

	this->m_pMouse->SetDataFormat(&c_dfDIMouse2);

	this->m_pMouse->SetCooperativeLevel(this->hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	this->m_pMouse->Acquire();
}
void Input::SetCoopNonExclusive()
{
	this->m_mouseX = this->m_screenWidth/2;
	this->m_mouseY = this->m_screenHeight/2;

	this->m_pMouse->Unacquire();

	this->m_pDirectInput->CreateDevice(GUID_SysMouse, &this->m_pMouse, NULL);

	this->m_pMouse->SetDataFormat(&c_dfDIMouse2);

	this->m_pMouse->SetCooperativeLevel(this->hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	this->m_pMouse->Acquire();
}