#pragma once

#ifndef _POW_BLOCK_H
#define _POW_BLOCK_H

#include "Texture2D.h"
#include "LevelMaps.h"
#include "Commons.h"

class PowBlock {
public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();

	void Render();
	Rect2D GetCollisionBox();
	void TakeAHit();
	bool IsAvailable() { return mHitsLeft > 0; };
	Vector2D GetPosition() { return mPosition; };

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	LevelMap* mLevelMap;

	float mFrameW;
	float mFrameH;

	Uint8 mHitsLeft;

protected:

};

#endif // !_POW_BLOCK_H
