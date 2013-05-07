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


class FlyLevelEditor
{
	public:
		enum CAMERA
		{
			CAM_TOP = 0,
			CAM_DEFAULT = 100,
		};

		virtual void FLYCALL Terminate()									= 0;
		virtual bool FLYCALL Initiate(HWND parent, int width, int height)	= 0;
		virtual void FLYCALL Frame()										= 0;
		virtual void FLYCALL Resize(int width, int height)					= 0;
		virtual void FLYCALL SetCamera(CAMERA cam)							= 0;
		virtual bool FLYCALL LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded)	= 0;
};

extern "C"
{
	FLY_EDIT_EXPORT FlyLevelEditor* FLYCALL GetEditorInstance();
}


#endif

