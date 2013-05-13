#include "Input.h"
#include <WindowsX.h>
#include <algorithm>
#include <ctime>

static bool procThreadBussy = false;
static bool procThreadWaitForUpdate = false;
static bool LBTNDOWN	= false;
static bool MBTNDOWN	= false;
static bool RBTNDOWN	= false;

static bool CONTROLDOWN = false;
static bool ALTDOWN		= false;
static bool SHIFTDOWN	= false;

static Input::MouseMoveData		MOUSE_INPUT_moveData = Input::MouseMoveData();
static Input::MouseBtnData		MOUSE_INPUT_btnData = Input::MouseBtnData() ;
static Input::KeyPressData		KEY_INPUT_keyData = Input::KeyPressData() ;

template<typename T, typename S>
int existsInVectorList(std::vector<T>& obj, S elem);


struct Input::_PrSt
{
	public:
		HANDLE procThreadHandle;
		bool isActive;

	public:
		HWND targetKeyApp;
		HWND targetMouseApp;
		std::vector<Input::KeyPressData> keyList;

	public:
		void proccessRawMouseData		(RAWMOUSE&);
		void proccessRawKeyboardData	(RAWKEYBOARD&);
		void proccessRawHidData			(RAWHID&);
};

static Input* gInstanceInput = NULL;


Input::Input								()
{
	this->_PrPtr					= new Input::_PrSt();
	this->_PrPtr->targetKeyApp		= 0;
	this->_PrPtr->targetMouseApp	= 0;
	this->_PrPtr->isActive			= false;
	this->_PrPtr->procThreadHandle	= CreateThread(NULL, 4*255, ProcThread, NULL, CREATE_SUSPENDED, NULL);
	ResumeThread(this->_PrPtr->procThreadHandle);
}
Input::~Input								()
{
	delete this->_PrPtr;
	this->_PrPtr = NULL;
}

Input* Input::self							()
{
	if(!gInstanceInput)
		gInstanceInput = new Input();

	return gInstanceInput;
}
void Input::destroy							()
{
	TerminateThread(gInstanceInput->_PrPtr->procThreadHandle, 0);
	delete gInstanceInput;
	gInstanceInput = NULL;
}

void Input::proccessRawDeviceData			(const LPARAM& lParam)
{
	if(!this->_PrPtr->isActive)
		return;
	
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{
		//Something went wrong
		MessageBox(0, L"Failed to read raw buffer data in input class", L"Error", 0);
		return;
	} 

	//Create and read the raw input data
	LPBYTE rawBufferIn = new BYTE[bufferSize];
	UINT readBytes = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawBufferIn, &bufferSize,  sizeof(RAWINPUTHEADER));
	if ( readBytes != bufferSize )
	{
		//Something went wrong
		delete [] rawBufferIn;
		return;
	}


	RAWINPUT* raw = (RAWINPUT*)rawBufferIn;

	switch (raw->header.dwType)
	{
		case RIM_TYPEMOUSE:
			this->_PrPtr->proccessRawMouseData(raw->data.mouse);
		break;

		case RIM_TYPEKEYBOARD:
			this->_PrPtr->proccessRawKeyboardData(raw->data.keyboard);
		break;

		default:
			this->_PrPtr->proccessRawHidData(raw->data.hid);
		break;
	}
   
	delete[] rawBufferIn; 
}

bool Input::Register						(const Input::GLARE_INPUT_INIT_DESC& desc)
{
	static bool keyReg = false;
	static bool mouseReg = false;
	bool retVal = true;
	HWND target = desc.target;
		if(desc.deviceType == Flags::REMOVE)
			target = 0;
	
	
	switch (desc.deviceType)
	{
		case Input::Flags::keyboard:
		{
			RAWINPUTDEVICE k = { 0x01, desc.deviceType, desc.deviceFlag, target };
			if(!RegisterRawInputDevices(&k, 1, sizeof(k)))
			{
				retVal = false;
				MessageBox(0, L"Failed to register [Keyboard] device", L"Error", MB_OK);
			}
			this->_PrPtr->targetKeyApp = desc.target;
		}
		break;
		
		case Input::Flags::mouse:
		{
			RAWINPUTDEVICE k = { 0x01, desc.deviceType, desc.deviceFlag, target };

			if(!RegisterRawInputDevices(&k, 1, sizeof(k)))
			{
				retVal = false;
				MessageBox(0, L"Failed to register [Mouse] device", L"Error", MB_OK);
			}
			this->_PrPtr->targetMouseApp = desc.target;
		}
		break;

		case Input::Flags::gamepad:
		case Input::Flags::joystick:
		case Input::Flags::keypad:
		case Input::Flags::multiAxisController:
		case Input::Flags::pointer:
		case Input::Flags::TabletPCcontrols:
		{
			RAWINPUTDEVICE k = { 0x01, desc.deviceType, desc.deviceFlag, desc.target };

			if(!RegisterRawInputDevices(&k, 1, sizeof(k)))
			{
				retVal = false;
				MessageBox(0, L"Failed to register [HID] dvice", L"Error", MB_OK);
			}
		}
		break;

		default:
			//No valid input device
			MessageBox(0, L"Error\nNo valid device", L"Error", MB_OK);
			retVal = false;
		break;
	}
	return retVal;
}
bool Input::Register						(const RAWINPUTDEVICE& iDevice)
{
	if(!RegisterRawInputDevices(&iDevice, 1, sizeof(iDevice)))
	{
		MessageBox(0, L"Failed to register device", L"Error", MB_OK);
		return false;
	}

	
	return true;
}
bool Input::Unregister						(Flags::DeviceType type)
{
	RAWINPUTDEVICE d = { 0x01, type, RIDEV_REMOVE, NULL };

	if(!RegisterRawInputDevices(&d, 1, sizeof(RAWINPUTDEVICE)))
	{
		MessageBox(0, L"Failed to unregister device", L"Error", MB_OK);
		return false;
	}

	return true;
}
void Input::ActivateCallback				()
{
	this->_PrPtr->isActive = true;
}
void Input::DeactivateCallback				()
{
	this->_PrPtr->isActive = false;
}



bool Input::getActive() const
{
	return this->_PrPtr->isActive;
}

void Input::_PrSt::proccessRawKeyboardData	(RAWKEYBOARD& k)
{
	if(Input::self()->_rawKeyboardData.getCount() > 0)
	{
		Input::self()->_rawKeyboardData.procEvent(k);
	}

	Input::KeyCodes::Key v = (Input::KeyCodes::Key)k.VKey;
	int i = existsInVectorList<Input::KeyPressData, Input::KeyCodes::Key>(this->keyList, v);

	//The key is released.
	if(k.Flags == RI_KEY_BREAK)
	{
		if( i != -1 )
		{
			while(procThreadBussy);
			procThreadWaitForUpdate = true;
				this->keyList.erase(this->keyList.begin() + i);
			procThreadWaitForUpdate = false;

			if(v == KeyCodes::K_Ctrl)	
			{
				CONTROLDOWN = false;
				KEY_INPUT_keyData.ctrl = false;
			}
			else if(v == KeyCodes::K_Shift)	
			{
				SHIFTDOWN	= false;
				KEY_INPUT_keyData.shift = false;
			}
			else if(v == KeyCodes::K_Alt)	
			{
				ALTDOWN		= false;
				KEY_INPUT_keyData.alt = false;
			}

			Input::self()->_keyUpProc.procEvent(KEY_INPUT_keyData); 
		}
		
	}
	//The key is pressed.
	else if (k.Flags == RI_KEY_MAKE)
	{
		if(i == -1)
		{
			KEY_INPUT_keyData.key = v;
			while(procThreadBussy);
			procThreadWaitForUpdate = true;
				this->keyList.push_back(KEY_INPUT_keyData);
			procThreadWaitForUpdate = false;

			if(v == KeyCodes::K_Ctrl)	
			{
				CONTROLDOWN = true;
				KEY_INPUT_keyData.ctrl = true;
			}
			else if(v == KeyCodes::K_Shift)	
			{
				SHIFTDOWN	= true;
				KEY_INPUT_keyData.shift = true;
			}
			else if(v == KeyCodes::K_Alt)	
			{
				ALTDOWN		= true;
				KEY_INPUT_keyData.alt = true;
			}
		}
	}
		
	//RI_KEY_E0 = This is the left version of the key.
	//RI_KEY_E1 = This is the right version of the key.
		

}
void Input::_PrSt::proccessRawMouseData		(RAWMOUSE& m)
{
	if(Input::self()->_rawMouseData.getCount() > 0)
	{
		Input::self()->_rawMouseData.procEvent(m);
		return;
	}

	POINT p;
	//Get Mouse position
	GetCursorPos(&p);
	MOUSE_INPUT_moveData.relativeX = m.lLastX;
	MOUSE_INPUT_moveData.relativeY = m.lLastY;
	MOUSE_INPUT_moveData.screenX = p.x; 
	MOUSE_INPUT_moveData.screenY = p.y;
	ScreenToClient(this->targetMouseApp, &p);
	MOUSE_INPUT_moveData.clientX = p.x;
	MOUSE_INPUT_moveData.clientY = p.y;

	if(m.lLastX != 0 || m.lLastY != 0)
	{
		Input::self()->_mouseMove.procEvent(MOUSE_INPUT_moveData);
	}
	if( m.usButtonFlags > 0 )
	{
		MOUSE_INPUT_btnData.MousePos_clientX = MOUSE_INPUT_moveData.clientX;
		MOUSE_INPUT_btnData.MousePos_clientY = MOUSE_INPUT_moveData.clientY;
		MOUSE_INPUT_btnData.MousePos_relativeX = MOUSE_INPUT_moveData.relativeX;
		MOUSE_INPUT_btnData.MousePos_relativeY = MOUSE_INPUT_moveData.relativeY;
		MOUSE_INPUT_btnData.MousePos_screenX = MOUSE_INPUT_moveData.screenX;
		MOUSE_INPUT_btnData.MousePos_screenY = MOUSE_INPUT_moveData.screenY;

		switch (m.usButtonFlags)
		{
			//Mouse button pressed
			case RI_MOUSE_LEFT_BUTTON_DOWN:
			case RI_MOUSE_RIGHT_BUTTON_DOWN:
			case RI_MOUSE_MIDDLE_BUTTON_DOWN:
			{
				//MessageBox(0, L"InputDown", L"", 0);
				if(m.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)	
				{
					MOUSE_INPUT_btnData.key = KeyCodes::M_LeftBtn;
					LBTNDOWN = true;
				}
				else if(m.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_DOWN)	
				{	
					MOUSE_INPUT_btnData.key = KeyCodes::M_MiddleBtn;
					MBTNDOWN = true;
				}
				else if(m.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)	
				{	
					MOUSE_INPUT_btnData.key = KeyCodes::M_RightBtn;
					RBTNDOWN = true;
				}
			}
			break;

			//Mouse button Released
			case RI_MOUSE_LEFT_BUTTON_UP:
			case RI_MOUSE_RIGHT_BUTTON_UP:
			case RI_MOUSE_MIDDLE_BUTTON_UP:
			{
				//MessageBox(0, L"InputUp", L"", 0);
				if(m.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)	
				{
					MOUSE_INPUT_btnData.key = KeyCodes::M_LeftBtn;
					LBTNDOWN = false;
				}
				else if(m.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_UP)	
				{
					MOUSE_INPUT_btnData.key = KeyCodes::M_MiddleBtn;
					MBTNDOWN = false;
				}
				else if(m.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)	
				{
					MOUSE_INPUT_btnData.key = KeyCodes::M_RightBtn;
					RBTNDOWN = false;
				}

				Input::self()->_mouseBtnUp.procEvent(MOUSE_INPUT_btnData);
			}
			break;
			
			case Input::KeyCodes::M_Scroll:
			{
				int delta = ((int)m.usButtonData);
				if(delta > 120)
					delta = -1;
				else
					delta = 1;
				Input::self()->_mouseScroll.procEvent(delta);
			}
			break;
		}
	}
}
void Input::_PrSt::proccessRawHidData		(RAWHID& h)
{
	//Send hid data to client

	if(Input::self()->_rawHidData.getCount() > 0)
		Input::self()->_rawHidData.procEvent(h);
}


template<typename T, typename S>
/* Returns position of element or -1 if not found */
int existsInVectorList(std::vector<T>& obj, S elem)
{
	int pos = -1;

	for (int i = 0; i < (int)obj.size(); i++)
	{
		if(obj[i].key == elem)
		{
			pos = i;
			continue;
		}
	}

	//std::vector<T>::iterator i = std::find(obj.begin(), obj.end(), elem);
	//if(i != obj.end())
	//{
	//	pos = (int)(i - obj.begin());
	//}

	return pos;
}






//##########################################################//
//				THREADING FUNCTION FOR PROC					//
//##########################################################//

DWORD WINAPI Input::ProcThread(LPVOID lpParameter)
{
	KeyCodes::Key lb	  =  KeyCodes::M_LeftBtn;
	KeyCodes::Key rb	  =  KeyCodes::M_RightBtn;
	KeyCodes::Key mb	  =  KeyCodes::M_LeftBtn; 
	while (true)
	{
		Sleep(10);
		int size = (int)Input::self()->_PrPtr->keyList.size();
		if(size)
		{
			while(procThreadWaitForUpdate);
			procThreadBussy = true;
			for (int i = 0; i < size; i++)
				Input::self()->_keyDownProc.procEvent(Input::self()->_PrPtr->keyList[i]);
			procThreadBussy = false;
		}

		if(LBTNDOWN)	
		{
			//MessageBox(0, L"LBTNDOWN in Input", L"", 0);
			Input::self()->_mouseBtnDown.procEvent(MOUSE_INPUT_btnData);
		}
		if(RBTNDOWN)	Input::self()->_mouseBtnDown.procEvent(MOUSE_INPUT_btnData);
		if(MBTNDOWN)	Input::self()->_mouseBtnDown.procEvent(MOUSE_INPUT_btnData);
	}
	return NULL;
}

//##########################################################//

