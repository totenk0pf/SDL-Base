#pragma once

#ifndef _CHARACTER_MARIO
#define _CHARACTER_MARIO

#include "Character.h"

class CharacterMario : public Character {
public:
	CharacterMario(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map);
	~CharacterMario();

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

#endif // !_CHARACTER_MARIO