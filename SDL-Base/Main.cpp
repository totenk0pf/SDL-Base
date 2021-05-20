#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Commons.h"
#include "Constants.h"
#include "Texture2D.h"
#include "GameScreenManager.h"

using namespace std;

SDL_Renderer* gRenderer = nullptr;
SDL_Window* gWindow = nullptr;
GameScreenManager* gameScreenManager;
Uint32 gOldTime;

bool InitSDL();
void CloseSDL();
bool Update();

bool InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL did not initialize. Error: " << SDL_GetError();
		return false;
	} else {
		gWindow = SDL_CreateWindow(
			"SDL Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);
		if (gWindow == nullptr) {
			std::cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer != nullptr) {
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cout << "SDL_Image could not initialize. Error: " << IMG_GetError();
				return false;
			}
		} else {
			std::cout << "Renderer could not initialize. Error: " << SDL_GetError();
			return false;
		}
		return true;
	}
}

void CloseSDL() {
	delete gameScreenManager;
	gameScreenManager = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	IMG_Quit();
	SDL_Quit();
}

bool Update() {
	Uint32 newTime = SDL_GetTicks();
	SDL_Event e;
	SDL_PollEvent(&e);
	gameScreenManager->Update((float)(newTime - gOldTime) / 1000.0f, e);
	gOldTime = newTime;
	switch (e.type) {
		case SDL_QUIT:
			return true;
			break;
	}
	return false;
}

int main(int argc, char* args[]) {
	bool quit = false;
	if (InitSDL()) {
		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL1);
		gOldTime = SDL_GetTicks();
		while (!quit) {
			gameScreenManager->Render();
			quit = Update();
		}
	}
	CloseSDL();
	return 0;
}