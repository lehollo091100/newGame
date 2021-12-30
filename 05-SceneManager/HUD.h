#pragma once
#include "GameObject.h"
#include "Game.h"
#include "debug.h"
#include "Mario.h"
#include "StackSpeed.h"
#define RANGE1	0.1f
#define RANGE2	0.115f
#define RANGE3	0.13f
#define RANGE4	0.15f
#define RANGE5	0.17f
#define RANGE6	0.19f
#define HUD_HEIGHT	38
#define ID_ANI_HUD	961
class HUD:public CGameObject
{
	static HUD* __instance;
	CMario* mario = CMario::GetInstance();
public:
	//int Point;
	//int Coin;
	//int time;
	int Stack;
	//bool IsInScene1;
	//DWORD count;
	string p;
	StackSpeed* stack = new StackSpeed();
	//string p = (string)(Point);
	HUD() {
		//time = 300;
		//count = 0;
		Stack = 0;
	};
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		return;
	};
	static HUD* GetInstance();
	//void SetPoint(int p) {
	//	this->Point = p;
	//};
	void SetStack(int s)
	{
		this->Stack = s;
	};
	//void SetCoin(int c)
	//{
	//	this->Coin = c;
	//}

};

