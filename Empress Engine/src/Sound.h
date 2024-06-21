#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

#include <map>
#include <string>

class SoundManager {
public:
	SoundManager();
	void playSoundEffect(std::string identifier, int channel);
	int getSoundEffectVolume(int channel);
	void setSoundEffectVolume(int channel, int volume);
	int loadSoundEffect(std::string identifier, std::string filepath);
	void unloadSoundEffect(std::string identifier);
	void unloadAllSoundEffects();
private:
	std::map<std::string, Mix_Chunk*> myEffects;

};

void loadSound();
void playSound();

#endif // SOUND_H