#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	IsAllowRender = IsAllowUpdate = false;
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
	type = -1;
}

bool CGameObject::CheckSkipCollision(LPGAMEOBJECT coObject, int nx, int ny)
{
	if (coObject->type == OBJECT_TYPE_COLORBRICK)
	{
		if (nx != 0)
		{
			return true;
		}
		if (ny < 0)
		{
			return true;
		}
	}
	return false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	//CGame::GetInstance()->Draw(x - cx, y - cy, bbox, rect.left, rect.top, rect.right, rect.bottom, 0.25f);
}

CGameObject::~CGameObject()
{

}