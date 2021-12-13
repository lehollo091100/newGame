#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_A: {
		if (mario->isAttacking == false)
		{
			mario->isAttacking = true;
			break;
		}
		else
		{
			break;
		}
	}
	case DIK_S:
	{
		if (mario->level != MARIO_LEVEL_TAIL)
		{
			if (mario->isOnPlatform)
			{
				mario->SetState(MARIO_STATE_JUMP);
			}
		}
		else
		{
			if(mario->isOnPlatform && (mario->state != MARIO_STATE_RUNMAXRIGHT && mario->state != MARIO_STATE_RUNMAXLEFT))
			{
				mario->SetState(MARIO_STATE_JUMP);
			}
			if(mario->isOnPlatform && (mario->state == MARIO_STATE_RUNMAXRIGHT || mario->state == MARIO_STATE_RUNMAXLEFT))
			{
				mario->SetState(MARIO_STATE_FLYING);
			}
			if (mario->isOnPlatform==false && mario->state == MARIO_STATE_RELEASE_JUMP)
			{
				mario->SetState(MARIO_STATE_FALLINGSLOW);
			}
			if (!mario->isOnPlatform && mario->state == MARIO_STATE_RELEASEFLY)
			{
				mario->SetState(MARIO_STATE_FLYING);
			}
		}
		break;
	}
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->level != MARIO_LEVEL_TAIL)
		{
			if (mario->state != MARIO_STATE_RELEASE_JUMP)
			{
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
		}
		else
		{
			if (mario->state != MARIO_STATE_FLYING && mario->isOnPlatform == false)
			{
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
			else if (mario->state == MARIO_STATE_FLYING && mario->isOnPlatform == false)
			{
				mario->SetState(MARIO_STATE_RELEASEFLY);
			}
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A:
	{
		if (mario->koo) {
			mario->koo->isHold = false;
			mario->koo->SetState(KOOPAS_STATE_ATTACKUP);
			mario->SetState(MARIO_STATE_KICK);
			mario->isHolding = false;
			mario->holdtime = 0;
			mario->koo = NULL;
		}
		break;
	}
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	/// <summary>
	/// neu ko co if isonplatform thi doi chieu dc. ko co thi bi loi render
	/// </summary>
	/// <param name="states"></param>
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (!mario->isOnPlatform) {
				mario->maxVx = MARIO_WALKING_SPEED;
				mario->ax = MARIO_ACCEL_WALK_X;
				mario->nx = 1;
			}
			else
			{
				if (game->IsKeyDown(DIK_A))
				{
					if (mario->state != MARIO_STATE_RUNMAXRIGHT) {
						mario->SetState(MARIO_STATE_RUNNING_RIGHT);
					}
					else
					{
						mario->SetState(MARIO_STATE_RUNMAXRIGHT);
					}
				}
				else/* if(mario->state!=MARIO_STATE_JUMP)*/
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (!mario->isOnPlatform) {
				mario->maxVx = -MARIO_WALKING_SPEED;
				mario->ax = -MARIO_ACCEL_WALK_X;
				mario->nx = -1;
			}
			else
			{
				if (game->IsKeyDown(DIK_A)) {
					if (mario->state != MARIO_STATE_RUNMAXLEFT) {

						mario->SetState(MARIO_STATE_RUNNING_LEFT);
					}
					else
					{
						mario->SetState(MARIO_STATE_RUNMAXLEFT);
					}
				}

				else
					mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
		}
		else {
			if (mario->isOnPlatform)
			{
				mario->SetState(MARIO_STATE_IDLE);
			}
			else
			{
				mario->ax = 0;
			}
		}
	
}