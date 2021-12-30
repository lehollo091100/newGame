#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Debris.h"
#define SBRICK_WIDTH	16
#define SBRICK_STATE_NORMAL	0
#define SBRICK_STATE_COIN	100
#define ID_ANI_SBRICK_NORMAL	945
#define ID_ANI_DEBRIS	946
#define ID_ANI_COIN		700
#define TIME_TO_RETURN	5000
class ShinningBrick : public CGameObject
{
	float initX, initY;
	bool isCoin;
	DWORD time;
public:
	Debris* d1;
	Debris* d2;
	Debris* d3;
	Debris* d4;
	ShinningBrick(float x, float y) : CGameObject(x, y)
	{
		isCoin = false;
		type = OBJECT_TYPE_SHINNINGBRICK;
		SetState(SBRICK_STATE_NORMAL);
		initX = x;
		initY = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual bool IsItem() {
		if (state == SBRICK_STATE_COIN)
		{
			return true;
		}
		return false;
	}
};



