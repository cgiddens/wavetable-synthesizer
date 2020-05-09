#include "filter.h"

static int filter_taps[FILTER_TAP_NUM] = {
  5005,
  5005
};

void Filter_init(Filter* f) {
  int i;
  for(i = 0; i < FILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;

  // HACK: add gain to account for ~10dB passband attenuation
  f->gain = 2 * (1 / 0.31622776601683794);
}

void Filter_put(Filter* f, int input) {
  f->history[(f->last_index++) & 1] = input;
}

int Filter_get(Filter* f) {
  long long acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < FILTER_TAP_NUM; ++i) {
    acc += (long long)f->history[(index--) & 1] * filter_taps[i];
  };

  // HACK: add gain to account for ~10dB passband attenuation
  return ((int)(acc * f->gain)) >> 16;
}