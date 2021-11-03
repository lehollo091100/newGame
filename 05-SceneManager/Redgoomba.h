#pragma once
#define ID_ANI_WALKINGWING	900
#define ID_ANI_JUMPING	901
#define ID_ANI_FLYING	902
#define ID_ANI_WALKING	903
#define ID_ANI_DIE	904
#define REDGOOMBA_STATE_WALKING	0
#define	REDGOOMBA_STATE_JUMPING	100
#define REDGOOMBA_STATE_FLYING	200
#define REDGOOMBA_STATE_NORMAL	300
#define REDGOOMBA_STATE_DIE	400
#define VX	0.03f
#define REDGOOMBA_WIDTH	16
#define READGOOMBA_HEIGHT	16
#define	REDGOOMBA_JUMPVY	0.15f
#define REDGOOMBA_FLYVY	0.3f
#define AY	0.001f
#include "AssetIDs.h"
#include "GameObject.h"
#include "Collision.h"
#include "Goomba.h"	
#include "Mushroom.h"
#include "debug.h"
#include "Koopas.h"
#include "Koopasitem.h"
class Redgoomba:public CGameObject
{
	DWORD time;
	int jumpcount;
	int flycount;
public:
	Redgoomba(float x, float y) :CGameObject(x, y) {
		jumpcount = 0;
		flycount = 0;
		type = OBJECT_TYPE_REDGOOMBA;
		SetState(REDGOOMBA_STATE_WALKING);
		nx = 1;
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);
};

