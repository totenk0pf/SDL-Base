#pragma once

#ifndef _GAME_SCREEN_MANAGER_H
#define _GAME_SCREEN_MANAGER_H

#include <SDL.h>
#include "Commons.h"

class GameScreen;
class GameScreenManager {
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, const Uint8* keyState);
	void ChangeScreen(SCREENS newScreen);

	GAMESTATE GetCurrentGameState();
	GAMESTATE GetNextGameState();

private:
	SDL_Renderer* mRenderer;
	GameScreen* mCurrentScreen;
};

#endif // !_GAME_SCREEN_MANAGER_H