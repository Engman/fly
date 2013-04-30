#ifndef FLY_ENGINE_H
#define FLY_ENGINE_H

#include "Core\CoreInclude.h"

class FlyEngine
{
	private:
		bool InitD3D(HWND hWnd, int width, int height, bool multisampling, bool vSync, bool fullScreen);
		bool InitGBuffers();
		bool InitColorShader();
		bool InitLightShader();
		bool InitMatrixBuffer();

	private:
		SmartPtrStd<IShader>				gbufferShader;
		SmartPtrStd<IShader>				colorShader; 
		SmartPtrStd<IShader>				lightShader;

		SmartPtrStd<BaseBuffer>				pMatrixBuffer;

		std::vector<SmartPtrStd<Entity>>	objects;
		SmartPtrStd<FullScreenQuad>			fsq;

		FlyCamera							mainCamera;

	public:
		FlyEngine();
		virtual~FlyEngine();

		bool Initialize(HWND hWnd, int width = 800, int height = 600, bool multisampling = false, bool vSync = false, bool fullScreen = false);
		void Shutdown();

		void Update();
		bool Render();
		void DeferedRendering();

		int LoadResources(std::wstring folder);
		int LoadResources(std::vector<std::wstring> resources);
		void Resize(int width, int height);
		SmartPtrStd<std::vector<SmartPtrStd<Entity>>> GetEntityList();
		Entity* PickObject(int x, int y);
		FlyCamera* getCamera();
};


#endif

