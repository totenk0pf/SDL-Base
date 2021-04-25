#pragma once

#ifndef _COMMONS_H
#define _COMMONS_H

struct Vector2D {
	Vector2D() {
		x = 0.0f;
		y = 0.0f;
	};
	float x;
	float y;
};

struct InitVector2D {
	InitVector2D() {
		initx = 0.0f;
		inity = 0.0f;
	};
	float initx;
	float inity;
};

enum SCREENS {
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

#endif // !_COMMONS_H