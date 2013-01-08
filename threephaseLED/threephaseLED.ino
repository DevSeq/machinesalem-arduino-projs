/*  2012-05-07 @machinesalem,  (cc) https://creativecommons.org/licenses/by/3.0/

  "Three phase LED"
  
  Makes a smooth three-phase curve, approximating https://en.wikipedia.org/wiki/File:3_phase_AC_waveform.svg
  See 'data.xls' for some sample output, captured from the serial port and charted.
  The algorithm probably has a name, but I just call it "low-budget sinewaves".
  This is a three-phase version, because the LED has three colors!
  A two-phase version is even simpler, and can be used to draw quite good circles very fast, like this:
  loop() {
    x = x + d*y;
    y = y - d*x;
    }
*/

int pinR = 9;    // LED connected to digital pins 9 thru 11 (red, green, blue)
int pinG = 10;
int pinB = 11;

// The main values are floating point numbers range +/- 1.0
// (For ultimate performance, you could use long-integers and scale them down).
float valR = 1;
float valG = -0.5;
float valB = -0.5;

// The algorithm is surprisingly robust to value of 'd'
// Smaller values mean slower change!  Also you can control speed by varying the delay of course.
float d = 0.01;

// integer values that are output to the PWM pins
int vR;
int vG;
int vB;
int brightness = 30;  /* max 127 */

void setup()  {
  // Serial monitor is just to see the values, for debugging!
  Serial.begin(19200);
}

void loop()  {
  delay(10);

  valR = valR -(d * valG) +(d * valB);
  valG = valG +(d * valR) -(d * valB);
  valB = valB -(d * valR) +(d * valG);

  vR = brightness * (1+valR);
  vG = brightness * (1+valG);
  vB = brightness * (1+valB);

  Serial.print( vR );
  Serial.print( ", " );
  Serial.print( vG );
  Serial.print( ", " );
  Serial.println( vB );

  analogWrite(pinR, vR);
  analogWrite(pinG, vG);
  analogWrite(pinB, vB);
}

