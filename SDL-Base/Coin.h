#pragma once

#include "Character.h"

#ifndef _COIN_H
#define _COIN_H

class Coin : public Character {
public:
	Coin(SDL_Renderer* renderer, Vector2D startPosition, LevelMap* map);
	~Coin();

	void Render();
	void Update(float deltaTime);

	Rect2D GetCollisionBox();

private:
	float mFrameDelay;
	float mCurrentFrame;


protected:
};

#endif // !_COIN_H