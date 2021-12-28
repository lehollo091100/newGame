#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define ID_ANI_INTROOPTION_UP	1110
#define ID_ANI_INTROOPTION_DOWN	1111
#define INTROOPTION_STATE_UP	0
#define INTROOPTION_STATE_DOWN	100
#define INTROOPTION_VY	0.1f
#define INTROOPTION_HEIGHT	200
class IntroOption :public CGameObject
{
public:
	static IntroOption* __instance;
	IntroOption(float x, float y) : CGameObject(x, y) {
		type = OBJECT_TYPE_INTROOPTION;
		SetState(INTROOPTION_STATE_UP);
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
	static IntroOption* GetInstance();
};


