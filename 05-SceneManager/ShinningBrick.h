#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Debris.h"
#define SBRICK_WIDTH	16
#define SBRICK_STATE_NORMAL	0
#define ID_ANI_NORMAL	945
#define ID_ANI_DEBRIS	946
class ShinningBrick : public CGameObject
{
	float initX, initY;

public:
	Debris* d1;
	Debris* d2;
	Debris* d3;
	Debris* d4;
	ShinningBrick(float x, float y) : CGameObject(x, y)
	{
		type = OBJECT_TYPE_SHINNINGBRICK;
		SetState(SBRICK_STATE_NORMAL);
		initX = x;
		initY = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};



