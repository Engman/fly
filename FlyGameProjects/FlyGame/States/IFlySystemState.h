#ifndef FLY_SYSTEM_STATE_H
#define FLY_SYSTEM_STATE_H

class FlyGame;

class IFlySystemState
{
	protected:
		FlyGame* entryInstance;

	public:
		virtual ~IFlySystemState(){};

		virtual void Initiate(FlyGame*) = 0;
		virtual void Frame() = 0;

		virtual void Release() = 0;
};


#endif

