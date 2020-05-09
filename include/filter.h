#ifndef FILTER_H_
#define FILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 48000 Hz

fixed point precision: 16 bits

* 0 Hz - 20000 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 22000 Hz - 24000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define FILTER_TAP_NUM 2
#define GAIN_MAG 0.31622776601683794

typedef struct {
  int history[FILTER_TAP_NUM];
  unsigned int last_index;
  float gain;
} Filter;

void Filter_init(Filter* f);
void Filter_put(Filter* f, int input);
int Filter_get(Filter* f);

#endif