#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>

using namespace std;

Texture2D::Texture2D(SDL_Renderer* renderer) {
	mRenderer = renderer;
	mTexture = NULL;
	int mWidth = 0;
	int mHeight = 0;
};

Texture2D::~Texture2D() {
	Free();
	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(string path) {
	Free();
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != nullptr) {
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));
		mWidth = pSurface->w;
		mHeight = pSurface->h;	
		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mTexture == nullptr) {
			cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(pSurface);	
		return mTexture != nullptr;
	} else {
		cout << "Unable to create texture from surface. Error: " << IMG_GetError() << endl;
	}
};

void Texture2D::Free() {
	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
		SDL_Texture* mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle) {
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);
	SDL_Rect renderLocation = { 0, 0, mWidth, mHeight };
	SDL_RenderCopyEx(mRenderer, mTexture, nullptr, &renderLocation, angle, nullptr, flip);
	SDL_RenderPresent(mRenderer);
}