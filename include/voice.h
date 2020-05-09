
#ifndef VOICE_H__ // Multiple inclusion guard
#define VOICE_H__

#include "wavetable.h"
#include "envelope.h"
#include "lfo.h"

typedef struct Voice {
    Wavetable *table;
    Envelope *envelope;
    LFO *lfo;
    float freq;
    float phase_inc;
    float phasor;
    float gain;
    struct Voice *next_entry;
} Voice;

void AddEnvelope(Voice *voice, Envelope *envelope);
void AddLfo(Voice *voice, LFO *lfo);

#endif // VOICE_H__