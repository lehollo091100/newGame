#pragma once
#include "GameObject.h"
#include "Collision.h"
#define RANGE_DISAPPEAR	30
#define ID_ANI_DEBRIS	946
#define AY	0.001f
#define WIDTH	10
#define DEBRIS_STATE_NORMAL	0
#define DEBRIS_STATE_MOVING	1
#define DEBRIS_RANGE	3
#define DEBRIS_VY_BIG	0.3f
#define DEBRIS_VY_SMALL	0.1f
#define DEBRIS_VX		0.05f
class Debris : public CGameObject
{
	float initY;
	float Vx;
	float Vy;
public:

	Debris(float x, float y, float vx1, float vy1,int n1	) :CGameObject(x, y) {
		initY = y;
		type = OBJECT_TYPE_DEBRIS;
		vx = 0; vy = 0;
		SetState(DEBRIS_STATE_NORMAL);
		Vx = vx1;
		Vy = vy1;
		nx = n1;
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

