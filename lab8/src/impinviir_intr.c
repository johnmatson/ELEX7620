// impinviir_intr.c

#include "audio.h"
volatile int16_t audio_chR=0;    
volatile int16_t audio_chL=0;    

void I2S_HANDLER(void) {  

int16_t audio_out_chL = 0;
int16_t audio_out_chR = 0;
float32_t xn, yn;
	
audio_IN = i2s_rx();	
audio_chL = (audio_IN & 0x0000FFFF);       //Separate 16 bits channel left
audio_chR = ((audio_IN >>16)& 0x0000FFFF); //Separate 16 bits channel right
	
xn = (float32_t)(audio_chL);  // Compute only the left channel

		//Add code here to implement the difference equation for the IIR filter
		//Note that you will also need to declare variables to store the past samples
		//
		//	YOUR CODE HERE
		//

audio_out_chL = (int16_t)(yn);
	
audio_OUT = ((audio_out_chR<<16 & 0xFFFF0000)) + (audio_out_chL & 0x0000FFFF);	//Put the two channels toguether again
i2s_tx(audio_OUT);

}


int main(void)
{
	audio_init ( hz8000, line_in, intr, I2S_HANDLER); 
 
	// Don't forget to adjust register settings such that the line in to headphone out gain is 1
	
  while(1){}
}