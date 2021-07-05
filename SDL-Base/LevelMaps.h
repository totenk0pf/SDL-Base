#pragma once

#ifndef _LEVELMAP_H
#define _LEVELMAP_H

#include "Constants.h"

class Vector2D;
class LevelMap {
public:
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();

	int GetTileAt(unsigned int w, unsigned int h);
	void ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue);
	Vector2D GetWorldPos(unsigned int w, unsigned int h);

	int** GetMap() { return mMap; };

private:
	int** mMap;
};

#endif // !_LEVELMAP_H
