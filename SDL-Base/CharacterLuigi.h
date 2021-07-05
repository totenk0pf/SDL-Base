#pragma once

#ifndef _CHARACTER_LUIGI
#define _CHARACTER_LUIGI

#include "Character.h"

class CharacterLuigi : public Character {
public:
	CharacterLuigi(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map);
	~CharacterLuigi();

	void Update(float deltaTime, const Uint8* keyState);
	void Render();

	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);

private:
	FACING mFacingDirection;
	bool mMovingLeft;
	bool mMovingRight;
	LevelMap* mLevelMap;

protected:
};

#endif // !_CHARACTER_LUIGI