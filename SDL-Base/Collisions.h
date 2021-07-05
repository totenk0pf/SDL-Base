#pragma once

#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include "Commons.h"
class Character;

class Collisions {
public:
	static Collisions* Instance();
	~Collisions();

	bool Circle(Circle2D circle1, Circle2D circle2);
	bool Box(Rect2D rect1, Rect2D rect2);

private:
	static Collisions* mInstance;

protected:

};

#endif !__COLLISIONS_H