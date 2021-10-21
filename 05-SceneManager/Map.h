#pragma once
#include <iostream>
#include <fstream>
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "Utils.h"
class Map
{
	int TexRow, TexCol, MapRow, MapCol;
	UINT TexW, TexH;
	int mapId;
	int map[300][300];
	//char* FileName;
	//string MapLink;
	int nMap;
	string MapLink;
	unordered_map<int, LPTEXTURE> maps;
	static Map* _instance;
	CTextures* textures=CTextures::GetInstance();
public:
	Map() {
		TexH = TexW = 16;
		//SetMap();
		//ReadMap();
	};
	~Map() {};
	void ReadMap();
	void AddSprites();
	void Drawmap();
	void SetMap(int id);
	void Clear();
	static Map* GetInstance();
};



