#include "Character.h"
#include "Constants.h"
#include "Texture2D.h"
#include "Collisions.h"
#include "LevelMaps.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map) {
	mRenderer = renderer;
	mTexture = new Texture2D(mRenderer);
	mTexture->LoadFromFile(imagePath);
	SetPosition(startPosition);
	mAlive = true;
	mCanJump = false;
	mJumping = false;
	mCanMoveLeft = true;
	mCanMoveRight = true;
	mGrounded = true;
	mCollisionRadius = 15.0f;
	mLevelMap = map;
	int mScore = 0;
	int mCoins = 0;
}

Character::~Character() {
	mRenderer = nullptr;
}

void Character::Update(float deltaTime, const Uint8* keyState) {
	std::vector<int> colMatrix = GetCollisionMatrix();
	if (colMatrix[6] || colMatrix[7] || colMatrix[8] && !IsJumping()) {
		SetFalling(false);
		SetCanJump(true);
	} else {
		SetFalling(true);
	}
	if (IsJumping()) {
		if (!colMatrix[1] && (!colMatrix[0] && !colMatrix[2])) {
			mPosition.y -= mJumpForce * deltaTime;
			mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
			if (mAlive) {
				if (colMatrix[6] || colMatrix[7] || colMatrix[8]) {
					CancelJump();
					mFalling = false;
					mJumping = false;
					mCanJump = true;
				}
			}
		} else {
			if (!colMatrix[1] && (!colMatrix[0] || !colMatrix[2])) {
				mPosition.y -= mJumpForce * deltaTime;
				mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
			}
			if (colMatrix[6] || colMatrix[7] || colMatrix[8]) {
				if (colMatrix[2] || colMatrix[5]) {
					SetCanMoveRight(false);
				} else {
					SetCanMoveRight(true);
				}
				if (colMatrix[0] || colMatrix[3]) {
					SetCanMoveLeft(false);
				} else {
					SetCanMoveLeft(true);
				}

				SetFalling(true);
			} else if (!colMatrix[6] && !colMatrix[7] && !colMatrix[8]) {
				CancelJump();
				SetFalling(true);
			}
		}
	}
	if (!IsJumping() || !IsFalling()) {
		if (colMatrix[2] || colMatrix[5]) {
			SetCanMoveRight(false);
		} else {
			SetCanMoveRight(true);
		}
		if (colMatrix[0] || colMatrix[3]) {
			SetCanMoveLeft(false);
		} else {
			SetCanMoveLeft(true);
		}
	}
	Rect2D _tmpCol = GetCollisionBox();
	if (_tmpCol.x <= 0) {
		SetCanMoveLeft(false);
	}
	if (_tmpCol.x + _tmpCol.w > SCREEN_WIDTH) {
		SetCanMoveRight(false);
	}
	if (IsFalling()) {
		if (colMatrix[6] || colMatrix[7] || colMatrix[8]) {
			SetCanJump(true);
			SetFalling(false);
		} else {
			SetCanJump(false);
			AddGravity(deltaTime);
		}
	}
}

void Character::CancelJump() {
	mJumpForce = 0;
}

void Character::MoveLeft(float deltaTime) {
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MOVEMENT_SPEED * deltaTime;
}

void Character::MoveRight(float deltaTime) {
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MOVEMENT_SPEED * deltaTime;
}

void Character::Render() {
	if (mAlive) {
		if (mFacingDirection == FACING_RIGHT) {
			mTexture->Render(mPosition);
		} else {
			mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
		}
	}
}

void Character::Jump(float deltaTime) {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::Die(float deltaTime) {
	Jump(deltaTime);
	mCurrentFrame = 7;
	mInitFrame = 7;
	PlayAnim(deltaTime, 0, 0.1f);
	AddGravity(deltaTime);
}

void Character::AddGravity(float deltaTime) {
	mPosition.y += (GRAV_FORCE * GRAV_FORCE) * 2 * deltaTime;
}

std::vector<int> Character::GetCollisionMatrix(int wCount, int hCount) {
	std::vector<std::pair<float, float>> matrixPos{
		{-0.1f,0},{0.5f,0},{1.1f,0},
		{-0.1f,0.5f},{0.5f,0.5f},{1.1f,0.5f},
		{-0.1f,1.02f},{0.5f,1.02f},{1.1f,1.02f}
	};
	Rect2D col = GetCollisionBox();
	std::vector<int> collisionMatrix;
	for (Uint8 i = 0; i < matrixPos.size(); i++) {
		int col = mLevelMap->GetTileAt((int)(GetPosition().x + ((GetCollisionBox().w / wCount) * matrixPos[i].first)) / TILE_WIDTH, (int)(GetPosition().y + ((GetCollisionBox().h / hCount) * matrixPos[i].second)) / TILE_HEIGHT);
		collisionMatrix.push_back(col);
	}
	return collisionMatrix;
}

Rect2D Character::GetCollisionBox() {
	return Rect2D(mPosition.x, mPosition.y, mFrameW, mFrameH);
}

void Character::PlayAnim(float deltaTime, int frameCount, float animDelay) {
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f) {
		mFrameDelay = animDelay;
		mCurrentFrame++;
		if (mCurrentFrame > mInitFrame + frameCount) {
			mCurrentFrame = mInitFrame;
		}
	}
}
