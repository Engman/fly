#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include "..\..\Util\Proxy.h"

class DirectionLight
{
private:
	DirectionalLightProxy dirLight; 

public:
	DirectionLight();
	~DirectionLight();

	void Initialize(DirectionalLightProxy data);


};
#endif