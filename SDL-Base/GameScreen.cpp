#include "GameScreen.h"
#include <SDL.h>
#include <iostream>

GameScreen::GameScreen(SDL_Renderer* renderer) {
	mRenderer = renderer;
}

GameScreen::~GameScreen() {
	mRenderer = nullptr;
}

void GameScreen::Update(float deltaTime, const Uint8* keyState) {}

void GameScreen::Render() {};