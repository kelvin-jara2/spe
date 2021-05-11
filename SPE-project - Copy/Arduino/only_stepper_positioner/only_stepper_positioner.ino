
#include <EEPROM.h>
#include <PID_v1.h>

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
double kp=20,ki=1.0,kd=0.0;
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


// Arduino stepper motor control code

#include <Stepper.h> // Include the header file

// change this to the number of steps on your motor
#define STEPS 200
float stepperPos;
// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 9, 10, 11);
int val = 0;

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
  Serial.begin (115200);
  attachInterrupt(0, encoder_A, RISING);

  stepper.setSpeed(200);
  stepperPos = 0;
} 


float angle;
String msg;
int x;
int y ;
void loop(){
  if(Serial.available() > 0) {
    char data = Serial.read();
    if( data == '#'){
      String xval = getValue(msg, ':', 0);
      String yval = getValue(msg, ':', 1);
      if (xval != ""){
        x = xval.toInt();
        y = yval.toInt();
        int steps = getSteps(x);
        stepper.step(steps);
        position_A = (360 * (float) encodercount_A) / (450*2); 
        encoder0Pos = position_A;
        input = encoder0Pos;
        setpoint=y;
        //Serial.println(String(x) + "-" + String(y));
        Serial.println("Steps :" + String(steps));  
      }
      msg = "";
    }else{
      msg = msg + data;    }
    }
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

void endstop (){
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

int getSteps(float angle){
  float diff = angle - stepperPos;
  int steps = diff*(200.0/360.0);
  
  stepperPos = stepperPos + diff;
  return steps;
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
