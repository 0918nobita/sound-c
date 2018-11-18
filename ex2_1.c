#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void) {
  MONO_PCM pcm;
  pcm.fs = 44100;  // 標本化周波数 44.1kHz
  pcm.bits = 16;   // 量子化精度 16 bit
  pcm.length = pcm.fs * 1;  // 1 秒間
  pcm.s = calloc(pcm.length, sizeof(double));

  double a = 0.1,  // 振幅
        f0 = 500.0;  // 周波数

  for (int n = 0; n < pcm.length; n++)
    pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);

  wave_write_16bit_mono(&pcm, "ex2_1.wav");

  free(pcm.s);

  return 0;
}
