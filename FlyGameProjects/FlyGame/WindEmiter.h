#ifndef WINDEMITER_H
#define WINDEMITER_H

#include "..\FlyGameEngine\Util\SmartPtrs.h"
#include "..\FlyGameEngine\Util\BoundingVolumes.h"
#include "..\FlyGameEngine\Util\CollisionLib.h"
#include <vector>


class WindEmiter
{
public: 
	struct WindSphere
	{
		BoundingSphere sphere;
		D3DXVECTOR3 direction; 
	};

private:

	

	ViewFrustum viewFrustum;
	SmartPtrStd<std::vector<WindSphere>> windSpheres;
	char* scriptFile;
	SmartPtrStd<std::vector<WindSphere*>> collidedSpheres;

	void checkCollideSphere(BoundingSphere playerSphere);

public:
	WindEmiter();
	~WindEmiter();

	bool initialize(char* scriptFile, ViewFrustum frustum);
	bool Update();


};
#endif