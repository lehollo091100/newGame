#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#define ID_ANI_COIN	700
#define COINITEN_AY	0.001f
#define COINITEM_VY	0.25f
#define COINITEM_RANGEY	60
#define COIN_BBOX_WIDTH	10
#define COIN_BBOX_HEIGHT	16
#define COIN_STATE_NORMAL	0
#define COIN_STATE_APPEAR	100
#define COIN_STATE_DISAPPEAR	200
class CoinItem :public CGameObject
{
	float initY;
public:
	CoinItem(float x, float y) :CGameObject(x,y) {
		SetState(COIN_STATE_NORMAL);
		vx = vy = 0;
		initY = y;
		type = OBJECT_TYPE_COINITEM;
	};
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

