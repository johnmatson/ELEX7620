// ELEX7620 Lab 9 - Fast Fourier Transform (FFT) using
// the CMSIS-DSP library function
// 
// fft_CMSIS.c


#include "audio.h"
#include "arm_const_structs.h"
#include "gpio.h"


#define N 16
#define fIN 1000.0
#define fSAMP 8000.0


float32_t samples[2*N];

int main()
{    
  int n;
	gpio_set_mode(TEST_PIN, Output);
	
  for(n=0 ; n<N ; n++)
  {
    samples[2*n] = arm_cos_f32(2.0*PI*fIN/fSAMP*(float32_t)(n));
    samples[2*n+1] = 0.0;
  }
	gpio_set(TEST_PIN, HIGH);
  arm_cfft_f32(&arm_cfft_sR_f32_len16, samples, 0, 1);
	gpio_set(TEST_PIN, LOW);
  while(1){}
}	
