#ifndef AUDIO_HDR
#define AUDIO_HDR

#include "vender/miniaudio.h"

#include "Foundation/Array.hpp"


void init();
void play();
void selectAudioDevice();
void loadAudio();
void shutdown();


#endif // !AUDIO_HDR
