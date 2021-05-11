


// Arduino stepper motor control code

#include <Stepper.h> // Include the header file

// change this to the number of steps on your motor
#define STEPS 200
float stepperPos;
// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 9, 10, 11);
int val = 0;

void setup() { 
  Serial.begin (115200);

  stepper.setSpeed(100);
  stepperPos = 0;
} 


float angle;
String msg;
int x;
int y ;
long i = 0;
int steps;
int check = 1;
int check2 = 1;
void loop(){
  if(Serial.available() > 0) {
    /*
      if (check == 1 ){
        i = Serial.parseInt();
        
        steps = getSteps(i);
        Serial.println(i);
        stepper.step(steps);
      }*/
      i = Serial.parseInt();
       if (i != 0){
        steps = getSteps(i);
        Serial.println("comming :" + String(i) + " now: " + String(stepperPos)+ " steps: " + steps);
        stepper.step(steps);
       }
    }
}
int getSteps(float angle){
  float diff = angle - stepperPos;
  int steps = diff*(200.0/360.0);
  check = 0;
  stepperPos = stepperPos + diff;
  return 30*steps;
}
