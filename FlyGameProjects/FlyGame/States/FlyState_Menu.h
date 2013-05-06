#ifndef FLY_MENU_H
#define FLY_MENU_H

#include "IFlySystemState.h"

class FlyState_Menu		:public IFlySystemState
{
	private:


	public:
		FlyState_Menu();
		virtual~FlyState_Menu();

		void Initiate(FlyGame*) override;
		void Frame() override;
};


#endif

