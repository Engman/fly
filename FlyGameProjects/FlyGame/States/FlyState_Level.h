#ifndef FLY_LEVEL_H
#define FLY_LEVEL_H

#include "IFlySystemState.h"

class FlyState_Level		:public IFlySystemState
{
	private:


	public:
		FlyState_Level();
		virtual~FlyState_Level();

		void Initiate(FlyGame*) override;
		void Frame() override;
};


#endif

