#include "Character.h"
#include "Texture2D.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition) {
	mRenderer = renderer;
	mTexture->LoadFromFile(imagePath);
}

void Character::Render() {
	mTexture->Render(mPosition);
}

void Character::Update(float deltaTime, SDL_Event e) {
	switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_LEFT | SDLK_a:
					mPosition.x -= 1;
					break;
				case SDLK_RIGHT | SDLK_d:
					mPosition.x += 1;
					break;
			}
		break;
	}
}

void Character::SetPosition(Vector2D newPosition) {
	mPosition = newPosition;
}

Vector2D Character::GetPosition() {
	return mPosition;
}

