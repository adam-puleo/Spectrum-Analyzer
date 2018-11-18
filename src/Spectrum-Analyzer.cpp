/*
 * Project Spectrum-Analyzer
 * Description: Displays two spectrum displays. One for the left and right
 *              channel frequencies, and one for the left and right volume.
 * Author: Adam Puleo (adam.puleo@icloud.com)
 * Date: 2018-11-17
 */

#include "Particle.h"
#include "msgeq7.h"
#include "neopixelmatrix.h"
#include "spectrum_display.h"

// Two MSGEQ7 chips, one for the left channel, and one for the right.
Msgeq7 msgeq7s(D0, D1, A0, A1);

// The Flexible LED Matrix - WS2812B (COM-13304) from sparkfun.com.
const uint8_t MAX_WIDTH = 32;
const uint8_t MAX_HEIGHT = 8;

// This is the NeoPixelMatrix
NeoPixelMatrix matrix(MAX_WIDTH, MAX_HEIGHT, D2, WS2812B);

// Display to show the frequencies.
Spectrum_display spectrum_matrix(&matrix, 0, 0, 28, MAX_HEIGHT,
                                 0x00, 0x00, 0x0F, // Blue - low volume
                                 20, 20, 20, //Grey - High volume
                                 true,
                                 1000,
                                 0x7F, 0x52, 0x00) // Orange - linger indicator
                                 //66, 66, 99);
                                 //0x0F, 0x00, 0x00); //Red

// Display to show the volume.
Spectrum_display volume_matrix(&matrix, 28, 0, 4, MAX_HEIGHT,
                               0x25, 0x00, 0x41, // Purple
                               0x0F, 0x00, 0x00
                               true,
                               1000,
                               0x0F, 0x00, 0x00); //Red

void setup() {
  msgeq7s.begin();
  matrix.begin();
  matrix.clear();
  matrix.show();
}

void loop() {
  msgeq7s.read();

  for (uint8_t freq = msgeq7s.FREQ63; freq <= msgeq7s.FREQ16k; freq++) {
    for (uint8_t channel = 0; channel < 2; channel++) {
      uint8_t x = (freq * 2 + channel) * 2;
      uint8_t y = (msgeq7s.get(channel, static_cast<Msgeq7::FREQS>(freq)) * MAX_HEIGHT) / msgeq7s.MAX_VALUE;
      spectrum_matrix.MaxPixel(x, y);
      spectrum_matrix.MaxPixel(x + 1, y); // Each frequency band receives two vertical bars, just to fill things up.
    }
  }

  uint8_t volume = (msgeq7s.get_volume(0) * MAX_HEIGHT) / msgeq7s.MAX_VALUE;
  volume_matrix.MaxPixel(0, volume);
  volume_matrix.MaxPixel(1, volume);

  volume = (msgeq7s.get_volume(1) * MAX_HEIGHT) / msgeq7s.MAX_VALUE;
  volume_matrix.MaxPixel(2, volume);
  volume_matrix.MaxPixel(3, volume);

  matrix.show();
}
