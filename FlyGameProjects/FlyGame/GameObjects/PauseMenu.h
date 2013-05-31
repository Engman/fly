#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "../../FlyGameEngine/Core/Mesh/FlyMesh.h"
#include "../../FlyGameEngine/Core/Entity.h"
#include "../FlyGame.h"
#include "../../FlyGameEngine/FlyEngine/FlyEngine.h"
#include "../../FlyGameEngine/Core/Input.h"

class PauseMenu
{
	private:
		vector<Entity*> models;
		Entity* currentRender;

		float windowHeight, windowWidth;
		bool hover; 

	public:
		PauseMenu();
		virtual ~PauseMenu();

		bool Initialize(FlyGame* entry, float windowHeight, float windowWidth);

		void Render(ViewFrustum f, bool one, bool two, bool three);
		int Update(int mouseX, int mouseY, FlyGame* entry); // 0 Render again, 1 Continue, 2 Exit

		void Release();
};

#endif