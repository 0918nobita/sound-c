#include <stdio.h>
#include "ex1_1.h"

int main(void) {
  MONO_PCM pcm0;
  wave_read_16bit_mono(&pcm0, "a.wav");
  return 0;
}

void wave_read_16bit_mono(MONO_PCM *pcm, char *file_name) {
  FILE *fp = fopen(file_name, "rb");

  char riff_chunk_ID[4];  // 'R', 'I', 'F', 'F' 固定
  fread(riff_chunk_ID, 1, 4, fp);
  printf("%c%c%c%c\n", riff_chunk_ID[0], riff_chunk_ID[1], riff_chunk_ID[2], riff_chunk_ID[3]);

  fclose(fp);
}
