#include "LevelMaps.h"
#include "Commons.h"

class Vector2D;
LevelMap::LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]) {
	mMap = new int* [MAP_HEIGHT];
	for (unsigned int y = 0; y < MAP_HEIGHT; y++) {
		mMap[y] = new int[MAP_WIDTH];
	}
	for (unsigned int y = 0; y < MAP_HEIGHT; y++) {
		for (unsigned int x = 0; x < MAP_WIDTH; x++) {
			mMap[y][x] = map[y][x];
		}
	}
}

LevelMap::~LevelMap() {
	for (unsigned int y = 0; y < MAP_HEIGHT; y++) {
		delete [] mMap[y];
	}
	delete [] mMap;
}

int LevelMap::GetTileAt(unsigned int w, unsigned int h) {
	if (h < MAP_HEIGHT && w < MAP_WIDTH) {
		return mMap[h][w];
	}
	return 0;
}

void LevelMap::ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue) {
	mMap[column][row] = newValue;
}

Vector2D LevelMap::GetWorldPos(unsigned int w, unsigned int h) {
	return Vector2D(32*h, 32*w);
}