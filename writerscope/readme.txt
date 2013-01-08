"writerscope"
Arduino Vector-text-writer-oscilloscope

This arduino writes with TrueType fonts, on an oscilloscope.
See:
  photo(1).JPG
  photo(2).JPG
The font used here is "black ops one", from Google Fonts, which you can see on the homepage
  http://machinesalem.com/


It uses arduino digital out, with an integrator connected to channels X and Y of an oscilloscope,
to write lettering derived from TrueType font outline vectors.


The simple way to do this would be with analogWrite(), and then a capacitor to smooth the pulse-width-modulated output
before it hits the oscilloscope.  But that limits how complex your vector drawing can be -- the default PWM signal is
approximately 490Hz, which means that a drawing with several hundred lines would only draw every second.  For a regular
X-Y oscilloscope, you want to draw the whole picture at least a couple dozen times per second.

To do this, we used a simple approach:  hold pins "high" or "low" for a period of time corresponding to the distance.
An integrator converts this on/off into a smooth voltage ramp.  The slope of the ramp depends on the time-constant of
the integrator, which depend on the resistor and capacitor used to build the circuit.

The integrator is an opamp (one channel for each X and Y), in this prototype TLE2071 (pick one that will work with a 5v rail).

               +---||---+
               |   1u5  |
pin5 ---[1k0]--+        |
               | |\     |
pin9 ---[10k]--+-|-\    |
                 |  \___|___ to scope
                 |  /
          vRef---|+/
                 |/
         
where vRef is half vCC, derived with a couple resistors.
So pin5 is a "ten times faster" version of pin9.  Set them high for a short period, and you get a straight line on the scope.
Same thing for the "Y" scope channel, using pin6 and pin10.


The vector text is derived from a TrueType font using the code in black_ops_one_regular.writerscope.js,
which is generated using typeface.js (http://typeface.neocracy.org/) and then reformatted into the Arduino syntax.
From a command prompt,
  cscript -nologo black_ops_one_regular.writerscope.js  > include_me.txt
and then pasted into the arduino program.

**TODO:
- Wire up a "Z" (turning the beam on-off) for scopes that have that
- Build in a "Reset" to avoid integrator-drift
- Use a smaller capacitor = faster speed, this is about 10x too slow really
- Listen for a data stream (vector-instructions in the same format as data[]) on the serial port.  Draw it right away.
- Build a node.js server that runs a webpage, where you type stuff, and then sends outline data to the arduino.
- Use interrupts to draw the data, instead of the regular loop.
- Figure out a reliable way to "moveto" as well as the "lineto".  (Or just punt, if "Z" is done).
- Implement bezier and quadratic curves.
- Drive a laser-projector instead of an oscilloscope.  Project onto the old town hall.
  (Can you suggest galvaometer/mirror components to do that?)

