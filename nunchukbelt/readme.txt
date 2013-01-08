LED belt controlled by a Nunchuk.

Hardware
========

The belt is a metre of Adafruit addressable LED strip (https://www.adafruit.com/products/306).
This is powered by 5V, from 4xAA rechargeable batteries.  For safety, and to drop 0.5v or so,
there's a IN4003 diode in line with the +ve lead.

So far: this looks exactly like the Adafruit LED Belt Kit (http://www.ladyada.net/make/ledbelt/).

But we used regular Arduino, instead of the Atmega breakout board.  Just because they were handy.
The Arduino itself needs more than 5v power supply, so we used a separate 9v battery.

To connect the belt:
- 5v from the battery pack (via diode).
- Data to Arduino pin 11
- Clock to Arduino pin 13
- Ground to Arduino gnd.
These pins 11/13 are "Hardware SPI" interface.  See the LPD8806 library for details - it's OK
to use different pins, but these can run faster.

Plus, we add a Wii Nunchuk.  The Nunchuk contains an accelerometer (sensitive to movement) and
a joystick and a couple switches.  We chopped the connector off the Nunchuck and soldered up to
Arduino pins A4 and A5, and ground and +3.3v.

Software
========

This Arduino sketch is mostly based on the Adafruit sample code ("advancedLEDbeltKit" in the
LPO8806 library, which you can download from https://github.com/adafruit/LPD8806).  That runs
the belt with several patterns and randomly fades between them.

We added a new new patterns:
- a gentle smoothly-fading color-chase (my favorite!),
- a Knight Rider-style moving pattern (aka "Larson scanner"),
- a "dither" (lifted from LEDbeltKit),
- a bouncy VU meter

Then, there's less randomness, and more Nunchuk.
Approximately:
- The accelerometer detects "bounce", and uses that to flash the LED pattern, so it flashes
   nicely in time with your dancing.
- The "C" button switches between "mostly on, dancing flashes the lights off" and "mostly off,
  bouncing flashes the lights on".
- The "Z" button switches to a new pattern (randomly selecting one).
- In a couple of the modes, the Nunchuk tilt angle controls color.
- In several of the modes, the Nunchuck joystick (when off center) controls color.

