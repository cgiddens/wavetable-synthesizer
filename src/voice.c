#include "voice.h"
#include "envelope.h"

void AddEnvelope(Voice *voice, Envelope *envelope) {
    voice->envelope = envelope;
}

void AddLfo(Voice *voice, LFO *lfo) {
    voice->lfo = lfo;
}