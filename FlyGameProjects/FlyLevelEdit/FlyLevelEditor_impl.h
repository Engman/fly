#ifndef FLY_LEVEL_EDITOR_IMPL_H
#define FLY_LEVEL_EDITOR_IMPL_H


#pragma comment(lib, "FlyGameEngineDx86.lib")


#include "FlyLevelEditor.h"
#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "..\FlyGameEngine\Util\SmartPtrs.h"
#include "..\FlyGameEngine\Core\Input.h"


class FlyLevelEditor_impl		:public FlyLevelEditor
{
	private:
		struct CameraObject
		{
			SmartPtrStd<Camera> camera;
			std::wstring name;
		};

	private:
		FlyEngine* core;
		std::vector<Entity*> mesh;
		std::vector<Entity*> lights;
		std::vector<SmartPtrStd<CameraObject>> cameras;
		Entity* selected;
		bool flyMode;

	private:
		void MouseScrollEvent(int &delta);
		void KeyDownEvent(Input::MouseBtnData &data);
		void KeyReleaseEvent(Input::MouseBtnData &data);
		void MouseMoveEvent(Input::MouseMoveData &data);
		void KeyPress(Input::KeyPressData &data);

	public:
		FlyLevelEditor_impl();
		virtual~FlyLevelEditor_impl();

		virtual void FLYCALL Terminate()													override;
		virtual bool FLYCALL Initiate(HWND parent, int width, int height)					override;
		virtual void FLYCALL Frame()														override;
		virtual void FLYCALL Resize(int width, int height)									override;
		virtual bool FLYCALL SetCamera(int id)												override;
		virtual bool FLYCALL Entity_Select(int id)											override;
		virtual void FLYCALL GetCameras(std::map<std::wstring, int>* outCameras)			override;
		virtual void FLYCALL FlyLevelEditor_impl::SetRotation(float x, float y, float z)	override;
		virtual void FLYCALL FlyLevelEditor_impl::SetScale(float x, float y, float z)		override;
		virtual bool FLYCALL SetName(const std::wstring& name)								override;
		virtual void FLYCALL FreeFlyMode(bool set)											override;
		virtual bool FLYCALL GetFlyStatus()													override;
		virtual bool FLYCALL Entity_LoadData(std::vector<const wchar_t*>& files, std::map<std::wstring, int>* loaded) override;
		virtual bool FLYCALL GetSelected (std::wstring& name, int& id, float& rx, float& ry, float& rz, float& sx, float& sy, float& sz) override;
};

extern FlyLevelEditor_impl* gEditorInstance;


#endif





