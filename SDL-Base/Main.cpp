#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Commons.h"
#include "Constants.h"
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "imgui_sdl.h"
#include "imgui.h"
#include "Imgui/backends/imgui_impl_sdl.h"


SDL_Renderer* gRenderer = nullptr;
SDL_Window* gWindow = nullptr;
GameScreenManager* gameScreenManager;
Uint32 gOldTime;

bool InitSDL();
void CloseSDL();
bool Update();

bool InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_Rect rendererRect {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderSetClipRect(gRenderer, &rendererRect);
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
		if (TTF_Init() < 0) {
			std::cout << "Error: " << TTF_GetError() << std::endl;
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
	//TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Update() {
	Uint32 newTime = SDL_GetTicks();
	ImGuiIO& io = ImGui::GetIO();
	SDL_Event e;
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	gameScreenManager->Update((float)(newTime - gOldTime) / 1000.0f, keyState);
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				return true;
				break;
			case SDL_RENDER_TARGETS_RESET:
				gameScreenManager->Render();
				break;
			case SDL_RENDER_DEVICE_RESET:
				gameScreenManager->Render();
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym) {
					case SDLK_RETURN:
						if (gameScreenManager->GetNextGameState() == GAME_STATE) {
							gameScreenManager->ChangeScreen(SCREEN_LEVEL1);
						} else if (gameScreenManager->GetCurrentGameState() == GAME_STATE) {
							gameScreenManager->ChangeScreen(SCREEN_INTRO);
						} else if (gameScreenManager->GetNextGameState() == EXIT_STATE) {
							return true;
						}
						break;
				}
				break;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	gOldTime = newTime;
	return false;
}

int main(int argc, char* args[]) {
	bool quit = false;
	if (InitSDL()) {
		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL1);
		gOldTime = SDL_GetTicks();
		ImGui::CreateContext();
		ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
		while (!quit) {
			Uint64 start = SDL_GetPerformanceCounter();
			gameScreenManager->Render();
			quit = Update();
			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;
		}
	}
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	CloseSDL();
	return 0;
}