#pragma once

#ifndef _DEBUG_H
#define _DEBUG_H

#include <SDL.h>
#include "Commons.h"
#include "Constants.h"

#include "imgui_sdl.h"
#include "imgui.h"
#include "Imgui/backends/imgui_impl_sdl.h"

class Character;
class Debug {
public:
	Debug(SDL_Renderer* renderer);
	~Debug();

	void ShowDebugWindow();
	void Update(SDL_Event* e);
	void Render();

	//std::string marioPos;
	//std::string luigiPos;

private:
	SDL_Renderer* mRenderer;

protected:

};

#endif !_DEBUG_H