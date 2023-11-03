/*
EKT-1016 Rev A Stepper Motor Test Driver
ElectronicKit.com
10/25/2013 3pm

This sketch steps all 16 drivers on port A and B
the upper and lower 4 bits make up 1 stepper motor drive
at 100ms / step (change step time in last line of sketch)
User can change the card address and enable pin
per the EKT-1016 User's Manual at
 http://www.antona.com/EKT/kitproduct/ekt-1016/man1016.pdf or 
http://www.electronickit.com/kitproduct/ekt-1016/man1016.pdf
*/

#include "Wire.h"

// Enable Driver Output
// Enable only one line below
// Card shipped with enb = on always

 const int EnbAB = 0; // Always Enabled - as shipped
// const int EnbAB = 2; // use PD2
// const int EnbAB = 3; // use PD3
// const int EnbAB = 7; // use PD7
// const int EnbAB = 8; // use PB0

// Choose I2C Shield Address
// 0 = jumper on pins, 1 = no jumper
// Card shipped with adr = 000 (all 3 jumpered)
// Enable only 1 line below
//
//                          A2 A1 A0  
 const byte Adr = 0x00;   //0  0  0 - as shipped
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

int i = 0;
int Shift = 0x11;

void setup()
{
if (EnbAB > 0)
{
 pinMode(EnbAB, OUTPUT);
 digitalWrite(EnbAB, HIGH); // disable all drivers
}
 Wire.begin(); // start
 
// Set Port A and B to Output

 Wire.beginTransmission(Base);   // Base address
 Wire.write(0x00); 		 // IODIRA set port A I/O
 Wire.write(0x00); 		 // Port A = output
 Wire.endTransmission();

 Wire.beginTransmission(Base); // Base address
 Wire.write(0x01); 		 // IODIRB set port B I/O
 Wire.write(0x00); 		 // Port B = output
 Wire.endTransmission();
}
void loop()
{
 for (i=0; i<8; i++)
 {
 Wire.beginTransmission(Base);
 Wire.write(GpioA); // port A
 Wire.write(~Shift); // invert data to Port A
 Wire.endTransmission();
 
 Wire.beginTransmission(Base);
 Wire.write(GpioB); // port B
 Wire.write(~Shift); // invert data to Port B
 Wire.endTransmission();
 
 Shift = Shift + Shift;
 if (Shift > 0x99)
 {
   Shift = 0x11;
 }
if (EnbAB > 0)
{
 digitalWrite(EnbAB, LOW); // enable all drivers
}
 delay(100); // 1/10 second delay
 }
}
 
