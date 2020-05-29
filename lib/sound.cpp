#include "sound.h"

namespace MyAscii {

    void Sound::start() {
        PlaySound(TEXT("./sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    void Sound::stop() {
        PlaySound(NULL, 0, 0);
    }

}