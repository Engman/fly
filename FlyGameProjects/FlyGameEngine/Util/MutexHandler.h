#ifndef MUTEX_HANDLER_H
#define MUTEX_HANDLER_H

#include <Windows.h>

enum MutexResource
{
	MATERIAL_HANDLER,
	RESOURCE_IMPORTER,
};

class MutexHandler
{
	public:
		/** Use this function to acces or return a resource importer*/
		static bool SetMutex(MutexResource res, bool get = false)
		{
			static HANDLE material = CreateMutex(NULL, 0, NULL);
			static HANDLE importer = CreateMutex(NULL, 0, NULL);

			
			if(res == MATERIAL_HANDLER)
			{
				if(!get)
				{
					if(ReleaseMutex(material) == FALSE)
						return false;
				}
				else
				{
					DWORD result = WaitForSingleObject(material, INFINITE);
					//Something bad happend
					if(WAIT_OBJECT_0 != result)
						return false;
				}

				return true;
			}
			else if(res == RESOURCE_IMPORTER)
			{
				if(!get)
				{
					if(ReleaseMutex(importer) == FALSE)
						return false;
				}
				else
				{
					DWORD result = WaitForSingleObject(importer, INFINITE);
					//Something bad happend
					if(WAIT_OBJECT_0 != result)
						return false;
				}

				return true;
			}

			return true;
		}

};


#endif

