#include <SPI.h>
#include <Adafruit_PCD8544.h>

const int aPin = A3;
const int bPin = A2;
const int cPin = 4;
const int dPin = 3;
const int sPin = A1;
const int xPin = A0;
const int yPin = A6;
const int tPin = A7;

Adafruit_PCD8544 lcd = Adafruit_PCD8544(6, 8, 10, 12, 11);

void setup() {
  Serial.begin(9600);
  pinMode(aPin, INPUT);
  pinMode(bPin, INPUT);
  pinMode(cPin, INPUT);
  pinMode(dPin, INPUT);
  pinMode(sPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(tPin, INPUT);

  lcd.begin();
  lcd.setContrast(50);
  lcd.setTextSize(1);
  lcd.clearDisplay();
}

void loop() {
  int a = digitalRead(aPin);
  int b = digitalRead(bPin);
  int c = digitalRead(cPin);
  int d = digitalRead(dPin);
  int s = digitalRead(sPin);
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int t = analogRead(tPin);

  lcd.clearDisplay();
  lcd.setCursor(0, 0);
  lcd.println(String(a) + "  " + String(b) + "  " + String(c) + "  " + String(d));
  lcd.println(String(x) + "  " + String(y));
  lcd.println(String(s) + "  " + String(t));
  lcd.display();

  //String r = String(a) + " - " + String(b) + " - " + String(c) + " - " + String(d) + " - " + String(s) + " - " + String(x) + " - " + String(y) + " - " + String(t);
  //Serial.println(r);
  delay(100);
}
