#pragma once
#include "debug.h"
#include "GameObject.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "Mushroom.h"
#include "Goomba.h"
#include "Brick.h"
#include "Pipe.h"
#include "Koopasitem.h"
#define ID_ANI_WALKINGLEFT	861
#define ID_ANI_WALKINGRIGHT	862
#define ID_ANI_DEFENDDOWN	863
#define KOOPAS_STATE_WALKING	0
#define KOOPAS_STATE_DEFENDDOWN	100
#define KOOPAS_STATE_ATTACKDOWN	200
#define KOOPAS_STATE_HOLDDOWN	300
#define KOOPAS_STATE_DEFENDUP	400
#define KOOPAS_STATE_ATTACKUP	500
#define KOOPAS_STATE_HOLDUP		600
#define KOOPAS_VX	0.03f
#define KOOPAS_ATTACK_VX	0.1f
#define AY	0.001f
#define KOOPAS_WIDTH	18
#define KOOPAS_HEIGHT	26
#define RANGE_STAND_UP	10
class Koopas:public CGameObject
{
	DWORD begin;
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e, DWORD dt);
public:
	Koopasitem* item;
	Koopas(float x,float y) :CGameObject(x,y) {
		type = OBJECT_TYPE_KOOPAS;
		SetState(KOOPAS_STATE_WALKING);
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

