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
	myCharacter->Update(deltaTime, e);
}

void GameScreenLevel1::Render() {
	mBackgroundTexture->Render(Vector2D());
	myCharacter->Render();
}

bool GameScreenLevel1::SetUpLevel() {
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Images/Test.bmp")) {
		std::cout << "Failed to load background texture!";
		return false;
	}
	myCharacter = new Character(mRenderer, "Images/Mario.png", Vector2D(64, 330));
}