# EKT-1016
Example programs to exercise and use the ElectronicKit.com EKT-1016 Power Digital Shield

Hello:

Here are a collection of sketches that will work with the EKT-1016 card

The 'step1016' sketch is the same routine used in the KickStarter video with 2 different step speeds.
A 30ms/step for the stepper motors and a slower 100ms speed for the 8 green LEDs (video was better at
a slow speed).

This sketch works with the factory default jumper setting on the EKT-1016.

Note that the user turns a power driver output ON by setting its port binary bit to a '0' and OFF with a '1'.

-------------------------

The 'state1016' sketch is a simple 16 state machine.  The user can add states as needed (limited only by code
space). The 'machine' coded here will turn on/off each of the 16 drivers in sequence starting from Port A bit 0 
to Port B bit 7 and then repeat.

This sketch uses the Driver Enable Jumper Option.  Set JP1 to '8' (data bit 8) from 'G' (Grounded - always enabled).
All other jumpers are the factory default settings.

Note that the user turns a power driver output ON by setting its port binary bit to a '0' and OFF with a '1'.

-------------------------

The 'ripple1016' steps through each driver on both ports at 100ms starting from D0 and going to D7 before repeating the cycle.

User can change the ripple rate by changing the value of the last line of the sketch.

This sketch works with the factory default jumper setting on the EKT-1016.

Note that the user turns a power driver output ON by setting its port binary bit to a '0' and OFF with a '1'.

-------------------------

The 'flip1016' turns all drivers off, then steps through each driver on 1 at a time on both ports at 100ms starting from D0 and going to D7. Next, the driver's are all disabled for 200ms. The process now repeats with all driver outputs 'flipped' on and turns each bit off 1 at a time with a 100ms rate through all bits.  After another 200ms driver off pause, the process starts from the beginning.

User can change the ripple rate by changing the value of the last line of the sketch.

The driver output on JP1 is set to '8' (see on the silkscreen) for the 
driver enable test.  Note that leaving the JP1 jumper on 'G' will work as well, but does not test the driver enable function.

All other jumpers are the factory default settings.

-------------------------
'StepFull.ino' is a collection of 4 processes that can control 4 stepper motors on Port A and B. Each time a process is invoked, the stepper routine named will move 1 full step.  The user specifies direction by using the boolean true/false as a parameter on entry for a forward/backward step. More detailed info is in the file text heading.
This sketch repeats stepping all 4 motors 10 steps/second to see how it operates.  The user would use repeated process executions to move multiple steps per the desired step rate.
---------------------
 'StepWFrevC.ino'
 Stepper Motor Controller       Rev C
 Wave, Full or Half Stepper Motor Drivers
 
 This test driver sketch wave-steps, full-steps or half-steps
 all 16 drivers on port A and B.
 The upper and lower 4 bits make up 2 stepper motor drives
 at 100ms / step (change time in main loop).
 
 Choose either wave, full or half step operation by
 enabling one of the following "#define" pseudo ops:
 
 #define WAVE
 #define FULL
 #define HALF (default code setting)
-------------------------

These examples should be a good starting point for using the EKT-1016 Shield and getting a basic understanding of how the drivers are controlled. 

Regards, Bob Mikkelson
electronickit.com
