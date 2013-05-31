#include "GID.h"


#if defined(_DEBUG) || defined (DEBUG)

using namespace std;

static vector<int> gidAlloc = vector<int>();

GID::GID() 
{ 
	this->id = usft();
	gidAlloc.push_back(id);
}  
GID::~GID()
{
	for (int i = 0; i < (int)gidAlloc.size(); i++)
		if(gidAlloc[i] == id)
		{
			gidAlloc[i] = -1;
		}
}
vector<int>& GID::getAllocList()
{
	return gidAlloc;
}

#else

GID::GID() 
{ 
	this->id = usft();
}  
GID::~GID()
{

}

#endif