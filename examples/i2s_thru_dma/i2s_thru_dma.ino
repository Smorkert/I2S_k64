/*
  I2S w/ DMA digital audio demonstrator for Teensy 3.5
  The current configuration is set up fo sample rate = 96k, 32 bit values, MCLK = 24.576MHz
  See header file for specifics
  TESTED and WORKING (only transmit tested)
  PINS: (pin pattern 2)
  Tx pin 3 : I2S0_TXD0     <--> TX Data
  Rx pin 13: I2S0_RXD0     <--> RX Data
  Tx pin 4 : I2S0_TX_FS    <--> LRCLK
  Tx pin 9 : I2S0_TX_BCLK  <--> BCLK
  Tx pin 11: I2S0_MCLK     <--> MCLK
  
*/

//I2S digital audio
#include "i2s.h"
#define CLOCK_TYPE             (I2S_CLOCK_INTERNAL)  // Sample Rate

// Circular buffer for audio samples, interleaved left & right channel
const uint16_t buffersize = DMA_BUFFER_SIZE; // must be a multiple of DMA_BUFFER_SIZE
volatile int16_t buffer[buffersize];
uint16_t nTX = 0;
uint16_t nRX = 0;

/* --------------------- DMA I2S Receive, we get callback to read 2 words from the FIFO ----- */

void dma_rx_callback( int16_t *pBuf, uint16_t len )
{
  while( len>0 )
  {
    buffer[nRX++] = *pBuf++;
    buffer[nRX++] = *pBuf++;
    len--;
    len--;
  }
  if( nRX>=buffersize ) nRX=0;
}

/* --------------------- DMA I2S Transmit, we get callback to put 2 words into the FIFO ----- */

void dma_tx_callback( int16_t *pBuf, uint16_t len )
{
  while( len>0 )
  {
    *pBuf++ = buffer[nTX++];
    *pBuf++ = buffer[nTX++];
    len--;
    len--;
  }
  if( nTX>=buffersize ) nTX=0;
}

/* ----------------------- begin -------------------- */

void setup()
{
  Serial.println( "Initializing" );
  
  delay(2000); 
  Serial.println( "Initializing." );
  
  //configure Audio Codec
  
  delay(1000);
  I2SRx0.begin( CLOCK_TYPE, dma_rx_callback );
  I2STx0.begin( CLOCK_TYPE, dma_tx_callback );
  
  // Before starting tx/rx, set the buffer pointers
  nRX = 0;
  nTX = 0;
  
  I2STx0.start();
  I2SRx0.start();
}


/* --------------------- main loop ------------------ */
void loop()
{
}
