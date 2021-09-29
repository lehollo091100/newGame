#include "Map.h"
#include "debug.h"
#define MAP_TEXTURE_ID	30
Map* Map::_instance = NULL;
void Map::ReadMap()
{
	ifstream ifs(L"textures\\out.txt", ios::in);
	DebugOut(L"what");
	ifs >> MapRow >> MapCol;
	DebugOut(L"%d %d", MapRow, MapCol);
	for (int i = 0; i < MapRow; i++)
		for (int j = 0; j < MapCol; j++)
		{
			ifs >> map[i][j];
		}
	ifs.close();
}
void Map::Drawmap()
{
	for (UINT i = 0; i < MapRow; i++)
	{
		for (UINT j = 0; j < MapCol; j++)
		{
			float x = TexW * j;
			float y = TexH * i;
			CSprites::GetInstance()->Get(map[i][j])->Draw(x, y - 240);
		}
	}
}
void Map::SetMap()
{
	/// add textures of the map(TILEMAP)
	CTextures* textures = CTextures::GetInstance();
	LPTEXTURE texMap = textures->Get(MAP_TEXTURE_ID);
	//textures->Get(MAP_TEXTURE_ID);
	// 
	//Load Spite for the map
	int id_sprite = 1;
	for (UINT i = 0; i < TexRow; i++)
	{
		for (UINT j = 0; j < TexCol; j++)
		{
			CSprites::GetInstance()->Add(id_sprite, TexW * j, TexH * i, TexW * (j + 1), TexH * (i + 1), texMap);
			id_sprite = id_sprite + 1;
		}
	}

}
Map* Map::GetInstance()
{
	if (_instance == NULL) _instance = new Map();
	return _instance;
}
