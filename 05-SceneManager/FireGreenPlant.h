#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "AssetIDs.h"
#include "PlantFire.h"
#include "Mario.h"
#define FIREGREENPLANT_TIME	2000
#define FIREGREENPLANT_VY	0.03f
#define FIREGREENPLANT_WIDTH	18
#define FIREGREENPLANT_HEIGHT	26
#define FIREGREENPLANT_RANGE	(FIREGREENPLANT_HEIGHT+4)
#define FIREGREENPLANT_STATE_START	0
#define FIREGREENPLANT_STATE_MOVINGUP	100
#define FIREGREENPLANT_STATE_STOP	200
#define FIREGREENPLANT_STATE_MOVINGDOWN	300
#define ID_ANI_FIREGREENPLANT_OPENDOWNLEFT	966
#define ID_ANI_FIREGREENPLANT_OPENUPLEFT	967
#define ID_ANI_FIREGREENPLANT_MOVINGLEFTUP	968
#define ID_ANI_FIREGREENPLANT_MOVINGLEFTDOWN	969
#define ID_ANI_FIREGREENPLANT_OPENDOWNRIGHT	970
#define ID_ANI_FIREGREENPLANT_OPENUPRIGHT	971
#define ID_ANI_FIREGREENPLANT_MOVINGRIGHTUP	972
#define ID_ANI_FIREGREENPLANT_MOVINGRIGHTDOWN	973
#define FIRE_VYFAR	0.014f
#define FIRE_VYNEAR	0.018f
#define FIRE_VX	0.03f
#define FIRE_GREENPLANT_RANGE	50


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
		SetState(FIREGREENPLANT_STATE_START);
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



