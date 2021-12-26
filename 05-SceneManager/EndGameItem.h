#pragma once
#include "AssetIDs.h"
#include "GameObject.h"
#include "Collision.h"
#include "Mario.h"
#include "Game.h"
#define EGITEM_MOVINGUP_RANGE	20
#define TIME_CHANGE	500
#define EGITEM_MOVINGUP_SPEED	0.03f
#define EGITEM_STATE_NORMAL		0
#define EGITEM_STATE_MOVINGUP	100
#define EGITEM_STATE_WORD		200
#define EGITEM_WIDTH	16
#define ID_ANI_EGSTAR	1001
#define ID_ANI_EGMUSHROOM	1002
#define ID_ANI_EGPLANT	1003
#define ID_ANI_EGSTAR_MOVING	1004
#define ID_ANI_EGMUSHROOM_MOVING	1005
#define ID_ANI_EGPLANT_MOVING	1006
#define ID_ANI_EGSTAR_SUCCESS	1007
#define ID_ANI_EGMUSHROOM_SUCCESS	1008
#define ID_ANI_EGPLANT_SUCCESS	1009
class EndGameItem : public CGameObject
{
	float initY, initX;
	bool IsStar, IsPlant, IsMushroom;
	DWORD timechange;
	CMario* player = CMario::GetInstance();
public:
	EndGameItem(float x, float y) :CGameObject(x, y) {
		IsStar = true;
		IsPlant = IsMushroom = false;
		initY = y;
		type = OBJECT_TYPE_ENDGAMEITEM;
		SetState(EGITEM_STATE_NORMAL);
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

