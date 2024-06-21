#include "Sound.h"
#include "Logger.h"

#include <SDL.h>
#include <SDL_mixer.h>

// Constructor
SoundManager::SoundManager() { }

// Destructor
SoundManager::~SoundManager() {
	unloadAllMusic();
	unloadAllSoundEffects();
}

// Play a sound effect on a given channel. If channel is -1, it plays it on the first available channel. Returns the channel played on
int SoundManager::playSoundEffect(std::string identifier, int channel) {
	if (myEffects.find(identifier) == myEffects.end()) {
		LOG_ERROR("Requested to play a sound effect that doesn't exist: ", identifier);
		return - 1;
	}
	return Mix_PlayChannel(channel, myEffects[identifier], 0);
}

// Get the sound effect volume for a given channel, or -1 for the average of the channels (range is 0 to 128)
int SoundManager::getSoundEffectVolume(int channel) {
	return Mix_Volume(channel, -1);
}

// Set the sound effect volume of a channel, or -1 for all channels. Volume should be in range 0 to 128
void SoundManager::setSoundEffectVolume(int channel, int volume) {
	Mix_Volume(channel, volume);
}

// Load a single sound effect from a file, and store it in the myEffects map by identifier
int SoundManager::loadSoundEffect(std::string identifier, std::string filepath)
{
	Mix_Chunk* loadedSound;
	loadedSound = Mix_LoadWAV(filepath.c_str());
	if (loadedSound == nullptr) {
		LOG_ERROR("Could not load sound effect: ", identifier);
		return 1;
	}
	if (myEffects[identifier] != 0) { unloadSoundEffect(identifier); }
	myEffects[identifier] = loadedSound;
	return 0;
}

// Unload a single sound effect by identifier, properly freeing its memory if the sound exists
int SoundManager::unloadSoundEffect(std::string identifier) {
	if (myEffects[identifier] == 0) {
		LOG_ERROR("Tried to free the memory of a sound effect that doesn't exist: ", identifier);
		myEffects.erase(identifier);
		return 1;
	}
	Mix_FreeChunk(myEffects[identifier]);
	myEffects.erase(identifier);
	return 0;
}

// Unload all sound effects, properly freeing the memory and clearing the myEffects map
void SoundManager::unloadAllSoundEffects() {
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	myEffects.clear();
}

// Get the music volume (range is 0 to 128)
int SoundManager::getMusicVolume() {
	return Mix_VolumeMusic(-1);
}

// Set the music volume. Should be in range 0 to 128
void SoundManager::setMusicVolume(int volume) {
	Mix_VolumeMusic(volume);
}

// Play a music track if it exists
void SoundManager::playMusic(std::string identifier, bool loop) {
	if (myMusics.find(identifier) == myMusics.end()) {
		LOG_ERROR("Requested to play a music track that doesn't exist: ", identifier);
		return;
	}
	int loops = loop ? -1 : 0;
	Mix_PlayMusic(myMusics[identifier], loops);
}

// Stop playing music
void SoundManager::stopMusic() { Mix_HaltMusic(); }

// Pause music
void SoundManager::pauseMusic() { Mix_PauseMusic(); }

// Resume music
void SoundManager::resumeMusic() { Mix_ResumeMusic(); }

// Load a single music track from a file, and store it in the myMusics map by identifier
int SoundManager::loadMusic(std::string identifier, std::string filepath)
{
	Mix_Music* loadedMusic;
	loadedMusic = Mix_LoadMUS(filepath.c_str());
	if (loadedMusic == nullptr) {
		LOG_ERROR("Could not load music track: ", identifier);
		return 1;
	}
	if (myMusics[identifier] != 0) { unloadMusic(identifier); }
	myMusics[identifier] = loadedMusic;
	return 0;
}

// Unload a single music track by identifier, properly freeing its memory if the track exists
int SoundManager::unloadMusic(std::string identifier) {
	if (myMusics[identifier] == 0) {
		LOG_ERROR("Tried to free the memory of music that doesn't exist: ", identifier);
		myMusics.erase(identifier);
		return 1;
	}
	Mix_FreeMusic(myMusics[identifier]); 
	myMusics.erase(identifier);
	return 0;
}

// Unload all music tracks, properly freeing the memory and clearing the myMusics map
void SoundManager::unloadAllMusic() {
	for (auto it = myMusics.begin(); it != myMusics.end(); ++it) {
		Mix_FreeMusic(it->second);
	}
	myMusics.clear();
}