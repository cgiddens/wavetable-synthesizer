#ifndef MIXER_H__ // Multiple inclusion guard
#define MIXER_H__

#include "voice.h"

typedef struct Mixer {
    Voice *first_entry;
    int voice_count;
} Mixer;

void InitializeMixer(Mixer *mixer, Voice *voice);
void AddVoice(Mixer *mixer, Voice *voice);
void Mix(Mixer *mixer, FILE *fp, int fs, int num_samples);

#endif // MIXER_H__