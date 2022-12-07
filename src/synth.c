
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
    voice.table       = &triangle_table;
    voice.freq        = 900;
    voice.phasor      = 0; // Phase accumulator / initial phase
    voice.gain        = 0.8;
    voice.next_entry  = NULL; // For linked mixer list, init to NULL

    const float td    = 0.08; // pulse duration -- 80ms
    const float tr    = 0.01; // attack time -- 10ms
    const float tf    = 0.03; // decay time -- 30ms
    const float x     = 0.05; // shortest interburst interval -- 50ms
    const float s     = td - tr - tf; // sustain -- 40ms

    Voice voice2      = voice;
    Voice voice3      = voice;
    Voice voice4      = voice;
    Voice voice5      = voice;
    Voice voice6      = voice;
    Voice voice7      = voice;
    Voice voice8      = voice;
    Voice voice9      = voice;
    Voice voice10     = voice;

    // Create envelopes
    Envelope envelope;
    envelope.delay = 0;
    envelope.attack = tr;
    envelope.decay = 0;
    envelope.sustain = s;
    envelope.sustain_level = 1;
    envelope.release = tf;

    Envelope envelope2 = envelope;
    envelope2.delay = td + x;
    Envelope envelope3 = envelope;
    envelope3.delay = envelope2.delay + td + x;
    Envelope envelope4 = envelope;
    envelope4.delay = envelope3.delay + td + (2*x) + td;
    Envelope envelope5 = envelope;
    envelope5.delay = envelope4.delay + td + x;
    Envelope envelope6 = envelope;
    envelope6.delay = envelope5.delay + td + 0.55;
    Envelope envelope7 = envelope;
    envelope7.delay = envelope6.delay + td + x;
    Envelope envelope8 = envelope;
    envelope8.delay = envelope7.delay + td + x;
    Envelope envelope9 = envelope;
    envelope9.delay = envelope8.delay + td + (2*x) + td;
    Envelope envelope10 = envelope;
    envelope10.delay = envelope9.delay + td + x;

    // Create LFO
    // LFO lfo; // no LFO needed
    // lfo.table = &sine_table;
    // lfo.envelope = NULL; // Not implemented yet
    // lfo.assignment = FREQ;
    // lfo.freq = 2;
    // lfo.phasor = 0;
    // lfo.gain = 0.001; // NOTE: this is already an aggressive pitch gain. Need to figure out scaling

    // Add envelopes to voices
    AddEnvelope(&voice, &envelope);
    AddEnvelope(&voice2, &envelope2);
    AddEnvelope(&voice3, &envelope3);
    AddEnvelope(&voice4, &envelope4);
    AddEnvelope(&voice5, &envelope5);
    AddEnvelope(&voice6, &envelope6);
    AddEnvelope(&voice7, &envelope7);
    AddEnvelope(&voice8, &envelope8);
    AddEnvelope(&voice9, &envelope9);
    AddEnvelope(&voice10, &envelope10);

    // Add lfo to voices
    // AddLfo(&voice, &lfo); // no LFO needed

    // Initialize mixer
    Mixer mixer;
    InitializeMixer(&mixer, &voice);

    // Add additional voice to mixer
    AddVoice(&mixer, &voice2);
    AddVoice(&mixer, &voice3);
    AddVoice(&mixer, &voice4);
    AddVoice(&mixer, &voice5);
    AddVoice(&mixer, &voice6);
    AddVoice(&mixer, &voice7);
    AddVoice(&mixer, &voice8);
    AddVoice(&mixer, &voice9);
    AddVoice(&mixer, &voice10);

    // Render audio
    GenerateSamples(&mixer, 48000, 3 * 48000); // Generate a 3-second file

    return 0;
}
