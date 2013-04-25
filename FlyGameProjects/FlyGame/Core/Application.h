#ifndef APPLICATION_H
#define APPLICATION_H


#include "WindowShell.h"
#include "D3DShell.h"
#include "Render\GBufferShader.h"
#include "Render\ColorShader.h"
#include "Mesh\Plane.h"
#include "Input.h"
#include "Entity.h"
#include "..\Util\Camera.h"
#include "..\Util\FlyCamera.h"
#include "..\Util\Proxy.h"

//test values
#include "Mesh\Cube.h"
#include "Light\LightHolder.h"
#include "Render\LightShader.h"

#include "Mesh\FullScreenQuad.h"

typedef Input::KeyCodes Key;

class Application
{
	private:
		bool InitD3D(Point2D size, HWND hWnd);
		bool InitWindow(HINSTANCE& hinst, Point2D size);
		bool InitInput();
		bool InitGBuffers();
		bool InitColorShader();
		bool InitLightShader();
		bool InitMatrixBuffer();
		bool LoadResources();
		void ShowMenu();
		void PlayLevel();
		//----------------
		//IShader::PER_FRAME_DATA getWVPBuffer();
		void initTestData();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

		void KeyPressEvent(Input::KeyCodes::Key k);
		void MouseMoveEvent(Input::MouseMoveData d);

	private:
		GBufferShader			gBufferShader;
		ColorShader				g_colorShader; 
		LightShader				g_lightShader;

		SmartPtrStd<BaseBuffer> pMatrixBuffer;

		//test values 
		//SmartPtrStd<Plane>		g_plane;
		//SmartPtrStd<Cube>		g_cube;
		SmartPtrStd<FullScreenQuad>	g_FullscreenQuad;
		SmartPtrStd<LightHolder>g_lightHolder;
		//------ 
		std::vector<SmartPtrStd<Entity>> objects;

		Camera mainCamera;
		FlyCamera flyCamera;

	public:
		Application();
		Application(const Application& other);
		virtual~Application();

		bool Initialize(HINSTANCE hInst, int width, int height);
		bool Initialize(HWND hInst, int width, int height);
		void Run();
		void Shutdown();
		bool Render();
		void Update();
		void DeferedRendering();

		bool Frame();
};

#endif