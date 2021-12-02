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
/*		ID_ANI_RED	*/
#define ID_ANI_WALKINGLEFT_RED	861
#define ID_ANI_WALKINGRIGHT_RED	862
#define ID_ANI_DEFENDDOWN_RED	863
#define ID_ANI_DOWNATTACK_RED	864
#define ID_ANI_DEFENDUP_RED		871
#define ID_ANI_ATTACKUP_RED		872
/*		ID_ANI_GREEN	*/	
#define ID_ANI_WALKINGLEFT_GREEN	867
#define ID_ANI_WALKINGRIGHT_GREEN	868
#define ID_ANI_DEFENDDOWN_GREEN	869
#define ID_ANI_DOWNATTACK_GREEN	870

#define ID_ANI_DEFENDUP_GREEN	873
#define ID_ANI_ATTACKUP_GREEN	874

#define ID_ANI_FLYINGLEFT	865
#define ID_ANI_FLYINGRIGHT	866


#define KOOPAS_STATE_WALKING	0
#define KOOPAS_STATE_DEFENDDOWN	100
#define KOOPAS_STATE_ATTACKDOWN	200
#define KOOPAS_STATE_HOLDDOWN	300
#define KOOPAS_STATE_DEFENDUP	400
#define KOOPAS_STATE_ATTACKUP	500
#define KOOPAS_STATE_HOLDUP		600
#define	KOOPAS_STATE_FLYING		700
#define KOOPAS_VX	0.03f
#define KOOPAS_ATTACK_VX	0.1f
#define AY	0.001f
#define KOOPAS_WIDTH	18
#define KOOPAS_HEIGHT	26
#define RANGE_STAND_UP	10
#define TIME_STANDUP	5000
#define ITEM_VY	0.016f
#define KOOPAS_FLYING_SPEED	0.25f
#define KOOPAS_DEFENDUP_BOUNDING_SPEED	0.3f
class Koopas:public CGameObject
{
	DWORD begin;
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e, DWORD dt);
	int Color;
public:
	Koopasitem* item;
	Koopas(float x,float y,int t,int color) :CGameObject(x,y) {
		type = OBJECT_TYPE_KOOPAS;
		SetState(t);
		nx = 1;
		Color = color;
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

