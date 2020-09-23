I created this enhanced Arduino Framework for the Longan Nano module. The module is inexpensive and has lots of nice features, but the Arduino framework lags behind the native SDK for the GD32V. I've added some crude code for USB Serial, I2C and SPI. Its far from complete but should be useful as a starting point.

Notes:
1. For USB Serial to work the CPU clock must be set to 96 MHz instead of the the default 108 MHz
2. To use this framework instead of the default in platformio you add the following to platformio.ini:
  platform_packages = framework-arduino-gd32v @ https://github.com/mpmarks/framework-arduino-gd32v.git
  
  


