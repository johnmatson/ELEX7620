// ELEX7620 Lab 9 Discrete Fourier Transform
// dft.c
//
#include <math.h> 
#include "platform.h"
#include "gpio.h"

#define PI 3.14159
#define N 8
#define fIN 1000.0
#define fSAMP 8000.0
	

typedef struct complex_t  {
	float real;
	float imag;
} complex_t;

complex_t samples[N];

void dft(complex_t *x)  {
	complex_t X[N] = 0; // DFT output array
	float ohmegaNot = 2*pi()/N; // bin spacing constant

	// implement DFT equation
	for (int k = 0; k<N; k++) {
		for (int n = 0; n<N; n++) {
			X[k].real += x[n].real * cos(-k*ohmegaNot*n);
			X[k].imag += x[n].real * sin(-k*ohmegaNot*n);
		}
	}

}

int main()  {
	int n;

	gpio_set_mode(TEST_PIN, Output);

	for(n=0 ; n<N ; n++)  {

		samples[n].real = cos(2.0*PI*fIN/fSAMP*(float)(n));
		samples[n].imag = 0.0;
	}

	gpio_set(TEST_PIN, HIGH);
	dft(samples);          //call DFT function
	gpio_set(TEST_PIN, LOW);

	while(1){}
}
