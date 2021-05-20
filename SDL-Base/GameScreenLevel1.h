#pragma once

#ifndef _GAME_SCREEN_LEVEL_1_H
#define _GAME_SCREEN_LEVEL_1_H

#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"

class Texture2D;
class Character;

class GameScreenLevel1 : GameScreen {
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();
	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	Texture2D* mBackgroundTexture;
	bool SetUpLevel();
	Character* myCharacter;
};

#endif // !_GAME_SCREEN_LEVEL_1_H
