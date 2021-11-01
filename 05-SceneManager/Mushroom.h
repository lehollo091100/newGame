#pragma once
#include "AssetIDs.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Collision.h"
#define MUSHROOM_STATE_INVISIBLE	0
#define MUSHROOM_STATE_UP	100
#define MUSHROOM_STATE_MOVING	200
#define ID_ANI_MUSHROOM 850
#define VY	0.03f
#define	VX	0.04f
#define RANGE	16
#define AX	0.002f
class Mushroom : public CGameObject
{
	float initY;
public:

	Mushroom(float x, float y) :CGameObject(x,y) {
		nx = 1;
		initY = y;
		type = OBJECT_TYPE_MUSHROOM;
		vx = 0; vy = 0;
		SetState(MUSHROOM_STATE_INVISIBLE);
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e,DWORD dt);
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

