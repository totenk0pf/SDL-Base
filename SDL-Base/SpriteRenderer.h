#pragma once

#ifndef _SPRITE_RENDERER_H
#define _SPRITE_RENDERER_H

#include <SDL.h>
#include <string>
#include "Commons.h"
#include "Texture2D.h"

class SpriteRenderer {
public:
	SpriteRenderer(SpriteRenderer& other) = delete;
	void operator=(const SpriteRenderer&) = delete;
	static SpriteRenderer* Instance(SDL_Renderer* renderer);
	~SpriteRenderer();

	bool LoadFromFile(std::string path);
	void Render(Vector2D position, SDL_RendererFlip flip, double angle);

private:
	static SpriteRenderer* mSpriteRenderer;
	static Texture2D* mTexture;
	static SDL_Renderer* mRenderer;

protected:
	SpriteRenderer() {};
};

#endif // !_SPRITE_RENDERER_H