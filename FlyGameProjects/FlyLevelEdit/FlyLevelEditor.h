#ifndef FLY_LEVEL_EDITOR_H
#define FLY_LEVEL_EDITOR_H

class FlyEngine;
struct FLY_ENGINE_INIT_DESC;

#include <vector>
#include <Windows.h>
#include <map>
#include <string>


#ifdef FLYEDITDLL
#define FLY_EDIT_EXPORT __declspec(dllexport)
#else
#define FLY_EDIT_EXPORT
#endif
#define FLYCALL  __stdcall


#define KEY_W				0x57
#define KEY_A				0x41
#define KEY_S				0x53
#define KEY_D				0x44
#define KEY_ESC				0x1B
#define KEY_SPACE			0x20
#define KEY_LCTRL			0xA2
#define KEY_RCTRL			0xA3
#define KEY_ALT				0x12
#define KEY_LSHIFT			0xA0
#define KEY_RSHIFT			0xA1
#define KEY_MOUSE_LBTN		0x01
#define KEY_MOUSE_RBTN		0x02
#define KEY_MOUSE_MBTN		0x04



class FlyLevelEditor
{
	public:
		virtual void FLYCALL Terminate()									= 0;
		virtual bool FLYCALL Initiate(HWND parent, int width, int height)	= 0;
		virtual void FLYCALL Frame()										= 0;
		virtual void FLYCALL Resize(int width, int height)					= 0;
		virtual bool FLYCALL SetCamera(int id)								= 0;
		virtual bool FLYCALL Entity_Select(int id)							= 0;
		virtual bool FLYCALL Entity_LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded)	= 0;
		virtual void FLYCALL GetCameras (std::map<std::wstring, int>* outCameras) = 0;
		virtual bool FLYCALL GetSelected (std::wstring& name, int& id, float& rx, float& ry, float& rz, float& sx, float& sy, float& sz) = 0;
		virtual void FLYCALL SetRotation(float x, float y, float z)			= 0;
		virtual void FLYCALL SetScale(float x, float y, float z)			= 0;
		virtual bool FLYCALL SetName(const std::wstring& name)				= 0;
		virtual void FLYCALL SetFlyMode(bool flyMode)						= 0;
		virtual void FLYCALL GetSpeed(float& speed)							= 0;
		virtual void FLYCALL SetSpeed(float speed)							= 0;
		virtual bool FLYCALL LoadLevel(const std::wstring& path, std::map<std::wstring, int>* meshEntity, std::map<std::wstring, int>* lights, std::map<std::wstring, int>* pickups)			= 0;
		virtual bool FLYCALL SaveLevel(const std::wstring& path, const std::wstring& levelName)			= 0;

		virtual void FLYCALL OnKeyEvent(int key, bool released, bool ctrl, bool shift, bool alt)					= 0;
		virtual void FLYCALL OnMouseBtnEvent(int key, bool released, bool ctrl, bool shift, bool alt)				= 0;
		virtual void FLYCALL OnMouseMoveEvent(int cx, int cy, int rx, int ry, bool ctrl, bool shift, bool alt)		= 0;
		virtual void FLYCALL OnMouseScrollEvent(int delta, bool ctrl, bool shift, bool alt)							= 0;
};

extern "C"
{
	FLY_EDIT_EXPORT FlyLevelEditor* FLYCALL GetEditorInstance();
}


#endif

