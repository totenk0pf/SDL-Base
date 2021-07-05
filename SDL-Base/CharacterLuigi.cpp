#include "CharacterLuigi.h"
#include "Constants.h"
#include "Texture2D.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map) : Character(renderer, "Images/Luigi.png", startPosition, map) {
	mFacingDirection = FACING_LEFT;
	mLevelMap = map;
	mName = "Luigi";
}

CharacterLuigi::~CharacterLuigi() {
	Character::~Character();
}

void CharacterLuigi::Render() {
	if (mAlive) {
		if (mFacingDirection == FACING_RIGHT) {
			mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
		} else {
			mTexture->Render(mPosition);
		}
	}
}

void CharacterLuigi::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MOVEMENT_SPEED * deltaTime;
}

void CharacterLuigi::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MOVEMENT_SPEED * deltaTime;
}

void CharacterLuigi::Update(float deltaTime, const Uint8* keyState) {
	Character::Update(deltaTime, keyState);
	mAlive = Character::GetAlive();
	if (mAlive) {
		if (keyState[SDL_SCANCODE_LEFT] && mCanMoveLeft) {
			mMovingLeft = true;
			mMovingRight = false;
			MoveLeft(deltaTime);
		}
		if (keyState[SDL_SCANCODE_RIGHT] && mCanMoveRight) {
			mMovingRight = true;
			mMovingLeft = false;
			MoveRight(deltaTime);
		}
		if (keyState[SDL_SCANCODE_UP]) {
			Jump(deltaTime);
		}
	}
}

