#pragma once
#include "GameObject.h"
#include "debug.h"
#include "AssetIDs.h"
#define ID_ANI_1	990
#define ID_ANI_2	991
#define ID_ANI_3	992
#define ID_ANI_4	993
#define ID_ANI_5	994
class Pipe:public CGameObject
{
	float width;
	float height;

public:
	int scene,direction,ani;
	float nextx, nexty;
	Pipe(float x, float y, float bwidth, float bheight,int s,int d,int a,float x1,float y1) : CGameObject(x, y)
	{
		ani = a;
		nextx = x1;
		nexty = y1;
		direction = d;
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

