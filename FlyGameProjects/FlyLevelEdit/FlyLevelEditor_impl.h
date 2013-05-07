#ifndef FLY_LEVEL_EDITOR_IMPL_H
#define FLY_LEVEL_EDITOR_IMPL_H


#pragma comment(lib, "FlyGameEngineDx86.lib")


#include "FlyLevelEditor.h"
#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\FlyGameEngine\Util\SmartPtrs.h"


class FlyLevelEditor_impl		:public FlyLevelEditor
{
	private:
		FlyEngine* core;
		std::vector<Entity*> mesh;
		std::vector<Entity*> lights;
		std::vector<SmartPtrStd<Camera>> cameras;

	public:
		FlyLevelEditor_impl();
		virtual~FlyLevelEditor_impl();
		virtual void FLYCALL Terminate();
		virtual bool FLYCALL Initiate(HWND parent, int width, int height);
		virtual void FLYCALL Frame() override;
		virtual void FLYCALL Resize(int width, int height);
		virtual void FLYCALL SetCamera(CAMERA cam);
		virtual bool FLYCALL LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded);
};

extern FlyLevelEditor_impl* gEditorInstance;


#endif





