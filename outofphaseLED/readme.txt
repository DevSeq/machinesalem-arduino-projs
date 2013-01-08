This is another three-color LED sketch.

This version covers the whole color-space, from black (all off) to white (all on), through every combination in between,
using three sinewave-based brightness channels that each have slightly different periods.  So the "red" channel is slightly
faster than green, which is slightly faster than blue.  Over time, all combinations are visible.

This sketch additionally uses a brightness function of "(sin(x)+1)-squared" instead of "sin(x)+1".
I'm not sure why, but it just looks nicer - relatively more time is spent on the darker colors.
