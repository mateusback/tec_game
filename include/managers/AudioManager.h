#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <unordered_map>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <string_view>

namespace Manager {

    class AudioManager {
    private:
        //trocar para map de mylib
        std::unordered_map<std::string, Mix_Chunk*> soundEffects;
        std::unordered_map<std::string, Mix_Music*> musics;

    public:
        AudioManager();
        ~AudioManager();

        bool loadSoundEffect(const std::string_view id, const std::string& filePath);
        bool loadMusic(const std::string_view id, const std::string& filePath);
        bool loadMusicIfNotLoaded(const std::string_view id, const std::string& filePath);

        void playSoundEffect(const std::string_view id, int loops = 0);
        void playMusic(const std::string_view id, int loops = -1);
        void playMusicIfNotPlaying(const std::string_view id, int loops = -1);

        void stopMusic();
        void stopAllSounds();

        void setMusicVolume(int volume);
        void setSoundEffectsVolume(int volume);

        void clear();
    };

}

#endif
