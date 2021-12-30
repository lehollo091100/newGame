#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "CoinItem.h"
#include "Mushroom.h"
#include "Leaf.h"
#define QBRICK_WIDTH	16
#define ID_ANI_QBRICK_NORMAL	801
#define ID_ANI_QBRICK_NOTHING	802
#define QBSTATE_NORMAL	0
#define QBSTATE_UP	100
#define QBSTATE_MOVING	200
#define QBSTATE_NOTHING	300
#define RANGE_UP	5
#define QBRICK_VY 0.02f
class QuestionBrick: public CGameObject
{
	float initX, initY;

public:
	LPGAMEOBJECT item;
	QuestionBrick(float x, float y) : CGameObject(x, y)
	{
		type = OBJECT_TYPE_QUESTIONBRICK;
		vy = 0;
		SetState(QBSTATE_NORMAL);
		initX = x;
		initY = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

