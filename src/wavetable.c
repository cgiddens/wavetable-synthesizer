#include "wavetable.h"

void LoadWavetable(Wavetable *table, enum Waveform waveform) {

    // Get file pointer
    FILE *fp;

    // Select wavetable
    switch (waveform) {
        case SINE : 
            fp = fopen("./data/sine.raw", "r");
            break;
        case TRIANGLE : 
            fp = fopen("./data/triangle.raw", "r");
            break;
    }

    // Read data to memory
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, 0);
    size_t tmp = fread(table->data, size, 1, fp); // Use return val to avoid compiler complaints
    fclose(fp);

    // Set parameters
    table->length = sizeof(table->data) / sizeof(int16_t);
    int fs = sample_rate;
    table->freq = (float)fs / (4 * table->length);
}

/*void GenerateWavetable(Wavetable *table) {

    // Calculate table parameters
    int fs = sample_rate;
    table->length = sizeof(table->data) / sizeof(int16_t);
    table->freq = (float)fs / (4 * table->length);

    // Generate rising edge of triangle wave
    for (int i = 0; i <= table->length; i++) {
        table->data[i] = (((float)i) / table->length) * __INT16_MAX__;
        printf("%0x\n", table->data[i]);
    }

    // Prevent integer overflow in loop (prevent compiler complaints during optimization)
    //table->data[table->length] = __INT16_MAX__;

    // Write wavetable to file
    FILE *fp;
    fp = fopen("./triangle.raw", "w");
    fwrite(table->data, sizeof(table->data) + 2, 1, fp); // Why do I have to write sizeof()+2???
    fclose(fp);

} */

int16_t TraverseWavetable(Wavetable *table, int index) {

    // Wrap through quarter-cycle to emulate full cycle
    if (index <= table->length) {
        return table->data[index];
    } else if (index <= 2 * table->length) {
        index -= table->length;
        return table->data[table->length - index];
    } else if (index <= 3 * table->length) {
        index -= 2 * table->length;
        return -1 * table->data[index];
    } else if (index <= 4 * table->length) {
        index -= 3 * table->length;
        return -1 * table->data[table->length - index];
    }

}

float GetWavetableOutput(Wavetable *table, float phase) {

    // Linear interpolation
    float index = phase * (4 * table->length);
    int index1 = (int)index; // Truncate to get prev sample index
    int index2 = index1 + 1;
    float di = index - index1;

    // Wrap through wavetable
    int16_t sample1 = TraverseWavetable(table, index1);
    int16_t sample2 = TraverseWavetable(table, index2);

    return sample1 + (di * (sample2 - sample1));

}