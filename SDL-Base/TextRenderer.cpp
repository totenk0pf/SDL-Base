#include "TextRenderer.h"

TextRenderer::TextRenderer(int fontSize) {
	font = TTF_OpenFont("smb.ttf", fontSize);
	color = {255,255,255,255};
}

TextRenderer::~TextRenderer() {
	text = nullptr;
}

void TextRenderer::Render(SDL_Renderer* renderer, const char* message, int x, int y) {
	if (text) {
		SDL_DestroyTexture(text);
	}
	textSurface = TTF_RenderText_Solid(font, message, color);
	text = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	textRect.x = x;
	textRect.y = y;
	SDL_QueryTexture(text, nullptr, nullptr, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, text, nullptr, &textRect);
}