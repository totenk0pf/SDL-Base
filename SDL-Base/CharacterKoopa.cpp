#include "CharacterKoopa.h"
#include "Constants.h"
#include "Texture2D.h"

CharacterKoopa::CharacterKoopa(SDL_Renderer* renderer, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed) : Character(renderer, "Images/Koopa.png", startPosition, map) {
	mFacingDirection = startFacing;
	mMovementSpeed = movementSpeed;
	mPosition = startPosition;

	mAlive = true;
	mInjured = false;
	mJumping = true;

	mFrameW = mTexture->GetWidth() / 2;
	mFrameH = mTexture->GetHeight();

}

CharacterKoopa::~CharacterKoopa() {
	Character::~Character();
}

void CharacterKoopa::Update(float deltaTime, const Uint8* keyState) {
	if (mAlive) {
		Character::Update(deltaTime, keyState);
		std::vector<int> colMatrix = GetCollisionMatrix();
		if (!mInjured) {
			switch (mFacingDirection) {
			case FACING_LEFT:
				mMovingLeft = true;
				mMovingRight = false;
				break;
			case FACING_RIGHT:
				mMovingLeft = false;
				mMovingRight = true;
				break;
			}
			if (!colMatrix[3] || !colMatrix[5]) {
				switch (mFacingDirection) {
					case FACING_LEFT:
						MoveLeft(deltaTime);
						break;
					case FACING_RIGHT:
						MoveRight(deltaTime);
						break;
				}
			} else {
				if (colMatrix[3] && mFacingDirection == FACING_LEFT) {
					Flip();
				} else if (colMatrix[5] && mFacingDirection == FACING_RIGHT) {
					Flip();
				}
			}
		} else {
			mMovingLeft = false;
			mMovingRight = false;
			mInjuredTime -= deltaTime;
			if (mInjuredTime <= 0.0f) {
				Flip();
			}
		}
	}
}

void CharacterKoopa::Render() {
	float sprX = 0.0f;
	if (mInjured) {
		sprX = mFrameW;
	}
	SDL_Rect frame = { sprX, 0, mFrameW, mFrameH };
	SDL_Rect destRect = { (int)mPosition.x, (int)mPosition.y, mFrameW, mFrameH };
	if (mFacingDirection == FACING_RIGHT) {
		mTexture->Render(frame, destRect, SDL_FLIP_NONE);
	} else {
		mTexture->Render(frame, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void CharacterKoopa::Flip() {
	switch (mFacingDirection) {
	case FACING_LEFT:
		mFacingDirection = FACING_RIGHT;
		break;
	case FACING_RIGHT:
		mFacingDirection = FACING_LEFT;
		break;
	}
	mInjured = false;
	Jump();
}

void CharacterKoopa::TakeDamage() {
	mInjured = true;
	mInjuredTime = INJURED_TIME;
	Jump();
}

void CharacterKoopa::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE / 3;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	mPosition.x -= mMovementSpeed * deltaTime;
}

void CharacterKoopa::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	mPosition.x += mMovementSpeed * deltaTime;
}

Rect2D CharacterKoopa::GetCollisionBox() {
	return Rect2D(mPosition.x, mPosition.y, mFrameW, mFrameH);
}