// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 
int servoPin2 = 7; 
// Create a servo object 
Servo Servo1; 
Servo Servo2; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin);
   Servo2.attach(servoPin2); 
}
void loop(){ 
   // Make servo go to 0 degrees 
   Servo1.write(0);
   Servo2.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   Servo2.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
   Servo1.write(180); 
   Servo2.write(180); 
   delay(1000); 
}
