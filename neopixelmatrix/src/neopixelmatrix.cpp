/* neopixelmatrix library by Adam Puleo
 */

#include "neopixelmatrix.h"

NeoPixelMatrix::NeoPixelMatrix(uint8_t width,
                               uint8_t height,
                               uint8_t pin,
                               uint8_t pixel_type): Adafruit_NeoPixel(width * height, pin, pixel_type) {
    this->WIDTH = width;
    this->HEIGHT = height;
  }

void NeoPixelMatrix::setPixelColor(uint8_t x,
                                   uint8_t y,
                                   uint8_t red,
                                   uint8_t green,
                                   uint8_t blue) {
    if (x < this->WIDTH && y < this->HEIGHT) {
      uint8_t offset;

      if ((x & 1) == 1) {
        // On odd numbered rows, as y increases the pixel address also increases
        offset = y;
      } else {
        // On even numbered rows, as y increases the pixel address decreases
        offset = this->HEIGHT - 1 - y;
      }

      Adafruit_NeoPixel::setPixelColor(this->HEIGHT * x + offset, red, green, blue);
    }
}

NeoPixelMatrix::~NeoPixelMatrix() {
}
