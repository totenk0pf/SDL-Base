#pragma once

#ifndef _SOUND_EFFECT_H
#define _SOUND_EFFECT_H

#include <SDL_mixer.h>
#include <string>
#include <iostream>

class SoundEffect {
public:
	SoundEffect();
	~SoundEffect();

	bool Load(std::string path);
	void Play(bool loop);
	void Free();

private:
	Mix_Music* mMusic;

protected:
};

#endif // !_SOUND_EFFECT_H
