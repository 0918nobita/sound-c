#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex1_1.h"

int main(void) {
  MONO_PCM pcm0, pcm1;

  wave_read_16bit_mono(&pcm0, "a.wav");

  pcm1.fs = pcm0.fs;
  pcm1.bits = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.s = calloc(pcm1.length, sizeof(double));

  for (int i = 0; i < pcm0.length; i++) pcm1.s[i] = pcm0.s[i];

  wave_write_16bit_mono(&pcm1, "b.wav");

  free(pcm0.s);
  free(pcm1.s);

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
  }

  fclose(fp);
}

void wave_write_16bit_mono(MONO_PCM *pcm, char *file_name) {
  char riff_chunk_ID[4] = { 'R', 'I', 'F', 'F' };
  int riff_chunk_size = 36 + pcm->length;
  char file_format_type[4] = { 'W', 'A', 'V', 'E' };
  char fmt_chunk_ID[4] = { 'f', 'm', 't', ' ' };
  int fmt_chunk_size = 16;
  short wave_format_type = 1;
  short channel = 1;

  // 標本化周波数
  int samples_per_sec = pcm->fs;
  // 量子化精度
  int bits_per_sample = pcm->bits;
  // 1 時刻の音データを記憶するのに必要なデータ量
  short block_size = bits_per_sample * channel / 8;  // bit -> byte の変換のために 8 で割る
  // 1 秒の音データを記録するのに必要なデータ量
  int bytes_per_sec = block_size * samples_per_sec;

  char data_chunk_ID[4] = { 'd', 'a', 't', 'a' };
  int data_chunk_size = pcm->length;

  FILE *fp = fopen(file_name, "wb");

  fwrite(riff_chunk_ID, 1, 4, fp);
  fwrite(&riff_chunk_size, 4, 1, fp);
  fwrite(file_format_type, 1, 4, fp);
  fwrite(fmt_chunk_ID, 1, 4, fp);
  fwrite(&fmt_chunk_size, 4, 1, fp);
  fwrite(&wave_format_type, 2, 1, fp);
  fwrite(&channel, 2, 1, fp);
  fwrite(&samples_per_sec, 4, 1, fp);
  fwrite(&bytes_per_sec, 4, 1, fp);
  fwrite(&block_size, 2, 1, fp);
  fwrite(&bits_per_sample, 2, 1, fp);
  fwrite(data_chunk_ID, 1, 4, fp);
  fwrite(&data_chunk_size, 4, 1, fp);

  for (int n = 0; n < pcm->length; n++) {
    double s = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

    // クリッピング
    if (s > 65535.0) {
      s = 65535.0;
    } else if (s < 0.0) {
      s = 0.0;
    }

    short data = (short)((int)(s + 0.5) - 32768);
    fwrite(&data, 2, 1, fp);
  }

  fclose(fp);
}
