// fir_intr.c 
// ELEX7620 Lab 6 - FIR filter

#include "audio.h"

#include "lab6coeffs.h"

#define LINEVOL 0x17
#define OUTVOL 0x7F

volatile int16_t audio_chR=0;    
volatile int16_t audio_chL=0;    

// include FIR coefficient header file here
// note that N is defined within the header file
// the impulse response h[] is also defined in the header file

float32_t x[N];
int16_t k = 0;

int16_t i;

/****** I2S Interruption Handler *****/
void I2S_HANDLER(void) {                  

float32_t yn = 0.0;
int i;
	
audio_IN = i2s_rx();	
audio_chL = (audio_IN & 0x0000FFFF);       
audio_chR = ((audio_IN >>16)& 0x0000FFFF); 

	  gpio_set(TEST_PIN, HIGH);
	
		//Store ADC output sample in array x
    x[k++] = (float32_t)(audio_chL);
	
		//Array x is being used as a circular buffer
		//If k exceeds the upper end of x, reset it and point to beginning of the array
    if (k >= N) k = 0;
	
		//Add code here to implement the FIR filter
		//Using a for loop is one possible implementation
		//Don't forget to check k and make sure you don't create an out-of-range index on x[]
		//Store the filter output in the variable yn
		//Use discrete time convolution as noted in the lab manual
		
		for (i=0;i<N;i++)
		{
			int sampleIndex = k-i;
			if(sampleIndex<0)
				sampleIndex += N;
			yn += h[i] * x[sampleIndex];
		}
    
		//Write filter output yn to the DAC
 	  audio_chL= (int16_t)(yn);
    gpio_set(TEST_PIN, LOW);
		
audio_OUT = ((audio_chL <<16 & 0xFFFF0000)) + (audio_chL & 0x0000FFFF);	
i2s_tx(audio_OUT);

}

int main(void)
{
  gpio_set_mode(TEST_PIN, Output);
  audio_init ( hz8000, line_in, intr, I2S_HANDLER); 

  Codec_WriteRegister ( WM8731_LLINEIN, LINEVOL);
  Codec_WriteRegister ( WM8731_RLINEIN, LINEVOL);
  Codec_WriteRegister ( WM8731_LHEADOUT, OUTVOL);
  Codec_WriteRegister ( WM8731_RHEADOUT, OUTVOL);
		
  while(1){}
}
