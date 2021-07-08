#include "CharacterMario.h"
#include "Constants.h"
#include "Texture2D.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map) : Character(renderer, "Images/mario_sheet.png", startPosition, map) {
	mFacingDirection = FACING_RIGHT;
	mLevelMap = map;
	mName = "MARIO";
	mFrameW = mTexture->GetWidth() / 8;
	mFrameH = mTexture->GetHeight();
}

CharacterMario::~CharacterMario() {
	Character::~Character();
}

void CharacterMario::Render() {
	SDL_Rect frame = { mFrameW * mCurrentFrame, 0, mFrameW, mFrameH };
	SDL_Rect destRect = { (int)mPosition.x, (int)mPosition.y, mFrameW, mFrameH };
	if (mFacingDirection == FACING_RIGHT) {
		mTexture->Render(frame, destRect, SDL_FLIP_HORIZONTAL);
	} else {
		mTexture->Render(frame, destRect);
	}
}

void CharacterMario::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	if (mAcceleration < MAX_ACCELERATION) {
		mAcceleration += ACCELERATION_RATE;
	} else {
		mAcceleration = MAX_ACCELERATION;
	}
	mPosition.x -= MOVEMENT_SPEED * deltaTime * mAcceleration;
}

void CharacterMario::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	if (mAcceleration < MAX_ACCELERATION) {
		mAcceleration += ACCELERATION_RATE + deltaTime;
	} else {
		mAcceleration = MAX_ACCELERATION;
	}
	mPosition.x += MOVEMENT_SPEED * deltaTime * mAcceleration;
}

void CharacterMario::Update(float deltaTime, const Uint8* keyState) {
	mAlive = Character::GetAlive();
	Character::Update(deltaTime, keyState);
	if (mAlive) {
		if (keyState[SDL_SCANCODE_A] && mCanMoveLeft) {
			mMovingLeft = true;
			mMovingRight = false;
			mInitFrame = 1;
			Character::PlayAnim(deltaTime, 2, 0.1f);
			MoveLeft(deltaTime);
		} else if (keyState[SDL_SCANCODE_D] && mCanMoveRight) {
			mMovingRight = true;
			mMovingLeft = false;
			mInitFrame = 1;
			Character::PlayAnim(deltaTime, 2, 0.1f);
			MoveRight(deltaTime);
		} else {
			mMovingLeft = false;
			mMovingRight = false;
		}
		if (keyState[SDL_SCANCODE_SPACE]) {
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

