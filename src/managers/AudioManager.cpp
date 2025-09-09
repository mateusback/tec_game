#include "managers/AudioManager.h"
#include <iostream>

namespace Manager {

    AudioManager::AudioManager() {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    }

    AudioManager::~AudioManager() {
        clear();
        Mix_CloseAudio();
    }

    bool AudioManager::loadSoundEffect(const std::string_view id, const std::string& filePath) {
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk) {
            std::cerr << "Erro ao carregar efeito sonoro: " << Mix_GetError() << "\n";
            return false;
        }
        soundEffects[std::string(id)] = chunk;
        return true;
    }

    bool AudioManager::loadMusic(const std::string_view id, const std::string& filePath) {
        Mix_Music* music = Mix_LoadMUS(filePath.c_str());
        if (!music) {
            std::cerr << "Erro ao carregar música: " << Mix_GetError() << "\n";
            return false;
        }
        musics[std::string(id)] = music;
        return true;
    }

    bool AudioManager::loadMusicIfNotLoaded(const std::string_view id, const std::string& filePath) {
        if (musics.contains(std::string(id))) return true;
        return loadMusic(id, filePath);
    }

    void AudioManager::playSoundEffect(const std::string_view id, int loops) {
        auto it = soundEffects.find(std::string(id));
        if (it != soundEffects.end()) {
            Mix_PlayChannel(-1, it->second, loops);
        }
    }

    void AudioManager::playMusic(const std::string_view id, int loops) {
        auto it = musics.find(std::string(id));
        if (it != musics.end()) {
            Mix_PlayMusic(it->second, loops);
        }
    }

    void AudioManager::playMusicIfNotPlaying(const std::string_view id, int loops) {
        if (Mix_PlayingMusic()) return;

        auto it = musics.find(std::string(id));
        if (it != musics.end()) {
            Mix_PlayMusic(it->second, loops);
        }
    }

    void AudioManager::stopMusic() {
        Mix_HaltMusic();
    }

    void AudioManager::stopAllSounds() {
        Mix_HaltChannel(-1);
    }

    void AudioManager::setMusicVolume(int volume) {
        Mix_VolumeMusic(volume);
    }

    void AudioManager::setSoundEffectsVolume(int volume) {
        Mix_Volume(-1, volume);
    }

    void AudioManager::clear() {
        for (auto& [id, chunk] : soundEffects)
            Mix_FreeChunk(chunk);
        soundEffects.clear();

        for (auto& [id, music] : musics)
            Mix_FreeMusic(music);
        musics.clear();
    }

}
