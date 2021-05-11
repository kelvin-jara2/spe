/* This one is not using any PinChangeInterrupt library */

/*
   This program uses an Arduino UNO for a closed-loop control of a DC-motor. 
   Motor motion is detected by a quadrature encoder.
   Two inputs named STEP and DIR allow changing the target position.
   Serial port prints current position and target position every second.
   Serial input can be used to feed a new location for the servo (no CR LF).
   
   Pins used:
   Digital inputs 2 & 8 are connected to the two encoder signals (AB).
   Digital input 3 is the STEP input.
   Analog input A0 is the DIR input.
   Digital input 4 is the HOME output. asserted if encoder0Pos is smaller than one.
   Digital outputs 9 & 10 control the PWM outputs for the motor (I am using half L298 here).
   Please note PID gains kp, ki, kd need to be tuned to each different setup. 
*/
#include <EEPROM.h>
#include <PID_v1.h>
#include <Wire.h>
#include "AMS_5600.h"

#define M1            30
#define M2            31  // motor's PWM outputs
#define ENDSTOP 28
#define STEP 3

#define outputA 6
#define outputB 7

int counter = 0; 
int aState;
int aLastState;

String lastResponse;


byte pos[1000]; int p=0;

double kp=10,ki=1.0,kd=0.0;
double input=0, output=0, setpoint=0;
PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
volatile long encoder0Pos = 0;
boolean isHome=true;
boolean auto1=false, auto2=false,counting=false;
long previousMillis = 0;        // will store last time LED was updated

long target1=0;  // destination location at any moment

//for motor control ramps 1.4
bool newStep = false;
bool oldStep = false;
bool dir = false;
byte skip=0;

// and for the encoder:
#define ENC_A_A 2 // pin connected to external interrupt
#define ENC_A_B 4 // pin connected to normal pin
// variables
float position_A; // controller variables as globals
// timing:
unsigned long loopTime, printTime; // timing
volatile long encodercount_A; 
volatile unsigned long lasttime_A, transitiontime_A;

void setup() { 
  // motor
  pinMode(ENDSTOP, OUTPUT);  
  pinMode (outputA,INPUT);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(-255,255);
  
  //encoder
  pinMode(ENC_A_A, INPUT_PULLUP); // encoder A
  pinMode(ENC_A_B, INPUT_PULLUP);
  Serial.begin (9600);
  attachInterrupt(0, encoder_A, RISING);

  
} 



void loop(){
     // Updates the previous state of the outputA with the current state
     position_A = (360 * (float) encodercount_A) / (450*2); // degrees: 100 PPR, 1:30 gear, 360 deg/rev. in [rad] it would be 2*pi*(float)encodercount/(100*30)

     encoder0Pos = position_A;
     input = encoder0Pos;
    setpoint=45;
    while(!myPID.Compute())
           {endstop();} // wait till PID is actually computed in the mean time assert the endstop pins
    
    if(input==setpoint)pwmOut(0); else pwmOut(output); 
    
    printPos();
    //if(counting && abs(input-target1)<15) counting=false; 
   // if(counting &&  (skip++ % 5)==0 ) {pos[p]=encoder0Pos; if(p<999) p++; else counting=false;}
    endstop(); // output status of endstop
  
}

void pwmOut(int out) {
   if(out<0) { analogWrite(M1,0); analogWrite(M2,abs(out)); }
   else { analogWrite(M2,0); analogWrite(M1,abs(out)); }
  }


void printPos() {
  Serial.print(F("Position=")); Serial.print(encoder0Pos); Serial.print(F(" PID_output=")); 
  Serial.print(output); Serial.print(F(" Target=")); Serial.print(setpoint);
  if(!isHome)  Serial.print(" NOT");  Serial.println(" home");
}
void encoder_A() {
  transitiontime_A = micros() - lasttime_A; // this bit for sensing pulse duration
  if (digitalRead(ENC_A_B))
    encodercount_A++;                   // counting the pulses (position)
  else
    encodercount_A--;
  lasttime_A = micros();
}
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */    
  float retVal = newAngle * 0.0879;
  return retVal;
}



void endstop (){
   // endstop detection.  it is interlocked.  under normal operation it would not
  // send a digital write, just on transition smaller than 2;
  // this would not significantly affect normal operation. 
  if(encoder0Pos<2)
    {
      if(encoder0Pos<=0&&!isHome)  
      {
        isHome=true; 
        digitalWrite(ENDSTOP,isHome);
      }
  else if(encoder0Pos>0&&isHome)
      {
        isHome=false; 
        digitalWrite(ENDSTOP,isHome);
      }
    }
}
