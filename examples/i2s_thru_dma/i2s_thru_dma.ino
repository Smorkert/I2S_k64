/*
  I2S w/ DMA for Teensy 3.x in use with WM8731 Codec (Any codec should work though)
  The current configuration is set up for sample rate = 88.2kHz, 32 bit values, MCLK_EXT = 11.2896
  See header file for specifics
  TESTED and WORKING
  PINS: (I2S_CODEC_PIN_PATTERN)
  Tx pin 3 : I2S0_TXD0     <--> TX Data
  Rx pin 13: I2S0_RXD0     <--> RX Data
  Tx pin 4 : I2S0_TX_FS    <--> LRCLK
  Tx pin 9 : I2S0_TX_BCLK  <--> BCLK
  Tx pin 11: I2S0_MCLK     <--> MCLK
  
*/

//I2S digital audio
#include "i2s.h"
#include <Wire.h>
#include "AudioCodec.h"
#define CLOCK_TYPE             (I2S_CLOCK_EXTERNAL)  // Sample Rate
#define SAMPLE_RATE 88         //88 or 44, change BCLK_DIV in i2s.h (0: 88.2KHz-32-bit, 1:88.2KHz-16-bit, 1: 44.1KHz-32-bit, 3: 44.1KHz-16-bit)

// Circular buffer for audio samples, interleaved left & right channel, change intXX_t for 32-bit or 16-bit audio
const uint16_t buffersize = DMA_BUFFER_SIZE; // must be a multiple of DMA_BUFFER_SIZE
volatile int32_t Buffer[buffersize];
uint16_t nTX = 0;
uint16_t nRX = 0;

/* --------------------- DMA I2S Receive, we get callback to read 2 words from the FIFO ----- */

void dma_rx_callback( int32_t *pBuf, uint16_t len )
{
  while( len>0 )
  {
    Buffer[nRX++] = *pBuf++;
    Buffer[nRX++] = *pBuf++;
    len--;
    len--;
  }
  if( nRX>=buffersize ) nRX=0;
}

/* --------------------- DMA I2S Transmit, we get callback to put 2 words into the FIFO ----- */

void dma_tx_callback( int32_t *pBuf, uint16_t len )
{
  while( len>0 )
  { 
    *pBuf++ = Buffer[nTX++];
    *pBuf++ = Buffer[nTX++];
    len--;
    len--;
  }
  if( nTX>=buffersize ) nTX=0;
}

/* ----------------------- begin -------------------- */

void setup()
{
  AudioCodec_init(); // setup codec registers
  Serial.println( "Initializing" );
  
  delay(200); 
  Serial.println( "Initializing." );
  
  //configure Audio Codec
  
  delay(100);
  I2SRx0.begin(CLOCK_TYPE, dma_rx_callback);
  I2STx0.begin(CLOCK_TYPE, dma_tx_callback);
  
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
