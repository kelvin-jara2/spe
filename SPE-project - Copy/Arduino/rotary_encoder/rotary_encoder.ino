

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
  pinMode(ENC_A_A, INPUT_PULLUP); // encoder A
  pinMode(ENC_A_B, INPUT_PULLUP);
  Serial.begin(115200);
  attachInterrupt(0, encoder_A, RISING);
}


int n;
int mvalue=1;
uint16_t stepf=100;
int laststepf;
int stepr=300;
int laststepr;


void loop() {
  if (millis() > loopTime + 49) { // 20 Hz output
    loopTime = millis();      // reset looptimer
    position_A = (360 * (float) encodercount_A) / (450*2); // degrees: 100 PPR, 1:30 gear, 360 deg/rev. in [rad] it would be 2*pi*(float)encodercount/(100*30)


// serial monitor output
    Serial.print("\t");
    Serial.println((int)position_A);
  }
}

////// ===== DON'T start messing with the code below this line ====////
// encoder interrupt routine
void encoder_A() {
  transitiontime_A = micros() - lasttime_A; // this bit for sensing pulse duration
  if (digitalRead(ENC_A_B))
    encodercount_A++;                   // counting the pulses (position)
  else
    encodercount_A--;
  lasttime_A = micros();
}
