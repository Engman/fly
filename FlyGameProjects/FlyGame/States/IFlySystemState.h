#ifndef FLY_SYSTEM_STATE_H
#define FLY_SYSTEM_STATE_H

class IFlySystemState
{
	public:
		virtual void Update() = 0;
		virtual void Render() = 0;
};


#endif

