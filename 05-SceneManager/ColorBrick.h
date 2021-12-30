#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
//#define ID_ANI_BRICK 10000
class ColorBrick : public CGameObject
{
public:
	//int type = OBJECT_TYPE_COLORBRICK;
	float width;
	float height;
public:
	ColorBrick(float x, float y, float bwidth, float bheight) : CGameObject(x, y)
	{
		type = OBJECT_TYPE_COLORBRICK;
		width = bwidth;
		height = bheight;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual bool IsStaticObj() {
		return true;
	}
};

