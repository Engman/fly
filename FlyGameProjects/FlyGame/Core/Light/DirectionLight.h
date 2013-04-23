#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include "..\..\Util\Proxy.h"
//#include "..\Entity.h"

class DirectionLight //: public Entity
{
private:
	DirectionalLightProxy dirLight; 

public:
	DirectionLight();
	~DirectionLight();

	void Initialize(DirectionalLightProxy data);


};
#endif