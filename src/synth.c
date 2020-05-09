
#include "wavetable.h"
#include "voice.h"
#include "envelope.h"
#include "mixer.h"

#define DEBUG

#ifndef DEBUG
#define printf
#endif //DEBUG

void GenerateSamples(Mixer *mixer, int fs, int num_samples) {

    // Create output file
    FILE *fp;
    fp = fopen("./output.raw", "w");

    // Mix samples
    Mix(mixer, fp, fs, num_samples);

    fclose(fp);
}

int main() {
    
    // Create wavetables
    Wavetable sine_table;
    Wavetable triangle_table;
    LoadWavetable(&sine_table, SINE);
    LoadWavetable(&triangle_table, TRIANGLE);

    // Create voices
    Voice voice;
    voice.table = &triangle_table;
    voice.freq = 220;
    voice.phasor = 0; // Phase accumulator / initial phase
    voice.gain = 0.8;
    voice.next_entry = NULL; // For linked mixer list, init to NULL

    Voice voice2;
    voice2.table = &triangle_table;
    voice2.freq = 440;
    voice2.phasor = 0;
    voice2.gain = 0.8;
    voice2.next_entry = NULL;

    Voice voice3;
    voice3.table = &triangle_table;
    voice3.freq = 659.255;
    voice3.phasor = 0;
    voice3.gain = 0.8;
    voice3.next_entry = NULL;

    // Create envelopes
    Envelope envelope;
    envelope.delay = 0;
    envelope.attack = .096458;
    envelope.decay = .041333;
    envelope.sustain = 0;
    envelope.sustain_level = .2;
    envelope.release = .011813;

    Envelope envelope2;
    envelope2.delay = 0.1496;
    envelope2.attack = .025583;
    envelope2.decay = .025000;
    envelope2.sustain = .013771;
    envelope2.sustain_level = 0.10;
    envelope2.release = .031500;

    Envelope envelope3;
    envelope3.delay = 0.2455;
    envelope3.attack = .003938;
    envelope3.decay = .070854;
    envelope3.sustain = 1.033458;
    envelope3.sustain_level = 0.05;
    envelope3.release = .149604;

    // Create LFO
    LFO lfo;
    lfo.table = &sine_table;
    lfo.envelope = NULL; // Not implemented yet
    lfo.assignment = FREQ;
    lfo.freq = 2;
    lfo.phasor = 0;
    lfo.gain = 0.001; // NOTE: this is already an aggressive pitch gain. Need to figure out scaling

    // Add envelopes to voices
    AddEnvelope(&voice, &envelope);
    AddEnvelope(&voice2, &envelope2);
    AddEnvelope(&voice3, &envelope3);

    // Add lfo to voices
    AddLfo(&voice, &lfo);
    AddLfo(&voice2, &lfo);
    AddLfo(&voice3, &lfo);

    // Initialize mixer
    Mixer mixer;
    InitializeMixer(&mixer, &voice);

    // Add additional voice to mixer
    AddVoice(&mixer, &voice2);
    AddVoice(&mixer, &voice3);

    // Render audio
    GenerateSamples(&mixer, 48000, sine_table.length * 4 * 256); // Generate 256 cycles at table base freq (47Hz)

    return 0;
}