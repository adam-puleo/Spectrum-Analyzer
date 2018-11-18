#pragma once

/* spectrum_display library by Adam Puleo

   The Spectrum Display class display color gradient lines on a NeoPixelMatrix.
   There is an option (linger_max_volume) in the constructor to have the
   maximum value NeoPixel stay on for a period of time (linger_time).
*/

#include "Particle.h"
#include "neopixelmatrix.h"

class Spectrum_display
{
public:
  Spectrum_display(NeoPixelMatrix *matrix, // NeoPixelMatrix that the lines should be drawn on.
                   uint8_t start_x, // Where on matrix the spectrum should be displayed.
                   uint8_t start_y, // Where on matrix the spectrum should be displayed.
                   uint8_t width, // Number of pixels wide the spectrum display should be.
                   uint8_t height,// Number of pixels high the spectrum display should be.
                   uint8_t start_red, // Amount of red the gradient should start with.
                   uint8_t start_green, // Amount of green the gradient should start with.
                   uint8_t start_blue, // Amount of blue the gradient should start with.
                   uint8_t end_red, // Amount of red the gradient should end with.
                   uint8_t end_green, // Amount of green the gradient should end with.
                   uint8_t end_blue, // Amount of blue the gradient should end with.
                   bool    linger_max_volume, // If the max NeoPixel should stay lit for linger_time.
                   unsigned long linger_time, // Number of milliseconds the maximum NeoPixel should stay on for.
                   uint8_t max_volume_red, // Amount of red for the maximum NeoPixel.
                   uint8_t max_volume_green, // Amount of green for the maximum NeoPixel.
                   uint8_t max_volume_blue); // Amount of blue for the maximum NeoPixel.

  /* This is the maximum NeoPixel of the line to draw. The line is drawn
     vertically, from x,1 to x,y. If y is zero all of the NeoPixels in the
     column are turned off except for the previous max NeoPixels if
     linger_max_volume is true. If the x,y value is 0,1 the NeoPixel at start_x,
     start_y is turned on.

     If x or y are out of range the function just returns without doing
     anything.
  */
  void MaxPixel(uint8_t x, // 0 to WIDTH - 1
                uint8_t y);// 0 to HEIGHT. If 0, nothing is displayed.

  ~Spectrum_display();

private:
  NeoPixelMatrix *neo_matrix;
  uint8_t START_X;
  uint8_t START_Y;
  uint8_t WIDTH;
  uint8_t HEIGHT;

  uint8_t START_RED;
  uint8_t START_GREEN;
  uint8_t START_BLUE;

  uint8_t END_RED;
  uint8_t END_GREEN;
  uint8_t END_BLUE;

  bool LINGER;
  unsigned long LINGER_TIME;
  uint8_t MAX_VOLUME_RED;
  uint8_t MAX_VOLUME_GREEN;
  uint8_t MAX_VOLUME_BLUE;

  struct max_pixel_info {
    unsigned long live_until;
    uint8_t y;
  };
  max_pixel_info *last_max_y;

  // The spectrum matrix y starts at 1, while the NeoPixelMatrix starts at 0.
  // Additionally the spectrum matrix can be anywhere on the NeoPixelMatrix.
  // This function does the translation between the two sets of coordinates.
  void setPixelColor(uint8_t x, uint8_t y,
                     uint8_t red, uint8_t green, uint8_t blue);
};
