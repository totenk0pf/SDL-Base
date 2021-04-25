#include "GameScreen.h"
#include <SDL.h>
#include <iostream>

using namespace std;

GameScreen::GameScreen(SDL_Renderer* renderer) {
	mRenderer = renderer;
}

GameScreen::~GameScreen() {
	mRenderer = nullptr;
}

void GameScreen::Update(float deltaTime, SDL_Event e) {}
void GameScreen::Render() {};