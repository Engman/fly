#ifndef GLARE_EVENT_LISTENER_H
#define GLARE_EVENT_LISTENER_H

#include "IEventListener.h"


template<typename Target, typename RetVal, typename Param>
/** 
*	Holds a single subscriber
*	Callback function and a pointer to the subscriber
*/
class EventListener	:public IEventListener<RetVal, Param>
{
	private:
		typedef RetVal (Target::*func)(Param);
		long	id;
		Target *subscriber;
		func	method;

	public:
		EventListener(Target* _subscriber, func _method);
		bool operator ==(func fn);
		RetVal Process(Param value) override;
		void* getMethodPtr() override;
};



template<typename Target, typename RetVal, typename Param>
EventListener<typename Target, typename RetVal, typename Param>::EventListener(Target* _subscriber, func _method)
	:subscriber(_subscriber), method(_method), id(0)
{  }

template<typename Target, typename RetVal, typename Param>
bool EventListener<typename Target, typename RetVal, typename Param>::operator ==(func fn)
{
	return this->method == fn;
}

template<typename Target, typename RetVal, typename Param>
RetVal EventListener<typename Target, typename RetVal, typename Param>::Process(Param value)
{
	return (this->subscriber->*this->method)(value);
}

template<typename Target, typename RetVal, typename Param>
void* EventListener<typename Target, typename RetVal, typename Param>::getMethodPtr() 
{
	return &this->method;
}


#endif