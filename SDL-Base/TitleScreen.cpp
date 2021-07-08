#include <iostream>
#include "TitleScreen.h"
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

TitleScreen::TitleScreen(SDL_Renderer* renderer) : GameScreen(renderer) {
	mLevelMap = nullptr;
	titleSpr = new Texture2D(mRenderer);
	selectSpr = new Texture2D(mRenderer);
	mBgMusic = new SoundEffect;
	opt = 0;
	if (mBgMusic->Load("Sounds/01.ogg")) {
		mBgMusic->Play(true);
	}
	SetUpLevel();
	SetGameState(INTRO_STATE);
	SetNextGameState(INTRO_STATE);
}

TitleScreen::~TitleScreen() {
}

void TitleScreen::Update(float deltaTime, const Uint8* keyState) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_DOWN:
				if (opt < 2) {
					opt++;
				} else {
					opt = 0;
				}
				break;
			case SDLK_UP:
				if (opt <= 0) {
					opt = 2;
				} else {
					opt--;
				}
				break;
			case SDLK_RETURN:
				if (!opt) {
					SetNextGameState(GAME_STATE);
				} else {
					SetNextGameState(EXIT_STATE);
				}
				break;
			}
			break;
		}
	}
}

void TitleScreen::Render() {
	SDL_RenderClear(mRenderer);
	titleSpr->Render(Vector2D((SCREEN_WIDTH * 0.5f) - titleSpr->GetWidth() * 0.5f, (SCREEN_HEIGHT * 0.3f)));
	selectSpr->Render(Vector2D((SCREEN_WIDTH * 0.5f) - 80, 240 + (20 * opt + 1)));
	UIText->Render(mRenderer, "1 PLAYER GAME A", (SCREEN_WIDTH * 0.5f) - UIText->GetSize("1 PLAYER GAME A").first * 0.5f, 240);
	UIText->Render(mRenderer, "2 PLAYER GAME B", (SCREEN_WIDTH * 0.5f) - UIText->GetSize("2 PLAYER GAME B").first * 0.5f, 260);
	UIText->Render(mRenderer, "EXIT", (SCREEN_WIDTH * 0.5f) - UIText->GetSize("EXIT").first * 0.5f, 280);
	SDL_RenderPresent(mRenderer);
}

bool TitleScreen::SetUpLevel() {
	UIText = new TextRenderer(8);
	titleSpr->LoadFromFile("Images/title.png");
	selectSpr->LoadFromFile("Images/title_select.png");
	return true;
}