#pragma once
#include "AssetIDs.h"
#include "GameObject.h"
#include "Collision.h"
#define LEAF_STATE_INVISIBLE	0
#define LEAF_STATE_MOVING	100
#define ID_ANI_LEAF_MOVELEFT	981
#define ID_ANI_LEAF_MOVERIGHT	982
#define LEAF_AY	0.02f
#define MOVINGUP_VY	0.3f
#define LEAF_RANGE	10
#define LEAF_VX	0.02f
#define LEAF_RANGEX	10
#define	LEAF_WIDTH	18
#define LEAF_HEIGHT	16
class Leaf : public CGameObject
{
	float initY,initX;
public:

	Leaf(float x, float y) :CGameObject(x, y) {
		nx = 1;
		initY = y;
		initX = x;
		type = OBJECT_TYPE_LEAF;
		vx = 0; vy = 0;
		SetState(LEAF_STATE_INVISIBLE);
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

