/*
  I2S w/ DMA digital audio demonstrator for Teensy 3.5
  The current configuration is set up fo sample rate = 96k, 32 bit values, MCLK = 24.576MHz
  See header file for specifics
  TESTED and WORKING (only transmit tested)

  PINS: (pin pattern 2)
  Tx pin 3 : I2S0_TXD0     <--> Data
  Tx pin 4 : I2S0_TX_FS    <--> LRCLK
  Tx pin 9 : I2S0_TX_BCLK  <--> BCLK
  Tx pin 11: I2S0_MCLK     <--> MCLK


*/

//I2S digital audio
#include "i2s.h"
#define CLOCK_TYPE             (I2S_CLOCK_96K_INTERNAL)  // Sample Rate

// audio data
int32_t audL, audR;

//Audio generation algorithm 
void test_data(void){
  audL = 0; //for testing
  audR = 0xFAAAAAAA; //for testing
  
}

/* ----------------------- DMA transfer, we get callback to fill one of the ping-pong buffers ------ */
void dma_tx_callback( int32_t *pBuf, uint16_t len )
{
  while( len>0 )
  {
    *pBuf++ = audL; //Left Channel
    *pBuf++ = audR; //Right Channel 
    test_data(); //call audio generation algorithm here samples here
    len--;
    len--;
  }
}


/* ----------------------- begin -------------------- */

void setup()
{
  Serial.println( "Initializing" );
  
  delay(2000); 
  Serial.println( "Initializing." );

  delay(1000);
  I2STx0.stop();
  I2STx0.begin( CLOCK_TYPE, dma_tx_callback );
  Serial.println( "Initialized I2S with DMA" );
  
  I2STx0.start();  
}


/* --------------------- main loop ------------------ */
void loop()
{
}

