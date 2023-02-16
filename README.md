# STM32 MEDIA DRIVER TOOLSET

This package provides a toolset for developing applications with video features.
It implements a VGA driver, a basic primitive drawing library and a very basic audio driver.

This is an adaptation of an old project that I developed in 2014.

IMPORTANT: The adaptation is still ongoing...

## VGA DRIVER

The driver generates a VGA signal with 256 colors RGB 2-3-3

A minimal DAC is needed to convert the 8 bit signal to the 3 channel VGA analog signals. This DAC can be easily built with only resistors.
The following picture describes how to build the DAC and how to connect it to the VGA adapter:

<img alt="DAC circuit schematics" src="./docs/dac-schematics.svg" width="400" style="background: white; border-radius: 5px">

By default, the video output in the board uses the following pins:

- B0 -> PC8
- B1 -> PC9
- B2 -> PC10
- G0 -> PC11
- G1 -> PC12
- G2 -> PC13
- R0 -> PC14
- R1 -> PC15

But in the future this will be a configurable option.

If you don't want to build the DAC, it is still possible to generate the output without it, but the color combinations will be reduced to 8.

## Example

Here is a basic example which demonstrates how to initialize the video driver.

```c

#include <stdio.h>

#include <stm32f4xx.h>
#include "libs/graphics.h"
#include "libs/text.h"


void example(void) {

    GRAPHICS_InitTypeDef graphicsCfg = {
        .useHardwareAcceleration = true,
        .useSDRAM = false,
        .mainCtxHeight = 200,
        .mainCtxWidth = 320,
        .videoDriver = VIDEO_DRIVER_VGA,
    };

    GRAPHICS_Init(&graphicsCfg);

    while (1) {

        ClearBitmap(0x00);
        
        DrawText("Hello world!", 10, 10, 0xFF);
        
        WaitForVSync();
        
        SwapContextBuffers();

    }

}

```
