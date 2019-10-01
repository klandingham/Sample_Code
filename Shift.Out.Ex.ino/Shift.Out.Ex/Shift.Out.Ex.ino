/*
 * This is an example I found on YouTube that is part of a sketch for a bumper robot.
 * What's important is the technique used to output control signals to stepper motors
 * connected via one or more shift registers (74HC595's in this case). Using shift
 * registers (which can be cascaded) allows you to control several stepper motors
 * using only 3 output lines on the Arduino/NodeMCU: serial data, clock and latch.
 * There are plenty of examples on the 'net for doing this, but this is the only
 * one that serializes signals in integer form, using binary-to-decimal conversions.
 *
 * For instance, with two steppers and 1 shift register:
 *
 *	to output  0 1 1 0 to stepper 2 and 0 0 1 1 to stepper 1, compute the
 *	decimal equivalent (stepper 2 getting the high order bits):
 *		0 1 1 0 0 0 1 1 = 99
 *	so shifting out the value 99 will send the low order 4 bits to
 *	stepper 1 and the high order 4 bits to stepper2 as desired.
 *
 * This example from:
 *	https://www.youtube.com/watch?v=OeqQPlD3mNA
 *	http://duinos.net/show/?id=234
 *	http://duinos.net/files/2015/arduino_bumper_robot_595version2.ino
 *
 * NOTES:
 *	1. Putting Linux calculator in Binary mode and entering binary digits immediately shows the equivalent decimal value...couldn't get any easier!
 */

/*
   Bumper robot code written by: Kristian BlÃ¥sol 6/9/2015
   Original BYJ48 Stepper motor code written By :Mohannad Rawashdeh 28/9/2013, Rewritten for use with a 595 Shift Register instead by Kristian BlÃ¥sol 16/9/2015.
   This program is for making a really simple (and cheap) "first" robot to play with Arduino. It is a bumper robot. This particular version
   Uses a 74HC595 to drive the Two motors, 28BYJ-48. This minimizes the pins used for the motors, from the original 8 to three for both motors in this
   Sketch.
   See more on
   Youtube:
   or
   Duinos.net:
*/

int latchPin = 2; //pin 12 on the 595
int dataPin = 3;  //pin 14 on the 595
int clockPin = 4; //pin 11 on the 595


#define leftBumper 5
#define rightBumper 6

int stepper1val;
int stepper2val;
int Steps1 = 0;
int Steps2 = 0;
//defaults to forward
boolean Direction1 = false;// gre
boolean Direction2 = true;// gre
//dir1=1 dir2=0 goes backwards
//dir1=1 dir2=1 turns right
//dir1=0 dir2=0 turns left
//dir1=0 dir2=1 goes forward

unsigned long last_time;
unsigned long currentMillis ;
int steps_left1=4095; //4095 is a whole rotation
int steps_left2=4095; //4095 is a whole rotation
// long time;


void setup()
{
  //Serial.begin(115200);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);  
}

void loop()
{
  while(1>0){
    //check bumpers
    int readLeft = digitalRead(leftBumper);
    int readRight = digitalRead(rightBumper);    
   if (readLeft == HIGH) {
     //back up a bit
     steps_left1=1024;
     steps_left2=1024;
    while(steps_left1>0){
       //dir1=1 dir2=0 goes backwards
       Direction1 = true;
       Direction2 = false;
       currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper1(1); 
        stepper2(1);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, stepper1val+stepper2val);
        digitalWrite(latchPin, HIGH); 
        //time=time+micros()-last_time;
        last_time=micros();
        steps_left1--;
        steps_left2--;
      }
    }     
      //turn right
      steps_left1=1024;
      steps_left2=1024;
      //dir1=1 dir2=1 turns right
    while(steps_left1>0){
       Direction1 = true;
       Direction2 = true;
       currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper1(1); 
        stepper2(1); 
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, stepper1val+stepper2val);
        digitalWrite(latchPin, HIGH);
        //time=time+micros()-last_time;
        last_time=micros();
        steps_left1--;
        steps_left2--;
      }     
    }
    }
    if (readRight == HIGH) {
      //back up a bit
     steps_left1=1024;
     steps_left2=1024;
     //dir1=1 dir2=0 goes backwards
    while(steps_left1>0){
       Direction1 = true;
       Direction2 = false;
       currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper1(1); 
        stepper2(1);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, stepper1val+stepper2val);
        digitalWrite(latchPin, HIGH); 
        //time=time+micros()-last_time;
        last_time=micros();
        steps_left1--;
        steps_left2--;
      }
    }
      //turn left
      steps_left1=1024;
      steps_left2=1024;
      //dir1=0 dir2=0 turns left
    while(steps_left1>0){
       Direction1 = false;
       Direction2 = false;
       currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper1(1); 
        stepper2(1); 
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, stepper1val+stepper2val);
        digitalWrite(latchPin, HIGH);
        //time=time+micros()-last_time;
        last_time=micros();
        steps_left1--;
        steps_left2--;
      }     
    }

    }
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      //moving forward
      //defaults to moving forward
      Direction1 = false;
      Direction2 = true;
      stepper1(1); 
      stepper2(1); 
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, stepper1val+stepper2val);
      digitalWrite(latchPin, HIGH);
      // time=time+micros()-last_time;
      last_time=micros();
    }
  }

}
// motor 1 is 1 2 4 8
void stepper1(int xw){
  for (int x=0;x<xw;x++){
switch(Steps1){
   case 0:
     stepper1val=8;    
   break; 
   case 1:
     stepper1val=12;
   break; 
   case 2:
     stepper1val=4;
   break; 
   case 3:
     stepper1val=6;
   break; 
   case 4:
     stepper1val=2;
   break; 
   case 5:
     stepper1val=3;
   break; 
     case 6:
     stepper1val=1;
   break; 
   case 7:
     stepper1val=9;
   break; 
   default:
     stepper1val=0;
   break; 
}
SetDirection1();
}
} 

// 16 32 64 128
void stepper2(int xw){
  for (int x=0;x<xw;x++){
switch(Steps2){
   case 0:
     stepper2val=128;
   break; 
   case 1:
     stepper2val=192;
   break; 
   case 2:
     stepper2val=64;
   break; 
   case 3:
     stepper2val=96;
   break; 
   case 4:
     stepper2val=32;
   break; 
   case 5:
     stepper2val=48;
   break; 
     case 6:
     stepper2val=16;
   break; 
   case 7:
     stepper2val=144;
   break; 
   default:
     stepper2val=8;
   break; 
}
SetDirection2();
}
}


void SetDirection1(){
if(Direction1==1){ Steps1++;}
if(Direction1==0){ Steps1--; }
if(Steps1>7){Steps1=0;}
if(Steps1<0){Steps1=7; }
}
void SetDirection2(){
if(Direction2==1){ Steps2++;}
if(Direction2==0){ Steps2--; }
if(Steps2>7){Steps2=0;}
if(Steps2<0){Steps2=7; }
}

