#ifndef WINDEMITER_H
#define WINDEMITER_H

#include "Util/misc.h"
#include "Util/SmartPtrs.h"
#include "Util/BoundingVolumes.h"
#include "Util/CollisionLib.h"


class WindEmiter
{
public: 
	struct WindSphere
	{
		BoundingSphere sphere;
		D3DXVECTOR4 direction; 
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