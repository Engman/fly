#ifndef FLY_CUTSCENE_H
#define FLY_CUTSCENE_H

enum FlyCutsceneType
{
	FlyCutsceneType_Intro,
	FlyCutsceneType_Level1,
	FlyCutsceneType_Level2,
	FlyCutsceneType_Level3,
	NONE,
	FlyCutsceneType_COUNT
};

class FlyEngine;

class FlyCutscene
{
	public:
		static bool RunCutscene(FlyCutsceneType scene, FlyEngine* _core);
};


#endif

