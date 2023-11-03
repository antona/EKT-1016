/*
EKT-1016 State Machine Test Driver Rev A
ElectronicKit.com
10/25/2013 3pm

This sketch acts like a simple state machine
using all 16 drivers on port A and B
user can add as many states as needed with multiple drivers
on at the same time

'stateset' function (subroutine) allows user to enter with
the following parameters:

adata = byte of A port data
bdata = byte of B port data
ontime = integer of driver on time duration
offtime = integer of driver pause time before next driver command

NOTE  a '1'(high bit) turns a driver OFF
      a '0'(low bit) turns a driver ON

User can change the card address and enable pin
per the EKT-1016 User's Manual at 
 http://www.antona.com/EKT/kitproduct/ekt-1016/man1016.pdf or
http://www.electronickit.com/kitproduct/ekt-1016/man1016.pdf
*/
#include "Wire.h"

// Enable Driver Output
// Enable only one line below

// NOTE: SET TO ENABLE WITH D8 PIN

// const int EnbAB = 0; // Always Enabled - as shipped
// const int EnbAB = 2; // use PD2
// const int EnbAB = 3; // use PD3
// const int EnbAB = 7; // use PD7
 const int EnbAB = 8; // use PB0 

// Choose I2C Shield Address
// 0 = jumper on pins, 1 = no jumper

// Enable only 1 line below
//
//                          A2 A1 A0  
 const byte Adr = 0x00; //0  0  0 - as shipped
// const byte Adr = 0x01; //0  0  1
// const byte Adr = 0x02; //0  1  0
// const byte Adr = 0x03; //0  1  1
// const byte Adr = 0x04; //1  0  0
// const byte Adr = 0x05; //1  0  1
// const byte Adr = 0x06; //1  1  0
// const byte Adr = 0x07; //1  1  1

const byte Base = 0x20 + Adr; // shield base address
const byte GpioA = 0x12;      // port A data reg
const byte GpioB = 0x13;      // port B data reg

const byte adata = B00000000;      //user data to A port
const byte bdata = B00000000;      //user data to B port

unsigned long ontime;
unsigned long offtime;

int i = 0;

// Initialize Drivers OFF, Ports to OUPTPUT

void setup()
{
if (EnbAB > 0)
{
 pinMode(EnbAB, OUTPUT);
 digitalWrite(EnbAB, HIGH); // disable all drivers
}
 Wire.begin(); // start
 
// Initialize Port A and B to Output
// always do this for the EKT-1016 shield

 Wire.beginTransmission(Base);   // Base address
 Wire.write(0x00); 		 // IODIRA set port A I/O
 Wire.write(0x00); 		 // Port A = output
 Wire.endTransmission();

 Wire.beginTransmission(Base); // Base address
 Wire.write(0x01); 		 // IODIRB set port B I/O
 Wire.write(0x00); 		 // Port B = output
 Wire.endTransmission();

// Enable 

if (EnbAB > 0)
{
 digitalWrite(EnbAB, LOW); // enable all drivers
 }
}
// 16 State Machine -
// This sample code sets the EKT-1016 driver outputs in sequential order
// for a 1000 ms (1 sec) duration and a 500 ms (1/2 sec) pause before
// the next state

// Note the sketch just moves from state to state, but
// the programmer can condition state movement by digital or analog input
// reading(s), time-of-day, temperature, etc.
//
// Though this sketch turns drivers on/off 1-at-a-time, 
// user can turn on multiple drivers at each state
// 
// The 17 lines of code below are the whole state machine
//
// the 'stateset' function can be invoked using variables though I 
// have used a binary integer to show how a single driver is turned on
// sequentially

void loop()
{
//           Port A     Port B   on-tme pause
   stateset(B00000001, B00000000, 1000, 500);
   stateset(B00000010, B00000000, 1000, 500);
   stateset(B00000100, B00000000, 1000, 500);
   stateset(B00001000, B00000000, 1000, 500);
   stateset(B00010000, B00000000, 1000, 500);    
   stateset(B00100000, B00000000, 1000, 500);
   stateset(B01000000, B00000000, 1000, 500);   
   stateset(B10000000, B00000000, 1000, 500);
 
   stateset(B00000000, B00000001, 1000, 500);   
   stateset(B00000000, B00000010, 1000, 500);  
   stateset(B00000000, B00000100, 1000, 500);   
   stateset(B00000000, B00001000, 1000, 500); 
   stateset(B00000000, B00010000, 1000, 500);   
   stateset(B00000000, B00100000, 1000, 500); 
   stateset(B00000000, B01000000, 1000, 500);  
   stateset(B00000000, B10000000, 1000, 500);
}

// Set port A and B driver outputs for a fixed time
// then reset all drivers off, pause and return
//
// NOTE  a '1'(high bit) turns a driver OFF
//       a '0'(low bit) turns a driver ON
//
// adata = byte of A port data
// bdata = byte of B port data
// ontime = integer of driver on time duration
// offtime = integer of driver pause time before next driver command

void stateset(byte adata, byte bdata, unsigned long ontime, unsigned long offtime){
  
   Wire.beginTransmission(Base);
   Wire.write(GpioA); // port A
   Wire.write(~adata); // invert data to Port A
   Wire.endTransmission();
 
   Wire.beginTransmission(Base);
   Wire.write(GpioB); // port B
   Wire.write(~bdata); // invert data to Port B
   Wire.endTransmission();
   
   delay(ontime);  // turn on driver for this duration
   
   Wire.beginTransmission(Base);
   Wire.write(GpioA); // port A
   Wire.write(B11111111); // drivers off to Port A
   Wire.endTransmission();
 
   Wire.beginTransmission(Base);
   Wire.write(GpioB); // port B
   Wire.write(B11111111); // drivers off to Port B
   Wire.endTransmission();

   delay(offtime);  // pause before returning for next state set
 }

