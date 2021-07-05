#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>

Texture2D::Texture2D(SDL_Renderer* renderer) {
	mRenderer = renderer;
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
};

Texture2D::~Texture2D() {
	Free();
	mRenderer = nullptr;
}

bool Texture2D::LoadFromFile(std::string path) {
	Free();
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != nullptr) {
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));
		mWidth = pSurface->w;
		mHeight = pSurface->h;	
		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mTexture == nullptr) {
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(pSurface);	
		return mTexture != nullptr;
	} else { 
		std::cout << "Unable to create texture from surface. Error: " << IMG_GetError() << std::endl;
	}
};

void Texture2D::Free() {
	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle) {
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_Rect renderLocation = { newPosition.x, newPosition.y, mWidth, mHeight };
	SDL_RenderCopyEx(mRenderer, mTexture, nullptr, &renderLocation, angle, nullptr, flip);
}

void Texture2D::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle) {
	SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &destRect, angle, nullptr, flip); 
}