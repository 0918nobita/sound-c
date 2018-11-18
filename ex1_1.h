#ifndef _EX1_1_
#define _EX1_1_

typedef struct {
  int fs;      // 標本化周波数
  int bits;    // 量子化精度
  int length;  // 音データの長さ
  double *s;   // 音データ
} MONO_PCM;

void wave_read_16bit_mono(MONO_PCM *pcm, char *file_name);

void wave_write_16bit_mono(MONO_PCM *pcm, char *file_name);

#endif
