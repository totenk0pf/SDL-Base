#include "TextRenderer.h"
#include <iomanip>
#include <sstream>

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

void TextRenderer::Render(SDL_Renderer* renderer, const int score, int padding, int x, int y) {
	if (text) {
		SDL_DestroyTexture(text);
	}
	std::ostringstream ss;
	ss << std::setw(padding) << std::setfill('0') << score;
	std::string str = ss.str();
	textSurface = TTF_RenderText_Solid(font, str.c_str(), color);
	text = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	textRect.x = x;
	textRect.y = y;
	SDL_QueryTexture(text, nullptr, nullptr, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, text, nullptr, &textRect);
}