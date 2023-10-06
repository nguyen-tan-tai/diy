#include <Shifty.h>

Shifty myreg;
const int BIT_CNT = 16;

void setup() {
  myreg.setBitCount(BIT_CNT);
  myreg.setPins(5, 7, 6);
  off();
}

void loop() {
  off();
  delay(1000);
  d(0, 1);
  delay(1000);
  d(1, 2);
  delay(1000);
  d(2, 3);
  delay(1000);
  d(3, 4);
  delay(1000);
  d(4, 5);
  delay(1000);
  d(5, 6);
  delay(1000);
  d(6, 7);
  delay(1000);
  d(7, 8);
  delay(1000);
  d(8, 9);
  delay(1000);
  d(9, 0);
  delay(1000);
}

void d(byte d1, byte d2) {
  myreg.batchWriteBegin();
  w(0, d2);
  w(1, d1);
  myreg.batchWriteEnd();
}

void off() {
  myreg.batchWriteBegin();
  for (byte i = 0; i < BIT_CNT; i++) {
    myreg.writeBit(i, HIGH);
  }
  myreg.batchWriteEnd();
}

void w(byte p, byte d) {
  byte i = p * 8;
  if (d == 0) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, LOW);
    myreg.writeBit(i + 3, HIGH);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 1) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, HIGH);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, HIGH);
    myreg.writeBit(i + 4, HIGH);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, HIGH);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 2) {
    myreg.writeBit(i + 0, HIGH);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, LOW);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, HIGH);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 3) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, HIGH);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 4) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, HIGH);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, HIGH);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 5) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, HIGH);
  } else if (d == 6) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, LOW);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, HIGH);
  } else if (d == 7) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, HIGH);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, HIGH);
    myreg.writeBit(i + 4, HIGH);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 8) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, LOW);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  } else if (d == 9) {
    myreg.writeBit(i + 0, LOW);
    myreg.writeBit(i + 1, LOW);
    myreg.writeBit(i + 2, HIGH);
    myreg.writeBit(i + 3, LOW);
    myreg.writeBit(i + 4, LOW);
    myreg.writeBit(i + 5, HIGH);
    myreg.writeBit(i + 6, LOW);
    myreg.writeBit(i + 7, LOW);
  }
}
