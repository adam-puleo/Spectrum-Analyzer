/* spectrum_display library by Adam Puleo
 */

#include "spectrum_display.h"
#include <math.h>

// The following gradient code was found here: https://codepen.io/njmcode/pen/axoyD?editors=0010
struct rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct hsl {
  float hue;
  float saturation;
  float lightness;
};

float hue2rgb(float p, float q, float t) {
  if (t < 0.0) t += 1.0;
  if (t > 1.0) t -= 1.0;
  if (t < 1.0/6.0) return p + (q - p) * 6.0 * t;
  if (t < 1.0/2.0) return q;
  if (t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
  return p;
}

rgb hsl2rgb(float hue, float saturation, float lightness) {
  rgb result;
  float l = lightness;

  if (saturation == 0.0) {
    l = round(l*255.0);
    result.red = l;
    result.green = l;
    result.blue = l;
    return result;
  } else {
    float s = saturation;
    float q = (l < 0.5 ? l * (1.0 + s) : l + s - l * s);
    float p = 2.0 * l - q;
    float r = hue2rgb(p, q, hue + 1.0/3.0);
    float g = hue2rgb(p, q, hue);
    float b = hue2rgb(p, q, hue - 1.0/3.0);
    result.red = round(r * 255.0);
    result.green = round(g * 255.0);
    result.blue = round(b * 255.0);
    return result;
  }
};

hsl rgb2hsl(uint8_t start_r, uint8_t start_g, uint8_t start_b) {
  hsl result;

  float r_percent = start_r/255.0;
  float g_percent = start_g/255.0;
  float b_percent = start_b/255.0;

  float max = fmaxf(fmaxf(r_percent, g_percent), b_percent);
  float min = fminf(fminf(r_percent, g_percent), b_percent);
  float h = (max + min) / 2.0;
  float s = h;
  float l = h;

  if (max == min) {
    h = s = 0.0; // achromatic
  } else {
    float d = max - min;
    s = (l > 0.5 ? d / (2.0 - max - min) : d / (max + min));
    if (max == r_percent) h = (g_percent - b_percent) / d + (g_percent < b_percent ? 6.0 : 0.0);
    if (max == g_percent) h = (b_percent - r_percent) / d + 2.0;
    if (max == b_percent) h = (r_percent - g_percent) / d + 4.0;
    h /= 6.0;
  }

  result.hue = h;
  result.saturation = s;
  result.lightness = l;
  return result;
};


rgb interpolateHSL(hsl start, hsl end, float factor = 0.5) {
  return hsl2rgb(start.hue        + factor * (end.hue        - start.hue),
                 start.saturation + factor * (end.saturation - start.saturation),
                 start.lightness  + factor * (end.lightness  - start.lightness));
};

// End of code from https://codepen.io/njmcode/pen/axoyD?editors=0010

void Spectrum_display::setPixelColor(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue) {
  this->neo_matrix->setPixelColor(x + this->START_X, y - 1 + this->START_Y, red, green, blue);
}

Spectrum_display::Spectrum_display(NeoPixelMatrix *matrix,
                                   uint8_t start_x,
                                   uint8_t start_y,
                                   uint8_t width,
                                   uint8_t height,
                                   uint8_t start_red,
                                   uint8_t start_green,
                                   uint8_t start_blue,
                                   uint8_t end_red,
                                   uint8_t end_green,
                                   uint8_t end_blue,
                                   bool    linger_max_volume,
                                   unsigned long linger_time,
                                   uint8_t max_volume_red,
                                   uint8_t max_volume_green,
                                   uint8_t max_volume_blue)
{
  this->neo_matrix = matrix;
  this->START_X = start_x;
  this->START_Y = start_y;
  this->WIDTH = width;
  this->HEIGHT = height;

  this->START_RED = start_red;
  this->START_GREEN = start_green;
  this->START_BLUE = start_blue;

  this->END_RED = end_red;
  this->END_GREEN = end_green;
  this->END_BLUE = end_blue;

  this->LINGER = linger_max_volume;
  this->LINGER_TIME = linger_time;
  this->MAX_VOLUME_RED = max_volume_red;
  this->MAX_VOLUME_GREEN = max_volume_green;
  this->MAX_VOLUME_BLUE = max_volume_blue;

  if (linger_max_volume) {
    this->last_max_y = (max_pixel_info *)malloc(width * sizeof(max_pixel_info));
    if (this->last_max_y) {
      memset(this->last_max_y, 0, width * sizeof(max_pixel_info));
    } else {
      this->last_max_y = NULL;
    }
  } else {
    this->last_max_y = NULL;
  }
}

void Spectrum_display::MaxPixel(uint8_t x, uint8_t y) {
  if (x >= this->WIDTH || y > this->HEIGHT) {
    return;
  }

  if (y > 0) {
    // Color gradient up to and including the max pixel.
    rgb icol;
    hsl hsl_start = rgb2hsl(this->START_RED, this->START_GREEN, this->START_BLUE);
    hsl hsl_end = rgb2hsl(this->END_RED, this->END_GREEN, this->END_BLUE);
    float factor_step;
    if (y == 1) {
      factor_step = 0.0;
    } else {
      factor_step = 1.0 / (y - 1.0);
    }
    for (uint8_t current_y = 1; current_y <= y; current_y++) {
      icol = interpolateHSL(hsl_start, hsl_end, factor_step * (current_y - 1));
      setPixelColor(x, current_y, icol.red, icol.green, icol.blue);
    }
  }

  // Clear the remaining pixels;
  uint8_t stop;
  if (LINGER) {
    // up to, but not the previous max pixel.
    stop = this->last_max_y[x].y;
  } else {
    // all of the remaining pixels. For loop is just less then.
    stop = this->HEIGHT + 1;
  }
  for (uint8_t current_y = y + 1; current_y < stop; current_y++) {
      setPixelColor(x, current_y, 0, 0, 0);
  }

  if (this->LINGER) {
    if (millis() > this->last_max_y[x].live_until) {
      // Time has expired, update the max pixel.
      if (y < this->last_max_y[x].y) {
        setPixelColor(x, this->last_max_y[x].y, 0, 0, 0);
      }
      this->last_max_y[x].live_until = millis() + this->LINGER_TIME;
      this->last_max_y[x].y = y;
    } else if (y > this->last_max_y[x].y) {
      // Beat it, time to update it. Don't change the max pixel color.
      this->last_max_y[x].live_until = millis() + this->LINGER_TIME;
      this->last_max_y[x].y = y;
    } else if (y < this->last_max_y[x].y) {
      // Volume has dropped below the max pixel, turn the max pixel on.
      setPixelColor(x, this->last_max_y[x].y, this->MAX_VOLUME_RED, this->MAX_VOLUME_GREEN, this->MAX_VOLUME_BLUE);
    }
  }
}

Spectrum_display::~Spectrum_display() {
  if (this->last_max_y) {
    free(this->last_max_y);
  }
}
