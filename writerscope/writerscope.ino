/* ============== paste here from include_me.txt ======================= */
// text: machine
const int M = 1; /* moveto */
const int L = 2; /* lineto */
const int Q = 3; /* quadratic_curve_to */
const int B = 4; /* bezier curve to */
const int data[] = {M,357,-6,L,377,-6,L,377,-19,L,362,-35,L,332,-35,L,309,-11,L,309,81,L,357,81,L,357,-6,M,433,-6,L,452,-6,L,452,-20,L,437,-35,L,400,-35,L,384,-19,L,384,81,L,433,81,L,433,-6,M,508,-35,L,460,-35,L,460,81,L,508,81,L,508,-35,
M,210,-6,L,279,-6,L,279,-20,L,265,-35,L,192,-35,L,163,-6,L,163,81,L,210,81,L,210,-6,M,289,32,L,269,13,L,218,13,L,218,34,L,241,34,L,241,54,L,218,54,L,218,67,L,232,81,L,269,81,L,289,62,L,289,32,
M,87,-35,L,46,-35,L,17,-6,L,17,12,L,65,12,L,65,-6,L,87,-6,L,87,-35,M,143,-6,L,114,-35,L,94,-35,L,94,81,L,114,81,L,143,52,L,143,-6,M,87,54,L,65,54,L,65,40,L,17,40,L,17,52,L,46,81,L,87,81,L,87,54,
M,-3,-79,L,-51,-79,L,-51,81,L,-3,81,L,-3,-79,M,-80,-6,L,-58,-6,L,-58,-20,L,-73,-35,L,-99,-35,L,-128,-6,L,-128,81,L,-80,81,L,-80,-6,
M,-150,-80,L,-198,-80,L,-198,-49,L,-150,-49,L,-150,-80,M,-150,-35,L,-198,-35,L,-198,81,L,-150,81,L,-150,-35,
M,-298,-6,L,-278,-6,L,-278,-19,L,-294,-35,L,-321,-35,L,-347,-9,L,-347,81,L,-298,81,L,-298,-6,M,-221,-35,L,-270,-35,L,-270,81,L,-221,81,L,-221,-35,
M,-424,14,L,-447,14,L,-447,-6,L,-424,-6,L,-424,-35,L,-465,-35,L,-494,-6,L,-494,36,L,-424,36,L,-424,14,M,-368,-6,L,-397,-35,L,-416,-35,L,-416,81,L,-397,81,L,-368,52,L,-368,-6,M,-424,54,L,-489,54,L,-489,67,L,-474,81,L,-424,81,L,-424,54};
/* ====================================================================== */

/*  2012-07-06 @machinesalem,  (cc) https://creativecommons.org/licenses/by/3.0/

  "writerscope"
  
  Vector-text-writer-oscilloscope
  Uses arduino digital out, with an integrator connected to channels X and Y of an oscilloscope,
  to write lettering derived from TrueType font outline vectors.
  
  The integrator is an opamp (one channel for each X and Y), in this prototype TLE2071 (pick one that will work with a 5v rail).
  
                 +---||---+
                 |   1u5  |
  pin9 ---[10k]--+-|\     |
                   | >----+---- to scope
            vRef---|/
           
  where vRef is half vCC, derived with a couple resistors.
  Set pin9 ("X") high for a short period, and you get a horizontal straight line on the scope.
  Same thing for the "Y" scope channel, using pin10, making vertical lines.
  
  The "Z" channel controls whether the beam is on or off.
  
  The vector text (in the "data[]" array above) is derived from a TrueType font using the code in black_ops_one_regular.writerscope.js,
  which is generated using typeface.js (http://typeface.neocracy.org/) and then reformatted into the Arduino syntax.
  From a command prompt,
    cscript -nologo black_ops_one_regular.writerscope.js  > include_me.txt
  and then pasted in here.
  Eventually I'd like to have arduino listen for data in this format on the serial port and just draw it, and have a server with
  node.js to run the text-to-outline typeface.js thing and talk to the arduino over serial...
    
*/


// A few test shapes
//const unsigned int data[] = { M,100,50, L,150,100, L,100,150, L,50,100, L,100,50, L,50,50, M,0,0, L,400,0, L,400,400, L,0,400, L,0,0 };
//const unsigned int data[] = { M,100,50, L,150,100, L,100,150, L,50,100, L,100,50, L,50,50 };
//const unsigned int data[] = { M,1,1, L,500,1, L,500,500, L,1,500 };

const int R = 0; /* reset */

unsigned long tick = micros();

// code
const byte pinx = 9;
const byte piny = 10;
const byte pinz = 12;

int x = 0;
int y = 0;
int instr = 0;
int tgt_x = 0;
int tgt_y = 0;
unsigned int point = 0;
unsigned int length = sizeof(data)/sizeof(int);
const boolean debug = false;

unsigned long dx = 0;
unsigned long dy = 0;
unsigned long dd = 0;
unsigned long d = 0;
/* Scale, millis per point (so, higher values mean slower refresh speed) */
unsigned int sx = 2;
unsigned int sy = 4;

const int BEAM_OFF = HIGH;
const int BEAM_ON = LOW;

void setup()
{
  Serial.begin(19200);
  pinMode( pinx, INPUT ); digitalWrite( pinx, LOW );
  pinMode( piny, INPUT ); digitalWrite( piny, LOW );
  pinMode( pinz, OUTPUT ); digitalWrite( pinz, BEAM_OFF );
}


void loop()
{
  dx = dx * sx;
  dy = dy * sy;    
  d = (dx>dy) ? dx : dy;
  
  dd = micros()-tick;

  // Set the beam on or off
  if(instr==R || instr==M) { pinMode( pinz, OUTPUT ); digitalWrite( pinz, BEAM_OFF ); }
  else                     { pinMode( pinz, OUTPUT ); digitalWrite( pinz, BEAM_ON ); }

  // Line to the target (horizontally or vertically or diagonally - no smart lines or curves yet!)
  if( tgt_x<x )      { pinMode( pinx, OUTPUT ); digitalWrite( pinx, LOW ); }
  else if( tgt_x>x ) { pinMode( pinx, OUTPUT ); digitalWrite( pinx, HIGH ); }
  else               { pinMode( pinx, INPUT );  digitalWrite( pinx, LOW ); }
  
  if( tgt_y<y )      { pinMode( piny, OUTPUT ); digitalWrite( piny, LOW ); }
  else if( tgt_y>y ) { pinMode( piny, OUTPUT ); digitalWrite( piny, HIGH ); }
  else               { pinMode( piny, INPUT );  digitalWrite( piny, LOW ); }

  // Wait long enough
  while(dd<d)
  {
    if(dd>=dx) { pinMode( pinx, INPUT ); digitalWrite( pinx, LOW ); };
    if(dd>=dy) { pinMode( piny, INPUT ); digitalWrite( piny, LOW ); };
    dd = micros()-tick;
  };
  tick=micros();
  
  // we arrived
  if(instr==R)
  {
    x = 0;
    y = 0;
  }
  else
  {
    x = tgt_x;
    y = tgt_y;
  }

  // Turn the pins (and pullups) off
  pinMode( pinx, INPUT ); digitalWrite( pinx, LOW );
  pinMode( piny, INPUT ); digitalWrite( piny, LOW );
//pinMode( pinz, INPUT ); digitalWrite( pinz, LOW );
  
  // next instruction
  point+=3;
  if( point>=length )
  {
    point = 0;
    instr = R;
    tgt_x = -100;
    tgt_y = -100;
  }
  else
  {
    // read the next point
    instr = data[point];
    tgt_x = data[point+1];
    tgt_y = data[point+2];
  }
    
  // calculate timers
  if( tgt_x<x )      { dx = x-tgt_x; }
  else if( tgt_x>x ) { dx = tgt_x-x; }
  else               { dx = 0;       }
  
  if( tgt_y<y )      { dy = y-tgt_y; }
  else if( tgt_y>y ) { dy = tgt_y-y; }
  else               { dy = 0;       }

  if(debug)
  {
    Serial.print("Point ");
    Serial.print(point);
    Serial.print(" of ");
    Serial.print(length);
    Serial.print(" (");
    Serial.write(1.0*instr);
    Serial.write(",");
    Serial.print(1.0*tgt_x);
    Serial.print(",");
    Serial.print(1.0*tgt_y);
    Serial.print(" for (");
    Serial.print(1.0*dx);
    Serial.print(",");
    Serial.print(1.0*dy);
    Serial.println(")\n" );
  }  

}


