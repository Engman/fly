#ifndef FLY_EDITOR_H
#define FLY_EDITOR_H

#include "IFlySystemState.h"		

class FlyState_Editor		:public IFlySystemState
{
	private:


	public:
		FlyState_Editor();
		virtual~FlyState_Editor();

		bool Initiate(FlyGame*) override;
		void Frame() override;

		void Release() override;
};


#endif

