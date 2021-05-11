// Arduino stepper motor control code

#include <Stepper.h> // Include the header file

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 9, 10, 11);

int val = 0;

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(50);
}

void loop() {
    stepper.step(1000);
    delay(1000);
    stepper.step(-100);
    delay (1000);
}
