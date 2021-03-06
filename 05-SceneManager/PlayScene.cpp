#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#define MAP1_WIDTH	2810
#define MAP1_HEIGHT	424
#define MAP2_WIDTH	512
#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	//player = NULL;
	player = CMario::GetInstance();
	key_handler = new CSampleKeyHandler(this);
	map = new Map();
	mapid = id;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[static_cast<__int64>(i)+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO: {

		/*if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		DebugOut(L"[INFO] Player object has been created!\n");*/
		//obj = CMario::GetInstance();
		//player= (CMario*)obj;
		if (player == NULL)
		{
			player = CMario::GetInstance();
		}
		if (player->NextX != 0 && player->NextY != 0)
		{
			x = player->NextX;
			y = player->NextY;
		}
		player->SetPosition(x, y);
		Tail* obj1 = new Tail(x + KOOPAS_WIDTH, y);
		obj1->SetPosition(x - WIDTH, y);
		objects.push_back(obj1);
		CMario* a = dynamic_cast<CMario*>(player);
		a->tail = obj1;
		DebugOut(L"[INFO] Player object created!\n");

		break;
	}
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_BRICK: 
	{
		float bwidth = (float)atof(tokens[3].c_str());
		float bheight = (float)atof(tokens[4].c_str());
		obj = new CBrick(x,y,bwidth,bheight); 
		break;
	}
	case OBJECT_TYPE_COIN: 
	{
		obj = new CCoin(x, y); break;
	}
	case OBJECT_TYPE_COLORBRICK: {
		float bwidth = (float)atof(tokens[3].c_str());
		float bheight = (float)atof(tokens[4].c_str());
		obj = new ColorBrick(x, y, bwidth, bheight);
		break;
	}
	case OBJECT_TYPE_QUESTIONBRICK:
	{
		obj = new QuestionBrick(x,y);
		questionbricks.push_back(dynamic_cast<QuestionBrick*>(obj));
		break;
	}
	case OBJECT_TYPE_ITEM: {
		int typeItem = (int)atof(tokens[3].c_str());
		switch (typeItem)
		{
		case 0: {
			obj = new CoinItem(x, y);
			items.push_back(obj);
			break;
		}
		case 1: {
			obj = new Mushroom(x, y);
			items.push_back(obj);
			break;
		}
		default:
			break;
		}
		//obj = new Mushroom(x, y);
		//items.push_back(dynamic_cast<Mushroom*>(obj));
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_PIPE: {
		float pwidth = (float)atof(tokens[3].c_str());
		float pheight = (float)atof(tokens[4].c_str());
		int scene = (int)atof(tokens[5].c_str());
		int d = (int)atof(tokens[6].c_str());
		int a = (int)atof(tokens[7].c_str());
		float nextx = 0;
		float nexty = 0;
		if (scene != -1)
		{
			nextx = (float)atof(tokens[8].c_str());
			 nexty = (float)atof(tokens[9].c_str());
		}
		obj = new Pipe(x, y, pwidth, pheight, scene,d,a,nextx,nexty);
		break;
	}
	case OBJECT_TYPE_KOOPAS: {
		int type = (int)atof(tokens[3].c_str());
		int color = (int)atof(tokens[4].c_str());
		obj = new Koopas(x, y,type,color);
		Koopasitem* obj1 = new Koopasitem(x + KOOPAS_WIDTH, y);
		obj1->SetPosition(x + KOOPAS_WIDTH, y);
		objects.push_back(obj1);
		Koopas* a = dynamic_cast<Koopas*>(obj);
		a->item = obj1;
		break;
	}
	case OBJECT_TYPE_REDGOOMBA: {
		obj = new Redgoomba(x, y);
		break;
	}
	case OBJECT_TYPE_GREENPLANT: {
		obj = new GreenPlant(x, y);
		break;
	}
	case OBJECT_TYPE_FIREREDPLANT: {
		obj = new FireRedPlant(x, y);
		PlantFire* obj1 = new PlantFire(x, y);
		obj1->SetPosition(x, y);
		objects.push_back(obj1);
		FireRedPlant* a = dynamic_cast<FireRedPlant*>(obj);
		a->item = obj1;
		break;
	}
	case OBJECT_TYPE_FIREGREENPLANT: {
		obj = new FireGreenPlant(x, y);
		PlantFire* obj1 = new PlantFire(x, y);
		obj1->SetPosition(x, y);
		objects.push_back(obj1);
		FireGreenPlant* a = dynamic_cast<FireGreenPlant*>(obj);
		a->item = obj1;
		break;
	}
	case OBJECT_TYPE_SHINNINGBRICK: {
		obj = new ShinningBrick(x, y);
		ShinningBrick* a = dynamic_cast<ShinningBrick*>(obj);
		Debris* obj1 = new Debris(x, y, DEBRIS_VX, DEBRIS_VY_BIG, -1);
		obj1->SetPosition(x, y);
		objects.push_back(obj1);
		a->d1 = obj1;
		Debris* obj2 = new Debris(x , y , DEBRIS_VX, DEBRIS_VY_BIG, 1);
		obj2->SetPosition(x , y );
		objects.push_back(obj2);
		a->d2 = obj2;
		Debris* obj3 = new Debris(x, y, DEBRIS_VX, DEBRIS_VY_SMALL, -1);
		obj3->SetPosition(x, y);
		objects.push_back(obj3);
		a->d3 = obj3;
		Debris* obj4 = new Debris(x, y, DEBRIS_VX, DEBRIS_VY_SMALL, 1);
		obj4->SetPosition(x, y);
		objects.push_back(obj4);
		a->d4 = obj4;
		break;
	}
	//case OBJECT_TYPE_PLANTFIRE: {
	//	obj = new PlantFire(x, y);
	//	break;
	//}
	case OBJECT_TYPE_PBRICK: {
		obj = new PBrick(x, y);
		obj->SetPosition(x, y);
		PButton* obj1 = new PButton(x, y);
		obj1->SetPosition(x, y);
		objects.push_back(obj1);
		PBrick* a = dynamic_cast<PBrick*>(obj);
		a->button = obj1;
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_ENDGAMEITEM: {
		obj = new EndGameItem(x, y);
		break;
	}


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj != NULL)
		obj->SetPosition(x, y);
	//obj->SetPosition(x, y);
	if (obj != NULL)
	{
		objects.push_back(obj);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	DebugOut(L"mapid:%d", mapid);
	map->SetMap(mapid);
	f.close();
	for (int i = 0; i < questionbricks.size(); i++)
	{
		questionbricks[i]->item = items[i];
	}
	questionbricks = vector<QuestionBrick*>();
	items = vector<LPGAMEOBJECT>();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (player->level == MARIO_LEVEL_TAIL)
		{
			if (objects[i]->type == OBJECT_TYPE_QUESTIONBRICK)
			{
				QuestionBrick* qbrick = dynamic_cast<QuestionBrick*>(objects[i]);
				if (qbrick->item->type == OBJECT_TYPE_MUSHROOM)
				{
					float x1 = qbrick->x;
					float y1 = qbrick->y;
					qbrick->item->Delete();
					Leaf* leaf = new Leaf(x1, y1);
					objects.push_back(leaf);
					qbrick->item = leaf;
				}
				/*if (qbrick->item->type == OBJECT_TYPE_LEAF)
				{
					float x1 = qbrick->x;
					float y1 = qbrick->y;
					qbrick->item->Delete();
					Mushroom* mush = new Mushroom(x1, y1);
					objects.push_back(mush);
					qbrick->item = mush;
				}*/
			}

		}
		if (player->level <MARIO_LEVEL_TAIL)
		{
			if (objects[i]->type == OBJECT_TYPE_QUESTIONBRICK)
			{
				QuestionBrick* qbrick = dynamic_cast<QuestionBrick*>(objects[i]);
				if (qbrick->item->type == OBJECT_TYPE_LEAF)
				{
					float x1 = qbrick->x;
					float y1 = qbrick->y;
					qbrick->item->Delete();
					qbrick->Delete();
					QuestionBrick* newbrick = new QuestionBrick(x1,y1);
					Mushroom* mush = new Mushroom(x1, y1);
					newbrick->item = mush;
					objects.push_back(mush);
					objects.push_back(newbrick);
				}
			}

		}
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if ((objects[i]->x > CGame::GetInstance()->GetCamX()- CGame::GetInstance()->GetBackBufferWidth()) && (objects[i]->x <= CGame::GetInstance()->GetCamX() + CGame::GetInstance()->GetBackBufferWidth()*1.5))
		{
			objects[i]->Update(dt, &coObjects);
		}
	}

	player->Update(dt, &coObjects);
	hud->Update(dt);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 


	// Update camera to follow mario
	float cx, cy;

	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;
	if (cy <= 0)
		cy = 0;

	if (cx < 0) cx = 0;
	if (cy < 0) cy = 0;

	if (mapid == 2)
	{
		if(cx>=MAP2_WIDTH- game->GetBackBufferWidth())
		{
			cx = MAP2_WIDTH - float(game->GetBackBufferWidth());
		}
		cy = 0;
	}
	if (mapid == 1)
	{
		if (cx >= MAP1_WIDTH - game->GetBackBufferWidth())
		{
			cx = MAP1_WIDTH - float(game->GetBackBufferWidth());
		}
		if (cy >= MAP1_HEIGHT - game->GetBackBufferHeight() + HUD_HEIGHT) {
			cy = MAP1_HEIGHT -float( game->GetBackBufferHeight()) + HUD_HEIGHT;
		}
	}

	CGame::GetInstance()->SetCamPos(cx, cy);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	map->Drawmap();
	player->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();

}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	map->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}