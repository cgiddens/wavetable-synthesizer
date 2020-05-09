
#ifndef ENVELOPE_H__ // Multiple inclusion guard
#define ENVELOPE_H__

typedef struct {
    float delay;
    float attack;
    float decay;
    float sustain;
    float sustain_level;
    float release;
    float phasor;
} Envelope;

float GetEnvelopeGain(Envelope *envelope, float T);

#endif // ENVELOPE_H__