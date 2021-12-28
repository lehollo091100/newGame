#include "IntroScene.h"
#include "SampleKeyEventHandler.h"
#define ONE_100	40

IntroScene::IntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	//player = NULL;
	isDoneSeq1 = isDoneSeq2 =isDoneSeq3=isDoneSeq4= isDoneSeq5= false;
	key_handler = new CSampleKeyHandler(this);
	map = new Map();
	mapid = id;
	path = filePath;
}
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void IntroScene::_ParseSection_SPRITES(string line)
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

void IntroScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void IntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/


void IntroScene::LoadAssets(LPCWSTR assetFile)
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

void IntroScene::Load()
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
		//case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	DebugOut(L"mapid:%d", mapid);
	map->SetMap(mapid);
	f.close();
	//introbackground
	introbackground->SetPosition(CGame::GetInstance()->GetBackBufferWidth() / 2, -CGame::GetInstance()->GetBackBufferHeight());
	objects.push_back(introbackground);
	//introoption
	option->SetPosition(CGame::GetInstance()->GetBackBufferWidth() / 2, -CGame::GetInstance()->GetBackBufferHeight());
	objects.push_back(option);
	//platform
	brick->SetPosition(CGame::GetInstance()->GetBackBufferWidth() / 2, 185);
	objects.push_back(brick);
	//mario+tail
	redMario->SetPosition(10, 0);
	objects.push_back(redMario);
	Tail* obj1 = new Tail(10 + KOOPAS_WIDTH, 0);
	obj1->SetPosition(10 - WIDTH, 0);
	objects.push_back(obj1);
	CMario* a = dynamic_cast<CMario*>(redMario);
	a->tail = obj1;
	//green mario
	greenMario->SetPosition(CGame::GetInstance()->GetBackBufferWidth(), 0);
	objects.push_back(greenMario);
	Tail* obj2 = new Tail(CGame::GetInstance()->GetBackBufferWidth() + KOOPAS_WIDTH, 0);
	obj2->SetPosition(CGame::GetInstance()->GetBackBufferWidth() - WIDTH, 0);
	objects.push_back(obj2);
	GreenMario* b = dynamic_cast<GreenMario*>(greenMario);
	b->tail = obj2;
	//curtain
	curtain->SetPosition(CGame::GetInstance()->GetBackBufferWidth() / 2, CGame::GetInstance()->GetBackBufferHeight() / 2-10);
	objects.push_back(curtain);
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void IntroScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//DebugOut(L"object type :%d\n", objects[i]->type);
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)



	// Update camera to follow mario
	float cx, cy;
	ScriptIntro();

	CGame::GetInstance()->SetCamPos(0, 0);

	PurgeDeletedObjects();
}

void IntroScene::Render()
{
	map->Drawmap();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

}

void IntroScene::ScriptIntro()
{
	if (SequenceTime == 0)
		SequenceTime = GetTickCount64();
	if (isDoneSeq1 == false)
	{
		if (GetTickCount64() - SequenceTime >= Sequence1MaxTime*4)
		{

			SequenceTime = GetTickCount64();
			curtain->SetState(CURTAIN_STATE_MOVING);
			isDoneSeq1 = true;
		}
	}
	if (isDoneSeq1 && !isDoneSeq2)
	{
		if (GetTickCount64() - SequenceTime >= Sequence1MaxTime)
		{
			SequenceTime = GetTickCount64();
			redMario->SetState(MARIO_STATE_WALKING_RIGHT);
			greenMario->SetState(GREENMARIO_STATE_WALKING_LEFT);
			isDoneSeq2 = true;
		}
	}
	if (isDoneSeq2&&!isDoneSeq3)
	{
		if (GetTickCount64() - SequenceTime >= 900)
		{
			introbackground->SetState(INTROBACKGROUND_STATE_MOVING);
			redMario->SetState(MARIO_STATE_JUMP);
			//greenMario->SetState(MARIO_STATE_IDLE);
			isDoneSeq3 = true;
			SequenceTime = GetTickCount64();
		}
	}
	if (isDoneSeq3&&!isDoneSeq4)
	{
		
		if (GetTickCount64()-SequenceTime>=500)
		{
			if (redMario->isOnPlatform)
			{
				if (greenMario->isSitting) {
					greenMario->y -= GREENMARIO_SIT_HEIGHT_ADJUST;
					greenMario->isSitting = false;
					greenMario->SetState(GREENMARIO_STATE_IDLE);
				}

				//redMario->SetState(MARIO_STATE_IDLE);
				isDoneSeq4 = true;
			}
			SequenceTime = GetTickCount64();
		}
	}
	if (isDoneSeq4 && !isDoneSeq5) {
		if (GetTickCount64() - SequenceTime >= 3000)
		{
			//redMario->Delete();
			if (greenMario)
			{
				greenMario->SetState(GREENMARIO_STATE_WALKING_LEFT);
			}
			SequenceTime = GetTickCount64();
			option->SetPosition(CGame::GetInstance()->GetBackBufferWidth() / 2, CGame::GetInstance()->GetBackBufferHeight() / 2 +ONE_100);
			CGame::GetInstance()->allowKey = true;
			isDoneSeq5 = true;
		}
	}
}

/*
*	Clear all objects from this scene
*/
void IntroScene::Clear()
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
void IntroScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	map->Clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool IntroScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }



void IntroScene::PurgeDeletedObjects()
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
		std::remove_if(objects.begin(), objects.end(), IntroScene::IsGameObjectDeleted),
		objects.end());
}