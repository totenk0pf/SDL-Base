#pragma once

#ifndef _GAME_SCREEN_H
#define _GAME_SCREEN_H

#include <SDL.h>
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "LevelMaps.h"
#include "SoundEffect.h"

class GameScreen {
public:
	GameScreen(SDL_Renderer* renderer);
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, const Uint8* keyState);

	GAMESTATE GetGameState() { return mGameState; };
	GAMESTATE GetNextGameState() { return mNextGameState; };

private:

protected:
	SDL_Renderer* mRenderer;
	GAMESTATE mGameState;
	GAMESTATE mNextGameState;

	void SetGameState(GAMESTATE state) { mGameState = state; };
	void SetNextGameState(GAMESTATE state) { mNextGameState = state; };
};

#endif // !_GAME_SCREEN_H