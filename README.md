# I2S_k64

I2S library for Teensy sub family (Teensy 3.2/5/6)

Initializes an I2S interface using DMA with user defined MCLK, BCLK, LRCLK, buffer size and bit depth.

-Working with Teensy 3.2, 3.5, 3.6

SETUP:
1) Configure definitions in i2s.h
    - CPU_CLK: define CPU clock in MHz. Used to scale I2S clocks if MCLK internally generated (96, 120, 144, 168, 180, 240)
    - I2S_PIN_PATTERN: Configure pins for I2S bus. See documentation.
    - DMA_BUFFER_SIZE: Size of ping-pong buffers for DMA. Decrease size for lower latency, must be even.
    - I2S_FRAME_SIZE : Channel size of audio transfer. 1 = mono, 2 = stereo, etc.
    - I2S_IO_BIT_DEPTH: Desired bit depth of audio samples. 8, 16, or 32.
    - BCLK_DIV: BCLK divider. This configures sampling speed for I2S bus.
  
        - x/2-1 = BCLK_DIV, where x = MCLK/(LRCLK x 2 x I2S_IO_BIT_DEPTH). LRCLK == Sampling Frequency.
    - MCLK_DIV: Additional scale factor for MCLK if internally generated. 0 = 44.1k, 88.2k, 1 = 48k, 96k.

2) In main.cpp
    - define CLOCK_TYPE: I2S_CLOCK_EXTERNAL or I2S_CLOCK_INTERNAL. Internal or external MCLK.
    - verify variables are correct size: Buffer[buffersize] & pBuf must equal I2S_IO_BIT_DEPTH. (int8_t, int16_t, int32_t)
