#include "Sound.h"
#include "Logger.h"

#include <SDL.h>
#include <SDL_mixer.h>

// Constructor
SoundManager::SoundManager() { }

void SoundManager::playSoundEffect(std::string identifier, int channel) {
	if (myEffects.find(identifier) == myEffects.end()) {
		LOG_ERROR("Requested to play a sound effect that doesn't exist: ", identifier);
		return;
	}
	Mix_PlayChannel(channel, myEffects[identifier], 0);
}

// Get the sound effect volume for a given channel, or -1 for the average of the channels
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
void SoundManager::unloadSoundEffect(std::string identifier) {
	if (myEffects[identifier] != 0) { Mix_FreeChunk(myEffects[identifier]); }
	myEffects.erase(identifier);
}

// Unload all sound effects, properly freeing the memory and clearing the myEffects map
void SoundManager::unloadAllSoundEffects() {
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	myEffects.clear();
}