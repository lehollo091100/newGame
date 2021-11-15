#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#include "Mario.h"
#define TIME	2000
#define VY	0.03f
#define WIDTH	18
#define HEIGHT	30
#define RANGE	(HEIGHT+4)
#define FIREREDPLANT_STATE_START	0
#define FIREREDPLANT_STATE_MOVINGUP	100
#define FIREREDPLANT_STATE_STOP	200
#define FIREREDPLANT_STATE_MOVINGDOWN	300
#define ID_ANI_OPENDOWNLEFT	931
#define ID_ANI_OPENUPLEFT	932
#define ID_ANI_MOVINGLEFTUP	933
#define ID_ANI_MOVINGLEFTDOWN	934
#define ID_ANI_OPENDOWNRIGHT	935
#define ID_ANI_OPENUPRIGHT	936
#define ID_ANI_MOVINGRIGHTUP	937
#define ID_ANI_MOVINGRIGHTDOWN	938

class FireRedPlant :public CGameObject
{
	float initY;
	DWORD time;
public:
	CMario* mario = CMario::GetInstance();
	FireRedPlant(float x, float y) :CGameObject(x, y) {

		time = 0;
		initY = y;
		SetState(FIREREDPLANT_STATE_START);
		type = OBJECT_TYPE_FIREREDPLANT;
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);
};


