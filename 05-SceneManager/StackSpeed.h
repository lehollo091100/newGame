#pragma once
#include "GameObject.h"
#define ID_ANI_ARROW	962
class StackSpeed:public CGameObject
{
public:
	StackSpeed() {
		type = OBJECT_TYPE_STACKSPEED;
	};
	void Render1(float x, float y, int s);
	virtual void Render() {
		return;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) { return; };
	~StackSpeed() {};
};

