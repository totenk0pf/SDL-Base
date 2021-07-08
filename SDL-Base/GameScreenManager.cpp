#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "WinScreen.h"
#include "GameScreenManager.h"
#include "TitleScreen.h"
#include "GameOver.h"
#include "TextRenderer.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen) {
	mRenderer = renderer;
	overlayText = new TextRenderer(8);
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
	//overlayText->Render(mRenderer, "Development build", (SCREEN_WIDTH) - overlayText->GetSize("Development build").first, (SCREEN_HEIGHT) - overlayText->GetSize("Development build").second);
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
		case LVL1_STATE:
			ChangeScreen(SCREEN_LEVEL1);
			break;
		case LVL2_STATE:
			ChangeScreen(SCREEN_LEVEL2);
			break;
		case LOSE_STATE:
			ChangeScreen(SCREEN_GAMEOVER);
			break;
		case WIN_STATE:
			ChangeScreen(SCREEN_HIGHSCORES);
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
	GameScreenLevel2* level2;
	TitleScreen* mainMenu;
	WinScreen* winScreen;
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
		case SCREEN_LEVEL2:
			level2 = new GameScreenLevel2(mRenderer);
			mCurrentScreen = (GameScreen*) level2;
			level2 = nullptr;
			break;
		case SCREEN_GAMEOVER:
			gameOver = new GameOver(mRenderer);
			mCurrentScreen = (GameScreen*) gameOver;
			gameOver = nullptr;
			break;
		case SCREEN_HIGHSCORES:
			winScreen = new WinScreen(mRenderer);
			mCurrentScreen = (GameScreen*) winScreen;
			winScreen = nullptr;
			break;
	}
}

GAMESTATE GameScreenManager::GetCurrentGameState() {
	return mCurrentScreen->GetGameState();
};

GAMESTATE GameScreenManager::GetNextGameState() {
	return mCurrentScreen->GetNextGameState();
};