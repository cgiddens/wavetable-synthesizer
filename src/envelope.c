#include "envelope.h"

float GetEnvelopeGain(Envelope *envelope, float T) {

    if (T < envelope->delay) { // Envelope has not started yet
        return 0;
    } else if (T < (envelope->delay + envelope->attack)) { // In attack phase
        T -= envelope->delay;
        return (T / envelope->attack);
    } else if (T < (envelope->delay + envelope->attack + envelope->decay)) { // In decay phase
        T -= (envelope->delay + envelope->attack);
        return 1 - ((1 - envelope->sustain_level) * (T / envelope->decay));
    } else if (T < (envelope->delay + envelope->attack + envelope->decay + envelope->sustain)) { // In sustain phase
        return envelope->sustain_level;
    } else if (T < (envelope->delay + envelope->attack + envelope->decay + envelope->sustain + envelope->release)) { // In release phase
        T -= (envelope->delay + envelope->attack + envelope->decay + envelope->sustain);
        return envelope->sustain_level - (envelope->sustain_level * (T / envelope->release));
    } else return 0; // Envelope has ended
    
}