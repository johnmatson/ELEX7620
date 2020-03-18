// impinviir_intr.c

#include "audio.h"

#define Y1 0.8123
#define Y2 -0.4617
#define X0 0.1290
#define X1 0.2579
#define X2 0.1290

volatile int16_t audio_chR=0;    
volatile int16_t audio_chL=0;    

void I2S_HANDLER(void) {  

int16_t audio_out_chL = 0;
int16_t audio_out_chR = 0;
float32_t xn, yn;
float32_t x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	
audio_IN = i2s_rx();	
audio_chL = (audio_IN & 0x0000FFFF);       //Separate 16 bits channel left
audio_chR = ((audio_IN >>16)& 0x0000FFFF); //Separate 16 bits channel right
	
xn = (float32_t)(audio_chL);  // Compute only the left channel

	// implement the difference equation for the IIR filter

	x2 = x1;
	x1 = xn;

	yn = Y1*y1 + Y2*y2 + X0*xn + X1*x1 + X2*x2;

	y2 = y1;
	y1 = yn;

audio_out_chL = (int16_t)(yn);
	
audio_OUT = ((audio_out_chR<<16 & 0xFFFF0000)) + (audio_out_chL & 0x0000FFFF);	//Put the two channels toguether again
i2s_tx(audio_OUT);

}


int main(void)
{
	audio_init ( hz8000, line_in, intr, I2S_HANDLER); 
 
	Codec_WriteRegister ( WM8731_LLINEIN, LINEVOL);
	Codec_WriteRegister ( WM8731_RLINEIN, LINEVOL);
	Codec_WriteRegister ( WM8731_LHEADOUT, OUTVOL);
	Codec_WriteRegister ( WM8731_RHEADOUT, OUTVOL);

  while(1){}
}