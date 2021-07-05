#include "Character.h"
#include "Collisions.h"

Collisions* Collisions::mInstance = nullptr;

Collisions::~Collisions() {
	mInstance = nullptr;
}

Collisions* Collisions::Instance() {
	if (!mInstance) {
		mInstance = new Collisions;
	}
	return mInstance;
}

bool Collisions::Circle(Circle2D circle1, Circle2D circle2) {
	Vector2D vec = Vector2D( 
		circle1.x - circle2.x,
		circle1.y - circle2.y
	);
	double dist = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double totalDist = circle1.r + circle2.r;
	return dist < totalDist;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2) {
	SDL_Rect rectA{rect1.x, rect1.y, rect1.w, rect1.h};
	SDL_Rect rectB{rect2.x, rect2.y, rect2.w, rect2.h};
	SDL_Rect resultRect;
	if (SDL_IntersectRect(&rectA, &rectB, &resultRect)) {
		return true;
	}
	return false;
}