# STM32 MEDIA DRIVER TOOLSET

This package provides a toolset for developing applications with video features.
It implements a VGA driver, a basic primitive drawing library and a very basic audio driver.

This is an adaptation of an old project that I developed in 2014.

IMPORTANT: The adaptation is still ongoing...

## VGA DRIVER

The driver generates a VGA signal with 256 colors RGB 2-3-3

A minimal DAC is needed to convert the 8 bit signal to the 3 channel VGA analog signals. This DAC can be easily built with only resistors.
TODO: upload dac circuit schematics

It is still possible to generate the output without the DAC but the color combinations will be reduced to 8.
