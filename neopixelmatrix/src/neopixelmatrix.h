#pragma once

/* neopixelmatrix library by Adam Puleo

   This library is for addressing a NeoPixel matrix. The library takes x, y
   coordinates, where 0, 0 is the lower left corner of the matrix, and outputs
   an index to the Adafruit_NeoPixel library. This library assumes that the
   first NeoPixel, NeoPixel zero, is in the upper left corner.

   This library used the Adafruit_NeoPixel library to actually turn the
   NeoPixels on and off.
 */

#include "Particle.h"
#include "neopixel.h"

class NeoPixelMatrix: public Adafruit_NeoPixel {
  public:
    NeoPixelMatrix(uint8_t width, // Number of NeoPixels across the bottom, y.
                   uint8_t height, // Neumber of NeoPixels high the matrix is, x.
                   uint8_t pin, // Digital IO pin used to address the NeoPixels; this is passed directly to the Adafruit_NeoPixel library constructor.
                   uint8_t pixel_type); // NeoPixels type; this is passed directly to the Adafruit_NeoPixel library constructor.

    /* Sets the NeoPixel at x, y to the RGB color specified. The coordinates are
       translated to the NeoPixel index. This index and the RGB colors are
       passed to the Adafruit_NeoPixel setPixelColor function.

       If either x or y is out of range the function just returns.
    */
    void setPixelColor(uint8_t x,
                       uint8_t y,
                       uint8_t red,
                       uint8_t green,
                       uint8_t blue);

    ~NeoPixelMatrix();

  private:
    uint8_t WIDTH;
    uint8_t HEIGHT;
};
