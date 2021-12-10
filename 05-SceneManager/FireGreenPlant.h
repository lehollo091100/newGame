#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#include "PlantFire.h"
#include "Mario.h"
#define TIME	2000
#define VY	0.03f
#define WIDTH	18
#define HEIGHT	26
#define RANGE	(HEIGHT+4)
#define FIREREDPLANT_STATE_START	0
#define FIREREDPLANT_STATE_MOVINGUP	100
#define FIREREDPLANT_STATE_STOP	200
#define FIREREDPLANT_STATE_MOVINGDOWN	300
#define ID_ANI_OPENDOWNLEFT	966
#define ID_ANI_OPENUPLEFT	967
#define ID_ANI_MOVINGLEFTUP	968
#define ID_ANI_MOVINGLEFTDOWN	969
#define ID_ANI_OPENDOWNRIGHT	970
#define ID_ANI_OPENUPRIGHT	971
#define ID_ANI_MOVINGRIGHTUP	972
#define ID_ANI_MOVINGRIGHTDOWN	973
#define FIRE_VYFAR	0.014f
#define FIRE_VYNEAR	0.018f
#define FIRE_VX	0.03f
#define FIRERANGE	50


class FireGreenPlant :public CGameObject
{
	float initY;
	DWORD time;
public:
	CMario* mario = CMario::GetInstance();
	PlantFire* item;
	FireGreenPlant(float x, float y) :CGameObject(x, y) {

		time = 0;
		initY = y;
		SetState(FIREREDPLANT_STATE_START);
		type = OBJECT_TYPE_FIREGREENPLANT;
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



