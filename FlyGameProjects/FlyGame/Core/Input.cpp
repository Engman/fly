#include "Input.h"
#include <WindowsX.h>
#include <algorithm>
#include <ctime>



template<typename T>
int existsInVectorList(std::vector<T>& obj, T elem);


struct Input::_PrSt
{
	public:
		HANDLE procThreadHandle;

	public:
		HWND targetKeyApp;
		HWND targetMouseApp;
		std::vector<KeyCodes::Key> keyList;

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
	this->_PrPtr->procThreadHandle	= CreateThread(NULL, 4*255, ProcThread, NULL, CREATE_SUSPENDED, NULL);
	ResumeThread(this->_PrPtr->procThreadHandle);
}
Input::~Input								()
{
	delete this->_PrPtr;
	this->_PrPtr = NULL;
}

bool Input::registerInputDevice				(const Input::GLARE_INPUT_INIT_DESC& desc)
{
	static bool keyReg = false;
	static bool mouseReg = false;
	bool retVal = true;
	
	switch (desc.deviceType)
	{
	case Input::Flags::keyboard:
		{
			if(!keyReg)
			{
				RAWINPUTDEVICE k = { 0x01, desc.deviceType, RIDEV_NOLEGACY, desc.target };
				//int i = RIDEV_NOLEGACY;
				if(!RegisterRawInputDevices(&k, 1, sizeof(k)))
				{
					retVal = false;
					MessageBox(0, L"Failed to register [Keyboard] dvice", L"Error", MB_OK);
				}
				else
				{
					this->_PrPtr->targetKeyApp = desc.target;
					keyReg = true;
				}
			}
		}
		break;
		
		case Input::Flags::mouse:
		{
			if(!mouseReg)
			{
				RAWINPUTDEVICE k = { 0x01, desc.deviceType, desc.deviceFlag, desc.target };

				if(!RegisterRawInputDevices(&k, 1, sizeof(k)))
				{
					retVal = false;
					MessageBox(0, L"Failed to register [Mouse] dvice", L"Error", MB_OK);
				}
				this->_PrPtr->targetMouseApp = desc.target;
			}
			else
			{
				this->_PrPtr->targetMouseApp = desc.target;
				mouseReg = true;
			}
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
bool Input::registerInputDevice				(const RAWINPUTDEVICE& iDevice, HWND target)
{
	bool retVal = true;

	switch (iDevice.usUsage)
	{
		case Input::Flags::keyboard:
		{
			if(!RegisterRawInputDevices(&iDevice, 1, sizeof(iDevice)))
			{
				retVal = false;
				MessageBox(0, L"Failed to register [Keyboard] dvice", L"Error", MB_OK);
			}
			else
			{

			}
		}
		break;

		case Input::Flags::mouse:
		{
			if(!RegisterRawInputDevices(&iDevice, 1, sizeof(iDevice)))
			{
				retVal = false;
				MessageBox(0, L"Failed to register [Mouse] dvice", L"Error", MB_OK);
			}
		}
		break;

		case Input::Flags::gamepad:
		case Input::Flags::joystick:
		case Input::Flags::keypad:
		case Input::Flags::multiAxisController:
		case Input::Flags::pointer:
		case Input::Flags::TabletPCcontrols:
		{
			if(!RegisterRawInputDevices(&iDevice, 1, sizeof(iDevice)))
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
void Input::proccessRawDeviceData			(const LPARAM& lParam)
{
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{
		//Something went wrong
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



void Input::_PrSt::proccessRawKeyboardData	(RAWKEYBOARD& k)
{
	if(Input::self()->_rawKeyboardData.getCount() > 0)
	{
		Input::self()->_rawKeyboardData.procEvent(k);
	}

	Input::KeyCodes::Key v = (Input::KeyCodes::Key)k.VKey;
	int i = existsInVectorList<Input::KeyCodes::Key>(this->keyList, v);

	//The key is released.
	if(k.Flags == RI_KEY_BREAK)
	{
		if( i != -1 )
		{
			//Release key from list
			this->keyList.erase(this->keyList.begin() + i);
			Input::self()->_keyUpProc.procEvent(v);
		}
	}
	//The key is pressed.
	else if (k.Flags == RI_KEY_MAKE)
	{
		if(i == -1)
		{
			this->keyList.push_back(v);
			//Input::self()->_keyDownProc.procEvent(v);
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

	Input::MouseMoveData moveData;
	POINT p;

	//Get Mouse position
	if(m.lLastX != 0 || m.lLastY != 0)
	{
		GetCursorPos(&p);

		moveData.relativeX = m.lLastX;
		moveData.relativeY = m.lLastY;

		moveData.screenX = p.x; 
		moveData.screenY = p.y;

		ScreenToClient(this->targetMouseApp, &p);
		moveData.clientX = p.x;
		moveData.clientY = p.y;
			
		Input::self()->_mouseMove.procEvent(moveData);
	}
	else if( m.usButtonFlags > 0 )
	{
		Input::KeyCodes::Key v = (Input::KeyCodes::Key)m.usButtonFlags;
		//int i = existsInVectorList<Input::KeyCodes::Key>(this->keyList, v);

		switch (v)
		{
			case 0x0001:
			case 0x0004:
			case 0x0010:
			case 0x0040:
			case 0x100:
			{
				//if(i == -1)
				//{
					//this->keyList.push_back(v);
					Input::self()->_mouseBtnDown.procEvent(v);
				//}
			}
			break;

			case 0x0200:
			case 0x0080:
			case 0x0020:
			case 0x0008:
			case 0x0002:
			{
				//if(i != -1)
				//{
					//this->keyList.erase(this->keyList.begin() + i);
					Input::self()->_mouseBtnUp.procEvent(v);
				//}
			}
			break;
			
			case Input::KeyCodes::M_Scroll:
			{
				int delta = ((int)m.usButtonData);
				if(delta > 120)
					delta = -1;
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


template<typename T>
/* Returns position of element or -1 if not found */
int existsInVectorList(std::vector<T>& obj, T elem)
{
	int pos = -1;

	std::vector<T>::iterator i = std::find(obj.begin(), obj.end(), elem);
	if(i != obj.end())
	{
		pos = (int)(i - obj.begin());
	}

	return pos;
}






//##########################################################//
//				THREADING FUNCTION FOR PROC					//
//##########################################################//

DWORD WINAPI Input::ProcThread(LPVOID lpParameter)
{
	while (true)
	{
		Sleep(10);
		int size = Input::self()->_PrPtr->keyList.size();
		if(size)
		{
			for (int i = 0; i < size; i++)
			{
				Input::self()->_keyDownProc.procEvent(Input::self()->_PrPtr->keyList[i]);
			}
		}
	}
	return NULL;
}

//##########################################################//

