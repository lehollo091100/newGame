#include "EndGameItem.h"
#include "HUD.h"
void EndGameItem::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void EndGameItem::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}

void EndGameItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EGITEM_STATE_NORMAL: {
		timechange =DWORD( GetTickCount64());
		IsStar = true;
		IsMushroom = false;
		IsPlant = false;
		vx = 0;
		vy = 0;
		break;
	}
	case EGITEM_STATE_MOVINGUP: {
		vy = -EGITEM_MOVINGUP_SPEED;
		break;
	}
	default:
		break;
	}
}

void EndGameItem::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state==EGITEM_STATE_NORMAL)
	{
		if (IsStar)
		{
			animations->Get(ID_ANI_EGSTAR)->Render(x, y);
		}
		if (IsPlant)
		{
			animations->Get(ID_ANI_EGPLANT)->Render(x, y);
		}
		if (IsMushroom)
		{
			animations->Get(ID_ANI_EGMUSHROOM)->Render(x, y);
		}
	}
	if (state == EGITEM_STATE_MOVINGUP)
	{
		if (IsStar)
		{
			animations->Get(ID_ANI_EGSTAR_MOVING)->Render(x, y);
		}
		if (IsPlant)
		{
			animations->Get(ID_ANI_EGPLANT_MOVING)->Render(x, y);
		}
		if (IsMushroom)
		{
			animations->Get(ID_ANI_EGMUSHROOM_MOVING)->Render(x, y);
		}
	}
	if (state == EGITEM_STATE_WORD)
	{
		if (IsStar)
		{
			animations->Get(ID_ANI_EGSTAR_SUCCESS)->Render(x, y);
		}
		if (IsPlant)
		{
			animations->Get(ID_ANI_EGPLANT_SUCCESS)->Render(x, y);
		}
		if (IsMushroom)
		{
			animations->Get(ID_ANI_EGMUSHROOM_SUCCESS)->Render(x, y);
		}
	}
}

void EndGameItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"ismushroom:%d %d %d\n",IsStar, IsMushroom,IsPlant);
	if (state == EGITEM_STATE_MOVINGUP) {
		if ( initY-y >= EGITEM_MOVINGUP_RANGE)
		{
			vy = 0;
			float a = CGame::GetInstance()->GetCamX();
			float b= CGame::GetInstance()->GetCamY();
			float w = float(CGame::GetInstance()->GetBackBufferWidth());
			float h = float(CGame::GetInstance()->GetBackBufferHeight());
			SetPosition(a + w / 2, b + h / 2-HUD_HEIGHT);
			SetState(EGITEM_STATE_WORD);
		}
	}
	if (state == EGITEM_STATE_WORD)
	{
		CMario::GetInstance()->SetState(MARIO_STATE_IDLE);
		CGame::GetInstance()->allowKey = false;
	}
	if (state == EGITEM_STATE_NORMAL)
	{
		if (GetTickCount64() - timechange >= TIME_CHANGE)
		{
			if (IsStar==true)
			{
				IsStar = false;
				IsMushroom = true;

			}
			else if (IsMushroom==true)
			{
				IsMushroom = false;
				IsPlant = true;

			}
			else if (IsPlant==true)
			{
				IsPlant = false;
				IsStar = true;

			}
			timechange = DWORD(GetTickCount64());
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void EndGameItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - EGITEM_WIDTH / 2;
	t = y - EGITEM_WIDTH / 2;
	r = l + EGITEM_WIDTH;
	b = t + EGITEM_WIDTH;
}
