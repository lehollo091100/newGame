#include "Tail.h"
#include "Goomba.h"
#include "Koopas.h"
#include "QuestionBrick.h"
#include "Mario.h"
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WIDTH / 2;
	top = y - HEIGHT / 2;
	right = left + WIDTH;
	bottom = top + HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (state == TAIL_STATE_ATTACK) {
		if (nx >= 0)
		{
			if (x>initX+30)
			{
				SetState
			}
		}
	}*/
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Tail::Render()
{
	RenderBoundingBox();
}

void Tail::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TAIL_STATE_NOTHING: {
		break;
	}
	case TAIL_STATE_ATTACK: {
		initX = x;
		if (nx>=0)
		{
			vx = 0.1f;
		}
		else
		{
			vx = -0.1f;
		}
		break;
	}
	default:
		break;
	}
}

void Tail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Tail::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (state == TAIL_STATE_ATTACK)
	{
		if (dynamic_cast<Mushroom*>(e->obj)) return;
		if (dynamic_cast<QuestionBrick*>(e->obj))
		{
			QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);
			if (QBrick->GetState() == QBSTATE_NORMAL)
			{

				QBrick->nx = -this->nx;
				QBrick->SetState(QBSTATE_MOVING);
				if (QBrick->item->type == OBJECT_TYPE_COINITEM) {
					CMario::GetInstance()->coin++;
				}
			}
		}
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			goomba->SetState(GOOMBA_STATE_DIEUP);
		};
		if (dynamic_cast<Koopas*>(e->obj)) {
			Koopas* koopas = dynamic_cast<Koopas*>(e->obj);

			if (nx >= 0)
			{
				koopas->nx = 1;
			}
			else
			{
				koopas->nx = -1;
			}
			if (koopas->state==KOOPAS_STATE_DEFENDDOWN)
			{
				koopas->vx = koopas->nx * KOOPAS_VX;
				koopas->vy= -KOOPAS_DEFENDUP_BOUNDING_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDUP);
			}
			else
			{
				koopas->SetState(KOOPAS_STATE_DEFENDUP);
				koopas->vx = koopas->nx * KOOPAS_VX;
				koopas->vy = -KOOPAS_DEFENDUP_BOUNDING_SPEED;
			}
			//DebugOut(L"koopas right:%f THIS.Y:%f\n", koopas->y+KOOPAS_WIDTH/2,this->y);
		};
	}
}
