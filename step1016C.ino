// DUPLICATE OF "StepFull.ino", STEP1016C IS ITS ORIGINAL NAME
// NEEDS TO BE DELETED, BUT HOW?
/*
  EKT-1016 Rev C Full Step Motor Test Driver
  ElectronicKit.com
  10/31/2023 3pm

  This test driver sketch full-steps all 16 drivers on port A and B
  the upper and lower 4 bits make up 1 stepper motor drive
  at 100ms / step (change time in main loop).

  There are 4 processes to handle 4 unipolar/bipolar stepper motors
  using the digital power drivers on the EKT-1016 card.  Each process
  execution steps the selected motor 1 full step. Direction is set by
  specifing true/false for forward / backward steps respecively.  As
  examples:

  StepALwr(false); // Chan A Lower motor backward 1 step
  StepBUpr(true);  // Chan B Upper motor forward 1 step

  You can integrate the 4 "StepALwr/Upr(), StepBLwr/Upr()"
  processes into do, while, for loops to get repeated stepping per
  a desired count and time stepping by your code's timing:

  While (int i=0; i < 100; i++)
  {
  StepAUpr(true);  //motor forward 1 step
  delay(50);       //50ms step rate for 100 steps
  }

  User can change the card address and enable pin
  per the EKT-1016 User's Manual at
  http://www.electronickit.com/kitproduct/ekt-1016/man1016.pdf
  http://www.antona.com/EKT/kitproduct/ekt-1016/man1016.pdf

  Step C0 C1 C2 C3
  1  1  0  1  0
  2  0  1  1  0
  3  0  1  0  1
  4  1  0  0  1
*/

#include "Wire.h"

// Enable Driver Output
// Enable only one line below
// Card shipped with enb = on always

const int EnbAB = 0; 			// Always Enabled - as shipped
// const int EnbAB = 2; 		// use PD2
// const int EnbAB = 3; 		// use PD3
// const int EnbAB = 7; 		// use PD7
// const int EnbAB = 8; 		// use PB0

// Choose I2C Shield Address
// 0 = jumper on pins, 1 = no jumper
// Card shipped with adr = 000 (all 3 jumpered)
// Enable only 1 line below
//
//                          	  A2 A1 A0
const byte Adr = 0x00;   		//0  0  0 - as shipped
// const byte Adr = 0x01; 		//0  0  1
// const byte Adr = 0x02; 		//0  1  0
// const byte Adr = 0x03; 		//0  1  1
// const byte Adr = 0x04; 		//1  0  0
// const byte Adr = 0x05; 		//1  0  1
// const byte Adr = 0x06; 		//1  1  0
// const byte Adr = 0x07; 		//1  1  1

const byte Base = 0x20 + Adr; 	// shield base address
const byte GpioA = 0x12;      	// port A data reg
const byte GpioB = 0x13;      	// port B data reg

int i = 0;
byte Mot12 = B00000000;					//Step motor 1 and 2
byte Mot34 = B00000000;					//Step motor 3 and 4

/*
  FULL STEP TABLES
  Step C0 C1 C2 C3
  1  1  0  1  0
  2  0  1  1  0
  3  0  1  0  1
  4  1  0  0  1
*/
//Stepper Lower Nibble Masks
byte StepLwr[4] =                   //FULL STEPS LOWER NIBBLE
{
  B00001010,
  B00000110,
  B00000101,
  B00001001,
};
//Stepper Upper Nibble Masks
byte StepUpr[4] =                   //FULL STEPS UPPER NIBBLE
{
  B10100000,
  B01100000,
  B01010000,
  B10010000,
};

byte MaskUpr = B00001111;		//Mask out upr nibble
byte MaskLwr = B11110000;		//Mask out Lwr nibble
boolean Dirc = true;			//Step direction, forward = true, backward = false
int CtAUp = 0;					//State counter Chan A upper
int CtALw = 0;					//Chan A lower
int CtBUp = 0;					//Chan B upper
int CtBLw = 0;					//Chan B lower

void setup()
{
  Serial.begin(9600);                    //For test and log output
  if (EnbAB > 0)
  {
    pinMode(EnbAB, OUTPUT);
    digitalWrite(EnbAB, HIGH); 	// disable all drivers
  }
  Wire.begin(); 					// start

  // Set Port A and B to Output

  Wire.beginTransmission(Base);   // Base address
  Wire.write(0x00); 		 		      // IODIRA set port A I/O
  Wire.write(0x00); 		 		      // Port A = output
  Wire.endTransmission();

  Wire.beginTransmission(Base); 	// Base address
  Wire.write(0x01); 		 		      // IODIRB set port B I/O
  Wire.write(0x00); 		 		      // Port B = output
  Wire.endTransmission();
  /*
      ----------------------------
       M A I N  L O O P

       Each call to one of the Step A/B port Up/Lw nibble
       routine moves the stepper 1 full step forward/backward
       --------------------------
  */
}
void loop()
{
  StepAUpr(true);		  //Chan A upr forward 1 step
  StepALwr(false);		//Chan A Lwr backward 1 step
  StepBUpr(true);		  //Chan B upr forward 1 step
  StepBLwr(false);		//Chan B Lwr backward 1 step
  DrvAB();		        //Drive output pins
  Tp();                         //Serial output bit patterns
  delay(100); 			  // 1/10 second/step
}
/*
  ------------------------------
  Upper Stepper on A port
  ------------------------------
*/
void StepAUpr(boolean Dir)
{
  if (Dir)			      //Forward ?
  {
    CtAUp++;			    //Move ahead 1 full step
    CtAUp = CtAUp & B00000011;  //Reset to 00 if >= 4
  }

  else				        //Step Backward
  {
    CtAUp--;			    //Move back 1 full step
    if (CtAUp < 0)		//Below beginning of table?
    {
      CtAUp = 3;		  //Reset to state end
    }
  }
  Mot12 = ((Mot12 & MaskUpr) | StepUpr[CtAUp]);
}
/*
  ------------------------------
  Lower Stepper on A port
  ------------------------------
*/
void StepALwr(boolean Dir)
{
  if (Dir)			        //Forward ?
  {
    CtALw++;			      //Move ahead 1 full step
    CtALw = CtALw & B00000011;  //Reset to 00 if >= 4
  }

  else				          //Step Backward
  {
    CtALw--;			      //Move back 1 full step
    if (CtALw < 0)		  //Below beginning of table?
    {
      CtALw = 3;		    //Reset to state end
    }
  }
  Mot12 = ((Mot12 & MaskLwr) | StepLwr[CtALw]);
}
/*
  ------------------------------
  Upr Stepper on B port
  ------------------------------
*/
void StepBUpr(boolean Dir)
{
  if (Dir)			//Forward ?
  {
    CtBUp++;			//Move ahead 1 full step
    CtBUp = CtBUp & B00000011;  //Reset to 00 if >= 4
  }

  else				//Step Backward
  {
    CtBUp--;			//Move back 1 full step
    if (CtBUp < 0)		//Below beginning of table?
    {
      CtBUp = 3;		//Reset to state end
    }
  }
  Mot34 = ((Mot34 & MaskUpr) | StepUpr[CtBUp]);
}
/*
  ------------------------------
  Lower Stepper on B port
  ------------------------------
*/
void StepBLwr(boolean Dir)
{
  if (Dir)			//Forward ?
  {
    CtBLw++;			//Move ahead 1 full step
    CtBLw = CtBLw & B00000011;  //Reset to 00 if >= 4
  }

  else				//Step Backward
  {
    CtBLw--;			//Move back 1 full step
    if (CtBLw < 0)		//Below beginning of table?
    {
      CtBLw = 3;		//Reset to state end
    }
  }
  Mot34 = ((Mot34 & MaskLwr) | StepLwr[CtBLw]);
}
/*
  ------------------------------
  Output Stepper Drive bytes
  ------------------------------
*/
void DrvAB()
{
  digitalWrite(EnbAB, HIGH); 	//Disable all drivers

  Wire.beginTransmission(Base);
  Wire.write(GpioA); 			// port A
  Wire.write(~Mot12); 			// invert data to Port A
  Wire.endTransmission();

  Wire.beginTransmission(Base);
  Wire.write(GpioB);  			// port B
  Wire.write(~Mot34); 			// invert data to Port B
  Wire.endTransmission();

  digitalWrite(EnbAB, LOW); 		// enable all drivers
}
/*
  ------------------------------
  Output motor steps to serial port
  ------------------------------
*/
void Tp()
{
  Serial.print(CtAUp);    //Ref for Upr A motor only
  Serial.print(" ");
  if (Mot12 < B01111111)
  {
    Serial.print("0");    //Restore leading zero
  }
  Serial.print(Mot12, BIN);
  Serial.print(" ");
  if (Mot34 < B01111111)
  {
    Serial.print("0");    //Restore leading zero
  }
  Serial.println(Mot34, BIN);
}
