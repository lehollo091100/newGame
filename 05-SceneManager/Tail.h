#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"
#define TAIL_STATE_NOTHING	0
#define TAIL_STATE_ATTACK	100
#define WIDTH	10
#define HEIGHT	5

class Tail : public CGameObject
{
public:
	Tail(float x, float y) :CGameObject(x, y) {
		type = OBJECT_TYPE_TAIL;
		SetState(0);
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);
};
