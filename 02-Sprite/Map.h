#pragma once
#include <iostream>
#include <fstream>
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
class Map
{
	int TexRow, TexCol, MapRow, MapCol;
	UINT TexW, TexH;
	int mapId;
	int map[300][300];
	//char* FileName;
	//string MapLink;
	int nMap;
	unordered_map<int, LPTEXTURE> maps;
	static Map* _instance;

public:
	Map() {
		TexRow = 12;
		TexCol = 11;
		TexH = TexW = 16;
		SetMap();
		ReadMap();
	};
	~Map() {};
	void ReadMap();
	void Drawmap();
	void LoadSpiteMap();
	void SetMap();
	void Clear();
	static Map* GetInstance();
};

