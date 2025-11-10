// Audio facade (null backend initially)
#pragma once

#include <Base/UAE/Types.h>

namespace Audio {

enum class SoundMode { Off, PC, AdLib };
enum class MusicMode { Off, AdLib };

class Device {
public:
    static Device& Default();

    void Startup();
    void Shutdown();

    void SetSoundMode(SoundMode) {}
    void SetMusicMode(MusicMode) {}

    void PlaySound(int /*soundId*/) {}
    void StopSound() {}
    bool MusicPlaying() const { return false; }
};

}

// For compatibility with the original code structure
typedef int16_t soundnames;

