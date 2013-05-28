#ifndef FLY_SYSTEM_STATE_H
#define FLY_SYSTEM_STATE_H

class FlyGame;

enum InuptControlScheme
{
	CONTROL_Casual = 0,
	CONTROL_Advance = 1,
#if defined (_DEBUG) || (DEBUG)
	CONTROL_Debug = 2,
#else
	CONTROL_INVALID = -1,
#endif
};

class IFlySystemState
{
	protected:
		FlyGame* entryInstance;

	public:
		virtual~IFlySystemState	() {  }

		virtual bool Initiate	(FlyGame*) = 0;
		virtual void Frame		() = 0;
		virtual void Release	() = 0;
};


#endif

