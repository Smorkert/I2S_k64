/*
  I2S & DMA digital audio demonstrator for Teensy 3.5
  Using the hughpyle minsky-circles demo for the Teensy3 to demonstrate I2S TX with DMA
  DAC needs to be 16-bit for this demo to work (change to 24 bit by multiplying by 8388608 instead of 32767 and put in int32)
  
// Pin patterns
// Teensy 3.0 hardware has several ways to configure its I2S pins:
//      pin     alt4            alt6
//      3                       I2S0_TXD0           (transmit data, also on 22)
//      4                       I2S0_TX_FS          (transmit word clock, also on 23, 25)
//      9                       I2S0_TX_BCLK        (transmit bit clock, also on 24, 32)
//      11      I2S0_RX_BCLK    I2S0_MCLK           (receive bit clock, also on 27; or master clock, also on 28)
//      12      I2S0_RX_FS                          (receive word clock, also on 29)
//      13      I2S0_RXD0                           (receive data)
//      22                      I2S0_TXD0           (transmit data, also on 3)
//      23                      I2S0_TX_FS          (also on 4, 25)
//      24                      I2S0_TX_BCLK        (also on 9, 32)
//      25                      I2S0_TX_FS          (also on 4, 23)
//      27      I2S0_RX_BCLK                        (also on 11)
//      28      I2S0_MCLK                           (also on 11)
//      29      I2S0_RX_FS                          (also on 12)
//      32      I2S0_TX_BCLK                        (also on 9, 24)


/*
PINS: (pin pattern 2)
Tx pin 3 : I2S0_TXD0     <--> Data
Tx pin 4 : I2S0_TX_FS    <--> LRCLK
Tx pin 9 : I2S0_TX_BCLK  <--> BCLK
Tx pin 11: I2S0_MCLK     <--> MCLK
*/

/* I2S digital audio */
#include <i2s.h>
#define I2S_PIN_PATTERN     I2S_TX_PIN_PATTERN_2 // Transmit pins 3, 4, 9, 11
#define CLOCK_TYPE          (I2S_CLOCK_48K_INTERNAL)
#define clock_per_sec       48000

// audio data
int16_t audf, audx, audy, audd;
int32_t nnn=0;

void initsinevalue()
{
  audf = 45 + (rand() % 48);                                // midi note number
  float f = (440.0 / 32) * pow(2, ((float)audf - 9) / 12);  // Hz.  For realz, use a lookup table.
  audd = 2.0 * sin(PI*f/clock_per_sec) * 32767;             // delta (q15_t)
  audx = 0;
  audy = 0.9 * 32767;                                       // start somewhere near full-scale
}

void nextsinevalue() 
{
  nnn++;
  if(nnn>(clock_per_sec)) {nnn=0;initsinevalue();};                      // reset every second
//  if(nnn>24000){nnn=0;audx=audx<<1;if(audx==0)audx=1;b=audx;};return;  // marching blip
//  audx+=4;if(nnn>512){nnn=0;audx=-2048;};b=audx;return;                // stair
//  b = 0xACCF0010; audx=0xACCF; return;                                 // const pattern
  audx+=((audd*audy)>>15)&0xFFFFu; audy-=((audd*audx)>>15)&0xFFFFu;      // sinewaves http://cabezal.com/misc/minsky-circles.html
}


/* ----------------------- DMA transfer, we get callback to fill one of the ping-pong buffers ------ */
void dma_tx_callback( int16_t *pBuf, uint16_t len )
{
  while( len>0 )
  {
    *pBuf++ = audx;
    *pBuf++ = audy;
    nextsinevalue();
    len--;
    len--;
  }
  // Serial.println(audf,DEC);
}


/* ----------------------- begin -------------------- */

void setup()
{
  initsinevalue();
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
