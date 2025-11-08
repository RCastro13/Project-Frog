//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

typedef unsigned int SoundHandle;

struct HandleInfo {
    std::string mSoundName;
    int mChannel = -1;
    bool mIsLooping = false;
    bool mIsPaused = false;
};

class AudioSystem {
    public:
        AudioSystem(int numChannels = 8); // Criar um sistema de áudio com um dado número de trilhas
        ~AudioSystem(); // Destruir um sistema de som

        void Update(float deltaTime); // Atualiza estado dos sons ativos
        SoundHandle PlaySound(const std::string& soundName, bool looping = false);
        void PauseSound(SoundHandle sound); // Pausar um som se ele estiver tocando no momento
        void ResumeSound(SoundHandle sound); // Retomar um som se ele estiver pausado no momento
        void StopSound(SoundHandle sound); // Parar um som se ele estiver tocando no momento
        void StopAllSounds(); // Parar todos os sons em todas as trilhas
        void CacheSound(const std::string& soundName);
    private:
        struct Mix_Chunk* GetSound(const std::string& soundName);
        SoundHandle mLastHandle;
        std::vector<SoundHandle> mChannels;
        std::map<SoundHandle, HandleInfo> mHandleMap;
        std::unordered_map<std::string, Mix_Chunk*> mSounds;
};