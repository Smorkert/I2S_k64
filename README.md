# I2S_k64

I2S library for Teensy sub family (Teensy 3.2/5/6)

-Working with Teensy 3.2, 3.5, 3.6

SETUP:
1) Configure definitions in i2s.h
  - CPU_CLK: define CPU clock in MHz. Used to scale I2S clocks if MCLK internally generated (96, 120, 144, 168, 180, 240)
  - I2S_PIN_PATTERN: Configure pins for I2S bus. See documentation.
  - DMA_BUFFER_SIZE: Size of ping-pong buffers for DMA. Decrease size for lower latency, must be even.
  - I2S_FRAME_SIZE : Channel size of audio transfer. 1 = mono, 2 = stereo, etc.
  - I2S_IO_BIT_DEPTH: Desired bit depth of audio samples. 8, 16, or 32.
  - BCLK_DIV: BCLK divider. This configures sampling speed for I2S bus.
      x/2-1 = BCLK_DIV, where x = MCLK/(LRCLK*2*I2S_IO_BIT_DEPTH). LRCLK == Sampling Frequency.
  - MCLK_DIV: Additional scale factor for MCLK if internally generated. 0 = 44.1k, 88.2k, 1 = 48k, 96k.

