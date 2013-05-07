#ifndef GLARE_EVENT_TEMPLATE_H
#define GLARE_EVENT_TEMPLATE_H

#include <vector>

#include "EventListener.h"


template<typename RetVal, typename Param>
/** 
*	Core in the event callback system
*	Stores subsribers and procs
*/
class Event
{
	private:
		std::vector<IEventListener<RetVal, Param>*> subscribers;
			
	public:
		Event();
		virtual~Event();

		int getCount() const
		{
			return (int)this->subscribers.size();
		}


		std::vector<IEventListener<RetVal, Param>*>* getSubscribers()
		{
			return this->subscribers;
		}

		template<typename Target>
		void subscribe( Target* object, RetVal(Target::*func)(Param))
		{
			for (int i = 0; i < (int)this->subscribers.size(); i++)
			{
				if((*this->subscribers[i]).getMethodPtr() == &func)
					return;
			}
			this->subscribers.push_back(new EventListener<Target, RetVal, Param>(object, func));
		}
		template<typename Target>
		bool unsubscribe(RetVal(Target::*fn)(Param))
		{
			typedef void(Target::*func)(Param);
			for (int i = 0; i < (int)this->subscribers.size(); i++)
			{
				if(*(func*)(*this->subscribers[i]).getMethodPtr() == fn)
				{
					this->subscribers.erase(this->subscribers.begin() + i);
					return true;
				}
			}
			return false;
		}
		void procEvent(Param& data);
};


template<typename RetVal, typename Param>
Event<typename RetVal, typename Param>::Event()
{  }
template<typename RetVal, typename Param>
Event<typename RetVal, typename Param>::~Event()
{
	for (int i = 0; i < (int)this->subscribers.size(); i++)
	{
		delete this->subscribers[i];
		this->subscribers.erase(this->subscribers.begin() + i);
	}
}

template<typename RetVal, typename Param>
void Event<typename RetVal, typename Param>::procEvent(Param& data)
{
	for (int i = 0; i < (int)this->subscribers.size(); i++)
	{
		this->subscribers[i]->Process(data);
	}
}


#endif