#include <IRremote.hpp>
const int IR_RECEIVE_PIN = 5;
const int TRIAC_PIN = 6;
int fan_speed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TRIAC_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), zero_crossing, CHANGE);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void zero_crossing() {
  int chop_time = (200 * fan_speed);
  delayMicroseconds(chop_time);
  digitalWrite(TRIAC_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIAC_PIN, LOW);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    switch (IrReceiver.decodedIRData.command) {
      case 0x46:
        Serial.println("UP");
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case 0x15:
        Serial.println("DOWN");
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case 0x0:
        Serial.println("Too Fast, Cmd Repeat");
        break;
      default:
        Serial.println("Ooops, wrong key!");
    }
  }
  delay(100);
}
