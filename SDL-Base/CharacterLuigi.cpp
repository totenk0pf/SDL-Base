#include "CharacterLuigi.h"
#include "Constants.h"
#include "Texture2D.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map) : Character(renderer, "Images/luigi_sheet.png", startPosition, map) {
	mFacingDirection = FACING_LEFT;
	mLevelMap = map;
	mName = "LUIGI";
	mFrameW = mTexture->GetWidth() / 8;
	mFrameH = mTexture->GetHeight();
}

CharacterLuigi::~CharacterLuigi() {
	Character::~Character();
}

void CharacterLuigi::Render() {
	if (mAlive) {
		SDL_Rect frame = { mFrameW * mCurrentFrame, 0, mFrameW, mFrameH };
		SDL_Rect destRect = { (int)mPosition.x, (int)mPosition.y, mFrameW, mFrameH };
		if (mFacingDirection == FACING_RIGHT) {
			mTexture->Render(frame, destRect, SDL_FLIP_HORIZONTAL);
		} else {
			mTexture->Render(frame, destRect);
		}
	}
}

void CharacterLuigi::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	if (mAcceleration < MAX_ACCELERATION) {
		mAcceleration += ACCELERATION_RATE;
	} else {
		mAcceleration = MAX_ACCELERATION;
	}
	mPosition.x -= MOVEMENT_SPEED * deltaTime * mAcceleration;
}

void CharacterLuigi::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	if (mAcceleration < MAX_ACCELERATION) {
		mAcceleration += ACCELERATION_RATE + deltaTime;
	} else {
		mAcceleration = MAX_ACCELERATION;
	}
	mPosition.x += MOVEMENT_SPEED * deltaTime * mAcceleration;
}

void CharacterLuigi::Update(float deltaTime, const Uint8* keyState) {
	mAlive = Character::GetAlive();
	Character::Update(deltaTime, keyState);
	if (mAlive) {
		if (keyState[SDL_SCANCODE_LEFT] && mCanMoveLeft) {
			mMovingLeft = true;
			mMovingRight = false;
			mInitFrame = 1;
			Character::PlayAnim(deltaTime, 2, 0.1f);
			MoveLeft(deltaTime);
		} else if (keyState[SDL_SCANCODE_RIGHT] && mCanMoveRight) {
			mMovingRight = true;
			mMovingLeft = false;
			mInitFrame = 1;
			Character::PlayAnim(deltaTime, 2, 0.1f);
			MoveRight(deltaTime);
		} else {
			mMovingLeft = false;
			mMovingRight = false;
		}
		if (keyState[SDL_SCANCODE_UP]) {
			mCurrentFrame = 5;
			mInitFrame = 5;
			Character::PlayAnim(deltaTime, 0, 0.1f);
			Jump(deltaTime);
		}
		if (!mMovingLeft && !mMovingRight && !mJumping) {
			if (mAcceleration > 0) {
				mAcceleration -= 0.1f;
			} else {
				mAcceleration = 0.0f;
			}
			mCurrentFrame = 0;
		}
	} else {
		Character::Die(deltaTime);
	}
}

