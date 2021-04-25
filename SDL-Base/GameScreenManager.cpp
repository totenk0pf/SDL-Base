#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "GameScreenManager.h"

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

void GameScreenManager::Update(float deltaTime, SDL_Event e) {
	mCurrentScreen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen) {
	if (mCurrentScreen != nullptr) {
		delete mCurrentScreen;
	}
	GameScreenLevel1* tmpScreen;
	switch (newScreen) {
		case SCREEN_LEVEL1:
			tmpScreen = new GameScreenLevel1(mRenderer);
			mCurrentScreen = (GameScreen*)tmpScreen;
			tmpScreen = nullptr;
			break;
	}
}