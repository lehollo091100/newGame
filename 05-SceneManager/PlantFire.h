#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#define TIME	2000
#define VY	0.03f
#define WIDTH	12
#define RANGE	(HEIGHT+4)
#define FIREPLANT_START	0
#define FIREPLANT_MOVING	100
#define ID_ANI_NORMAL	941

class PlantFire :public CGameObject
{
public:
	PlantFire(float x, float y) :CGameObject(x, y) {
		SetState(FIREPLANT_START);
		type = OBJECT_TYPE_PLANTFIRE;
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



