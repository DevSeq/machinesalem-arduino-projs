
// MIDI input
#include <MIDI.h>

// Control-voltage provided by SPI DAC
#include <SPI.h>
#include <TLV5618.h>

#define DAC_CS_PIN     10

// Assign the DAC chip select pin using the value defined above.

TLV5618 dac  = TLV5618(DAC_CS_PIN);


// MIDI channel default to "omni" (listen to everything)
byte channel = MIDI_CHANNEL_OMNI;


uint16_t a, b;
byte m;
byte lastNote;
const byte n = 16;

void setup() 
{
  a=0;
  b=0;
  lastNote = 0;
  
  // TODO read channel from dip-switches
  
  MIDI.begin( channel );
  
  dac.begin();
}
  
void loop()
{
//  a = 4*analogRead(0);
  b = 4*analogRead(1);
  
//  a = 500 + (a/8);

  // Is there an incoming MIDI message?  
  if (MIDI.read())
  {                    // Is there a MIDI message incoming ?
    byte mt = MIDI.getType();
    
    if( mt==NoteOn )
    {
      lastNote = MIDI.getData1();
      a = 34 * lastNote;
    }
      
    if( mt==NoteOff )
    {
      if( lastNote == MIDI.getData1() )
        a = 0;
    }
      
    if( mt==ControlChange )
    {
      // CC 74 = cutoff frequency
      // CC 71 = resonance
      // CC 91 = reverb level
      byte c = MIDI.getData1();
      byte n = MIDI.getData2();
      if( c == 74 ) b = n;
    }
  }

  
  dac.write(a,b);
  delay(10);
}

