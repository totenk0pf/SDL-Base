#pragma once

#ifndef _COMMONS_H
#define _COMMONS_H

struct Vector2D {
	Vector2D(float x=0.0f, float y=0.0f) {
	};
	float x;
	float y;
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