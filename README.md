# I2S_k64

I2S library for Teensy sub family (Teensy 3.2/5/6)

-Working with Teensy 3.2, 3.5, 3.6

SETUP:
1) Configure definitions in i2s.h
  - CPU_CLK: define CPU clock in MHz. Used to scale I2S clocks if MCLK internally generated (96, 120, 144, 168, 180, 240)
  - I2S_PIN_PATTERN: Configure pins for I2S bus.

