THINGS TO DO:
=============

1. start reimplementing the code chunks piece by piece (the various functions)
2. add the LED outputs to the digitalWrite function
3. NICETOHAVE: start grouping the logic in a more object oriented way:
    ex: `output[x].on()` ...etc. (not a must)
4. reconsider the hardware design, using a single 2R voltage divider to get a 2V Vref or something and then distribute it on all op amps wired as comparators will cut down on a bunch of resistors! would simplify the circuit. 
5. implement a better control scheme for the navigation and adjustments of the output divisions using the LEDs as a sort of readout indicator
6. start implementing i2c slave commands atl east for just output ON/OFF
7. look into a way to start controlling the length fo the gate outputs (i think ith as to do with millis() or something?)

