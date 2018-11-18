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
  printf("RIFF Chunk ID: %c%c%c%c\n", riff_chunk_ID[0], riff_chunk_ID[1], riff_chunk_ID[2], riff_chunk_ID[3]);

  int riff_chunk_size;  // [byte] 36 + (data チャンクのサイズ)
  fread(&riff_chunk_size, 4, 1, fp);
  printf("RIFF Chunk Size: %d\n", riff_chunk_size);

  char file_format_type[4];  // 'W' 'A' 'V' 'E' 固定
  fread(file_format_type, 1, 4, fp);
  printf("File Format Type: %c%c%c%c\n", file_format_type[0], file_format_type[1], file_format_type[2], file_format_type[3]);

  char fmt_chunk_ID[4];  // 'f' 'm' 't' ' ' 固定
  fread(fmt_chunk_ID, 1, 4, fp);
  printf("FMT Chunk ID: %c%c%c%c\n", fmt_chunk_ID[0], fmt_chunk_ID[1], fmt_chunk_ID[2], fmt_chunk_ID[3]);

  int fmt_chunk_size;  // 16 固定
  fread(&fmt_chunk_size, 4, 1, fp);
  printf("FMT Chunk Size: %d\n", fmt_chunk_size);

  short wave_format_type;  // 音データの形式 (PCM の場合は 1)
  fread(&wave_format_type, 2, 1, fp);
  printf("WAVE Format Type: %d\n", wave_format_type);

  short channel;  // チャンネル数 (モノラルでは 1, ステレオでは 2)
  fread(&channel, 2, 1, fp);
  printf("Channel: %d\n", channel);

  int samples_per_sec;  // 標本化周波数
  fread(&samples_per_sec, 4, 1, fp);
  printf("Samples per Second: %d Hz\n", samples_per_sec);

  int bytes_per_sec;  // [byte] 1 秒の音データを記憶するのに必要なデータ量 (block_size * samples_per_sec)
  fread(&bytes_per_sec, 4, 1, fp);
  printf("Bytes per Second: %d byte\n", bytes_per_sec);

  fclose(fp);
}
