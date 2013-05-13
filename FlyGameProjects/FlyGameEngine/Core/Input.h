#ifndef GLARE_INPUT_H
#define GLARE_INPUT_H

#include <Windows.h>
#include "Event.h"
 


//! Input class with subscribe system
/**
*	Handles raw input.
*	Use [ getInstance() ] to obtain a instance of Input.
*	This class is implemented as a singleton and needs to be destroyed explicitly
*	-----------------------------------------------------------------------------
*	How to use
*	1. Use [Input::Input::[instance]->registerInputDevice({overloaded functions})] to register a device type
*	2. Capture the windows message "WM_INPUT" in your WndProc
*	3. Send data from "WM_INPUT" to Input::Input::[instance]->proccessRawDeviceData(LPARAM)
*	4. Use [Input::Events::[instance]->subscribe<Caller>{deviceType}({Caller* object}, {&Caller::{function}})]
*	5. And when you exit your application call [Input::Input::Destroy]
*/
class Input
{
	public:
		struct Flags
		{
			/** Describes various input devices */
			enum DeviceType
			{
				pointer				= 1,
				mouse				= 2,
				joystick			= 4,
				gamepad				= 5,
				keyboard			= 6,
				keypad				= 7,
				multiAxisController = 8,
				TabletPCcontrols	= 9,
			};
			enum DeviceFlags
			{
				DAFAULT			= 0x00000000,
				/**
				*	If set, the application command keys are handled. 
				*	RIDEV_APPKEYS can be specified only if RIDEV_NOLEGACY 
				*	is specified for a keyboard device.
				*/
				APPKEYS			= 0x00000400,
				/**
				*	If set, the mouse button click does not activate the 
				*	other window. 
				*/
				CAPTUREMOUSE	= 0x00000200,
				/**
				*	If set, this enables the caller to receive 
				*	WM_INPUT_DEVICE_CHANGE notifications for device arrival 
				*	and device removal.
				*	Windows XP:  This flag is not supported until Windows Vista 
				*/
				DEVNOTIFY		= 0x00002000,
				/**
				*	If set, this specifies the top level collections to 
				*	exclude when reading a complete usage page. This flag 
				*	only affects a TLC whose usage page is already specified 
				*	with RIDEV_PAGEONLY. 
				*/
				EXCLUDE			= 0x00000010,
				/**
				*	If set, this enables the caller to receive input in the 
				*	background only if the foreground application does not 
				*	process it. In other words, if the foreground application 
				*	is not registered for raw input, then the background 
				*	application that is registered will receive the input.
				*	Windows XP:  This flag is not supported until Windows Vista 
				*/
				EXINPUTSINK		= 0x00001000,
				/**
				*	If set, this enables the caller to receive the input even 
				*	when the caller is not in the foreground. Note that 
				*	hwndTarget must be specified. 
				*/
				INPUTSINK		= 0x00000100,
				/**
				*	If set, the application-defined keyboard device hotkeys 
				*	are not handled. However, the system hotkeys; for example, 
				*	ALT+TAB and CTRL+ALT+DEL, are still handled. By default, 
				*	all keyboard hotkeys are handled. RIDEV_NOHOTKEYS can be 
				*	specified even if RIDEV_NOLEGACY is not specified and 
				*	hwndTarget is NULL.
				*/
				NOHOTKEYS		= 0x00000200,
				/**
				*	If set, this prevents any devices specified by usUsagePage or 
				*	usUsage from generating legacy messages. 
				*	This is only for the mouse and keyboard.
				*/
				NOLEGACY		= 0x00000030,
				/**
				*	If set, this specifies all devices whose top level collection 
				*	is from the specified usUsagePage. Note that usUsage must be 
				*	zero. To exclude a particular top level collection, 
				*	use RIDEV_EXCLUDE. 
				*/
				PAGEONLY		= 0x00000020,
				/**
				*	If set, this removes the top level collection from the 
				*	inclusion list. This tells the operating system to stop 
				*	reading from a device which matches the top level collection. 
				*/
				REMOVE			= 0x00000001,
			};
		};

		/** See http://msdn.microsoft.com/en-us/library/windows/desktop/ms645536(v=vs.85).aspx for information */
		struct GLARE_INPUT_INIT_DESC
		{
			Flags::DeviceFlags deviceFlag;
			Flags::DeviceType deviceType;
			/** Targeted window */
			HWND target;
		};
		//! Contains keycodes
		/** 
		*	M_ stands for Mouse
		*	K_ stands for Key 
		*/
		struct KeyCodes
		{
			enum Key
			{
				M_LeftBtn			= 0x02,
				M_MiddleBtn			= 0x10,
				M_RightBtn			= 0x04,
				M_Scroll			= 0x0400,
				K_Backspace			= 0x08,
				K_Tab				= 0x09,
				K_Enter				= 0x0D,
				K_Shift				= 0x10,
				K_Ctrl				= 0x11,
				K_Alt				= 0x12,
				K_Pause				= 0x13,
				K_CapsLock			= 0x14,
				K_Escape			= 0x1B,
				K_Space				= 0x20,
				K_PgUp				= 0x21,
				K_PgDown			= 0x22,
				K_End				= 0x23,
				K_Home				= 0x24,
				K_Left				= 0x25,
				K_Up				= 0x26,
				K_Right				= 0x27,
				K_Down				= 0x28,
				K_PrintScreen		= 0x2C,
				K_Insert			= 0x2D,
				K_Delete			= 0x2E,
				K_0					= 0x30,
				K_1					= 0x31,
				K_2					= 0x32,
				K_3					= 0x33,
				K_4					= 0x34,
				K_5					= 0x35,
				K_6					= 0x36,
				K_7					= 0x37,
				K_8					= 0x38,
				K_9					= 0x39,
				K_A					= 0x41,
				K_B					= 0x42,
				K_C					= 0x43,
				K_D					= 0x44,
				K_E					= 0x45,
				K_F					= 0x46,
				K_G					= 0x47,
				K_H					= 0x48,
				K_I					= 0x49,
				K_J					= 0x4A,
				K_K					= 0x4B,
				K_L					= 0x4C,
				K_M					= 0x4D,
				K_N					= 0x4E,
				K_O					= 0x4F,
				K_P					= 0x50,
				K_Q					= 0x51,
				K_R					= 0x52,
				K_S					= 0x53,
				K_T					= 0x54,
				K_U					= 0x55,
				K_V					= 0x56,
				K_W					= 0x57,
				K_X					= 0x58,
				K_Y					= 0x59,
				K_Z					= 0x5A,
				K_LeftWindows		= 0x5B,
				K_RightWindows		= 0x5C,
				K_Numpad0			= 0x60,
				K_Numpad1			= 0x61,
				K_Numpad2			= 0x62,
				K_Numpad3			= 0x63,
				K_Numpad4			= 0x64,
				K_Numpad5			= 0x65,
				K_Numpad6			= 0x66,
				K_Numpad7			= 0x67,
				K_Numpad8			= 0x68,
				K_Numpad9			= 0x69,
				K_Multiply			= 0x6A,
				K_Plus				= 0x6B,
				K_Subtract			= 0x6D,
				K_Decimal			= 0x6E,
				K_Divide			= 0x6F,
				K_F1				= 0x70,
				K_F2				= 0x71,
				K_F3				= 0x72,
				K_F4				= 0x73,
				K_F5				= 0x74,
				K_F6				= 0x75,
				K_F7				= 0x76,
				K_F8				= 0x77,
				K_F9				= 0x78,
				K_F10				= 0x79,
				K_F11				= 0x7A,
				K_F12				= 0x7B,
				K_F13				= 0x7C,
				K_F14				= 0x7D,
				K_F15				= 0x7E,
				K_F16				= 0x7F,
				K_F17				= 0x80,
				K_F18				= 0x81,
				K_F19				= 0x82,
				K_F20				= 0x83,
				K_F21				= 0x84,
				K_F22				= 0x85,
				K_F23				= 0x86,
				K_F24				= 0x87,
				K_Numlock			= 0x90,
				K_ScrlLock			= 0x91,
				K_LeftShift			= 0xA0,
				K_RightShift		= 0xA1,
				K_LeftCtrl			= 0xA2,
				K_RightCtrl			= 0xA3,

				COUNT
			};
		};
		/** Contains mouse move data */
		struct MouseMoveData
		{
			int	relativeX;
			int	relativeY;
			int	clientX;
			int	clientY;
			int	screenX;
			int	screenY;
		};
		/** Contains mouse keypress data */
		struct MouseBtnData
		{
			int MousePos_relativeX;
			int MousePos_relativeY;
			int MousePos_clientX;
			int MousePos_clientY;
			int MousePos_screenX;
			int MousePos_screenY;
			KeyCodes::Key key;
			bool released;
		};
		/** Contains information about a keypress */
		struct KeyPressData
		{
			bool shift;
			bool alt;
			bool ctrl;
			KeyCodes::Key key;
			bool released;
		};

	private:
		/** Private data defined elsewhere */
		struct _PrSt;
		_PrSt *_PrPtr;

		Event<void, KeyPressData&>		_keyDownProc;
		Event<void, KeyPressData&>		_keyUpProc;
		Event<void, MouseBtnData&>		_mouseBtnDown;
		Event<void, MouseBtnData&>		_mouseBtnUp;
		Event<void, int>				_mouseScroll;
		Event<void, MouseMoveData>		_mouseMove;
		
		Event<void, RAWKEYBOARD&>		_rawKeyboardData;
		Event<void, RAWMOUSE&>			_rawMouseData;
		Event<void, RAWHID&>			_rawHidData;


	private:
		Input																();	
		~Input																();
		static DWORD WINAPI ProcThread										(LPVOID lpParameter);

	public:
		Input																(const Input&);
		Input* operator=													(const Input&);

		/* Send raw data to this method from the windows msg proc */
		void proccessRawDeviceData											(const LPARAM&);

		/* Registers a new input device */
		bool Register														(const GLARE_INPUT_INIT_DESC&);
		/* Registers a new input device, using specific [RAWINPUTDEVICE] description */
		bool Register														(const RAWINPUTDEVICE&);
		/** Unregisters a device */
		bool Unregister														(Flags::DeviceType type);
		/** Activates the callback proc */
		void ActivateCallback												();
		/** Deactivates the callback proc */
		void DeactivateCallback												();
		

		/* Don't forget to explicitly call GlareInputHandler::Destroy() */
		static Input* self													();
		/* Destroys the GlareInputHandler instance */
		static void destroy													();

		/* Returns true if a device is registered */
		bool Input::getActive() const;


	/** Event methods */
	public:
		/** Adds a callback function to proc keydown */
		template<typename T>	void subscribeKeyDown						(T* target, void(T::*function)(KeyPressData&))
		{
			this->_keyDownProc.subscribe(target, function);
		}
		/** Adds a callback function to proc keyup */
		template<typename T>	void subscribeKeyUp							(T* target, void(T::*function)(KeyPressData&))
		{
			this->_keyUpProc.subscribe(target, function);
		}
		/** Adds a callback function to proc Mouse btn down  */
		template<typename T>	void subscribeMouseBtnDown					(T* target, void(T::*function)(MouseBtnData&))
		{
			this->_mouseBtnDown.subscribe(target, function);
		}
		/** Adds a callback function to proc Mouse btn up  */
		template<typename T>	void subscribeMouseBtnUp					(T* target, void(T::*function)(MouseBtnData&))
		{
			this->_mouseBtnUp.subscribe(target, function);
		}
		/** Adds a callback function to proc Mouse move  */
		template<typename T>	void subscribeMouseMove						(T* target, void(T::*function)(MouseMoveData&))
		{
			this->_mouseMove.subscribe(target, function);
		}
		/** Adds a callback function to proc Mouse scroll  */
		template<typename T>	void subscribeMouseScroll					(T* target, void(T::*function)(int&))
		{
			this->_mouseScroll.subscribe(target, function);
		}
		/** Adds a callback function to proc Raw keyboard data  */
		template<typename T>	void subscribeRawKeyboard					(T* target, void(T::*function)(RAWKEYBOARD))
		{
			this->_rawKeyboardData.subscribe(target, function);
		}
		/** Adds a callback function to proc Raw mouse data  */
		template<typename T>	void subscribeRawMouse						(T* target, void(T::*function)(RAWMOUSE))
		{
			this->_rawMouseData.subscribe(target, function);
		}
		/** Adds a callback function to proc Raw HID data  */
		template<typename T>	void subscribeHID							(T* target, void(T::*function)(RAWHID))
		{
			this->_rawHidData.subscribe(target, function);
		}


		/** Removes a listener from the input event, returns true if success */
		/** Adds a callback function to proc keydown */
		template<typename T>	bool unsubscribeKeyDown						(void(T::*function)(KeyPressData&))
		{
			return this->_keyDownProc.unsubscribe(function);
		}
		/** Adds a callback function to proc keyup */
		template<typename T>	bool unsubscribeKeyUp						(void(T::*function)(KeyPressData&))
		{
			return this->_keyUpProc.unsubscribe(function);
		}
		/** Adds a callback function to proc Mouse btn down  */
		template<typename T>	bool unsubscribeMouseBtnDown				(void(T::*function)(MouseBtnData&))
		{
			return this->_mouseBtnDown.unsubscribe(function);
		}
		/** Adds a callback function to proc Mouse btn up  */
		template<typename T>	bool unsubscribeMouseBtnUp					(void(T::*function)(MouseBtnData&))
		{
			return this->_mouseBtnUp.unsubscribe(function);
		}
		/** Adds a callback function to proc Mouse move  */
		template<typename T>	bool unsubscribeMouseMove					(void(T::*function)(MouseMoveData&))
		{
			return this->_mouseMove.unsubscribe(function);
		}
		/** Adds a callback function to proc Mouse scroll  */
		template<typename T>	bool unsubscribeMouseScroll					(void(T::*function)(int))
		{
			return this->_mouseScroll.unsubscribe(function);
		}
		/** Adds a callback function to proc Raw keyboard data  */
		template<typename T>	bool unsubscribeRawKeyboard					(void(T::*function)(RAWKEYBOARD))
		{
			return this->_rawKeyboardData.unsubscribe(function);
		}
		/** Adds a callback function to proc Raw mouse data  */
		template<typename T>	bool unsubscribeRawMouse					(void(T::*function)(RAWMOUSE))
		{
			return this->_rawMouseData.unsubscribe(function);
		}
		/** Adds a callback function to proc Raw HID data  */
		template<typename T>	bool unsubscribeHID							(void(T::*function)(RAWHID))
		{
			return this->_rawHidData.unsubscribe(function);
		}
};




#endif



/*
*	Information on raw input usage
* ---------------------------------------------------
RAWINPUTDEVICE.usUsagePage
	1 for generic desktop controls
	2 for simulation controls
	3 for vr
	4 for sport
	5 for game
	6 for generic device
	7 for keyboard
	8 for leds
	9 button

RAWINPUTDEVICE.usUsage values when RAWINPUTDEVICE.usUsagePage is 1
	0 - undefined
	1 - pointer
	2 - mouse
	3 - reserved
	4 - joystick
	5 - game pad
	6 - keyboard
	7 - keypad
	8 - multi-axis controller
	9 - Tablet PC controls
*/