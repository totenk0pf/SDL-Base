#pragma once

#ifndef _COMMONS_H
#define _COMMONS_H

struct Vector2D {
	float x;
	float y;
	Vector2D() {
		x = 0.0f;
		y = 0.0f;
	};
	Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	};
};

struct Rect2D {
	float x;
	float y;
	float w;
	float h;
	Rect2D() {
		x = 0.0f;
		y = 0.0f;
		w = 0.0f;
		h = 0.0f;
	};

	Rect2D(float x, float y, float w, float h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	};
};

struct Circle2D {
	float x;
	float y;
	float r;
	Circle2D() {
		x = 0.0f;
		y = 0.0f;
		r = 0.0f;
	}
	Circle2D(float x, float y, float r) {
		this->x = x;
		this->y = y;
		this->r = r;
	}
};

enum SCREENS {
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING {
	FACING_LEFT = 0,
	FACING_RIGHT
};

enum GAMESTATE {
	EXIT_STATE = 0,
	INTRO_STATE,
	GAME_STATE,
	LOSE_STATE
};

#endif // !_COMMONS_H