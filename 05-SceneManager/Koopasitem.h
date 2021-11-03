#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"
#define KOOPAS_STATE_WALKING	0
#define ITEM_STATE_STOP	100
#define KOOPAS_VX	0.03f
#define AY	0.001f
#define KOOPAS_WIDTH	18
#define KOOPAS_HEIGHT	26
#define RANGE_STAND_UP	10
class Koopasitem:public CGameObject
{
public:
	bool onGround;
	Koopasitem(float x, float y) :CGameObject(x, y) {
		type = OBJECT_TYPE_KOOPASITEM;
		SetState(KOOPAS_STATE_WALKING);
		nx = 1;
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

