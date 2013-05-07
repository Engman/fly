#ifndef IGLARE_EVENT_LISTENER_H
#define IGLARE_EVENT_LISTENER_H



template<typename RetVal, typename Param>
/** 
*	The interface for creating events
*/
class IEventListener
{
	public:
		virtual RetVal Process(Param value) = 0;
		virtual void* getMethodPtr() = 0;
};



#endif