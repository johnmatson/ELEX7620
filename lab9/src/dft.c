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
//complex_t X[N];

/*
void dft(complex_t *x)  {
	float ohmegaNot = 2*PI/N; // bin spacing constant
	int k; // sample number
	int n; // DFT sums over n

	// implement DFT equation
	for (k = 0; k<N; k++) {
		for (n = 0; n<N; n++) {
			X[k].real += x[n].real * cos(k*ohmegaNot*n);
			X[k].imag += x[n].real * -sin(k*ohmegaNot*n);
		}
	}
}
*/

void dft(complex_t *x)  {
	float ohmegaNot = 2*PI/N; // bin spacing constant
	int k; // sample number
	int n; // DFT sums over n
	float x_temp[N]; // temporary input sample array
	
	// write to temporary sample array
	for (k = 0; k<N; k++) {
		x_temp[k] = x[k].real;
		x[k].real = 0;
	}

	// implement DFT equation
	for (k = 0; k<N; k++) {
		for (n = 0; n<N; n++) {
			x[k].real += x_temp[n] * cos(k*ohmegaNot*n);
			x[k].imag += x_temp[n] * -sin(k*ohmegaNot*n);
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
