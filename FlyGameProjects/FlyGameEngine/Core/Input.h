#ifndef INPUT_H
#define INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
	private:
		static bool instanceFlag;
		static Input* single;

		HRESULT ReadKeyboard();
		HRESULT ReadMouse();
		void ProcessInput();

		HWND hWnd;

		IDirectInput8* m_pDirectInput;
		IDirectInputDevice8* m_pKeyboard;
		IDirectInputDevice8* m_pMouse;

		unsigned char m_keyboardState[256];
		DIMOUSESTATE2 m_mouseState;

		int m_screenHeight;
		int m_screenWidth;

		int m_mouseX;
		int m_mouseY;

		Input();
		Input(const Input&);
		
	public:
		~Input();

		static Input* self();

		HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight);
		void ReleaseInput();
		HRESULT Frame();

		bool IsButtonPressed(int keyState);
		bool IsMouseButtonPressed(int key);
		void GetMouseLocation(int& mouseX, int& mouseY);
		void GetMouseRelative(int& mouseX, int& mouseY);

		void SetCoopExclusive();
		void SetCoopNonExclusive();
};

#endif