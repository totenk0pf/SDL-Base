#include <iostream>
#include "GameOver.h"
#include "Texture2D.h"
#include "Collisions.h"
#include "Debug.h"
#include "PowBlock.h"
#include "Coin.h"
#include "CharacterKoopa.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "GameManager.h"
#include "TextRenderer.h"
#include "DataParser.h"

//GameManager* gameManager = GameManager::Instance();

GameOver::GameOver(SDL_Renderer* renderer) : GameScreen(renderer) {
	mLevelMap = nullptr;
	titleSpr = new Texture2D(mRenderer);
	selectSpr = new Texture2D(mRenderer);
	mBgMusic = new SoundEffect;
	opt = 0;
	if (mBgMusic->Load("Sounds/01.ogg")) {
		mBgMusic->Play(true);
	}
	SetUpLevel();
	SetGameState(LOSE_STATE);
	SetNextGameState(LOSE_STATE);
}

GameOver::~GameOver() {
}

void GameOver::Update(float deltaTime, const Uint8* keyState) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_DOWN:
				if (opt < 1) {
					opt++;
				} else {
					opt = 0;
				}
				break;
			case SDLK_UP:
				if (opt <= 0) {
					opt = 1;
				} else {
					opt--;
				}
				break;
			case SDLK_RETURN:
				if (!opt) {
					SetNextGameState(INTRO_STATE);
				} else {
					SetNextGameState(EXIT_STATE);
				}
				break;
			}
			break;
		}
	}
}

void GameOver::Render() {
	SDL_RenderClear(mRenderer);
	headerText->Render(mRenderer, "GAME OVER", (SCREEN_WIDTH * 0.5f) - headerText->GetSize("GAME OVER").first * 0.5f, 160);
	selectSpr->Render(Vector2D((SCREEN_WIDTH * 0.5f) - 80, 240 + (20 * opt + 1)));
	UIText->Render(mRenderer, "BACK TO MENU", (SCREEN_WIDTH * 0.5f) - UIText->GetSize("BACK TO MENU").first * 0.5f, 240);
	UIText->Render(mRenderer, "EXIT", (SCREEN_WIDTH * 0.5f) - UIText->GetSize("EXIT").first * 0.5f, 260);
	SDL_RenderPresent(mRenderer);
}

bool GameOver::SetUpLevel() {
	UIText = new TextRenderer(8);
	headerText = new TextRenderer(24);
	selectSpr->LoadFromFile("Images/title_select.png");
	return true;
}