#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define ID_ANI_CURTAIN	1011
#define CURTAIN_STATE_NORMAL	0
#define CURTAIN_STATE_MOVING	100
class Curtain:public CGameObject
{
public:
	Curtain(float x, float y) : CGameObject(x, y) {
		type = OBJECT_TYPE_CURTAIN;

	}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

