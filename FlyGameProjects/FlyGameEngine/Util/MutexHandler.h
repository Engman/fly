#ifndef MUTEX_HANDLER_H
#define MUTEX_HANDLER_H

#include <Windows.h>

enum MutexResource
{
	MutexResource_MATERIAL_HANDLER,
	MutexResource_RESOURCE_IMPORTER,
	MutexResource_ALL,
};

class MutexHandler
{
	public:
		/** Use this function to acces or return a resource importer*/
		static bool SetMutex(MutexResource res, bool get = false)
		{
			static HANDLE material = CreateMutex(NULL, 0, NULL);
			static HANDLE importer = CreateMutex(NULL, 0, NULL);

			
			if(res == MutexResource_ALL)
			{
				if(!get)
				{
					if(ReleaseMutex(material) == FALSE)	return false;
					if(ReleaseMutex(importer) == FALSE)	return false;
				}
				else
				{
					if(WaitForSingleObject(material, INFINITE) != WAIT_OBJECT_0)	return false;
					if(WaitForSingleObject(importer, INFINITE) != WAIT_OBJECT_0 )	return false;
				}

				return true;
			}
			else if(res == MutexResource_MATERIAL_HANDLER)
			{
				if(!get)
					if(ReleaseMutex(material) == FALSE)	return false;
				else
					if(WAIT_OBJECT_0 != WaitForSingleObject(material, INFINITE))	return false;
				

				return true;
			}
			else if(res == MutexResource_RESOURCE_IMPORTER)
			{
				if(!get)
					if(ReleaseMutex(importer) == FALSE)		return false;
				
				else
					if(WAIT_OBJECT_0 != WaitForSingleObject(importer, INFINITE))	return false;
				
				return true;
			}

			return true;
		}

};


#endif

