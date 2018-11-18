# Spectrum-Analyzer
Uses the MSGEQ7 chips and a NeoPixel matrix to display frequency volumes using a Particle Photon.

This project uses two MSGEQ7 chips (COM-10468 on SparkFun.com) to analyze sound frequencies. One chip for the left, and the other chip for the right channel. Input is via a 3.5mm audio jack. The frequency volumes, and average volume is displayed with a NeoPixel matrix (COM-13304 on SparkFun.com). Data is processed using Particle's Photon (particle.io).

The Photon uses 3.3 volts on its analog and digital pins. The MSGEQ7 and NeoPixels are 5 volts. A SN74HCT125N is used to drive the inputs on the MSGEQ7 and NeoPixel from the Photon. To read the MSGEQ7s output a voltage divider circuit is used. The MSGEQ7 seems to output 4.8 volts when a frequency is at maximum volume thus a 1.5k ohm resistor is used.

Pin eight of the SN74HCT125N is used to drive the NeoPixel matrix. Gate B, output pin 6 of the SN74HCT125N is not used.
