#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "ColorBrick.h"
#include "QuestionBrick.h"
#include "CoinItem.h"
#include "Mushroom.h"
#include "Pipe.h"
#include "Koopas.h"
#include "Redgoomba.h"
#include "Koopasitem.h"
#include "GreenPlant.h"
#include "FireRedPlant.h"
#include "PlantFire.h"
#include "Tail.h"
#include "ShinningBrick.h"
#include "Debris.h"
#include "PBrick.h"
#include "HUD.h"
#include "FireGreenPlant.h"
#include "Leaf.h"
#include "EndGameItem.h"
#include "Curtain.h"
#define Sequence1MaxTime	2000

class IntroScene :public CScene
{
protected:
	// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	Map* map;


	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);


	void LoadAssets(LPCWSTR assetFile);

public:
	int mapid;
	//CMario* redMario = new CMario(0, 0);
	//CMario* greenMario = new CMario(0, 0);
	Curtain* curtain = new Curtain(0,0);
	DWORD SequenceTime;
	IntroScene(int id, LPCWSTR filePath);
	bool isDoneSeq1, isDoneSeq2, isFirstJump;
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void ScriptIntro()
	{
		/*if (SequenceTime == 0)
			SequenceTime = GetTickCount64();
		if (!isDoneSeq1)
		{
			if (GetTickCount64() - SequenceTime >= Sequence1MaxTime)
				isDoneSeq1 = true;
		}
		if (isDoneSeq1 && !isDoneSeq2)
		{

			redMario->SetState(MARIO_STATE_WALKING_RIGHT);
			greenMario->SetState(MARIO_STATE_WALKING_LEFT);
			if (GetTickCount64() - SequenceTime >= Sequence1MaxTime + 2000 && !isFirstJump)
			{
				redMario->SetState(MARIO_STATE_JUMP);
				isFirstJump = true;
			}
			redMario->IsAllowRender = true;
			greenMario->IsAllowRender = true;
			if (GetTickCount64() - SequenceTime >= Sequence1MaxTime + 1500)
			{
				redMario->IsAllowUpdate = true;
				greenMario->IsAllowUpdate = true;
			}
		}*/
	}
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef IntroScene* LPINTROSCENE;
