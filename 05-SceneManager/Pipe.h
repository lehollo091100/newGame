#pragma once
#include "GameObject.h"
#include "debug.h"
#include "AssetIDs.h"
class Pipe:public CGameObject
{
	float width;
	float height;

public:
	int scene;
	Pipe(float x, float y, float bwidth, float bheight,int s) : CGameObject(x, y)
	{
		scene = s;
		type = OBJECT_TYPE_PIPE;
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

