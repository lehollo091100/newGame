#include "GreenMario.h"
#include <algorithm>
#include "debug.h"
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
#include "FireRedPlant.h"
#include "FireGreenPlant.h"
#include "ShinningBrick.h"
#include "Debris.h"
#include "PBrick.h"
#include "Leaf.h"
#include "Pipe.h"
#include "EndGameItem.h"
void GreenMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"ax, vx:%d\n",isPiping);
	//vector<LPGAMEOBJECT>* itemObjects;
	/*if (state == GREENMARIO_STATE_RUNNING_LEFT || state == GREENMARIO_STATE_RUNNING_RIGHT)
	{
		if (state == GREENMARIO_STATE_RUNNING_RIGHT)
		{
			ax += 0.000005f;
		}
		else
		{
			ax -= 0.000005f;
		}
	}*/
	if (state == GREENMARIO_STATE_IDLE)
	{
		//stop when ax=0 
		if (vx > 0)
		{
			ax = 0;
			vx -= GREENMARIO_ACCEL_DECREASE_SPEED;
		}
		else if (vx < 0)
		{
			vx += GREENMARIO_ACCEL_DECREASE_SPEED;
			ax = 0;
		}
		if (nx >= 0)
		{
			if (vx <= 0)
			{
				vx = 0;
			}
		}
		else
		{
			if (vx >= 0)
			{
				vx = 0;
			}
		}
	}
	if (isPiping == false)
	{
		vy += ay * dt;
	}
	vx += ax * dt;
	if (isHolding)
	{
		if (GetTickCount64() - holdtime >= GREENMARIO_HOLD_TIME)
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
			if (koo->state != KOOPAS_STATE_DIE)
			{
				koo->nx = nx;
				koo->isHold = true;
				//koo->ay = 0;
				if (nx >= 0) {
					koo->SetPosition(x + GREENMARIO_HOLD_TURTLE_RANGE, y);
				}
				else
				{
					koo->SetPosition(x - GREENMARIO_HOLD_TURTLE_RANGE, y);
				}
			}
			else
			{
				koo->isHold = false;
				koo->nx = nx;
				isHolding = false;
				holdtime = 0;
				koo = NULL;
			}
		}
	}
	if (isAttacking && level == GREENMARIO_LEVEL_TAIL)
	{
		if (timeToAttack == 0)
		{
			timeToAttack = GetTickCount64();
			if (nx > 0 || vx > 0) {
				tail->nx = 1;
				//DebugOut(L"level: %d\n",nx);
				//tail->SetPosition(x + GREENMARIO_TAIL_X, y+ GREENMARIO_TAIL_Y);
			}
			else
			{
				tail->nx = -1;

				//tail->SetPosition(x - GREENMARIO_TAIL_X, y+ GREENMARIO_TAIL_Y);
			}
			tail->SetState(TAIL_STATE_ATTACK);
		}
		else {
			if (GetTickCount64() - timeToAttack > GREENMARIO_ATTACK_TAIL_TIME)
			{
				timeToAttack = 0;
				isAttacking = false;
			}
		}
	}
	else
	{
		tail->SetState(TAIL_STATE_NOTHING);
		if (nx >= 0) {
			tail->SetPosition(x - GREENMARIO_TAIL_X, y + GREENMARIO_TAIL_Y);
		}
		else
		{
			tail->SetPosition(x + GREENMARIO_TAIL_X, y + GREENMARIO_TAIL_Y);
		}
	}

	if (state == GREENMARIO_STATE_JUMP && vy >= 0)
	{
		SetState(GREENMARIO_STATE_RELEASE_JUMP);
	}

	if (x < 8)
	{
		x += 8;
	}

	if (abs(vx) > abs(maxVx))
	{
		vx = maxVx;
		if (nx >= 0)
		{
			if (isOnPlatform && state == GREENMARIO_STATE_RUNNING_RIGHT)
			{
				SetState(GREENMARIO_STATE_RUNMAXRIGHT);
			}
		}
		else
		{
			if (isOnPlatform && state == GREENMARIO_STATE_RUNNING_LEFT)
			{
				SetState(GREENMARIO_STATE_RUNMAXLEFT);
			}
		}
		/*if (state == GREENMARIO_STATE_RUNNING_LEFT)
		{
		}*/
	}

	if (isKicking)
	{
		if (GetTickCount64() - kicktime > GREENMARIO_KICK_TIME)
		{
			isKicking = false;
			kicktime = 0;
			if (nx >= 0)
			{

				SetState(GREENMARIO_STATE_WALKING_RIGHT);
			}
			else
			{
				SetState(GREENMARIO_STATE_WALKING_LEFT);
			}
		}
	}
	if (isFlying)
	{
		if (GetTickCount64() - timeToFly > GREENMARIO_FLY_TIME) {
			timeToFly = 0;
			isFlying = false;
			SetState(GREENMARIO_STATE_RELEASE_JUMP);
		}
	}
	if (isPiping) {
		if (GetTickCount64() - Pipetime >= GREENMARIO_PIPING_TIME) {
			ay = GREENMARIO_GRAVITY;
			isPiping = false;
			SetState(GREENMARIO_STATE_IDLE);
			CGame::GetInstance()->InitiateSwitchScene(scene);
		}
		else
		{
			//isOnPlatform = true;
			y += vy * dt;
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > GREENMARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;
	//handle press P brick change to coin
	if (pressP)
	{
		pressP = false;
		for (UINT i = 0; i < coObjects->size(); i++) {
			if (coObjects->at(i)->type == OBJECT_TYPE_SHINNINGBRICK)
			{
				ShinningBrick* sbrick = dynamic_cast<ShinningBrick*>(coObjects->at(i));
				sbrick->SetState(SBRICK_STATE_COIN);
			}
		}
	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->IsItem() == true)
		{

			bool t1 = CCollision::GetInstance()->isCollisionWithObj(this, coObjects->at(i));
			if (t1 == true) {
				switch (coObjects->at(i)->type)
				{
				case OBJECT_TYPE_MUSHROOM: {
					Mushroom* mushroom = dynamic_cast<Mushroom*>(coObjects->at(i));
					if (mushroom->state == MUSHROOM_STATE_MOVING)
					{
						if (level == GREENMARIO_LEVEL_SMALL)
						{
							SetLevel(GREENMARIO_LEVEL_BIG);
						}
						else if (level == GREENMARIO_LEVEL_BIG)
						{
							SetLevel(GREENMARIO_LEVEL_TAIL);
						}
						else {
							SetLevel(GREENMARIO_LEVEL_TAIL);
						}
						mushroom->Delete();
					}
					break;
				}
				case OBJECT_TYPE_LEAF: {
					if (coObjects->at(i) != NULL)
					{
						Leaf* leaf = dynamic_cast<Leaf*>(coObjects->at(i));
						if (leaf->state == LEAF_STATE_MOVING)
						{
							SetLevel(GREENMARIO_LEVEL_TAIL);
							leaf->Delete();
						}
					}
					break;
				}
				case OBJECT_TYPE_PLANTFIRE: {
					if (untouchable == 0)
					{
						if (level > GREENMARIO_LEVEL_SMALL)
						{
							level = GREENMARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
						{
							DebugOut(L">>> Mario DIE >>> \n");
							SetState(GREENMARIO_STATE_DIE);
						}
					}
					break;
				}
				case OBJECT_TYPE_ENDGAMEITEM: {
					if (coObjects->at(i) != NULL)
					{
						EndGameItem* egitem = dynamic_cast<EndGameItem*>(coObjects->at(i));
						if (egitem->state == EGITEM_STATE_NORMAL) {
							egitem->SetState(EGITEM_STATE_MOVINGUP);
						}
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void GreenMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void GreenMario::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
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
	else if (dynamic_cast<GreenPlant*>(e->obj) || dynamic_cast<FireGreenPlant*>(e->obj) || dynamic_cast<FireRedPlant*>(e->obj))
		OnCollisionWithGreenPlant(e);
	else if (dynamic_cast<ShinningBrick*>(e->obj)) {
		OnCollisionWithShinningBrick(e);
	}
	else if (dynamic_cast<PBrick*>(e->obj)) {
		OnCollisionWithPBrick(e);
	}
	else if (dynamic_cast<PButton*>(e->obj))
	{
		PButton* button = dynamic_cast<PButton*>(e->obj);
		if (e->ny < 0)
		{
			if (button->state != PBUTTON_STATE_NOTHING)
			{
				pressP = true;
				button->SetState(PBUTTON_STATE_NOTHING);
			}
		}
	}
	else if (dynamic_cast<Leaf*>(e->obj)) {
		OnCollisionWithLeaf(e);
	}
	else if (dynamic_cast<Pipe*>(e->obj)) {
		OnCollisionWithPipe(e);
	}
}

void GreenMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIEUP)
			{
				if (level > GREENMARIO_LEVEL_SMALL)
				{
					level = GREENMARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(GREENMARIO_STATE_DIE);
				}
			}
		}
	}
}
void GreenMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) {
	QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);
	if (e->ny > 0) {
		if (x > (QBrick->x - QBRICK_WIDTH / 2) && x < (QBrick->x + QBRICK_WIDTH / 2))
		{
			if (QBrick->GetState() == QBSTATE_NORMAL)
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

void GreenMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
	if (e->ny != 0)
	{
		if (e->ny < 0)
		{
			if (koopas->GetState() == KOOPAS_STATE_WALKING) {
				vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDDOWN);
			}
			else
				if (koopas->GetState() == KOOPAS_STATE_DEFENDDOWN)
				{
					vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
					if (level == GREENMARIO_LEVEL_SMALL)
					{
						if ((x + GREENMARIO_SMALL_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = -1;
							koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
						}
						if ((x + GREENMARIO_SMALL_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = 1;
							koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
						}
					}
					if (level == GREENMARIO_LEVEL_BIG || level == GREENMARIO_LEVEL_TAIL)
					{
						if ((x + GREENMARIO_BIG_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = -1;
							koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
						}
						if ((x + GREENMARIO_BIG_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = 1;
							koopas->SetState(KOOPAS_STATE_ATTACKDOWN);
						}
					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_DEFENDUP)
				{
					vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
					if (level == GREENMARIO_LEVEL_SMALL)
					{
						if ((x + GREENMARIO_SMALL_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = -1;
							koopas->SetState(KOOPAS_STATE_ATTACKUP);
						}
						if ((x + GREENMARIO_SMALL_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = 1;
							koopas->SetState(KOOPAS_STATE_ATTACKUP);
						}
					}
					if (level == GREENMARIO_LEVEL_BIG || level == GREENMARIO_LEVEL_TAIL)
					{
						if ((x + GREENMARIO_BIG_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = -1;
							koopas->SetState(KOOPAS_STATE_ATTACKUP);
						}
						if ((x + GREENMARIO_BIG_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
						{
							koopas->nx = 1;
							koopas->SetState(KOOPAS_STATE_ATTACKUP);
						}
					}
				}
				else
					if (koopas->GetState() == KOOPAS_STATE_ATTACKDOWN)
					{
						vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
						koopas->SetState(KOOPAS_STATE_DEFENDDOWN);
					}
					else
						if (koopas->GetState() == KOOPAS_STATE_ATTACKUP)
						{
							vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
							koopas->SetState(KOOPAS_STATE_DEFENDUP);
						}
						else if (koopas->GetState() == KOOPAS_STATE_FLYING)
						{
							vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
							koopas->SetState(KOOPAS_STATE_WALKING);
						}
		}
		else
		{
			if (untouchable == 0)
			{
				if (koopas->GetState() == KOOPAS_STATE_WALKING)
				{
					if (level > GREENMARIO_LEVEL_SMALL)
					{
						level = GREENMARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(GREENMARIO_STATE_DIE);
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
		if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_FLYING)
		{
			if (untouchable == 0)
			{
				if (level > GREENMARIO_LEVEL_SMALL)
				{
					level = GREENMARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(GREENMARIO_STATE_DIE);
				}
			}
		}
		else if (koopas->GetState() == KOOPAS_STATE_DEFENDDOWN)
		{
			if (state == GREENMARIO_STATE_WALKING_LEFT || state == GREENMARIO_STATE_WALKING_RIGHT)
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
			else if (state == GREENMARIO_STATE_RUNNING_LEFT || state == GREENMARIO_STATE_RUNNING_RIGHT || state == GREENMARIO_STATE_RUNMAXLEFT || state == GREENMARIO_STATE_RUNMAXRIGHT)
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
			if (state == GREENMARIO_STATE_WALKING_LEFT || state == GREENMARIO_STATE_WALKING_RIGHT)
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
			else if (state == GREENMARIO_STATE_RUNNING_LEFT || state == GREENMARIO_STATE_RUNNING_RIGHT || state == GREENMARIO_STATE_RUNMAXLEFT || state == GREENMARIO_STATE_RUNMAXRIGHT)
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

void GreenMario::OnCollisionWithRedgoomba(LPCOLLISIONEVENT e)
{
	Redgoomba* rgoomba = dynamic_cast<Redgoomba*>(e->obj);
	if (rgoomba->state == REDGOOMBA_STATE_DIE || rgoomba->state == REDGOOMBA_STATE_DIEUP)
		return;
	if (e->ny != 0) {
		if (e->ny < 0)
		{
			if (rgoomba->GetState() != REDGOOMBA_STATE_NORMAL && rgoomba->GetState() != REDGOOMBA_STATE_DIE)
			{
				rgoomba->SetState(REDGOOMBA_STATE_NORMAL);
				vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
			}
			else if (rgoomba->GetState() == REDGOOMBA_STATE_NORMAL)
			{
				rgoomba->SetState(REDGOOMBA_STATE_DIE);
				vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (e->ny > 0) {
			if (level > GREENMARIO_LEVEL_SMALL)
			{
				level = GREENMARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(GREENMARIO_STATE_DIE);
			}
		}
	}
	if (e->nx != 0)
	{
		if (rgoomba->state != REDGOOMBA_STATE_DIE) {
			if (level > GREENMARIO_LEVEL_SMALL)
			{
				level = GREENMARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(GREENMARIO_STATE_DIE);
			}
		}
	}
}

void GreenMario::OnCollisionWithGreenPlant(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level > GREENMARIO_LEVEL_SMALL)
		{
			level = GREENMARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(GREENMARIO_STATE_DIE);
		}
	}
}

void GreenMario::OnCollisionWithShinningBrick(LPCOLLISIONEVENT e)
{
	ShinningBrick* sbrick = dynamic_cast<ShinningBrick*>(e->obj);
	if (sbrick->state == SBRICK_STATE_COIN) {
		//sbrick->Delete();
		//coin++;
	}
	else
	{
		if (e->nx != 0)
			return;
		if (e->ny != 0) {
			if (e->ny > 0)
			{
				if (x > sbrick->x - SBRICK_WIDTH / 2 && x < sbrick->x + SBRICK_WIDTH / 2)
				{

					vy = 0;
					sbrick->d1->SetState(DEBRIS_STATE_MOVING);
					sbrick->d2->SetState(DEBRIS_STATE_MOVING);
					sbrick->d3->SetState(DEBRIS_STATE_MOVING);
					sbrick->d4->SetState(DEBRIS_STATE_MOVING);
					sbrick->Delete();
				}
			}
		}
	}
}

void GreenMario::OnCollisionWithPBrick(LPCOLLISIONEVENT e) {
	PBrick* pbrick = dynamic_cast<PBrick*>(e->obj);
	if (e->ny > 0)
	{
		if (pbrick->state != PBRICK_STATE_NOTHING)
		{
			pbrick->SetState(PBRICK_STATE_MOVING);
		}
	}
}

void GreenMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
	if (leaf->state == LEAF_STATE_MOVING)
	{
		SetLevel(GREENMARIO_LEVEL_TAIL);
		leaf->Delete();
	}

}

void GreenMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	Pipe* p = dynamic_cast<Pipe*>(e->obj);
	scene = p->scene;

	if (p->scene != -1) {
		NextX = p->nextx;
		NextY = p->nexty;
		if (state == GREENMARIO_STATE_JUMP)
		{
			//ay = 0;
			if (p->direction == -1)
			{
				isPiping = true;
				Pipetime = GetTickCount64();
				vy = -GREENMARIO_PIPING_SPEED;
				SetState(GREENMARIO_STATE_PIPING);
			}
			//CGame::GetInstance()->InitiateSwitchScene(p->scene);
		}
		if (isSitting)//tu tren xuong
		{
			//ay = 0;
			if (p->direction == 1)
			{
				isPiping = true;
				isSitting = false;
				Pipetime = GetTickCount64();
				vy = GREENMARIO_PIPING_SPEED;
				SetState(GREENMARIO_STATE_PIPING);
			}

		}

	}
	else return;
}

void GreenMario::OnCollisionWithCoin(LPCOLLISIONEVENT e, DWORD dt)
{
	e->obj->Delete();
	coin++;
}

void GreenMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}
void GreenMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e) {
	Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
	if (mushroom->state == MUSHROOM_STATE_MOVING)
	{
		SetLevel(GREENMARIO_LEVEL_BIG);
		mushroom->Delete();
	}
}
//
// Get animation ID for small Mario
//
int GreenMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isPiping)
		{
			aniId = ID_ANI_GREENMARIO_SMALL_PIPING;
		}
		else
		{
			if (abs(vx) == GREENMARIO_RUNNING_SPEED)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_SMALL_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_GREENMARIO_SMALL_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_SMALL_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_GREENMARIO_SMALL_JUMP_WALK_LEFT;
			}
		}
	}
	else

		if (isPiping)
		{
			aniId = ID_ANI_GREENMARIO_SMALL_PIPING;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_GREENMARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_GREENMARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_GREENMARIO_SMALL_BRACE_RIGHT;
				else if (ax == GREENMARIO_ACCEL_RUN_X)
					aniId = ID_ANI_GREENMARIO_SMALL_RUNNING_RIGHT;
				else if (ax == GREENMARIO_ACCEL_WALK_X)
					aniId = ID_ANI_GREENMARIO_SMALL_WALKING_RIGHT;
				else
					aniId = ID_ANI_GREENMARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_GREENMARIO_SMALL_BRACE_LEFT;
				else if (ax == -GREENMARIO_ACCEL_RUN_X)
					aniId = ID_ANI_GREENMARIO_SMALL_RUNNING_LEFT;
				else if (ax == -GREENMARIO_ACCEL_WALK_X)
					aniId = ID_ANI_GREENMARIO_SMALL_WALKING_LEFT;
				else
					aniId = ID_ANI_GREENMARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_GREENMARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int GreenMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_GREENMARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_GREENMARIO_SIT_LEFT;
		}
		else if (isPiping)
		{
			aniId = ID_ANI_GREENMARIO_PIPING;
		}
		else
		{
			//if(state==)
			if (vx == GREENMARIO_RUNNING_SPEED)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_GREENMARIO_JUMP_RUN_LEFT;

			}
			else
			{

				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_GREENMARIO_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_GREENMARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_GREENMARIO_SIT_LEFT;
		}
		else if (isHolding)
		{

		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_GREENMARIO_KICK_RIGHT;
			}
			if (nx < 0) {
				aniId = ID_ANI_GREENMARIO_KICK_LEFT;
			}
		}
		else if (isPiping) {
			aniId = ID_ANI_GREENMARIO_PIPING;
		}
		else
			if (vx == 0)
			{
				if (nx >= 0) aniId = ID_ANI_GREENMARIO_IDLE_RIGHT;
				else aniId = ID_ANI_GREENMARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_GREENMARIO_BRACE_RIGHT;
				else if (ax == GREENMARIO_ACCEL_RUN_X && vx >= maxVx)
					aniId = ID_ANI_GREENMARIO_RUNNING_RIGHT;
				else if (ax == GREENMARIO_ACCEL_RUN_X && vx < maxVx)
					aniId = ID_ANI_GREENMARIO_WALKING_RIGHT;
				else if (ax == GREENMARIO_ACCEL_WALK_X)
					aniId = ID_ANI_GREENMARIO_WALKING_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_WALKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_GREENMARIO_BRACE_LEFT;
				else if (ax == -GREENMARIO_ACCEL_RUN_X && vx == maxVx)
					aniId = ID_ANI_GREENMARIO_RUNNING_LEFT;
				else if (ax == -GREENMARIO_ACCEL_RUN_X && vx > maxVx)
					aniId = ID_ANI_GREENMARIO_WALKING_LEFT;
				else
					aniId = ID_ANI_GREENMARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_GREENMARIO_IDLE_RIGHT;

	return aniId;
}

int GreenMario::GetAniIdTail()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_GREENMARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_GREENMARIO_TAIL_SIT_LEFT;
		}
		else if (isAttacking)
		{
			if (nx >= 0)
			{
				aniId = ID_ANI_GREENMARIO_TAIL_ATTACK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_GREENMARIO_TAIL_ATTACK_LEFT;
			}
		}
		else if (isPiping) {
			aniId = ID_ANI_GREENMARIO_TAIL_PIPING;
		}

		else
		{

			if (state == GREENMARIO_STATE_JUMP)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_JUMP_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_JUMP_LEFT;
				}
			}
			else if (state == GREENMARIO_STATE_RELEASE_JUMP)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_FALLING_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_FALLING_LEFT;
				}
			}
			else if (state == GREENMARIO_STATE_FALLINGSLOW)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_FALLINGSLOW_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_FALLINGSLOW_LEFT;
				}
			}
			else if (state == GREENMARIO_STATE_FLYING)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_FLYUP_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_FLYUP_LEFT;
				}
			}
			else if (state == GREENMARIO_STATE_RELEASEFLY)
			{
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_FLYDOWN_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_FLYDOWN_LEFT;
				}
			}
			else {
				if (nx >= 0)
					aniId = ID_ANI_GREENMARIO_TAIL_FALLING_RIGHT;
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_FALLING_LEFT;
				}
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_GREENMARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_GREENMARIO_TAIL_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_GREENMARIO_TAIL_KICK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_GREENMARIO_TAIL_KICK_LEFT;
			}
		}
		else if (isHolding)
		{
			if (nx >= 0)
			{
				if (vx > 0) {
					aniId = ID_ANI_GREENMARIO_TAIL_HOLDING_RIGHT;
				}
				else
				{
					ID_ANI_GREENMARIO_TAIL_HOLDIDLE_RIGHT;
				}
			}
			else
			{
				if (vx < 0)
				{
					aniId = ID_ANI_GREENMARIO_TAIL_HOLDING_LEFT;
				}
				else
				{
					aniId = ID_ANI_GREENMARIO_TAIL_HOLDIDLE_LEFT;
				}
			}
		}
		else if (isAttacking)
		{
			if (nx >= 0)
			{
				aniId = ID_ANI_GREENMARIO_TAIL_ATTACK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_GREENMARIO_TAIL_ATTACK_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_GREENMARIO_TAIL_IDLE_RIGHT;
				else aniId = ID_ANI_GREENMARIO_TAIL_IDLE_LEFT;
			}
			else if (vx >= 0)
			{
				if (ax < 0)
					aniId = ID_ANI_GREENMARIO_TAIL_BRACE_RIGHT;
				else if (ax == GREENMARIO_ACCEL_RUN_X && vx >= maxVx)
					aniId = ID_ANI_GREENMARIO_TAIL_RUNNING_RIGHT;
				else if (ax == GREENMARIO_ACCEL_RUN_X && vx < maxVx)
					aniId = ID_ANI_GREENMARIO_TAIL_WALKING_RIGHT;
				else if (ax == GREENMARIO_ACCEL_WALK_X)
					aniId = ID_ANI_GREENMARIO_TAIL_WALKING_RIGHT;
				else aniId = ID_ANI_GREENMARIO_TAIL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_GREENMARIO_TAIL_BRACE_LEFT;
				else if (ax == -GREENMARIO_ACCEL_RUN_X && vx == maxVx)
					aniId = ID_ANI_GREENMARIO_TAIL_RUNNING_LEFT;
				else if (ax == -GREENMARIO_ACCEL_RUN_X && vx > maxVx)
					aniId = ID_ANI_GREENMARIO_TAIL_WALKING_LEFT;
				else if (ax == -GREENMARIO_ACCEL_WALK_X)
					aniId = ID_ANI_GREENMARIO_TAIL_WALKING_LEFT;
				else {
					aniId = ID_ANI_GREENMARIO_TAIL_WALKING_LEFT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_GREENMARIO_TAIL_IDLE_RIGHT;
	//DebugOut(L"aniid:%d %d %d\n", aniId,state, isOnPlatform);
	return aniId;
}

void GreenMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == GREENMARIO_STATE_DIE)
		aniId = ID_ANI_GREENMARIO_DIE;
	else if (level == GREENMARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == GREENMARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == GREENMARIO_LEVEL_TAIL) {
		aniId = GetAniIdTail();
	}
	//DebugOut(L"aniid:%d\n", aniId);
	//if (level == GREENMARIO_LEVEL_TAIL)
	//{
	//	if (nx >= 0)
	//	{
	//		animations->Get(aniId)->Render(x - GREENMARIO_TAIL_BBOX_WIDTH / 2, y);
	//	}
	//	else
	//	{
	//		animations->Get(aniId)->Render(x+ GREENMARIO_TAIL_BBOX_WIDTH / 2, y);
	//	}
	//}
	//else
	//{
	//}
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();

}

void GreenMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	//if (this->state == GREENMARIO_STATE_DIE) return;

	switch (state)
	{
	case GREENMARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = GREENMARIO_RUNNING_SPEED;
		ax = GREENMARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case GREENMARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -GREENMARIO_RUNNING_SPEED;
		ax = -GREENMARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case GREENMARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = GREENMARIO_WALKING_SPEED;
		ax = GREENMARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case GREENMARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -GREENMARIO_WALKING_SPEED;
		ax = -GREENMARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case GREENMARIO_STATE_JUMP:
		//if (isSitting) break;
		if (isOnPlatform)
		{
			isOnPlatform = false;
			if (abs(this->vx) == GREENMARIO_RUNNING_SPEED)
				vy = -GREENMARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -GREENMARIO_JUMP_SPEED_Y;
		}
		break;

	case GREENMARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += GREENMARIO_FALLSLOW_PLUS_VY;
		break;

	case GREENMARIO_STATE_SIT:
		if (isOnPlatform && level != GREENMARIO_LEVEL_SMALL)
		{
			state = GREENMARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += GREENMARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case GREENMARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = GREENMARIO_STATE_IDLE;
			y -= GREENMARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case GREENMARIO_STATE_IDLE:
		//ax = 0.0f;
		//vx = 0.0f;
		break;

	case GREENMARIO_STATE_DIE:
		vy = -GREENMARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case GREENMARIO_STATE_RUNMAXRIGHT: {
		if (isSitting) break;
		maxVx = GREENMARIO_RUNNING_SPEED;
		ax = GREENMARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	}
	case GREENMARIO_STATE_RUNMAXLEFT: {
		if (isSitting) break;
		maxVx = -GREENMARIO_RUNNING_SPEED;
		ax = -GREENMARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	}

	case GREENMARIO_STATE_FALLINGSLOW: {
		vy -= GREENMARIO_FALLSLOW_SPEED_Y;
		break;
	}
	case GREENMARIO_STATE_FLYING: {
		isFlying = true;
		if (timeToFly == 0)
		{
			timeToFly = GetTickCount64();
		}
		vy -= GREENMARIO_FLYUP_SPEED_Y;
		break;
	}
	case GREENMARIO_STATE_RELEASEFLY: {

		if (vy < 0) vy += FLY_GRAVITY_PLUS;
		break;
	}
	case GREENMARIO_STATE_KICK: {
		kicktime = GetTickCount64();
		isKicking = true;
		break;
	}
	case GREENMARIO_STATE_PIPING: {
		break;
	}
	}

	CGameObject::SetState(state);
}

void GreenMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == GREENMARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - GREENMARIO_BIG_BBOX_WIDTH / 2;
			top = y - GREENMARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + GREENMARIO_BIG_BBOX_WIDTH;
			bottom = top + GREENMARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - GREENMARIO_BIG_BBOX_WIDTH / 2;
			top = y - GREENMARIO_BIG_BBOX_HEIGHT / 2;
			right = left + GREENMARIO_BIG_BBOX_WIDTH;
			bottom = top + GREENMARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == GREENMARIO_LEVEL_SMALL)
	{
		left = x - GREENMARIO_SMALL_BBOX_WIDTH / 2;
		top = y - GREENMARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + GREENMARIO_SMALL_BBOX_WIDTH;
		bottom = top + GREENMARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == GREENMARIO_LEVEL_TAIL)
	{
		if (isSitting)
		{
			left = x - GREENMARIO_BIG_BBOX_WIDTH / 2;
			top = y - GREENMARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + GREENMARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + GREENMARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			if (nx >= 0)
			{
				left = x - GREENMARIO_TAIL_BBOX_WIDTH / 2;
				top = y - GREENMARIO_TAIL_BBOX_HEIGHT / 2;
				right = left + GREENMARIO_TAIL_BBOX_WIDTH;
				bottom = top + GREENMARIO_TAIL_BBOX_HEIGHT;
			}
			else
			{

				left = x - GREENMARIO_TAIL_BBOX_WIDTH / 2;
				top = y - GREENMARIO_TAIL_BBOX_HEIGHT / 2;
				right = left + GREENMARIO_TAIL_BBOX_WIDTH;
				bottom = top + GREENMARIO_TAIL_BBOX_HEIGHT;
			}
		}
	}
}

GreenMario* GreenMario::__instance = NULL;
GreenMario* GreenMario::GetInstance()
{
	if (__instance == NULL) __instance = new GreenMario(0, 0);
	return __instance;
}

void GreenMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == GREENMARIO_LEVEL_SMALL)
	{
		y -= (GREENMARIO_BIG_BBOX_HEIGHT - GREENMARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void GreenMario::Reset()
{
	SetState(GREENMARIO_STATE_IDLE);
	SetLevel(GREENMARIO_LEVEL_BIG);
	SetPosition(x, 100);
	vx = 0; ax = 0;
}