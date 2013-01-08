/*
  scream
  
  Sonar detector
  When someone approaches the punkin, Wilhelm scream.
  Also, since nobody ever comes to the door, just make it scream sometimes anyway.

  WilhelmScream from
    http://archive.org/details/WilhelmScreamSample
  
  To downsample to 16kHz 8 bit:
    sox WilhelmScream.wav -b 8 -t raw WilhelmScream16k8b.dat rate -v -I 16000 remix -
  
  To dump as header file:
    python ./dump.py WilhelmScream16k8b.dat scream.h scream
 
  To playback wav:
  - Make fastest-possible PWM (62kHz) on Timer2
  - Make a 16kHz clock from Timer1, use this to set the PWM overflow register
  based on PCMAudio
    http://www.arduino.cc/playground/Code/PCMAudio
  also see this, nice
    http://elasticsheep.com/2010/06/teensy2-usb-wav-player-%E2%80%93-part-2/
*/

#include <avr/pgmspace.h>

/* Table of wave data */
#include "scream.h"
#define WAVEDATA     scream
#define WAVEDATA_LEN scream_len

/* Some fade-in-and-out time to ramp from 0v to Vcc/2 */
const int fade_len = 127;
const int total_len = WAVEDATA_LEN + fade_len + fade_len;

int distance = 200;
int analogPin = 0;
int ledPin = 13; // blink for debug
int speakerPin = 11;

// variables used inside interrupt service declared as voilatile
volatile int icnt;               // var inside interrupt
volatile byte screaming;
float d = 0;

void setup()
{
  Serial.begin(115200);
//  analogReference(EXTERNAL);

  pinMode(5,INPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7,0);
  digitalWrite(6,1);
  
  //analogReference(INTERNAL);
  //pinMode(analogPin,INPUT);
  //pinMode(ledPin,OUTPUT);
//  startPlayback();
}

long since_last_scream = 0;
long timeout1 = 5000;

void loop()
{
  if( !screaming )
  {
    since_last_scream++;
  }
  /*
  // Read the sonar distance detector
  int n = analogRead(analogPin);         // 1 inch = Vcc/512.  Approximately, 1" =  per inch
  float d1 = ((float)n)/2;       // distance, inches
  d = ( 19*d + d1 ) / 20;        // moving average

  
  Serial.print(n,DEC);
  Serial.print(" ");
  Serial.println(d,DEC);
  
  // Scream if too close
  if( since_last_scream > 1000000 )
  {
    if( !screaming ) startPlayback( 1 );
    screaming = 1;
    since_last_scream = 0;
  }
  if( d < distance && since_last_scream > timeout1 )
  {
    if( !screaming ) startPlayback( 0 );
    screaming = 1;
    since_last_scream = 0;
    timeout1 = 2000+random(10000);
  }
  */
  byte detected = digitalRead(5);
  Serial.print(detected,DEC);
  Serial.print(" ");
  Serial.println(screaming,DEC);
  
  if( detected==1 && since_last_scream > timeout1 )
  {
    since_last_scream = 0;
    timeout1 = 2000+random(10000);
    if( !screaming ) startPlayback( 0 );
    screaming = 1;
  }

  digitalWrite( ledPin, screaming );
  
}



void startPlayback( int fast )
{
  pinMode(speakerPin, OUTPUT);

  // Set up Timer 2 to do Fast PWM

  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));

  // Set fast PWM mode  (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);

  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));

  // No prescaler (p.158)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  // Set initial pulse width to 0.
  OCR2A = 0;


  // Set up Timer 1 to send a sample every interrupt.

  cli();

  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

  // No prescaler (p.134)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  if( fast==1 )
    OCR1A = random(200,1000);
  else
    OCR1A = random(800,5000); // F_CPU / SAMPLE_RATE.  So nominally 16kHz => 1000

  // Enable interrupt when TCNT1 == OCR1A (p.136)
  TIMSK1 |= _BV(OCIE1A);

  icnt = 0;
  sei();
}

void stopPlayback()
{
  // Disable playback per-sample interrupt.
  TIMSK1 &= ~_BV(OCIE1A);

  // Disable the per-sample timer completely.
  TCCR1B &= ~_BV(CS10);

  // Disable the PWM timer.
  TCCR2B &= ~_BV(CS10);

  digitalWrite(speakerPin, LOW);
  screaming = 0;
}


// ISR for the 16kHz audio clock
// Load the sample into OCR
ISR(TIMER1_COMPA_vect)
{
  if (icnt >= total_len)
    stopPlayback();
    else if( icnt < fade_len )
    OCR2A = icnt;
  else if( icnt > WAVEDATA_LEN + fade_len )
    OCR2A = fade_len - ( icnt - WAVEDATA_LEN - fade_len );
  else
    OCR2A = pgm_read_byte(&WAVEDATA[icnt]);
    
  icnt++;
}


