#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"

#define WIDTH	16
#define SWIDTH	14
#define PLUS_RANGE	8
#define PBRICK_VY	0.05f
#define PBUTTON_STATE_NORMAL	0
#define PBUTTON_STATE_MOVING	100
#define PBUTTON_STATE_NOTHING	200
#define ID_ANI_NORMAL	950
#define ID_ANI_NOTHING	951
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
