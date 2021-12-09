#pragma once
#include "GameObject.h"
#include "Game.h"
#define HUD_HEIGHT	38
#define ID_ANI_HUD	961
class HUD:public CGameObject
{
	static HUD* __instance;
public:
	int Point;
	int Stack;
	int Coin;
	int time;
	bool IsInScene1;
	DWORD count;
	string p;
	//string p = (string)(Point);
	HUD() {
		time = 300;
		count = 0;
	};
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		return;
	};
	static HUD* GetInstance();
	void SetPoint(int p) {
		this->Point = p;
	};
	void SetStack(int s)
	{
		this->Stack = s;
	};
	void SetCoin(int c)
	{
		this->Coin = c;
	}

};

