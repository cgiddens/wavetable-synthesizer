
#ifndef WAVETABLE_H__ // Multiple inclusion guard
#define WAVETABLE_H__

#include <stdint.h>
#include <stdio.h>

#define table_size 255
#define sample_rate 48000

enum Waveform {
    SINE,
    TRIANGLE
};

typedef struct {
    int16_t data[table_size];
    int length;
    float freq;
    enum Waveform waveform;
} Wavetable;

void GenerateWavetable(Wavetable *table);
void LoadWavetable(Wavetable *table, enum Waveform waveform);
int16_t TraverseWavetable(Wavetable *table, int index);
float GetWavetableOutput(Wavetable *table, float phase);

#endif // WAVETABLE_H__