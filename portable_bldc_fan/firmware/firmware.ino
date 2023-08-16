#include <TimerOne.h>
const int fanPin = 9;
const int lightPin = 10;
const int lightSensor = A0;
const int fanSensor = A1;
const int startPin = 4;

void setup(void) {
  pinMode(lightPin, OUTPUT);
  pinMode(startPin, OUTPUT);
  Timer1.initialize(40); // 40 us = 25 kHz
  //  Serial.begin(9600);
  digitalWrite(startPin, LOW);
  Timer1.pwm(lightPin, 512);
}

void loop(void) {
  int lightBrightness = analogRead(lightSensor);
  int fanRevertedValue = 1023 - analogRead(fanSensor);
  int fanSpeed = map(fanRevertedValue, 0, 1023, 600, 860);
  Timer1.pwm(lightPin, lightBrightness);
  Timer1.pwm(fanPin, fanSpeed);
  //  Serial.print("light = ");
  //  Serial.print(lightBrightness);
  //  Serial.print("fan = ");
  //  Serial.println(fanSpeed);
  delay(100);
}
