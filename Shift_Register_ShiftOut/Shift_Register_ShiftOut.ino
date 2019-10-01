/*

  Shift_Register_ShiftOut.ini

  Test mod of Shift_Register_RAW that uses decimal values to control
  the stepper.

  This sketch controls a 28BYJ-48 stepper motor via a shift register.

  For details on usage, wiring, etc. see link below.
  http://projectsfromtech.blogspot.com/2014/03/arduino-shift-register-stepper-motor.html

  Last Edited: 3/1/2014
  by Matthew https://projectsfromtech.blogspot.com

  Thanks to DrLuke from http://bildr.org/2011/02/74hc595/
  for Shift Register Control code
*/

int dataPin = D7;   //pin 14 on the 75HC595
int latchPin = D2;  //pin 12 on the 75HC595
int clockPin = D5; //pin 11 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 1

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

/*
   Stepper Driver Values

   These values will be converted to bits (shiftOut) and sent to 74HC595
   which is then latched applying the bit pattern to the stepper controller

   these values use 16 bits (4 per stepper). for more steppers, cascade
   another 74HC595 and increase to 12 bits.

   Note: use addition to move multiple steppers at once, i.e. 4+12, etc.)
*/

const int fullStepValueCount = 4;
const int halfStepValueCount = 8;

// Values for one-phase, full-step operation (A->B->C->D)
int OF_stepperValues1[8] = {   8,     4,    2,    1};
int OF_stepperValues2[8] = { 128,    64,   32,   16};
int OF_stepperValues3[8] = {2048,  1024,  512,  256};

// Values for two-phase, full-step operation (AB->BC->CD->DA)
int TF_stepperValues1[8] = {  12,     6,    3,    9};
int TF_stepperValues2[8] = { 192,    96,   48,  144};
int TF_stepperValues3[8] = {3072,  1536,  768, 2304};

// Values for one/two-phase, half-step operation (AB->B->BC->C->CD->D->DA->A)
int TH_stepperValues1[16] = {  12,    4,    6,    2,    3,    1,     9,     8};
int TH_stepperValues2[16] = { 192,   64,   96,   32,   48,   16,   144,   128};
int TH_stepperValues3[16] = {3072, 1024, 1536,  512,  768,  256,  2304,  2048};



//set all register pins to LOW
void clearRegisters() {
  for (int i = numOfRegisterPins - 1; i >=  0; i--) {
    registers[i] = LOW;
  }
}


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters() {

  digitalWrite(latchPin, LOW);

  for (int i = numOfRegisterPins - 1; i >=  0; i--) {
    digitalWrite(clockPin, LOW);

    int val = registers[i];

    digitalWrite(dataPin, val);
    digitalWrite(clockPin, HIGH);

  }
  digitalWrite(latchPin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value) {
  registers[index] = value;
}

////////////////////////////////////////////////////////////////////
//declare variables for the motor pins
int motorPin1 = 0;    // Blue   - 28BYJ48 pin 1
int motorPin2 = 1;    // Pink   - 28BYJ48 pin 2
int motorPin3 = 2;    // Yellow - 28BYJ48 pin 3
int motorPin4 = 3;    // Orange - 28BYJ48 pin 4
// Red    - 28BYJ48 pin 5 (VCC)

int motorSpeed = 0;     //variable to set stepper speed
const int potPin = A0;     //potentiometer connected to A2
int potValue = 0;     //variable to read A0 input


//////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(115200);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);


  //reset all register pins
  // clearRegisters();
  ///writeRegisters();


}

uint8_t stepIndex = 0;
uint16_t serialValue;
//////////////////////////////////////////////////////////////////////////////
void loop() {
  potValue = analogRead(potPin);     // read the value of the potentiometer
  potValue = map(potValue, 25, 1023, 0, 200);
  Serial.println(potValue);          // View full range from 0 - 1024 in Serial Monitor
  // delayMicroseconds(potValue);
  // delay(potValue); // leaving in for now in case I need to adjust final version
  delay(10);  // this value seems right after testing
  // drive using one-phase full step first
  serialValue = TF_stepperValues1[stepIndex];	// get next value (bit pattern) to output
  // send value to shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, serialValue);
  digitalWrite(latchPin, HIGH);
  // advance value index (to get circular array)
  stepIndex++;
  if (stepIndex >= fullStepValueCount) {
    stepIndex = 0;
  }




  /*
    potValue = analogRead(potPin);     // read the value of the potentiometer
    Serial.println(potValue);          // View full range from 0 - 1024 in Serial Monitor
    if (potValue < 535){               // if potentiometer reads 0 to 535 do this
      motorSpeed = (potValue/15 + 5);  //scale potValue to be useful for motor
      clockwise();                     //go to the ccw rotation function
    }
    else {                             //value of the potentiometer is 512 - 1024
      motorSpeed = ((1024-potValue)/15 + 5); //scale potValue for motor speed
      counterclockwise(); //go the the cw rotation function
    }
  */


}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)

void counterclockwise () {
  // 1
  setRegisterPin(motorPin1, HIGH);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin4, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 2
  setRegisterPin(motorPin1, HIGH);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin4, LOW);
  writeRegisters();
  delay (motorSpeed);
  // 3
  setRegisterPin(motorPin1, LOW);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin4, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 4
  setRegisterPin(motorPin1, LOW);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin4, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 5
  setRegisterPin(motorPin1, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin4, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 6
  setRegisterPin(motorPin1, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin4, HIGH);
  writeRegisters();
  delay (motorSpeed);
  // 7
  setRegisterPin(motorPin1, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin4, HIGH);
  writeRegisters();
  delay(motorSpeed);
  // 8
  setRegisterPin(motorPin1, HIGH);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin4, HIGH);
  writeRegisters();
  delay(motorSpeed);
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 4 to 1
//delay "motorSpeed" between each pin setting (to determine speed)

void clockwise() {
  // 1
  setRegisterPin(motorPin4, HIGH);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin1, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 2
  setRegisterPin(motorPin4, HIGH);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin1, LOW);
  writeRegisters();
  delay (motorSpeed);
  // 3
  setRegisterPin(motorPin4, LOW);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin1, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 4
  setRegisterPin(motorPin4, LOW);
  setRegisterPin(motorPin3, HIGH);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin1, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 5
  setRegisterPin(motorPin4, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin1, LOW);
  writeRegisters();
  delay(motorSpeed);
  // 6
  setRegisterPin(motorPin4, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin2, HIGH);
  setRegisterPin(motorPin1, HIGH);
  writeRegisters();
  delay (motorSpeed);
  // 7
  setRegisterPin(motorPin4, LOW);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin1, HIGH);
  writeRegisters();
  delay(motorSpeed);
  // 8
  setRegisterPin(motorPin4, HIGH);
  setRegisterPin(motorPin3, LOW);
  setRegisterPin(motorPin2, LOW);
  setRegisterPin(motorPin1, HIGH);
  writeRegisters();
  delay(motorSpeed);
}

