#pragma once

/* msgeq7 library by Adam Puleo based on https://github.com/NicoHood/MSGEQ7
   MSGEQ7 datasheet: http://www.mix-sig.com/index.php/msgeq7-seven-band-graphic-equalizer-display-filter

   The MSGEQ7 chip analyzes a signal for the following frequencies: 63Hz, 160Hz,
   400Hz, 1kHz, 2.5kHz, 6.25kHz, and 16kHz.

   For each frequency band the MSGEQ7 outputs a voltage between 0 and 5 volts.
   Since the Photon's analog pins only operate on 0 - 3.3 volts you will need
   a voltage divider circuit between the MSGEQ7 and the Photon.

   The Photon returns a value between 0 and 4095 depending on the Phonto's input
   voltage.
 */

#include "Particle.h"

class Msgeq7 {
  public:
     // Frequency bands
     enum FREQS {FREQ63 = 0, FREQ160, FREQ400, FREQ1k, FREQ25k, FREQ625k, FREQ16k};

     // The maximum value that get and get_volume returns.
     const uint16_t MAX_VALUE = 4095;

     /*
      Call the appropriate constructor for how many MSGEQ7 ICs are connected to
      the Photon. For multiple MSGEQ7 ICs connect the reset pins to the same
      digital pin on the Photon. Same for the strobe pin.
     */
     Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin);
     Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2);
     Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2, uint8_t analogPi3);
     Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2, uint8_t analogPin3, uint8_t analogPin4);

    // Call this in your program's setup function.
    void begin();

    /* Reads the frequency bands from the chip(s) and stores the results in the
       private variable 'frequencies'.
    */
    void read();

    /* Retrieve a frequency's value that was read from one of the MSGEQ7 ICs.
       To read the first analog pin pass zero in channel; the second pin one,
       etc. Returns zero if channel or frequency is not valid.
    */
    uint16_t get(const uint8_t channel, const FREQS freq);

    /* Get a channel's (IC's) volume. The volume is calculated by taking an
       average of the frequency bands. Returns zero if the channel is invalid.
    */
    uint16_t get_volume(const uint8_t channel);

    // Destructor
    ~Msgeq7();

  private:
    uint8_t resetPin,
            strobePin,
            numChannels, // Number of ICs connected to the Photon.
            analogPins[4]; // Array to hold which analog pin each IC is connected to.

    uint16_t frequencies[4][FREQ16k + 1]; // Holds the values read for each frequency for each IC.
    uint16_t volume[4]; // Holds the volume for each IC.

    // Used by the constructors to setup the private variables.
    void init(uint8_t resetPin, uint8_t strobePin);
};
