#ifndef APPLICATION_H
#define APPLICATION_H


#include "WindowShell.h"
#include "D3DShell.h"
#include "Render\GBufferShader.h"
#include "Render\FinalShader.h"
#include "Mesh\Plane.h"
#include "Input.h"
#include "Entity.h"
#include "..\Util\Camera.h"
#include "..\Util\FlyCamera.h"
#include "..\Util\Proxy.h"
#include "Mesh\Terrain.h"

//test values
#include "Mesh\Cube.h"
#include "Light\LightHolder.h"
#include "Render\LightShader.h"
#include "Render\ShadowMapShader.h"
#include "Render\BlurShader.h"
#include "Render\GBufferAnimationShader.h"
#include "Mesh\FullScreenQuad.h"

typedef Input::KeyCodes Key;

class Application
{
	private:
		bool InitD3D(Point2D size, HWND hWnd);
		bool InitWindow(HINSTANCE& hinst, Point2D size);
		bool InitInput();
		bool InitGBuffers();
		bool InitFinalShader();
		bool InitLightShader();
		bool InitShadowMapShader();
		bool InitBlurShader();
		bool InitAnimationShader();
		bool InitMatrixBuffer();
		bool LoadResources();
		void ShowMenu();
		void PlayLevel();
		//----------------

		IShader::PER_FRAME_DATA getWVPBuffer();

		void initTestData();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

		void KeyPressEvent(Input::KeyCodes::Key k);
		void MouseMoveEvent(Input::MouseMoveData d);

	private:
		GBufferShader			gBufferShader;
		FinalShader				g_finalShader; 
		LightShader				g_dirLightShader;
		LightShader				g_pointLightShader;
		ShadowMapShader			g_shadowMapShader;
		BlurShader				g_blurHorizontShader;
		BlurShader				g_blurVerticalShader;
		GBufferAnimationShader  g_animationShader;
		SmartPtrStd<BaseBuffer> pMatrixBuffer;

		//test values 

		SmartPtrStd<Plane>		g_plane;
		SmartPtrStd<Cube>		g_cube;
		SmartPtrStd<Cube>		g_cube2;
		SmartPtrStd<Cube>		g_animatedCube;
		SmartPtrStd<FullScreenQuad>	g_FullscreenQuad;
		SmartPtrStd<LightHolder>g_lightHolder;
		SmartPtrStd<FullScreenQuad> g_dirLight;

		SmartPtrStd<Terrain> g_terrain;

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