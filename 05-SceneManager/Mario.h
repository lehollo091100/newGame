#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Tail.h"
#include "debug.h"
#include "Koopas.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0003f
#define MARIO_ACCEL_RUN_X	0.0001f

#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_JUMP_RUN_SPEED_Y	0.4f

#define MARIO_GRAVITY			0.001f

#define MARIO_JUMP_DEFLECT_SPEED  0.3f
#define MARIO_KICK_TIME	300

#define MARIO_FALLSLOW_SPEED_Y	0.18f
#define MARIO_FALLSLOW_PLUS_VY	0.1f
#define FLY_GRAVITY_PLUS	0.1f
#define MARIO_FLYUP_SPEED_Y		0.3f

#define MARIO_ATTACK_TAIL_TIME	300
#define MARIO_FLY_TIME	5000
#define MARIO_HOLD_TIME	5000

#define MARIO_PIPING_TIME	1500
#define MARIO_PIPING_SPEED	0.01f

#define MARIO_TAIL_X	15
#define MARIO_TAIL_Y	5

#define MARIO_HOLD_TURTLE_RANGE	20
//STATE

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500


#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_RUNMAXRIGHT		603
#define MARIO_STATE_RUNMAXLEFT		604

#define MARIO_STATE_FALLINGSLOW		605
#define MARIO_STATE_FLYING			606
#define MARIO_STATE_RELEASEFLY		607

#define MARIO_STATE_KICK			608

#define MARIO_STATE_PIPING			609


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 402
#define ID_ANI_MARIO_WALKING_LEFT 403

#define ID_ANI_MARIO_RUNNING_RIGHT 404
#define ID_ANI_MARIO_RUNNING_LEFT 405

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 406
#define ID_ANI_MARIO_JUMP_WALK_LEFT 407

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 408
#define ID_ANI_MARIO_JUMP_RUN_LEFT 409

#define ID_ANI_MARIO_SIT_RIGHT 410
#define ID_ANI_MARIO_SIT_LEFT 411

#define ID_ANI_MARIO_BRACE_RIGHT 413
#define ID_ANI_MARIO_BRACE_LEFT 412

#define ID_ANI_MARIO_KICK_RIGHT	414
#define ID_ANI_MARIO_KICK_LEFT	415

#define ID_ANI_MARIO_PIPING		416

#define ID_ANI_MARIO_DIE 499

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 500
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 501

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 502
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 503

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 504
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 505

#define ID_ANI_MARIO_SMALL_BRACE_LEFT 506
#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 507

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 508
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 509

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 510
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 511

#define ID_ANI_MARIO_SMALL_PIPING	512

// TAIL
#define ID_ANI_MARIO_TAIL_IDLE_LEFT		550
#define ID_ANI_MARIO_TAIL_IDLE_RIGHT	551

#define ID_ANI_MARIO_TAIL_WALKING_LEFT	552
#define ID_ANI_MARIO_TAIL_WALKING_RIGHT	553

#define ID_ANI_MARIO_TAIL_RUNNING_LEFT	554
#define ID_ANI_MARIO_TAIL_RUNNING_RIGHT	555

#define ID_ANI_MARIO_TAIL_JUMP_LEFT		556
#define ID_ANI_MARIO_TAIL_JUMP_RIGHT	557

#define ID_ANI_MARIO_TAIL_FALLING_LEFT	558
#define ID_ANI_MARIO_TAIL_FALLING_RIGHT	559

#define ID_ANI_MARIO_TAIL_FALLINGSLOW_LEFT	560
#define ID_ANI_MARIO_TAIL_FALLINGSLOW_RIGHT	561

#define ID_ANI_MARIO_TAIL_BRACE_LEFT	562
#define ID_ANI_MARIO_TAIL_BRACE_RIGHT	563

#define ID_ANI_MARIO_TAIL_FLYUP_LEFT		564
#define ID_ANI_MARIO_TAIL_FLYUP_RIGHT		565

#define ID_ANI_MARIO_TAIL_FLYDOWN_LEFT		566
#define ID_ANI_MARIO_TAIL_FLYDOWN_RIGHT		567

#define ID_ANI_MARIO_TAIL_ATTACK_LEFT	568
#define ID_ANI_MARIO_TAIL_ATTACK_RIGHT	569

#define ID_ANI_MARIO_TAIL_HOLDING_LEFT	570
#define ID_ANI_MARIO_TAIL_HOLDING_RIGHT	571

#define ID_ANI_MARIO_TAIL_HOLDIDLE_LEFT		572
#define ID_ANI_MARIO_TAIL_HOLDIDLE_RIGHT	573

#define ID_ANI_MARIO_TAIL_PIPING			574
#pragma endregion



#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 16


#define MARIO_TAIL_BBOX_WIDTH	16
#define MARIO_TAIL_BBOX_HEIGHT	26

#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	static CMario* __instance;
	bool pressP;
	DWORD kicktime;
	BOOLEAN isSitting;
	DWORD Pipetime;
	int untouchable; 
	ULONGLONG untouchable_start;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e,DWORD dt);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithRedgoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithGreenPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithShinningBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTail();

public:
	float NextX, NextY;
	int scene;
	bool isPiping;
	Koopas* koo=NULL;
	int coin; 
	bool isAttacking, isHolding,isFlying, isKicking;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	int level; 
	BOOLEAN isOnPlatform;
	Tail* tail;
	DWORD timeToFly, timeToAttack, holdtime;
	CMario(float x, float y) : CGameObject(x, y)
	{
		pressP = false;
		timeToAttack = 0;
		isFlying = false;
		timeToFly = 0;
		kicktime = 0;
		type = 0;
		isSitting = false;
		isHolding = false;
		isKicking = false;
		isPiping = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		NextX=NextY=0;
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e,DWORD dt);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	static CMario* GetInstance();
};