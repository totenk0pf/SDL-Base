#include "Coin.h"
#include "Constants.h"
#include "Texture2D.h"

Coin::Coin(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map) : Character(renderer, "Images/Coin.png", startPosition, map) {
	mFrameW = mTexture->GetWidth() / 3;
	mFrameH = mTexture->GetHeight();
}

Coin::~Coin() {
	Character::~Character();
}

void Coin::Render() {
	float sprX = 0.0f;
	SDL_Rect frame = { mFrameW * mCurrentFrame, 0, mFrameW, mFrameH };
	SDL_Rect destRect = { (int)mPosition.x, (int)mPosition.y, mFrameW, mFrameH };
	mTexture->Render(frame, destRect, SDL_FLIP_HORIZONTAL);
}

void Coin::Update(float deltaTime) {
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f) {
		mFrameDelay = ANIMATION_DELAY;
		mCurrentFrame++;

		if (mCurrentFrame > 2) {
			mCurrentFrame = 0;
		}
	}
}

Rect2D Coin::GetCollisionBox() {
	return Rect2D(mPosition.x, mPosition.y, mFrameW, mFrameH);
}