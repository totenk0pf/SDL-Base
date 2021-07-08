#include <iostream>
#include "GameScreenLevel2.h"
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

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(renderer) {
	mLevelMap = nullptr;
	SetUpLevel();
	mBgMusic = new SoundEffect;
	gameManager = new GameManager();
}

GameScreenLevel2::~GameScreenLevel2() {
	mBackgroundTexture = nullptr;
	delete mPowBlock;
	mPowBlock = nullptr;
	mEnemies.clear();
	mCoins.clear();
	mCharacters.clear();
}

void GameScreenLevel2::Update(float deltaTime, const Uint8* keyState) {
	if (mCharacters.size() <= 0) {
		SetNextGameState(LOSE_STATE);
	}
	if (mEnemies.size() <= 0 && mCoins.size() <= 0) {
		SetNextGameState(WIN_STATE);
	}
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
	for (Uint8 i = 0; i < mCharacters.size(); i++) {
		mCharacters[i]->Update(deltaTime, keyState);
	}
	//PlayerCollision();
	UpdatePOWBlock();
	UpdateEnemies(deltaTime, keyState);
	UpdateCoins(deltaTime);
}

void GameScreenLevel2::Render() {
	SDL_RenderClear(mRenderer);

	// Render the sky by drawing a filled rect
	SDL_SetRenderDrawColor(mRenderer, 0x61, 0x85, 0xF8, 0xFF);
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_RenderFillRect(mRenderer, &bgRect);
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);

	if (gameManager->GetDebug()) {
		for (unsigned int y = 0; y < 12; y++) {
			for (unsigned int x = 0; x < 17; x++) {
				SDL_Rect mRect{ 32 * x - 1, 32 * y - 1, 34, 34 };
				SDL_SetRenderDrawColor(mRenderer, 0x20, 0x20, 0x20, 0x30);
				SDL_RenderDrawRect(mRenderer, &mRect);
				SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}

	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);
	mPowBlock->Render();

	if (mPowBlock->IsAvailable()) {
		if (gameManager->GetDebug()) {
			SDL_Rect powRect{ mPowBlock->GetPosition().x, mPowBlock->GetPosition().y, mPowBlock->GetCollisionBox().w, mPowBlock->GetCollisionBox().h };
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(mRenderer, &powRect);
			SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
		}
	}

	for (Uint8 c = 0; c < mCharacters.size(); c++) {
		mCharacters[c]->Render();
		if (gameManager->GetDebug()) {
			SDL_Rect charRect{ mCharacters[c]->GetPosition().x, mCharacters[c]->GetPosition().y, mCharacters[c]->GetCollisionBox().w, mCharacters[c]->GetCollisionBox().h };
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(mRenderer, &charRect);
			SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);

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

	for (Uint8 i = 0; i < mCharacters.size(); i++) {
		UIText->Render(mRenderer, mCharacters[i]->mName.c_str(), SCREEN_WIDTH / 9, 40 * (i + 1));
		UIText->Render(mRenderer, mCharacters[i]->GetScore(), 6, SCREEN_WIDTH / 9, (40 * (i + 2)) - 20);
		UIText->Render(mRenderer, mCharacters[i]->GetCoins(), 2, (SCREEN_WIDTH / 9) * 3, (40 * (i + 2)) - 20);
	}

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
					ImGui::Text(("mCanJump: " + std::to_string(mCharacters[i]->canJump())).c_str());
					ImGui::Text(("mFalling: " + std::to_string(mCharacters[i]->IsFalling())).c_str());
					ImGui::Text(("mGrounded: " + std::to_string(mCharacters[i]->IsGrounded())).c_str());
					ImGui::Text(("mJumping: " + std::to_string(mCharacters[i]->IsJumping())).c_str());
					ImGui::Text(("mAlive: " + std::to_string(mCharacters[i]->GetAlive())).c_str());
					ImGui::Text(("mCurrentFrame: " + std::to_string(mCharacters[i]->GetCurrentFrame())).c_str());
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

void GameScreenLevel2::ScreenShake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		mEnemies[i]->TakeDamage();
	}
}

bool GameScreenLevel2::SetUpLevel() {
	SetGameState(LVL2_STATE);
	SetNextGameState(LVL2_STATE);
	mBackgroundTexture = new Texture2D(mRenderer);
	SetLevelMap();
	CharacterMario* charMario = new CharacterMario(mRenderer, Vector2D(64, 330), mLevelMap);
	CharacterLuigi* charLuigi = new CharacterLuigi(mRenderer, Vector2D(320, 330), mLevelMap);
	nlohmann::json data = DataParser::Instance()->DataFromFile("GameData/Level2/lvldata.json");
	mCharacters.push_back(charMario);
	mCharacters.push_back(charLuigi);
	for (Uint8 i = 0; i < data["enemies"].size(); i++) {
		Vector2D pos = Vector2D(data["enemies"][i]["pos"][0], data["enemies"][i]["pos"][1]);
		FACING dir;
		if (data["enemies"][i]["facing"] == "left") {
			dir = FACING_LEFT;
		} else {
			dir = FACING_RIGHT;
		}
		CreateKoopa(pos, dir, KOOPA_SPEED);
	}
	for (Uint8 i = 0; i < data["coins"].size(); i++) {
		Vector2D pos = Vector2D(data["coins"][i][0], data["coins"][i][1]);
		CreateCoin(pos);
	}
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;
	if (!mBackgroundTexture->LoadFromFile("Images/level2_bg.png")) {
		std::cout << "Failed to load background texture!";
		return false;
	}
	UIText = new TextRenderer(12);
}

void GameScreenLevel2::SetLevelMap() {
	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	if (mLevelMap) {
		delete mLevelMap;
	}
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel2::UpdatePOWBlock() {
	for (Uint8 i = 0; i < mCharacters.size(); i++) {
		if (Collisions::Instance()->Box(mPowBlock->GetCollisionBox(), mCharacters[i]->GetCollisionBox()) && mCharacters[i]->GetPosition().y < mPowBlock->GetPosition().y + mPowBlock->GetCollisionBox().h) {
			if (mPowBlock->IsAvailable()) {
				if (mCharacters[i]->IsJumping()) {
					ScreenShake();
					mPowBlock->TakeAHit();
					mCharacters[i]->CancelJump();
				}
			}
		}
	}
}

void GameScreenLevel2::PlayerCollision() {
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
	int idx = -1;
	for (unsigned int c = 0; c < mCharacters.size(); c++) {
		if (mCharacters[c]->GetPosition().y > 400) {
			idx = c;
		}
		if (idx != -1) {
			mCharacters.erase(mCharacters.begin() + idx);
		}
	}
}

void GameScreenLevel2::UpdateEnemies(float deltaTime, const Uint8* keyState) {
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
						} else {
							mCharacters[c]->SetAlive(false);
						}
					}
				}
				std::vector<int> colMatrix = mEnemies[i]->GetCollisionMatrix(3, 1);
				switch (mEnemies[i]->GetDirection()) {
				case FACING_LEFT:
					if (!colMatrix[7]) {
						mEnemies[i]->SetFalling(true);
					} else {
						mEnemies[i]->SetFalling(false);
					}
					break;
				case FACING_RIGHT:
					if (!colMatrix[7]) {
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

void GameScreenLevel2::UpdateCoins(float deltaTime) {
	if (!mCoins.empty()) {
		int idx = -1;
		for (Uint8 i = 0; i < mCoins.size(); i++) {
			mCoins[i]->Update(deltaTime);
			for (unsigned int c = 0; c < mCharacters.size(); c++) {
				if (Collisions::Instance()->Box(mCoins[i]->GetCollisionBox(), mCharacters[c]->GetCollisionBox())) {
					mCoins[i]->SetAlive(false);
					mCharacters[c]->SetCoins(mCharacters[c]->GetCoins() + 1);
					mCharacters[c]->SetScore(mCharacters[c]->GetScore() + 100);
				}
				if (!mCoins[i]->GetAlive()) {
					idx = i;
				}
			}
			if (idx != -1) {
				mCoins.erase(mCoins.begin() + idx);
			}
		}
	}
}

void GameScreenLevel2::CreateKoopa(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* _tmpKoopa = new CharacterKoopa(mRenderer, mLevelMap, position, direction, speed);
	mEnemies.push_back(_tmpKoopa);
}

void GameScreenLevel2::CreateCoin(Vector2D position) {
	Coin* _tmpCoin = new Coin(mRenderer, position, mLevelMap);
	mCoins.push_back(_tmpCoin);
}