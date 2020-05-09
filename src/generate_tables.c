#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define sample_rate 48000
#define num_samples 255
#define PI 3.14159265

void GenerateTriangleWave(int length, size_t sample_size) {

    // Calculate table parameters
    int16_t data[length];
    size_t table_size = sizeof(data) / sample_size;
    
    // Generate rising edge of triangle wave
    for (int i = 0; i <= table_size; i++) {
        data[i] = (((float)i) / table_size) * __INT16_MAX__;
    }

    // Prevent integer overflow in loop (prevent compiler complaints during optimization)
    //table->data[table->length] = __INT16_MAX__;

    // Write wavetable to file
    FILE *fp;
    fp = fopen("./data/triangle.raw", "w");
    fwrite(data, (2 * table_size) + 2, 1, fp); // Why do I have to add 2???
    fclose(fp);

}

void GenerateSineWave(int length, size_t sample_size) {

    // Calculate table parameters
    int16_t data[length];
    size_t table_size = sizeof(data) / sample_size;

    // Compute table
    double pi = PI;
    double table_rad = pi / 2;
    double dx = table_rad / (length + 1); // Need 256 for math, 255 for array ops
    double x[length];
    for (int i = 0; i <= length; i++) {
        x[i] = sin(i * dx);
        data[i] = (int16_t)(x[i] * __INT16_MAX__);
        //printf("%.2f %d\n", i * dx, data[i]);
    }

    // Write wavetable to file
    FILE *fp;
    fp = fopen("./data/sine.raw", "w");
    fwrite(data, (2 * table_size) + 2, 1, fp); // Why do I have to add 2???
    fclose(fp);

}

int main() {

    // Calculate table parameters
    int fs = sample_rate;

    GenerateTriangleWave(num_samples, sizeof(int16_t));
    GenerateSineWave(num_samples, sizeof(int16_t));
}
