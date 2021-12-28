#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define ID_ANI_CURTAIN	1011
#define INTROBACKGROUND_STATE_NORMAL	0
#define INTROBACKGROUND_STATE_MOVING	100
#define INTROBACKGROUND_STATE_CHANGE	200
#define INTROBACKGROUND_VY	0.1f
#define CURTAIN_HEIGHT	200
class IntroBackGround :public CGameObject
{
public:
	IntroBackGround(float x, float y) : CGameObject(x, y) {
		type = OBJECT_TYPE_INTROBACKGROUND;
		SetState(INTROBACKGROUND_STATE_NORMAL);
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual bool IsItem() {
		return true;
	}
};

