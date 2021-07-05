#include "CharacterMario.h"
#include "Constants.h"
#include "Texture2D.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map) : Character(renderer, "Images/Mario.png", startPosition, map) {
	mFacingDirection = FACING_RIGHT;
	mLevelMap = map;
	mName = "Mario";
}

CharacterMario::~CharacterMario() {
	Character::~Character();
}

void CharacterMario::Render() {
	if (mAlive) {
		if (mFacingDirection == FACING_RIGHT) {
			mTexture->Render(mPosition);
		} else {
			mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
		}
	}
}

void CharacterMario::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MOVEMENT_SPEED * deltaTime;
}

void CharacterMario::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MOVEMENT_SPEED * deltaTime;
}

void CharacterMario::Update(float deltaTime, const Uint8* keyState) {
	Character::Update(deltaTime, keyState);
	mAlive = Character::GetAlive();
	if (mAlive) {
		if (keyState[SDL_SCANCODE_A] && mCanMoveLeft) {
			if (mCanMoveLeft) {
				mMovingLeft = true;
				mMovingRight = false;
				MoveLeft(deltaTime);
			}
		}
		if (keyState[SDL_SCANCODE_D] && mCanMoveRight) {
			if (mCanMoveRight) {
				mMovingRight = true;
				mMovingLeft = false;
				MoveRight(deltaTime);
			}
		}
		if (keyState[SDL_SCANCODE_SPACE]) {
			Jump(deltaTime);
		}
	}
}

