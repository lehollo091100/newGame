#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "ColorBrick.h"
#include "Collision.h"
#include "QuestionBrick.h"
#include "Mushroom.h"
#include "Koopas.h"
#include "Redgoomba.h"
#include "GreenPlant.h"
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isHolding)
	{
		if (GetTickCount64() - holdtime >= MARIO_HOLD_TIME)
		{
			koo->isHold = false;
			koo->nx = nx;
			koo->vx = koo->nx * KOOPAS_VX;
			koo->vy = -KOOPAS_DEFENDUP_BOUNDING_SPEED;
			koo->SetState(KOOPAS_STATE_WALKING);
			isHolding = false;
			holdtime = 0;
			koo = NULL;
		}
		else
		{
			koo->nx = nx;
			koo->isHold = true;
			//koo->ay = 0;
			if (nx >= 0) {
				koo->SetPosition(x + MARIO_HOLD_TURTLE_RANGE, y);
			}
			else
			{
				koo->SetPosition(x - MARIO_HOLD_TURTLE_RANGE, y);
			}

		}
	}
	vy += ay * dt;
	vx += ax * dt;
	if (isAttacking && level==MARIO_LEVEL_TAIL)
	{
		if (timeToAttack == 0)
		{
			timeToAttack = GetTickCount64();
			if (nx > 0 || vx>0) {
				tail->nx = 1;
				//DebugOut(L"level: %d\n",nx);
				//tail->SetPosition(x + MARIO_TAIL_X, y+ MARIO_TAIL_Y);
			}
			else
			{
				tail->nx = -1;

				//tail->SetPosition(x - MARIO_TAIL_X, y+ MARIO_TAIL_Y);
			}
			tail->SetState(TAIL_STATE_ATTACK);
		}
		if (GetTickCount64() - timeToAttack > MARIO_ATTACK_TAIL_TIME)
		{
			timeToAttack = 0;
			isAttacking = false;
		}
	}
	else
	{
		tail->SetState(TAIL_STATE_NOTHING);
		if (nx >= 0) {
			tail->SetPosition(x - MARIO_TAIL_X, y+ MARIO_TAIL_Y);
		}
		else
		{
			tail->SetPosition(x + MARIO_TAIL_X, y+ MARIO_TAIL_Y);
		}
	}
	if (state == MARIO_STATE_JUMP && vy >= 0)
	{
		SetState(MARIO_STATE_RELEASE_JUMP);
	}
	if (x < 8)
	{
		x += 8;
	}
	if (abs(vx) > abs(maxVx))
	{
		vx = maxVx;
		if (nx>=0)
		{
			if (isOnPlatform && state==MARIO_STATE_RUNNING_RIGHT)
			{
				SetState(MARIO_STATE_RUNMAXRIGHT);
			}
		}
		else
		{
			if (isOnPlatform && state == MARIO_STATE_RUNNING_LEFT)
			{
				SetState(MARIO_STATE_RUNMAXLEFT);
			}
		}
		/*if (state == MARIO_STATE_RUNNING_LEFT)
		{
		}*/
	}

	if (isKicking)
	{
		if (GetTickCount64() - kicktime > MARIO_KICK_TIME)
			{
				isKicking = false;
				kicktime = 0;
				if (nx>=0)
				{

					SetState(MARIO_STATE_WALKING_RIGHT);
				}
				else
				{
					SetState(MARIO_STATE_WALKING_LEFT);
				}
			}
	}
	if (isFlying)
	{
		if (GetTickCount64() - timeToFly > MARIO_FLY_TIME) {
			timeToFly = 0;
			isFlying = false;
			SetState(MARIO_STATE_RELEASE_JUMP);
		}
	}
	

	//if (vx>0)
	//{
	//	tail->SetPosition(x - WIDTH, y);
	//}
	//else if(vx<0)
	//{
	//	tail->SetPosition(x + WIDTH, y);
	//}
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e,DWORD dt)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (e->ny < 0)
		{
			vy = 0;
			isOnPlatform = true;
		}
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		//vx = 0;
	}
	/*switch (dynamic_cast)
	{
	default:
		break;
	}*/
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<QuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e, dt);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<Mushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<Redgoomba*>(e->obj))
		OnCollisionWithRedgoomba(e);
	else if (dynamic_cast<GreenPlant*>(e->obj))
		OnCollisionWithGreenPlant(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState()!=GOOMBA_STATE_DIEUP)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) {
	QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);
	if (e->ny > 0) {
		if (x> (QBrick->x - QBRICK_WIDTH/2) && x < (QBrick->x + QBRICK_WIDTH / 2))
		{
			if (QBrick->GetState()==QBSTATE_NORMAL)
			{
				QBrick->nx = -this->nx;
				QBrick->SetState(QBSTATE_MOVING);
				if (QBrick->item->type == OBJECT_TYPE_COINITEM) {
					coin++;
				}
			}

		}
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
	if (e->ny!=0)
	{
		if (e->ny < 0)
		{
			if (koopas->GetState() == KOOPAS_STATE_WALKING) {
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDDOWN);
			}
			else
			if (koopas->GetState()==KOOPAS_STATE_DEFENDDOWN)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				if (level==MARIO_LEVEL_SMALL)
				{
					if ((x + MARIO_SMALL_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = -1;
						koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
					}
					if ((x + MARIO_SMALL_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
					}
				}
				if (level==MARIO_LEVEL_BIG||level==MARIO_LEVEL_TAIL)
				{
					if ((x + MARIO_BIG_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = -1;
						koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
					}
					if ((x + MARIO_BIG_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
					}
				}
			}
			else
			if (koopas->GetState()==KOOPAS_STATE_ATTACKDOWN)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDDOWN);
			}
			else if (koopas->GetState()==KOOPAS_STATE_FLYING)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopas->SetState(KOOPAS_STATE_WALKING);
			}
		}
		else
		{
			if (untouchable == 0)
			{
				if (koopas->GetState() == KOOPAS_STATE_WALKING)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}
				else
				{
					//vy += 0.005f;
				}
			}
		}
	}
	if (e->nx != 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState()==KOOPAS_STATE_FLYING)
		{
			if (untouchable == 0)
			{
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
			}
		}
		else if(koopas->GetState()==KOOPAS_STATE_DEFENDDOWN)
		{
			if (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_WALKING_RIGHT)
			{
				/// kick code here
				kicktime = GetTickCount64();
				isKicking = true;
				if (vx > 0)
				{
					koopas->nx = 1;
					koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
				}
				else if (vx < 0) {
					koopas->nx = -1;
					koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
				}
			}
			else if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNMAXLEFT || state == MARIO_STATE_RUNMAXRIGHT  )
			{
				if (!isHolding)
				{

					if (koo == NULL)
					{
						koo = dynamic_cast<Koopas*>(e->obj);
					}
					isHolding = true;
					holdtime = GetTickCount64();
				}
			}
		}
		else if (koopas->GetState() == KOOPAS_STATE_DEFENDUP)
		{
			if (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_WALKING_RIGHT)
			{
				/// kick code here
				if (isHolding == false)
				{
					kicktime = GetTickCount64();
					isKicking = true;
					if (vx > 0)
					{
						koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
					else if (vx < 0) {
						koopas->nx = -1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
				}
			}
			else if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNMAXLEFT || state == MARIO_STATE_RUNMAXRIGHT)
			{
				if (!isHolding)
				{
					if (koo == NULL)
					{
						koo = dynamic_cast<Koopas*>(e->obj);
					}
					koopas->SetState(KOOPAS_STATE_DEFENDUP);
					isHolding = true;
					holdtime = GetTickCount64();
				}
			}
		}
		
	}
}

void CMario::OnCollisionWithRedgoomba(LPCOLLISIONEVENT e)
{
	Redgoomba* rgoomba = dynamic_cast<Redgoomba*>(e->obj);
	if (e->ny != 0) {
		if (e->ny<0)
		{
			if (rgoomba->GetState() != REDGOOMBA_STATE_NORMAL && rgoomba->GetState() != REDGOOMBA_STATE_DIE)
			{
				rgoomba->SetState(REDGOOMBA_STATE_NORMAL);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if(rgoomba->GetState() == REDGOOMBA_STATE_NORMAL)
			{
				rgoomba->SetState(REDGOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (e->ny > 0) {
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
	if (e->nx != 0)
	{
		if (rgoomba->state != REDGOOMBA_STATE_DIE) {
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithGreenPlant(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e,DWORD dt)
{
		e->obj->Delete();
		coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e) {
	Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
	if (mushroom->state == MUSHROOM_STATE_MOVING)
	{
		SetLevel(MARIO_LEVEL_BIG);
		mushroom->Delete();
	}
}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isHolding)
		{

		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_KICK_RIGHT;
			}
			if (nx < 0) {
				aniId = ID_ANI_MARIO_KICK_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx>=maxVx)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx < maxVx)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X )
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx==maxVx)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx > maxVx)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdTail()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isAttacking)
		{
			if (nx >= 0)
			{
				aniId = ID_ANI_MARIO_TAIL_ATTACK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_MARIO_TAIL_ATTACK_LEFT;
			}
		}
		else
		{

			if (state == MARIO_STATE_JUMP )
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_JUMP_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_JUMP_LEFT;
				}
			}
			else if (state==MARIO_STATE_RELEASE_JUMP)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_FALLING_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_FALLING_LEFT;
				}
			}
			else if (state==MARIO_STATE_FALLINGSLOW)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_FALLINGSLOW_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_FALLINGSLOW_LEFT;
				}
			}
			else if (state==MARIO_STATE_FLYING)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_FLYUP_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_FLYUP_LEFT;
				}
			}
			else if (state==MARIO_STATE_RELEASEFLY)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_FLYDOWN_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_FLYDOWN_LEFT;
				}
			}
			else {
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TAIL_FALLING_RIGHT;
				else
				{
					aniId = ID_ANI_MARIO_TAIL_FALLING_LEFT;
				}
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_KICK_RIGHT;
			}
			else
			 {
				aniId = ID_ANI_MARIO_KICK_LEFT;
			}
		}
		else if (isHolding)
		{
			if (nx >= 0)
			{
				if (vx > 0) {
					aniId = ID_ANI_MARIO_TAIL_HOLDING_RIGHT;
				}
				else
				{
					ID_ANI_MARIO_TAIL_HOLDIDLE_RIGHT;
				}
			}
			else
			{
				if (vx < 0)
				{
					aniId = ID_ANI_MARIO_TAIL_HOLDING_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_TAIL_HOLDIDLE_LEFT;
				}
			}
		}
		else if (isAttacking)
		{
			if (nx >= 0)
			{
				aniId = ID_ANI_MARIO_TAIL_ATTACK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_MARIO_TAIL_ATTACK_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_TAIL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx >= maxVx)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx < maxVx)
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx == maxVx)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx > maxVx)
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_TAIL) {
		aniId = GetAniIdTail();
	}
	//DebugOut(L"aniid:%d\n", aniId);
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			isOnPlatform = false;
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_FALLSLOW_PLUS_VY;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_RUNMAXRIGHT: {
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	}
	case MARIO_STATE_RUNMAXLEFT: {
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	}
	
	case MARIO_STATE_FALLINGSLOW: {
		vy-= MARIO_FALLSLOW_SPEED_Y;
		break;
	}
	case MARIO_STATE_FLYING: {
		isFlying = true;
		if (timeToFly == 0)
		{
			timeToFly = GetTickCount64();
		}
		vy -= MARIO_FLYUP_SPEED_Y;
		break;
	}
	case MARIO_STATE_RELEASEFLY: {

		if (vy < 0) vy += FLY_GRAVITY_PLUS;
		break;
	}
	case MARIO_STATE_KICK: {
		kicktime = GetTickCount64();
		isKicking = true;
		break;
	}
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG|| level==MARIO_LEVEL_TAIL)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

CMario* CMario::__instance = NULL;
CMario* CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario(0, 0);
	return __instance;
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

