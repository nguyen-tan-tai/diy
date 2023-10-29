#include "alvn.h"
#include "OneWire.h"
#include "DallasTemperature.h"
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#include <Shifty.h>
#include "RTClib.h"
RTC_DS3231 rtc;
#include <FastLED_NeoPixel.h>
#define NUM_LEDS 30
#define DATA_PIN 9
FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;
#define NEO_OFF strip.Color(0, 0, 0)
const uint32_t COLORS[7] = {
  strip.Color(255, 0, 0),
  strip.Color(255, 127, 0),
  strip.Color(255, 255, 0),
  strip.Color(0, 255, 0),
  strip.Color(0, 0, 255),
  strip.Color(75, 0, 130),
  strip.Color(148, 0, 211)
};
int brightness = 100;
Shifty calendar;
const int BIT_CNT = 11 * 8;

void setup() {
  calendar.setBitCount(BIT_CNT);
  calendar.setPins(5, 7, 6);
  off();

  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    while (!rtc.begin()) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(900);
      Serial.println("Try reconnecting RTC...");
    };
  }
  Serial.println("RTC connected");
  if (rtc.lostPower()) {
    Serial.println("RTC lost power. Reset to the date this clock was made");
    rtc.adjust(DateTime(2023, 7, 4, 0, 0 , 0));
  }
  rtc.disable32K();

  strip.begin();
  strip.setBrightness(brightness);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    Serial.println(command);
    command.trim();
    if (command.startsWith("SET_TIME")) {
      //SET_TIME 2023-09-23T17:58:02
      String isoTime = command.substring(8, command.length());
      isoTime.trim();
      int y = isoTime.substring(0, 4).toInt();
      int mo = isoTime.substring(5, 7).toInt();
      int d = isoTime.substring(8, 10).toInt();
      int h = isoTime.substring(11, 13).toInt();
      int mi = isoTime.substring(14, 16).toInt();
      int s = isoTime.substring(17, 19).toInt();
      rtc.adjust(DateTime(y, mo, d, h, mi, s));
      Serial.print("Adjust time = ");
      Serial.print(y);
      Serial.print("-");
      Serial.print(mo < 10 ? "0" : "");
      Serial.print(mo);
      Serial.print("-");
      Serial.print(d < 10 ? "0" : "");
      Serial.print(d);
      Serial.print("T");
      Serial.print(h < 10 ? "0" : "");
      Serial.print(h);
      Serial.print(":");
      Serial.print(mi < 10 ? "0" : "");
      Serial.print(mi);
      Serial.print(":");
      Serial.print(s < 10 ? "0" : "");
      Serial.println(s);
    }
  }
  display_clock();
  display_calendar();
}

byte current_minute = -1;

void display_calendar() {
  DateTime now = rtc.now();
  byte minute = now.minute();
  if (current_minute == minute) {
    return;
  }
  current_minute = minute;
  byte day = now.day();
  byte gd1 = day / 10;
  byte gd2 = day  % 10;

  byte month = now.month();
  byte gm1 = month / 10;
  byte gm2 = month  % 10;

  GregorianDate gdate = GregorianDate(now.year(), month, day);
  LunarDate ldate = gdate.toLunarDate();
  int lunar_dd = ldate.d;
  int lunar_mm = ldate.m;
  Serial.println(day);
  Serial.println(month);
  Serial.println(now.year());
  Serial.println(lunar_dd);
  Serial.println(lunar_mm);

  byte ld1 = lunar_dd / 10;
  byte ld2 = lunar_dd  % 10;
  byte lm1 = lunar_mm / 10;
  byte lm2 = lunar_mm  % 10;

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("temperature: ");
  Serial.println(temperature);
  int int_temp = (int)(temperature);
  byte t1 = int_temp / 10;
  byte t2 = int_temp % 10;

  byte w = now.dayOfTheWeek() + 1;
  if (w == 1) {
    w = 8;
  }

  d(gd1, gd2, gm1, gm2, ld1, ld2, lm1, lm2, t1, t2, w);
}

void display_clock() {
  DateTime now = rtc.now();
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  uint32_t color = COLORS[now.dayOfTheWeek()];
  byte h0 = hour / 10;
  byte h1 = hour % 10;
  byte m0 = minute / 10;
  byte m1 = minute  % 10;
  display_7_segment(color, h0, 23);
  display_7_segment(color, h1, 16);
  if (second % 2 == 0) {
    strip.setPixelColor(14, color);
    strip.setPixelColor(15, color);
  } else {
    strip.setPixelColor(14, NEO_OFF);
    strip.setPixelColor(15, NEO_OFF);
  }
  display_7_segment(color, m0, 7);
  display_7_segment(color, m1, 0);
  strip.show();
}

void display_7_segment(uint32_t color, byte digit, byte start_index) {
  switch (digit) {
    case 0:
      strip.setPixelColor(start_index + 0, NEO_OFF);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 1:
      strip.setPixelColor(start_index + 0, NEO_OFF);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 2:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 3:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 4:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 5:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 6:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 7:
      strip.setPixelColor(start_index + 0, NEO_OFF);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 8:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 9:
      strip.setPixelColor(start_index + 0, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    default:
      break;
  }
}

void d(byte gd1, byte gd2, byte gm1, byte gm2, byte ld1, byte ld2, byte lm1, byte lm2, byte t1, byte t2, byte wd) {
  calendar.batchWriteBegin();
  w(0, wd);
  w(1, t2);
  w(2, t1);
  w(3, lm2);
  w(4, lm1);
  w(5, ld2);
  w(6, ld1);
  w(7, gm2);
  w(8, gm1);
  w(9, gd2);
  w(10, gd1);
  calendar.batchWriteEnd();
}

void off() {
  calendar.batchWriteBegin();
  for (byte i = 0; i < BIT_CNT; i++) {
    calendar.writeBit(i, HIGH);
  }
  calendar.batchWriteEnd();
}

void w(byte p, byte d) {
  byte i = p * 8;
  if (d == 0) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, LOW);
    calendar.writeBit(i + 3, HIGH);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 1) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, HIGH);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, HIGH);
    calendar.writeBit(i + 4, HIGH);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, HIGH);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 2) {
    calendar.writeBit(i + 0, HIGH);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, LOW);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, HIGH);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 3) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, HIGH);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 4) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, HIGH);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, HIGH);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 5) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, HIGH);
  } else if (d == 6) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, LOW);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, HIGH);
  } else if (d == 7) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, HIGH);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, HIGH);
    calendar.writeBit(i + 4, HIGH);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 8) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, LOW);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  } else if (d == 9) {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, HIGH);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, HIGH);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  }
}
