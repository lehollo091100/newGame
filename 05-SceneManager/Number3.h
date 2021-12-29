#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define ID_ANI_NUMBER3	1121
class Number3 :public CGameObject
{
public:
	Number3(float x, float y) : CGameObject(x, y) {
		type = OBJECT_TYPE_INTROOPTION;
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual bool IsItem() {
		return true;
	}
};


