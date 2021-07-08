#pragma once

#ifndef _CHARACTER_KOOPA_H
#define _CHARACTER_KOOPA_H

#include "Character.h"

class CharacterKoopa : public Character {
public:
	CharacterKoopa(SDL_Renderer* renderer, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed);
	~CharacterKoopa();

	void TakeDamage();
	void Jump();

	virtual void Render();
	virtual void Update(float deltaTime, const Uint8* keyState);

	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);

	Rect2D GetCollisionBox();
	bool GetInjured() { return mInjured; };
	void Flip(bool jump = false);

private:
	float mMovementSpeed;
	
	bool mInjured;
	float mInjuredTime;


	FACING mFacingDirection;
	bool mMovingLeft;
	bool mMovingRight;
	LevelMap* mLevelMap;

protected:


};

#endif // !_CHARACTER_KOOPA_H
