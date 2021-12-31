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
#include "FireRedPlant.h"
#include "FireGreenPlant.h"
#include "ShinningBrick.h"
#include "Debris.h"
#include "PBrick.h"
#include "Leaf.h"
#include "Pipe.h"
#include "EndGameItem.h"
#include "GreenMario.h"
#define INTROSCENE	3
#define EIGHT	8
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//DebugOut(L"mario state:%d\n",state);
	//vector<LPGAMEOBJECT>* itemObjects;
	/*if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT)
	{
		if (state == MARIO_STATE_RUNNING_RIGHT)
		{
			ax += 0.000005f;
		}
		else
		{
			ax -= 0.000005f;
		}
	}*/
	if (vx != 0&&CGame::GetInstance()->current_scene == INTROSCENE)
	{
		if (x >= CGame::GetInstance()->GetBackBufferWidth())
		{
			vx = 0;
			SetState(MARIO_STATE_IDLE);
		}
	}
	if (state == MARIO_STATE_IDLE)
	{
		//stop when ax=0 
		if (isOnPlatform)
		{
			if (vx > 0)
			{
				ax = 0;
				vx -= MARIO_ACCEL_DECREASE_SPEED;
			}
			else if (vx < 0)
			{
				vx += MARIO_ACCEL_DECREASE_SPEED;
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
	}
	if (isPiping == false)
	{
		vy += ay * dt;
	}
	vx += ax * dt;
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
			if (koo->state != KOOPAS_STATE_DIE)
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
	if (isAttacking && level==MARIO_LEVEL_TAIL)
	{
		if (timeToAttack == 0)
		{
			timeToAttack = DWORD(GetTickCount64());
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
		else {
			if (GetTickCount64() - timeToAttack > MARIO_ATTACK_TAIL_TIME)
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

	if (x < EIGHT)
	{
		x += EIGHT;
	}
	if (y<0)
	{
		y = 0;
	}
	if (true)
	{

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
	if (isPiping) {
		if (GetTickCount64() - Pipetime >= MARIO_PIPING_TIME) {
			ay = MARIO_GRAVITY;
			isPiping = false;
			SetState(MARIO_STATE_IDLE);
			CGame::GetInstance()->InitiateSwitchScene(scene);
		}
		else
		{
			//isOnPlatform = true;
			y += vy * dt;
		}
	}
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
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
			if (t1== true) {
				switch (coObjects->at(i)->type)
				{
				case OBJECT_TYPE_MUSHROOM: {
						Mushroom* mushroom = dynamic_cast<Mushroom*>(coObjects->at(i));
						if (mushroom->state == MUSHROOM_STATE_MOVING)
						{
							if (level == MARIO_LEVEL_SMALL)
							{
								SetLevel(MARIO_LEVEL_BIG);
							}
							else if (level == MARIO_LEVEL_BIG)
							{
								SetLevel(MARIO_LEVEL_TAIL);
							}
							else {
								SetLevel(MARIO_LEVEL_TAIL);
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
							SetLevel(MARIO_LEVEL_TAIL);
							leaf->Delete();
						}
					}
					break;
				}
				case OBJECT_TYPE_PLANTFIRE: {
					if (coObjects->at(i) != NULL)
					{
						PlantFire* plantfire = dynamic_cast<PlantFire*>(coObjects->at(i));
						if (plantfire->state == FIREPLANT_MOVING)
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

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
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
	else if (dynamic_cast<GreenPlant*>(e->obj)||dynamic_cast<FireGreenPlant*>(e->obj)|| dynamic_cast<FireRedPlant*>(e->obj))
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
	else if (dynamic_cast<Leaf*>(e->obj)){
		OnCollisionWithLeaf(e);
	}
	else if (dynamic_cast<Pipe*>(e->obj)) {
		OnCollisionWithPipe(e);
	}
	else if (dynamic_cast<GreenMario*>(e->obj)) {
		if (e->ny != 0)
		{
			vy = -MARIO_JUMP_RUN_SPEED_Y;
			vx = MARIO_WALKING_SPEED;
			//firstJumpIntro = true;
			GreenMario* green = dynamic_cast<GreenMario*>(e->obj);
			//green->SetState(GREENMARIO_STATE_SIT);
			green->SetState(GREENMARIO_STATE_IDLE);
			green->isSitting = true;
			green->vx = 0;
			//green->y += GREENMARIO_SIT_HEIGHT_ADJUST;
		}
	}
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
			else if (koopas->GetState() == KOOPAS_STATE_DEFENDUP)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				if (level == MARIO_LEVEL_SMALL)
				{
					if ((x + MARIO_SMALL_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = -1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
					if ((x + MARIO_SMALL_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
				}
				if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_TAIL)
				{
					if ((x + MARIO_BIG_BBOX_WIDTH / 2) > (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = -1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
					if ((x + MARIO_BIG_BBOX_WIDTH / 2) <= (koopas->x + KOOPAS_WIDTH / 2))
					{
						koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_ATTACKUP);
					}
				}
			}
			else
			if (koopas->GetState()==KOOPAS_STATE_ATTACKDOWN)
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDDOWN);
			}
			else
				if (koopas->GetState() == KOOPAS_STATE_ATTACKUP)
				{
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					koopas->SetState(KOOPAS_STATE_DEFENDUP);
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
		if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState()==KOOPAS_STATE_FLYING || koopas->GetState()==KOOPAS_STATE_ATTACKDOWN||koopas->GetState()==KOOPAS_STATE_ATTACKUP)
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
				kicktime = DWORD(GetTickCount64());
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
					holdtime = DWORD(GetTickCount64());
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
					kicktime = DWORD(GetTickCount64());
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
					holdtime = DWORD(GetTickCount64());
				}
			}
		}
		
	}
}

void CMario::OnCollisionWithRedgoomba(LPCOLLISIONEVENT e)
{
	Redgoomba* rgoomba = dynamic_cast<Redgoomba*>(e->obj);
	if (rgoomba->state == REDGOOMBA_STATE_DIE || rgoomba->state == REDGOOMBA_STATE_DIEUP)
		return;
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

void CMario::OnCollisionWithShinningBrick(LPCOLLISIONEVENT e)
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

void CMario::OnCollisionWithPBrick(LPCOLLISIONEVENT e) {
	PBrick* pbrick = dynamic_cast<PBrick*>(e->obj);
	if (e->ny > 0)
	{
		if (pbrick->state != PBRICK_STATE_NOTHING)
		{
			pbrick->SetState(PBRICK_STATE_MOVING);
		}
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
	if (leaf->state == LEAF_STATE_MOVING)
	{
		SetLevel(MARIO_LEVEL_TAIL);
		leaf->Delete();
	}

}

void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	Pipe* p = dynamic_cast<Pipe*>(e->obj);
	scene = p->scene;

	if (p->scene != -1) {
		NextX = p->nextx;
		NextY = p->nexty;
		if (state==MARIO_STATE_JUMP)
		{
			//ay = 0;
			if (p->direction == -1)
			{
				isPiping = true;
				Pipetime = DWORD(GetTickCount64());
				vy = -MARIO_PIPING_SPEED;	
				SetState(MARIO_STATE_PIPING);
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
				Pipetime = DWORD(GetTickCount64());
				vy = MARIO_PIPING_SPEED;
				SetState(MARIO_STATE_PIPING);
			}
			
		}
		
	}
	else return;
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
		if (isPiping)
		{
			aniId = ID_ANI_MARIO_SMALL_PIPING;
		}
		else
		{
			if (abs(vx) == MARIO_RUNNING_SPEED)
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
	}
	else

		if (isPiping)
		{
			aniId = ID_ANI_MARIO_SMALL_PIPING;
		}
		else if(isHolding)
		{
			if (nx >= 0)
			{
				if (vx == 0) {
					aniId = ID_ANI_MARIO_SMALL_HOLDRIGHT_IDLE;
				}
				else
				{
					aniId = ID_ANI_MARIO_SMALL_HOLDRIGHT_WALKING;
				}
			}
			else
			{
				if (vx == 0)
				{
					aniId = ID_ANI_MARIO_SMALL_HOLDLEFT_IDLE;
				}
				else
				{
					aniId = ID_ANI_MARIO_SMALL_HOLDLEFT_WALKING;
				}
			}
		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
			}
			else
			{
				aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
			}
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
				else
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
				else 
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
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isPiping)
		{
			aniId = ID_ANI_MARIO_PIPING;
		}
		else
		{
			//if(state==)
			if (vx == MARIO_RUNNING_SPEED)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;

			}
			else
			{

				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
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
		else if (isHolding)
		{
			if (nx>0)
			{
				if (vx == 0)
				{
					aniId = ID_ANI_MARIO_HOLD_RIGHT_IDLE;
				}
				else
				{
					aniId = ID_ANI_MARIO_HOLD_RIGHT_WALKING;
				}
			}
			else
			{
				if (vx == 0)
				{
					aniId = ID_ANI_MARIO_HOLD_LEFT_IDLE;
				}
				else
				{
					aniId = ID_ANI_MARIO_HOLD_LEFT_WALKING;
				}
			}
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
		else if (isPiping) {
			aniId = ID_ANI_MARIO_PIPING;
		}
		else
			if (vx == 0)
			{
				if (nx >= 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
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
				else
				{
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx==maxVx)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && vx > maxVx)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
				else 
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
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_SIT_LEFT;
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
		else if (isPiping) {
			aniId = ID_ANI_MARIO_TAIL_PIPING;
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
				aniId = ID_ANI_MARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_TAIL_KICK_RIGHT;
			}
			else
			 {
				aniId = ID_ANI_MARIO_TAIL_KICK_LEFT;
			}
		}
		else if (isHolding)
		{
			if (nx >= 0)
			{
				if (vx == 0) {
					aniId=ID_ANI_MARIO_TAIL_HOLDIDLE_RIGHT;
				}
				else
				{
					aniId = ID_ANI_MARIO_TAIL_HOLDING_RIGHT;
				}
			}
			else
			{
				if (vx == 0)
				{
					aniId = ID_ANI_MARIO_TAIL_HOLDIDLE_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_TAIL_HOLDING_LEFT;
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
			else if (vx >= 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx >= maxVx)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && vx < maxVx)
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
				else aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
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
				else {
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;
	//DebugOut(L"aniid:%d %d %d\n", aniId,state, isOnPlatform);
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
	//if (level == MARIO_LEVEL_TAIL)
	//{
	//	if (nx >= 0)
	//	{
	//		animations->Get(aniId)->Render(x - MARIO_TAIL_BBOX_WIDTH / 2, y);
	//	}
	//	else
	//	{
	//		animations->Get(aniId)->Render(x+ MARIO_TAIL_BBOX_WIDTH / 2, y);
	//	}
	//}
	//else
	//{
	//}
		animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	//if (this->state == MARIO_STATE_DIE) return;

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
		//if (isSitting) break;
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
		//ax = 0.0f;
		//vx = 0.0f;
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
			timeToFly = DWORD(GetTickCount64());
		}
		vy -= MARIO_FLYUP_SPEED_Y;
		break;
	}
	case MARIO_STATE_RELEASEFLY: {

		if (vy < 0) vy += FLY_GRAVITY_PLUS;
		break;
	}
	case MARIO_STATE_KICK: {
		kicktime = DWORD(GetTickCount64());
		isKicking = true;
		break;
	}
	case MARIO_STATE_PIPING: {
		break;
	}
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
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
	else if (level == MARIO_LEVEL_TAIL)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			if (nx >= 0)
			{
				left = x - MARIO_TAIL_BBOX_WIDTH / 2;
				top = y - MARIO_TAIL_BBOX_HEIGHT / 2;
				right = left + MARIO_TAIL_BBOX_WIDTH;
				bottom = top + MARIO_TAIL_BBOX_HEIGHT;
			}
			else
			{

				left = x - MARIO_TAIL_BBOX_WIDTH / 2;
				top = y - MARIO_TAIL_BBOX_HEIGHT / 2;
				right = left + MARIO_TAIL_BBOX_WIDTH;
				bottom = top + MARIO_TAIL_BBOX_HEIGHT;
			}
		}
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

void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(x, 100);
	vx = 0; ax = 0;
}