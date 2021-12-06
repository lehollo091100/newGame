#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "debug.h"
#define ID_ANI_COIN 700

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject 
{
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		type = OBJECT_TYPE_COIN;

	}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	virtual bool IsItem() {
		return true;
	}
};