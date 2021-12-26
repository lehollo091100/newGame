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
class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	/*LPGAMEOBJECT player;*/	
	CMario* player = CMario::GetInstance();
	HUD* hud = HUD::GetInstance();
	Map* map;
	vector<LPGAMEOBJECT> objects;
	vector<QuestionBrick*> questionbricks;
	vector<LPGAMEOBJECT> items;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	int mapid;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

