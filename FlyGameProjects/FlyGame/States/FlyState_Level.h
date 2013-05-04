#ifndef FLY_LEVEL_H
#define FLY_LEVEL_H

#include "IFlySystemState.h"

class FlyState_Level		:public IFlySystemState
{
	private:


	public:
		FlyState_Level();
		virtual~FlyState_Level();

		void Update() override;
		void Render() override;
};


#endif

