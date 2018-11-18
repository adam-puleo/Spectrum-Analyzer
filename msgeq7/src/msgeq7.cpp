/* msgeq7 library by Adam Puleo
 */

#include "msgeq7.h"

void Msgeq7::init(uint8_t resetPin, uint8_t strobePin) {
  // Setup private variables
  this->resetPin = resetPin;
  this->strobePin = strobePin;
}

/**
 * Constructors
 */
 Msgeq7::Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin) {
   this->numChannels = 1;
   this->analogPins[0] = analogPin;

   init(resetPin, strobePin);
 }

 Msgeq7::Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2) {
   this->numChannels = 2;
   this->analogPins[0] = analogPin;
   this->analogPins[1] = analogPin2;

   init(resetPin, strobePin);
 }

 Msgeq7::Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2, uint8_t analogPin3) {
   this->numChannels = 3;
   this->analogPins[0] = analogPin;
   this->analogPins[1] = analogPin2;
   this->analogPins[2] = analogPin3;

   init(resetPin, strobePin);
 }

 Msgeq7::Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin, uint8_t analogPin2, uint8_t analogPin3, uint8_t analogPin4) {
   this->numChannels = 4;
   this->analogPins[0] = analogPin;
   this->analogPins[1] = analogPin2;
   this->analogPins[2] = analogPin3;
   this->analogPins[3] = analogPin4;

   init(resetPin, strobePin);
 }

/**
 * Initialize hardware
 */
void Msgeq7::begin()
{
    // Initialize the Photon pins
    pinMode(this->resetPin, OUTPUT);
    pinMode(this->strobePin, OUTPUT);
    for (uint8_t channel = 0; channel < this->numChannels; channel++) {
      pinMode(this->analogPins[channel], AN_INPUT);
    }

    // Setup the MSGEQ7
    digitalWrite(this->resetPin, LOW);
    digitalWrite(this->strobePin, LOW);
}

void Msgeq7::read() {
  // With every read of the MSGEQ7 the output decays by 10%. Thus on every read
  // send a reset.
  digitalWrite(this->resetPin, HIGH); // tr - Needs to stay high for at least 100ns.
  digitalWrite(this->strobePin, LOW); // ts - Needs to stay high for at least 18us.
  delayMicroseconds(18);
  digitalWrite(this->resetPin, LOW);
  digitalWrite(this->strobePin, HIGH); // tss - One period must be at least 72us. So at this point the strobe pin needs to stay high at lease another 54us.
  delayMicroseconds(54);

  // read all 7 frequencies
	// 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
	for (uint8_t freq = FREQ63; freq < FREQ16k + 1; freq++) {
		// trigger next value
		digitalWrite(this->strobePin, LOW);

		// allow the output to settle - 'to' 36us min
		delayMicroseconds(36);

    for (uint8_t channel = 0; channel < this->numChannels; channel++) {
      this->frequencies[channel][freq] = analogRead(this->analogPins[channel]);
    };

    digitalWrite(this->strobePin, HIGH);
    /* Strobe to strobe delay (tss) is 72us min. Could probably shorten this
       as analogRead takes awhile. I'm not sure how long though.
    */
    delayMicroseconds(36);
  };
  digitalWrite(this->strobePin, LOW);

  // Recalculate the volume for each channel.
  for (uint8_t channel = 0; channel < this->numChannels; channel++) {
    uint32_t sum = 0;
    for (uint8_t freq = FREQ63; freq < FREQ16k + 1; freq++) {
      sum += this->frequencies[channel][freq];
    }
    this->volume[channel] = sum / (FREQ16k + 1);
  }
};

// Return the value read by the Photon for the frequency for the requested IC.
uint16_t Msgeq7::get(const uint8_t channel, const FREQS freq) {
  if (channel < this->numChannels && freq <= FREQ16k) {
    return this->frequencies[channel][freq];
  } else {
    return 0;
  }
}

// Return the channel's volume. The volume is calculated in the read function.
uint16_t Msgeq7::get_volume(const uint8_t channel) {
  if (channel < this->numChannels) {
    return this->volume[channel];
  } else {
    return 0;
  }
}

/**
 * Destructor
 */
Msgeq7::~Msgeq7()
{
    // Set the digital pins back to input.
    pinMode(this->resetPin, INPUT);
    pinMode(this->strobePin, INPUT);

    // The analoog pins were already set to input, nothing to do for them.
}
