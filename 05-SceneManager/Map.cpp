#include "Map.h"
#include "debug.h"
//#define MAP_TEXTURE_ID	30
#define MAP_DIR	L"textures\\TileMap.png"
#define MAP_RANGE	10
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
Map* Map::_instance = NULL;
void Map::ReadMap()
{
	ifstream ifs(MapLink, ios::in);
	DebugOut(L"what");
	ifs >> MapRow >> MapCol;
	DebugOut(L"%d %d", MapRow, MapCol);
	for (int i = 0; i < MapRow; i++)
		for (int j = 0; j < MapCol; j++)
		{
			ifs >> map[i][j];
			//DebugOut(L"mapij: %d", map[i][j]);
		}
	ifs.close();
}
void Map::SetMap(int id)
{
	mapId = id;
	string TexLink;
	ifstream ifs(L"mapInfo.txt", ios::in);
	ifs >> nMap;
	if (mapId > nMap)
		return;
	for (int i = 0; i < mapId; i++)
	{
		ifs >> MapLink;
		ifs >> TexLink;
		ifs >> TexRow >> TexCol;
	}
	/// add textures of the map(TILEMAP)
	wstring path = ToWSTR(TexLink);
	textures->Add(mapId, path.c_str());
	AddSprites();
}
void Map::AddSprites()
{
	//DebugOut(L"mapid: %d ", mapId);
	LPTEXTURE texMap = textures->Get(mapId);
	if (texMap == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", mapId);
		return;
	}
	int id_sprite = 1;
	for (UINT i = 0; i < TexRow; i++)
	{
		for (UINT j = 0; j < TexCol; j++)
		{

			CSprites::GetInstance()->Add(id_sprite, TexW * j, TexH * i, TexW * (j + 1), TexH * (i + 1), texMap);
			//DebugOut(L"idsprite: %d",id_sprite);
			id_sprite = id_sprite + 1;
		}
	}
	ReadMap();
}
void Map::Drawmap()
{

	for (UINT i = 0; i < UINT(MapRow); i++)
	{
		for (UINT j = 0; j <UINT(MapCol); j++)
		{
			//DebugOut(L"i: j: %d %d ", i, j);
			float x = float(TexW) * float(j);
			float y = float(TexH) * float(i);
			//DebugOut(L"%d", map[i][j]);
			float a, b;
			CGame::GetInstance()->GetCamPos(a,b);
			if (((x > a - MAP_RANGE) && (x < a + SCREEN_WIDTH - MAP_RANGE)) && ((y > b - MAP_RANGE) && (y < b - MAP_RANGE + SCREEN_HEIGHT)))
			{
				CSprites::GetInstance()->Get(map[i][j])->Draw(x,y);
			}
		}
	}
}


void Map::Clear()
{
	TexCol = TexRow = 0;

}
Map* Map::GetInstance()
{
	if (_instance == NULL) _instance = new Map();
	return _instance;
}
