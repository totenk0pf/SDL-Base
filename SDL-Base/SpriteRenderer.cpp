#include "SpriteRenderer.h"

SpriteRenderer* SpriteRenderer::mSpriteRenderer = nullptr;

SpriteRenderer* SpriteRenderer::Instance(SDL_Renderer* renderer) {
	if (!mSpriteRenderer) {
		mRenderer = renderer;
		mSpriteRenderer = new SpriteRenderer();
		mTexture = new Texture2D(renderer);
	}
	return mSpriteRenderer;
};

SpriteRenderer::~SpriteRenderer() {
	mSpriteRenderer = nullptr;
}

bool SpriteRenderer::LoadFromFile(std::string path) {
	if (mTexture->LoadFromFile(path.c_str())) {
		return true;
	}
	return false;
}

void SpriteRenderer::Render(Vector2D position, SDL_RendererFlip flip, double angle) {
	mTexture->Render(position, flip, angle);
}