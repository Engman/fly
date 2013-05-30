#include "WindEmiter.h"

static std::vector<WindEmiter*> WindHolder;


WindEmiter::WindEmiter()
{
	WindHolder.push_back(this);
}
WindEmiter::~WindEmiter()
{

}

bool WindEmiter::initialize(char* scriptFile, ViewFrustum frustum)
{
	this->scriptFile = scriptFile;
	this->viewFrustum = frustum;
	return true;


	////load external lib
	//openlualibs(luaState);

	////loading luaScriptFile
	//int rv = 0;
	//rv = luaL_dofile(luaState, scriptFile);
	//if(rv != 0)
	//{
	//	PrintLuaError();
	//	return false;
	//}
	//return true;
}

bool WindEmiter::Update()
{
	return true;
}

void WindEmiter::checkCollideSphere(BoundingSphere playerSphere)
{
	for(int i=0; i<(int)this->windSpheres->size(); i++)
	{
		//if the player collide with a windSphere 
		//add it to 
		if(SphereVSSphere(this->windSpheres->at(i).sphere, playerSphere))
		{
			this->collidedSpheres->push_back(&this->windSpheres->at(i));
		}
	}
}



