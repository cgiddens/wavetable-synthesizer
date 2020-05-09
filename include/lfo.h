
#ifndef LFO_H__ // Multiple inclusion guard
#define LFO_H__

#include "wavetable.h"
#include "envelope.h"

enum Assignment {
    FREQ,
    GAIN,
    FILTER
};

typedef struct LFO {
    Wavetable *table;
    Envelope *envelope; // Not implemented
    enum Assignment assignment;
    float freq;
    float phase_inc;
    float phasor;
    float gain;
} LFO;

//void AddEnvelope(Voice *voice, Envelope *envelope);

#endif // LFO_H__