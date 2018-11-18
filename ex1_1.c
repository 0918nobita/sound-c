#include <stdio.h>
#include <stdlib.h>
// #include <math.h>  // デバッグ用
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

  printf("---------- Fmt Chunk ----------\n");

  char fmt_chunk_ID[4];  // 'f' 'm' 't' ' ' 固定
  fread(fmt_chunk_ID, 1, 4, fp);
  printf("Fmt Chunk ID: %c%c%c%c\n", fmt_chunk_ID[0], fmt_chunk_ID[1], fmt_chunk_ID[2], fmt_chunk_ID[3]);

  int fmt_chunk_size;  // 16 固定
  fread(&fmt_chunk_size, 4, 1, fp);
  printf("Fmt Chunk Size: %d\n", fmt_chunk_size);

  short wave_format_type;  // 音データの形式 (PCM の場合は 1)
  fread(&wave_format_type, 2, 1, fp);
  printf("WAVE Format Type: %d\n", wave_format_type);

  short channel;  // チャンネル数 (モノラルでは 1, ステレオでは 2)
  fread(&channel, 2, 1, fp);
  printf("Channel: %d\n", channel);

  int samples_per_sec;  // [Hz] 標本化周波数
  fread(&samples_per_sec, 4, 1, fp);
  printf("Samples per Second: %d Hz\n", samples_per_sec);

  int bytes_per_sec;  // [byte] 1 秒の音データを記憶するのに必要なデータ量 (block_size * samples_per_sec)
  fread(&bytes_per_sec, 4, 1, fp);
  printf("Bytes per Second: %d byte\n", bytes_per_sec);

  short block_size;  // [byte] 1 時刻の音データを記憶するのに必要なデータ量
  fread(&block_size, 2, 1, fp);
  printf("Block Size: %d byte\n", block_size);

  short bits_per_sample; // [bit] 量子化精度
  fread(&bits_per_sample, 2, 1, fp);
  printf("Bits per Sample: %d bit\n", bits_per_sample);

  printf("---------- Data Chunk ----------\n");

  char data_chunk_ID[4]; // 'd' 'a' 't' 'a' 固定
  fread(data_chunk_ID, 4, 1, fp);
  printf("Data Chunk ID: %c%c%c%c\n", data_chunk_ID[0], data_chunk_ID[1], data_chunk_ID[2], data_chunk_ID[3]);

  int data_chunk_size;  // 音データの長さ * channel
  fread(&data_chunk_size, 4, 1, fp);
  printf("Data Chunk Size: %d\n", data_chunk_size);

  pcm->fs = samples_per_sec; 
  pcm->bits = bits_per_sample;
  pcm->length = data_chunk_size / channel;  // 音データの長さ
  pcm->s = calloc(pcm->length, sizeof(double));  // メモリ確保

  for (int n = 0; n < pcm->length; n++) {
    short data;
    fread(&data, 2, 1, fp);
    pcm->s[n] = (double)data / 32768.0;  // 音データを -1 以上 1 未満の範囲に正規化する

    /*
    if (n >= 1) {  // 値が変化している場合、前後の値を出力する
      double diff = pcm->s[n] - pcm->s[n-1];
      if (fabs(diff) != 0.0) printf("%lf, ", diff);
    }
    */
  }

  fclose(fp);
}
