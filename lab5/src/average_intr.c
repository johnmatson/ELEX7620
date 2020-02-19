// average_intr.c 
// ELEX7620 Lab 5 - Moving Average Filter

#include "audio.h"


#define LINEVOL 0x17 // 0dB config
#define OUTVOL 0x7F // +6dB config (corrects for -6dB attunation through board)

volatile int16_t audio_chR=0;    //16 bits audio data channel right
volatile int16_t audio_chL=0;    //16 bits audio data channel left

#define M 4
float32_t h[M+1];
float32_t x[M+1] = 0;
int16_t k = 0;
int16_t i;
/****** I2S Interruption Handler *****/

void I2S_HANDLER(void) {                   //I2S interruption	

float32_t yn = 0.0;
	
audio_IN = i2s_rx();	
audio_chL = (audio_IN & 0x0000FFFF);       //Separate 16 bits channel left
audio_chR = ((audio_IN >>16)& 0x0000FFFF); //Separate 16 bits channel right

		//Store ADC output sample in array x
    x[k++] = (float32_t)(audio_chL);
	
		//Array x is being used as a circular buffer
		//If k exceeds the upper end of x, reset it and point to beginning of the array
    if (k >= (M+1)) k = 0;
	
		//Add code here to implement the moving average filter
		//Using a for loop is one possible implementation
		//Don't forget to check k and make sure you don't create an out-of-range index on x[]
		//Store the filter output in the variable yn

	for (i=0; i<(M+1); i++)
	{
		yn += x[i];
	}
	yn = yn / ((float32_t)(M+1));
    
		//Write filter output yn to the DAC
 	  audio_chL= (int16_t)(yn);
	
audio_OUT = ((audio_chL <<16 & 0xFFFF0000)) + (audio_chL & 0x0000FFFF);	//Put the two channels toguether again
i2s_tx(audio_OUT);

}

int main(void)
{
  // Add code to define the impulse response here, in the array h[]
	//
	//	YOUR CODE HERE
	//
	
	audio_init ( hz8000, line_in, intr, I2S_HANDLER); 
 
	// Don't forget to adjust register settings such that the line in to headphone out gain is 1
	Codec_WriteRegister ( WM8731_LLINEIN, LINEVOL);
	Codec_WriteRegister ( WM8731_RLINEIN, LINEVOL);
	Codec_WriteRegister ( WM8731_LHEADOUT, OUTVOL);
	Codec_WriteRegister ( WM8731_RHEADOUT, OUTVOL);
	
  while(1){}
}
