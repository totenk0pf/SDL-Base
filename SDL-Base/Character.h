#pragma once

#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include <vector>

class LevelMap;
class Texture2D;
class Character {
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, const Uint8* keyState);

	void SetPosition(Vector2D newPosition) { mPosition = newPosition; };
	float GetCollisionRadius() { return mCollisionRadius; };
	Vector2D GetPosition() { return mPosition; };
	Rect2D GetCollisionBox();
	FACING GetDirection() { return mFacingDirection; };
	bool GetAlive() { return mAlive; };
	bool IsJumping() { return mJumping; };
	bool IsFalling() { return mFalling; };
	bool IsGrounded() { return mGrounded; };
	void CancelJump();
	int GetScore() { return mScore; };
	int GetCoins() { return mCoins; };
	int GetCurrentFrame() { return mCurrentFrame; };

	void SetCanMoveLeft(bool state) { mCanMoveLeft = state; };
	void SetCanMoveRight(bool state) { mCanMoveRight = state; };
	void SetFalling(bool state) { mFalling = state; };
	void SetCanJump(bool state) { mCanJump = state; };
	void SetJumping(bool state) { mJumping = state; };
	void SetAlive(bool state) { mAlive = state; };
	void SetGrounded(bool state) { mGrounded = state; };
	void SetScore(int newScore) { mScore = newScore; };
	void SetCoins(int newCoins) { mCoins = newCoins; };
	void Die(float deltaTime);

	bool canMoveLeft() { return mCanMoveLeft; };
	bool canMoveRight() { return mCanMoveRight; };
	bool canJump() { return mCanJump; };

	int tileL;
	int tileM;
	int tileR;

	std::vector<int> GetCollisionMatrix(int wCount=1, int hCount=1);
	std::string mName;

private:
	FACING mFacingDirection;
	bool mMovingLeft;
	bool mMovingRight;
	LevelMap* mLevelMap;

protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	int mScore;
	int mCoins;
	int mInitFrame;
	int mCurrentFrame;
	bool mAlive;
	bool mJumping;
	bool mCanJump;
	bool mCanMoveLeft;
	bool mCanMoveRight;
	bool mFalling;
	bool mGrounded;
	float mJumpForce;
	float mCollisionRadius;
	float mAcceleration;

	float mFrameW;
	float mFrameH;
	float mFrameDelay;

	void AddGravity(float deltaTime);
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void Jump(float deltaTime);
	virtual void PlayAnim(float deltaTime, int frameCount, float animDelay);
};


#endif // !_CHARACTER_H