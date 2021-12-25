#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "PButton.h"
#define PBRICK_WIDTH	16
#define PBRICK_VY	0.1f
#define PBRICK_STATE_NORMAL	0
#define PBRICK_STATE_MOVING	100
#define PBRICK_STATE_NOTHING	200
#define ID_ANI_NORMAL	945
#define ID_ANI_NOTHING	802
#define ID_ANI_DEBRIS	946
class PBrick : public CGameObject
{
	float initX, initY;

public:
	PButton* button;
	PBrick(float x, float y) : CGameObject(x, y)
	{
		type = OBJECT_TYPE_PBRICK;
		SetState(PBRICK_STATE_NORMAL);
		initX = x;
		initY = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};





