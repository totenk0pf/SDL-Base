#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "GameScreenManager.h"
#include "TitleScreen.h"
#include "GameOver.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen) {
	mRenderer = renderer;
	mCurrentScreen = nullptr;
	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager() {
	mRenderer = nullptr;
	delete mCurrentScreen;
	mCurrentScreen = nullptr;
}

void GameScreenManager::Render() {
	mCurrentScreen->Render();
}

bool GameScreenManager::Update(float deltaTime, const Uint8* keyState) {
	if (mCurrentScreen->GetGameState() != mCurrentScreen->GetNextGameState()) {
		switch (mCurrentScreen->GetNextGameState()) {
		case INTRO_STATE:
			ChangeScreen(SCREEN_MENU);
			break;
		case EXIT_STATE:
			return true;
			break;
		case GAME_STATE:
			ChangeScreen(SCREEN_LEVEL1);
			break;
		case LOSE_STATE:
			ChangeScreen(SCREEN_GAMEOVER);
			break;
		}
	}
	mCurrentScreen->Update(deltaTime, keyState);
	return false;
}

void GameScreenManager::ChangeScreen(SCREENS newScreen) {
	if (mCurrentScreen != nullptr) {
		delete mCurrentScreen;
	}
	GameScreenLevel1* level1;
	TitleScreen* mainMenu;
	GameOver* gameOver;
	switch (newScreen) {
		case SCREEN_MENU:
			mainMenu = new TitleScreen(mRenderer);
			mCurrentScreen = (GameScreen*) mainMenu;
			mainMenu = nullptr;
			break;
		case SCREEN_LEVEL1:
			level1 = new GameScreenLevel1(mRenderer);
			mCurrentScreen = (GameScreen*) level1;
			level1 = nullptr;
			break;
		case SCREEN_GAMEOVER:
			gameOver = new GameOver(mRenderer);
			mCurrentScreen = (GameScreen*) gameOver;
			gameOver = nullptr;
			break;
	}
}

GAMESTATE GameScreenManager::GetCurrentGameState() {
	return mCurrentScreen->GetGameState();
};

GAMESTATE GameScreenManager::GetNextGameState() {
	return mCurrentScreen->GetNextGameState();
};