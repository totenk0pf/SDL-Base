#pragma once

#ifndef _GAME_SCREEN_LEVEL_2_H
#define _GAME_SCREEN_LEVEL_2_H

#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include <vector>

class Texture2D;
class CharacterMario;
class CharacterLuigi;
class CharacterKoopa;
class GameManager;
class Coin;
class PowBlock;
class TextRenderer;

class GameScreenLevel2 : GameScreen {
public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2();
	void Render();
	void Update(float deltaTime, const Uint8* keyState);
	void UpdatePOWBlock();
	void PlayerCollision();
	void ScreenShake();
	void UpdateEnemies(float deltaTime, const Uint8* keyState);
	void UpdateCoins(float deltaTime);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateCoin(Vector2D position);


private:
	bool mScreenshake;
	float mScreenshakeTime;
	float mWobble;
	float mBackgroundYPos;

	Texture2D* mBackgroundTexture;
	LevelMap* mLevelMap;
	SoundEffect* mBgMusic;
	PowBlock* mPowBlock;
	TextRenderer* UIText;
	GameManager* gameManager;
	std::vector<CharacterKoopa*> mEnemies;
	std::vector<Character*> mCharacters;
	std::vector<Coin*> mCoins;

	bool SetUpLevel();
	void SetLevelMap();
};

#endif // !_GAME_SCREEN_LEVEL_2_H
