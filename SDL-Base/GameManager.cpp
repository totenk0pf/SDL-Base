#include "GameManager.h"

GameManager* GameManager::mGameManager = nullptr;

GameManager* GameManager::Instance() {
	if (!mGameManager) {
		mGameManager = new GameManager();
	}
	return mGameManager;
};

GameManager::~GameManager() {
	mGameManager = nullptr;
}