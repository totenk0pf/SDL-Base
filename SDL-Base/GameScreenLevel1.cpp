#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer) {
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() {
	mBackgroundTexture = nullptr;
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {

}

void GameScreenLevel1::Render() {
	mBackgroundTexture->Render(Vector2D());
}

bool GameScreenLevel1::SetUpLevel() {
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Images/Test.bmp")) {
		cout << "Failed to load background texture!";
		return false;
	}
}