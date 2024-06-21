#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

#include <map>
#include <string>

class SoundManager {
public:
	SoundManager();
	~SoundManager();

	// Sound Effects
	int playSoundEffect(std::string identifier, int channel);
	int getSoundEffectVolume(int channel);
	void setSoundEffectVolume(int channel, int volume);
	int loadSoundEffect(std::string identifier, std::string filepath);
	int unloadSoundEffect(std::string identifier);
	void unloadAllSoundEffects();

	// Music
	void playMusic(std::string identifier, bool loop = true);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();
	int getMusicVolume();
	void setMusicVolume(int volume);
	int loadMusic(std::string identifier, std::string filepath);
	int unloadMusic(std::string identifier);
	void unloadAllMusic();

private:
	std::map<std::string, Mix_Chunk*> myEffects;
	std::map<std::string, Mix_Music*> myMusics;
};

#endif // SOUND_H