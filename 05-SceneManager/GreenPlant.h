#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#define GREENPLANT_TIME	2000
#define GREENPLANT_RANGE	30
#define GREENPLANT_VY	0.03f
#define GREENPLANT_WIDTH	18
#define GREENPLANT_HEIGHT	24
#define GREENPLANT_STATE_START	0
#define GREENPLANT_STATE_MOVINGUP	100
#define GREENPLANT_STATE_STOP	200
#define GREENPLANT_STATE_MOVINGDOWN	300
#define ID_ANI_GREENPLANT_NORMAL	921

class GreenPlant:public CGameObject
{
	float initY;
	DWORD time;
public:
	GreenPlant(float x, float y) :CGameObject(x, y) {
		time = 0;
		initY = y;
		SetState(GREENPLANT_STATE_START);
		type = OBJECT_TYPE_GREENPLANT;
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

