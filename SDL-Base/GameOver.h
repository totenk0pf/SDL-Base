#pragma once

#ifndef _GAME_OVER_H
#define _GAME_OVER_H

#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include <vector>

class Texture2D;
class GameManager;
class TextRenderer;

class GameOver : GameScreen {
public:
	GameOver(SDL_Renderer* renderer);
	~GameOver();
	void Render();
	void Update(float deltaTime, const Uint8* keyState);

private:
	LevelMap* mLevelMap;
	SoundEffect* mBgMusic;
	TextRenderer* UIText;
	TextRenderer* headerText;
	Texture2D* titleSpr;
	Texture2D* selectSpr;

	bool SetUpLevel();
	int opt;
};

#endif // !_GAME_OVER_H