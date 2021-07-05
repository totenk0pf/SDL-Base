#include "PowBlock.h"
#include <string>
#include <iostream>

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map) {
	mRenderer = renderer;
	mLevelMap = map;
	std::string imgPath = "Images/PowBlock.png";
	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imgPath.c_str())) {
		std::cout << "Failed to load texture: " << imgPath << std::endl;
		return;
	}
	mFrameW = mTexture->GetWidth() / 3;
	mFrameH = mTexture->GetHeight();
	mHitsLeft = 3;
	mPosition = mLevelMap->GetWorldPos(8,7);
}

PowBlock::~PowBlock() {
	mRenderer = nullptr;
	delete mTexture;
	mTexture = nullptr;
	mLevelMap = nullptr;
}

void PowBlock::Render() {
	if (mHitsLeft > 0) {
		int x = mFrameW * (mHitsLeft - 1);
		SDL_Rect frame = {x, 0, mFrameW, mFrameH};
		SDL_Rect dest = {(int)(mPosition.x), (int)(mPosition.y), mFrameW, mFrameH};
		mTexture->Render(frame, dest, SDL_FLIP_NONE);
	}
}

void PowBlock::TakeAHit() {
	mHitsLeft--;
	if (mHitsLeft <= 0) {
		mHitsLeft = 0;
		mLevelMap->ChangeTileAt(7, 8, 0);
	}
}

Rect2D PowBlock::GetCollisionBox() {
	return Rect2D(mPosition.x, mPosition.y, mFrameW, mFrameH);
}