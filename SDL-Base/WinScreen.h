#pragma once

#ifndef _WIN_SCREEN_H
#define _WIN_SCREEN_H

#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include <vector>

class Texture2D;
class GameManager;
class TextRenderer;

class WinScreen : GameScreen {
public:
	WinScreen(SDL_Renderer* renderer);
	~WinScreen();
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

#endif // !_WIN_SCREEN_H