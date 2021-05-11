// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 
int servoPin2 = 7; 
// Create a servo object 
Servo Servo1; 
Servo Servo2;

void setup() {
  Serial.begin(115200);

  // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin);
   Servo2.attach(servoPin2);
}
String msg;
int x;
int y ;
void loop() {
  
  if(Serial.available() > 0) {
    char data = Serial.read();
    if( data == '#'){
      String xval = getValue(msg, ':', 0);
      String yval = getValue(msg, ':', 1);
      
      if (xval != ""){
        x = xval.toInt();
        y = xval.toInt();
        int val = map(x, -180, 180, 0, 180);
        int val2 = map(y, -180, 180, 0, 180);
        Servo1.write(val);
        Servo2.write(val2);
        Serial.println(String(val) + "-" + String(val2));  
      }
      msg = "";
    }else{
      msg = msg + data;    }
    
  }
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
