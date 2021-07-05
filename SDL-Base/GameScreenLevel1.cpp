#include <iostream>
#include "GameScreenLevel1.h"
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

GameManager* gameManager = GameManager::Instance();

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer) {
	mLevelMap = nullptr;
	SetUpLevel();
	mBgMusic = new SoundEffect;
	if (mBgMusic->Load("Sounds/01.mp3")) {
		mBgMusic->Play(true);
	}
}

GameScreenLevel1::~GameScreenLevel1() {
	mBackgroundTexture = nullptr;
	delete mPowBlock;
	mPowBlock = nullptr;
	mEnemies.clear();
}

void GameScreenLevel1::Update(float deltaTime, const Uint8* keyState) {
	if (mScreenshake) {
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;
		if (mScreenshakeTime <= 0.0f) {
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_h:
				gameManager->SetDebug(!gameManager->GetDebug());
				break;
			}
			break;
		}
	}
	charMario->Update(deltaTime, keyState);
	charLuigi->Update(deltaTime, keyState);
	PlayerCollision();
	UpdatePOWBlock();
	UpdateEnemies(deltaTime, keyState);
	UpdateCoins(deltaTime);
}

void GameScreenLevel1::Render() {
	SDL_RenderClear(mRenderer);

	// Drawing the sky by declaring a rect
	SDL_SetRenderDrawColor(mRenderer, 0x61, 0x85, 0xF8, 0xFF);
	SDL_Rect bgRect {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	SDL_RenderFillRect(mRenderer, &bgRect);
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);

	if (gameManager->GetDebug()) {
		for (unsigned int y = 0; y < 12; y++) {
			for (unsigned int x = 0; x < 17; x++) {
				SDL_Rect mRect{ 32 * x-1, 32 * y-1, 34, 34 };
				SDL_SetRenderDrawColor(mRenderer, 0x20, 0x20, 0x20, 0xFF);
				SDL_RenderDrawRect(mRenderer, &mRect);
				SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}

	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);
	mPowBlock->Render();

	if (mPowBlock->IsAvailable()) {
		if (gameManager->GetDebug()) {
			SDL_Rect powRect{mPowBlock->GetPosition().x, mPowBlock->GetPosition().y, mPowBlock->GetCollisionBox().w, mPowBlock->GetCollisionBox().h};
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(mRenderer, &powRect);
			SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
		}
	}

	for (Uint8 c = 0; c < mCharacters.size(); c++) {
		if (mCharacters[c]->GetAlive()) {
			mCharacters[c]->Render();
			if (gameManager->GetDebug()) {
				SDL_Rect charRect{ mCharacters[c]->GetPosition().x, mCharacters[c]->GetPosition().y, mCharacters[c]->GetCollisionBox().w, mCharacters[c]->GetCollisionBox().h};
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawRect(mRenderer, &charRect);
				SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);

			}
		}
	}

	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		mEnemies[i]->Render();
		if (gameManager->GetDebug()) {
			SDL_Rect kRect{ mEnemies[i]->GetPosition().x, mEnemies[i]->GetPosition().y, mEnemies[i]->GetCollisionBox().w, mEnemies[i]->GetCollisionBox().h };
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(mRenderer, &kRect);
			SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
		}
	}

	for (unsigned int i = 0; i < mCoins.size(); i++) {
		mCoins[i]->Render();
		if (gameManager->GetDebug()) {
			SDL_Rect cRect{ mCoins[i]->GetPosition().x, mCoins[i]->GetPosition().y, mCoins[i]->GetCollisionBox().w, mCoins[i]->GetCollisionBox().h };
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(mRenderer, &cRect);
			SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
		}
	}

	UIText->Render(mRenderer, "MARIO", 40, 30);
	UIText->Render(mRenderer, "000000", 40, 50);
	UIText->Render(mRenderer, "LUIGI", 40, 70);
	UIText->Render(mRenderer, "000000", 40, 90);

	if (gameManager->GetDebug()) {
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		if (ImGui::BeginTabBar("charBar")) {
			for (unsigned int i = 0; i < mCharacters.size(); i++) {
				if (ImGui::BeginTabItem(mCharacters[i]->mName.c_str())) {
					std::string charPos = "Pos: (" + std::to_string((int)mCharacters[i]->GetPosition().x) + ", " + std::to_string((int)mCharacters[i]->GetPosition().y) + ")";
					ImGui::Text(charPos.c_str());
					ImGui::Text(("mCanMoveLeft: " + std::to_string(mCharacters[i]->canMoveLeft())).c_str());
					ImGui::Text(("mCanMoveRight: " + std::to_string(mCharacters[i]->canMoveRight())).c_str());
					ImGui::Text(("mFalling: " + std::to_string(mCharacters[i]->IsFalling())).c_str());
					ImGui::Text(("mGrounded: " + std::to_string(mCharacters[i]->IsGrounded())).c_str());
					ImGui::Text(("mJumping: " + std::to_string(mCharacters[i]->IsJumping())).c_str());
					ImGui::Text(("mCanJump: " + std::to_string(mCharacters[i]->canJump())).c_str());
					ImGui::Text(("mAlive: " + std::to_string(mCharacters[i]->GetAlive())).c_str());
					ImGui::Text("Collision matrix: ");
					std::vector<int> colMatrix = mCharacters[i]->GetCollisionMatrix();
					for (Uint8 i = 0; i < colMatrix.size(); i++) {
						if (i % 3 == 0) {
						} else {
							ImGui::SameLine();
						}
						ImGui::Text(std::to_string(colMatrix[i]).c_str());
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
		ImGui::EndFrame();

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}


	SDL_RenderPresent(mRenderer);
}

void GameScreenLevel1::ScreenShake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		mEnemies[i]->TakeDamage();
	}
}

bool GameScreenLevel1::SetUpLevel() {
	SetGameState(GAME_STATE);;
	mBackgroundTexture = new Texture2D(mRenderer);
	SetLevelMap();
	charMario = new CharacterMario(mRenderer, Vector2D(64, 330), mLevelMap);
	mCharacters.push_back(charMario);
	charLuigi = new CharacterLuigi(mRenderer, Vector2D(256, 330), mLevelMap);
	mCharacters.push_back(charLuigi);
	CreateKoopa(Vector2D(150, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), FACING_LEFT, KOOPA_SPEED);
	CreateCoin(Vector2D(172, 352));
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;
	if (!mBackgroundTexture->LoadFromFile("Images/BackgroundMB.png")) {
		std::cout << "Failed to load background texture!";
		return false;
	}
	UIText = new TextRenderer(12);
}

void GameScreenLevel1::SetLevelMap() {
	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	if (mLevelMap) {
		delete mLevelMap;
	}
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock() {
	if (Collisions::Instance()->Box(mPowBlock->GetCollisionBox(), charMario->GetCollisionBox()) ^
		Collisions::Instance()->Box(mPowBlock->GetCollisionBox(), charLuigi->GetCollisionBox())) {
		if (mPowBlock->IsAvailable()) {
			if (charMario->IsJumping()) {
				ScreenShake();
				mPowBlock->TakeAHit();
				charMario->CancelJump();
			} else if (charLuigi->IsJumping()) {
				ScreenShake();
				mPowBlock->TakeAHit();
				charLuigi->CancelJump();
			}
		}
	}
}

void GameScreenLevel1::PlayerCollision() {
	// Tilemap collision
	for (unsigned int i = 0; i < mCharacters.size(); i++) {
		int footPos = (int)(mCharacters[i]->GetPosition().y + mCharacters[i]->GetCollisionBox().h) / TILE_HEIGHT;
		std::vector<int> colMatrix =  mCharacters[i]->GetCollisionMatrix();
		if (mCharacters[i]->IsJumping()) {
			if (colMatrix[2] || colMatrix[5] || colMatrix[8]) {
				mCharacters[i]->SetCanMoveRight(false);
			} else {
				mCharacters[i]->SetCanMoveRight(true);
			}

			if (colMatrix[0] || colMatrix[3] || colMatrix[6]) {
				mCharacters[i]->SetCanMoveLeft(false);
			} else {
				mCharacters[i]->SetCanMoveLeft(true);
			}
		} else {
			mCharacters[i]->SetCanMoveRight(true);
			mCharacters[i]->SetCanMoveLeft(true);
		}
		if (!colMatrix[7]) {
			mCharacters[i]->SetFalling(true);
		} else {
			mCharacters[i]->SetFalling(false);
			mCharacters[i]->SetCanJump(true);
		}
	}
	// Player collision
	for (unsigned int c1 = 0; c1 < mCharacters.size(); c1++) {
		Rect2D c1Col = mCharacters[c1]->GetCollisionBox();
		std::vector<int> c1Matrix = mCharacters[c1]->GetCollisionMatrix();
		for (unsigned int c2 = 0; c2 < mCharacters.size(); c2++) {
			if (mCharacters[c1] != mCharacters[c2]) {
				Rect2D c2Col = mCharacters[c2]->GetCollisionBox();
				std::vector<int> c2Matrix = mCharacters[c2]->GetCollisionMatrix();
				if (Collisions::Instance()->Box(c1Col, c2Col)) {
					if (mCharacters[c1]->GetAlive() && mCharacters[c2]->GetAlive()) {
						if (c1Matrix[7] && c2Matrix[7]) {
							if (c1Col.x + c1Col.w <= c2Col.x + 4) {
								mCharacters[c1]->SetCanMoveLeft(true);
								mCharacters[c1]->SetCanMoveRight(false);
								mCharacters[c2]->SetCanMoveLeft(false);
								mCharacters[c2]->SetCanMoveRight(true);
							} else if (c2Col.x + c2Col.w <= c1Col.x + 4) {
								mCharacters[c1]->SetCanMoveLeft(false);
								mCharacters[c1]->SetCanMoveRight(true);
								mCharacters[c2]->SetCanMoveLeft(true);
								mCharacters[c2]->SetCanMoveRight(false);
							}
						} else {
							mCharacters[c1]->SetCanMoveLeft(true);
							mCharacters[c1]->SetCanMoveRight(true);
							mCharacters[c2]->SetCanMoveLeft(true);
							mCharacters[c2]->SetCanMoveRight(true);
						}
						if (c1Col.y + c1Col.h >= c2Col.y) {
							mCharacters[c1]->SetCanJump(true);
							mCharacters[c1]->SetGrounded(true);
							mCharacters[c1]->SetFalling(false);
							mCharacters[c1]->SetJumping(false);
						}
						if (c2Col.y + c2Col.h >= c1Col.y) {
							mCharacters[c2]->SetCanJump(true);
							mCharacters[c2]->SetGrounded(true);
							mCharacters[c2]->SetFalling(false);
							mCharacters[c2]->SetJumping(false);
						}
					}
				}
			}
		}
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, const Uint8* keyState) {
	if (!mEnemies.empty()) {
		int enemyIdx = -1;
		for (unsigned int i = 0; i < mEnemies.size(); i++) {
			if (mEnemies[i]->GetPosition().y > 300.0f) {
				if (mEnemies[i]->GetPosition().x < (float)(-mEnemies[i]->GetCollisionBox().w * 0.5f) ||
					mEnemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemies[i]->GetCollisionBox().w * 0.55f)) {
					mEnemies[i]->SetAlive(false);
				}
			}
			if ((mEnemies[i]->GetPosition().y > 300.0f || mEnemies[i]->GetPosition().y <= 64.0f) &&
				(mEnemies[i]->GetPosition().x < 64.0f || mEnemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f)) {
			} else {
				for (unsigned int c = 0; c < mCharacters.size(); c++) {
					if (Collisions::Instance()->Box(mEnemies[i]->GetCollisionBox(), mCharacters[c]->GetCollisionBox())) {
						if (mEnemies[i]->GetInjured()) {
							mEnemies[i]->SetAlive(false);
							mEnemies[i]->~CharacterKoopa();
						} else {
							mCharacters[c]->SetAlive(false);
							mCharacters[c]->Die();
						}
					}
				}
				int footPos = (int)(mEnemies[i]->GetPosition().y + mEnemies[i]->GetCollisionBox().h) / TILE_HEIGHT;
				std::vector<int> colMatrix = mEnemies[i]->GetCollisionMatrix(3, 1);
				switch (mEnemies[i]->GetDirection()) {
					case FACING_LEFT:
						if (!colMatrix[8]) {
							mEnemies[i]->SetFalling(true);
						} else {
							mEnemies[i]->SetFalling(false);
						}
						break;
					case FACING_RIGHT:
						if (!colMatrix[6]) {
							mEnemies[i]->SetFalling(true);
						} else {
							mEnemies[i]->SetFalling(false);
						}
						break;
				}
			}
			mEnemies[i]->Update(deltaTime, keyState);
			if (!mEnemies[i]->GetAlive()) {
				enemyIdx = i;
			}
		}
		if (enemyIdx != -1) {
			mEnemies.erase(mEnemies.begin() + enemyIdx);
		}
	}
}

void GameScreenLevel1::UpdateCoins(float deltaTime) {
	if (!mCoins.empty()) {
		for (Uint8 i = 0; i < mCoins.size(); i++) {
			mCoins[i]->Update(deltaTime);
		}
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* _tmpKoopa = new CharacterKoopa(mRenderer, mLevelMap, position, direction, speed);
	mEnemies.push_back(_tmpKoopa);
}

void GameScreenLevel1::CreateCoin(Vector2D position) {
	Coin* _tmpCoin = new Coin(mRenderer, position, mLevelMap);
	mCoins.push_back(_tmpCoin);
}