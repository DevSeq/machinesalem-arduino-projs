/*  2012-05-11 @machinesalem,  (cc) https://creativecommons.org/licenses/by/3.0/

  "Three out-of-phase LED"
  
  Makes three independent sinewaves, of slightly different periods.
  Unlike the three-phase sketch, this covers the complete colorspace,
  and it's constantly varied.
  In this version, brightness is squared, which makes for richer colors.
*/

int pinR = 9;    // LED connected to digital pins 9 thru 11 (red, green, blue)
int pinG = 10;
int pinB = 11;

// The main values are floating point numbers range +/- 1.0
float valRx = 1, valRy = 0;
float valGx = 1, valGy = 0;
float valBx = 1, valBy = 0;

float dR = 0.01;
float dG = 0.011;
float dB = 0.012;

// integer values that are output to the PWM pins
int vR;
int vG;
int vB;
int brightness = 43; // max 63

void setup()  {
  // Serial monitor is just to see the values, for debugging!
  Serial.begin(19200);
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);
}

void loop()  {
  delay(60);

  valRx = valRx + (dR * valRy);  valRy = valRy - (dR * valRx);
  valGx = valGx + (dG * valGy);  valGy = valGy - (dG * valGx);
  valBx = valBx + (dB * valBy);  valBy = valBy - (dB * valBx);

  vR = brightness * (1+valRx);
  vG = brightness * (1+valGx);
  vB = brightness * (1+valBx);

/*
  Serial.print( vR );
  Serial.print( ", " );
  Serial.print( vG );
  Serial.print( ", " );
  Serial.println( vB );  
*/
  analogWrite(pinR, vR);
  analogWrite(pinG, vG);
  analogWrite(pinB, vB);
}

