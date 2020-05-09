#include "mixer.h"
#include "filter.h"

void InitializeMixer(Mixer *mixer, Voice *voice) {
    
    // Set up first node
    mixer->first_entry = voice;
    mixer->voice_count = 0;
    (mixer->first_entry)->next_entry = NULL;

}

void AddVoice(Mixer *mixer, Voice *voice) {
    
    Voice *temp = mixer->first_entry;

    // Iterate through list
    while (temp->next_entry != NULL) {
        temp = temp->next_entry;
    }

    // Add node
    temp->next_entry = voice;
    mixer->voice_count += 1;

}

void UpdateLfoPhase(LFO *lfo, float dt) {

    // Update phase accumulators
    lfo->phasor += lfo->phase_inc;
    //(lfo->envelope)->phasor += dt; // Not implemented
            
    // Wrap phase
    if (lfo->phasor > 1) {
        lfo->phasor -= 1;
    }
}

void UpdateVoicePhase(Voice *voice, float dt) {

    // Update phase accumulators
    voice->phasor += voice->phase_inc;
    (voice->envelope)->phasor += dt;

    // Wrap phase
    if (voice->phasor > 1) {
        voice->phasor -= 1;
    }
}

void Mix(Mixer *mixer, FILE *fp, int fs, int num_samples) {

    // Create mix bus lowpass
    Filter filter;
    Filter_init(&filter);

    // Get time step
    float dt = 1 / (float)fs;

    // Iterate through samples
    for (int current_sample = 0; current_sample < num_samples; current_sample++) {

        // Get current time
        float T = current_sample * dt;

        // Start at the first voice
        Voice *temp = mixer->first_entry;

        // Initialize mixed output sample
        int32_t mixed_sample = 0;
        int16_t clipped_mixed_sample = 0;

        // Core mixer loop
        for (int current_voice = 0; current_voice <= mixer->voice_count; current_voice++) {

            // Create freq accumulator to account for LFO modulation of voice freq
            float voice_freq = temp->freq;

            // If voice has freq LFO, adjust freq accordingly
            if (temp->lfo != NULL && (temp->lfo)->assignment == FREQ) {
                // Set LFO phase increment
                (temp->lfo)->phase_inc = dt * ((temp->lfo)->table)->freq * ((temp->lfo)->freq / ((temp->lfo)->table)->freq);

                // Get LFO wavetable output
                int16_t lfo_raw_out = GetWavetableOutput((temp->lfo)->table, (temp->lfo)->phasor);
                
                // Apply LFO gain, accumulate voice frequency
                float lfo_out = (temp->lfo)->gain * (float)lfo_raw_out;

                // Accumulate frequency
                voice_freq += lfo_out;

                // Clip frequency (freq<0 = segfault)
                if (voice_freq < 0) {
                    voice_freq = 0;
                }

                // Update phase
                UpdateLfoPhase(temp->lfo, dt);
            }

            // Set voice phase increment
            temp->phase_inc = dt * (temp->table)->freq * (voice_freq / (temp->table)->freq);

            // Get voice wavetable output
            int16_t voice_raw_out = GetWavetableOutput(temp->table, temp->phasor);

            // Get voice envelope gain
            float voice_env_gain = GetEnvelopeGain(temp->envelope, T);

            // Calculate voice output
            int16_t voice_output = (int16_t) (voice_env_gain * (float)voice_raw_out * temp->gain);

            // Mix
            mixed_sample += voice_output;

            // Clip
            if (mixed_sample > __INT16_MAX__) {
                mixed_sample = __INT16_MAX__;
            } else if (mixed_sample < (-1 * __INT16_MAX__)) {
                mixed_sample = -1 * __INT16_MAX__;
            }

            // Cast to 16 bit
            clipped_mixed_sample = (int16_t) mixed_sample;

            // Update phase
            UpdateVoicePhase(temp, dt);

            // If there is another voice, set node pointer accordingly
            if (temp->next_entry != NULL) {
                temp = temp->next_entry;
            }

        }

        // Filter
        Filter_put(&filter, clipped_mixed_sample);
        clipped_mixed_sample = Filter_get(&filter);

        // Write to file
        fwrite(&clipped_mixed_sample, sizeof(int16_t), 1, fp);

        // Reset node pointer
        temp = mixer->first_entry;
    
    }

}