
#include <PID_v1.h>

#define M1            6
#define M2            7  // motor's PWM outputs


double kp=1.42,ki=1.270,kd=0.3500;
double input=0, output=0, setpoint=0;
PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
volatile long encoder0Pos = 0;
long previousMillis = 0;        // will store last time LED was updated


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
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255,255);
  
  //encoder
  pinMode(ENC_A_A, INPUT_PULLUP); // encoder A
  pinMode(ENC_A_B, INPUT_PULLUP);
  Serial.begin (115200);
  attachInterrupt(0, encoder_A, RISING);
  setpoint = 0;
  
} 


int i =0;
void loop(){
     // Updates the previous state of the outputA with the current state
     position_A = (360 * (float) encodercount_A) / (450*2);
     encoder0Pos = position_A;
     input = encoder0Pos;
     if(Serial.available() > 0) {
      i = Serial.parseInt();
       if (i != 0){
        Serial.println(i);
        setpoint = -i;
       } 
    }
    myPID.Compute();// wait till PID is actually computed in the mean time assert the endstop pins
    if(input==setpoint){
      pwmOut(0);output = 0; }
    else pwmOut(output); 
    printPos();
  
}

void pwmOut(int out) {
   if(out<0){
    analogWrite(M1,0);
    analogWrite(M2,abs(out));
    }
   else {
    analogWrite(M2,0);
    analogWrite(M1,abs(out));
    }
  }


void printPos() {
  Serial.print(F("Position=")); Serial.print(encoder0Pos); Serial.print(F(" PID_output=")); 
  Serial.print(output); Serial.print(F(" Target=")); Serial.println(setpoint);
}
void encoder_A() {
  transitiontime_A = micros() - lasttime_A; // this bit for sensing pulse duration
  if (digitalRead(ENC_A_B))
    encodercount_A++;                   // counting the pulses (position)
  else
    encodercount_A--;
  lasttime_A = micros();
}
