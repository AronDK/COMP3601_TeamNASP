/* Factored discrete Fourier transform, or FFT, and its inverse iFFT */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "misc.h"
#include "FFT.h"
#include "audio_i2s.h"

void fft( complex *v, int n, complex *tmp ) {
  if(n>1) {			/* otherwise, do nothing and return */
    int k,m;    complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    fft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    fft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w.Im = -sin(2*PI*m/(double)n);
      w.Re = cos(2*PI*m/(double)n);
      z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
      z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
      v[  m  ].Re = ve[m].Re + z.Re;
      v[  m  ].Im = ve[m].Im + z.Im;
      v[m+n/2].Re = ve[m].Re - z.Re;
      v[m+n/2].Im = ve[m].Im - z.Im;
    }
  }
  return;
}

complex *DirAvg(DIR *dir, char *profileName, int transLen, int transRuns) {
  printf("Calculating average waveform...\n");
  struct dirent *de;
  int i = 0;
  complex *cur = (complex *)malloc(transLen * transRuns * sizeof(complex));
  complex *next = (complex *)malloc(transLen * transRuns * sizeof(complex));
  
  // Initialize cur to first waveform of directory
  printf("Reading directory...\n");
  while ((de = readdir(dir)) != NULL) {
    FILE *fp = fopen(de->d_name, "r");
    for (i = 0; fscanf(fp, "%f,%f", &cur[i].Re, &cur[i].Im) != EOF; i++);
    fclose(fp);
    break;
  }
  avgWF *tmp = (avgWF *)malloc(sizeof(avgWF));
  tmp->length = i;
  tmp->wave = cur;
  

  while ((de = readdir(dir)) != NULL) {     
    FILE *fp = fopen(de->d_name, "r");
    for (i = 0; fscanf(fp, "%f,%f", &next[i].Re, &next[i].Im) != EOF; i++);
    fclose(fp);
    tmp = Comparison(next, tmp, transLen, transRuns);
  }
  free(next);
  printf("Done calculating average waveform\n");
  return tmp->wave;
}

complex *fft_setup(uint32_t frames[TRANSFER_RUNS][TRANSFER_LEN], int transLens, int transRuns) {
  complex *fft_input = (complex *)malloc(transLens * transRuns * sizeof(complex));
  printf("the segfault is in setup\n");
  int i, j = 0;
  for (i = 0; i < transRuns; i++) {
      for (j = 0; j < transLens; j++) {
          fft_input[i * transLens + j].Re = frames[j][i];
          fft_input[i * transLens + j].Im = frames[j][i];
      }
  }
  printf("%d\n", (i-1) * transLens + (j-1));
  complex *tmp = (complex *)malloc(transLens * transRuns * sizeof(complex));
  fft(fft_input, transLens * transRuns, tmp);
  free(tmp);
  printf("the segfault isn't setup\n");
  return fft_input;
}

void saveWaveform(DIR *dir, char *filename, complex *waveform, int size) {
  printf("Saving waveform..., size = %d\n", size);
  if (filename == NULL) {
    time_t seconds;
    seconds = time(NULL);
    char time_str[20];
    sprintf(time_str, "%ld", seconds);
    filename = strcat(time_str, ".txt");
  }
  
  // Save wave to file
  FILE *fp = fopen(filename, "w");
  for (int i = 0; i < size - 1; i++) {
    fprintf(fp, "%f,%f\n", waveform[i].Re, waveform[i].Im);
  }
  fclose(fp);
  printf("Saved waveform to %s\n", filename);
}

avgWF *Comparison(complex *waveform, avgWF *original, int transLen, int transRuns) {
  printf("Comparing waveforms...\n");
  int count = 0; int *matched = (int *)malloc(original->length * sizeof(int));
  for (int i = 0; i < transLen * transRuns; i++) {
    // Within 5% of average (Uncertainty)
    if (waveform[i].Re <= (original->wave[i].Re + original->wave[i].Re * 0.05) && 
      waveform[i].Re >= (original->wave[i].Re - original->wave[i].Re * 0.05) && 
      waveform[i].Im <= (original->wave[i].Im + original->wave[i].Im * 0.05) && 
      waveform[i].Im >= (original->wave[i].Im - original->wave[i].Im * 0.05)) {
      count++;
      matched[i] = 1;
    }
  }
  printf("Matched %d/%d samples\n", count, transLen * transRuns);

  if (count >= (transLen * transRuns) * 0.8) {
    printf("Matched sound profile\n");
    // Remove points in original that do not match with new waveform
    for (int i = 0; i < transLen * transRuns; i++) {
      if (matched[i] == 0) {
        original->wave[i].Re = -1;
        original->wave[i].Im = -1;
      }
    }
  }

  original->length = count;
  free(matched);
  printf("Done comparing waveforms\n");
  return original;
}

void saveAvg(DIR *dir, char *filename, complex *wf, int size) {
  printf("Saving average waveform...\n");
  FILE *fp = fopen(filename, "w");
  fprintf(fp, "%d\n", size);
  for (int i = 0; i < size; i++) {
    fprintf(fp, "%f,%f\n", wf[i].Re, wf[i].Im);
  }
  fclose(fp);
  printf("Saved average waveform to %s\n", filename);
}