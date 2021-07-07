#include "SoundEffect.h"

SoundEffect::SoundEffect() {
	mMusic = nullptr;
}

SoundEffect::~SoundEffect() {
	Free();
}

bool SoundEffect::Load(std::string path) {
	int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
	if (Mix_Init(flags) & flags != flags) {
		std::cout << "Mixer could not initialize! Error: " << Mix_GetError() << std::endl;
	}
	mMusic = Mix_LoadMUS(path.c_str());
	if (!mMusic) {
		std::cout << "Failed to load audio! Error: " << Mix_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 2048) < 0) {
		std::cout << "Mixer could not initialize! Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void SoundEffect::Play(bool loop) {
	Mix_VolumeMusic(50);
	if (Mix_PlayingMusic() == 0) {
		switch (loop) {
			case true:
				Mix_PlayMusic(mMusic, -1);
				break;
			case false:
				Mix_PlayMusic(mMusic, 1);
				break;
		}
	}
}

void SoundEffect::Free() {
	if (!mMusic) {
		Mix_FreeMusic(mMusic);
		mMusic = nullptr;
	}
}