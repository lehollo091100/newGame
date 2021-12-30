#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"
#define PBUTTON_NOTHING_HEIGHT	5
#define PBUTTON_WIDTH	14
#define PLUS_RANGE	8
#define PBUTTON_VY	0.05f
#define PBUTTON_STATE_NORMAL	0
#define PBUTTON_STATE_MOVING	100
#define PBUTTON_STATE_NOTHING	200
#define ID_ANI_PBUTTON_NORMAL	950
#define ID_ANI_PBUTTON_NOTHING	951
class PButton : public CGameObject
{
	float initX, initY;
public:
	PButton(float x, float y) : CGameObject(x, y)
	{
		type = OBJECT_TYPE_PBUTTON;
		SetState(PBUTTON_STATE_NORMAL);
		vx = vy = 0;
		initX = x;
		initY = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
