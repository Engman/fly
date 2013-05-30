#ifndef FLY_MENU_H
#define FLY_MENU_H

#include "IFlySystemState.h"
#include "..\..\FlyGameEngine\Util\Camera.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"



class FlyState_Menu		:public IFlySystemState
{
	private:
		//Menu components
		vector<Entity*> ui;
		//Pointers to check collision
		vector<Entity*> uiBtn;

		//Pointer to current submenu
		Entity	*subMenu;
		int		highlightBtn;
		Camera	mainMenuCam;

		void update();
		void render();
		void input();
		void PickMenu();
		bool ReadData();

	public:
		FlyState_Menu();
		virtual~FlyState_Menu();

		bool Initiate(FlyGame*) override;
		void Frame() override;
		void Release() override;
};


#endif

