#include "alvn.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <Shifty.h>
#include "RTClib.h"
#include <FastLED_NeoPixel.h>

// Pin definition
#define ONE_WIRE_BUS 2
#define NEO_PIXEL_PIN 9
#define SHIFT_DATA 5
#define SHIFT_CLOCK 7
#define SHIFT_LATCH 6
#define MODE_PIN 8
#define ADJUST_UP 10
#define ADJUST_DOWN 11


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

RTC_DS3231 rtc;

#define NUM_LEDS 30 // 4 seven-segment digits plus two point
FastLED_NeoPixel<30, NEO_PIXEL_PIN, NEO_GRB> strip;
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
#define BIT_CNT = 88 // 11 seven-segment digits

void setup() {
  calendar.setBitCount(BIT_CNT);
  calendar.setPins(SHIFT_DATA, SHIFT_CLOCK, SHIFT_LATCH);
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
    rtc.adjust(DateTime(2023, 7, 4, 0, 0, 0));
  }
  rtc.disable32K();

  strip.begin();
  strip.setBrightness(brightness);
}

enum class RUN_MODE { CLOCK, ADJUST_YEAR, ADJUST_MONTH, ADJUST_DAY, ADJUST_HOUR, ADJUST_MINUTE, ADJUST_SECOND};

RUN_MODE mode = RUN_MODE::CLOCK;

void loop() {
  do_serial_command();
  if (digitalRead(MODE_PIN)) {
    mode
  }

  if (mode == RUN_MODE::CLOCK) {
    display_clock();
  }
}

void adjust_clock() {

}

void do_serial_command() {
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
}

byte current_minute = -1;

void display_calendar() {
  DateTime now = rtc.now();
  byte year = now.year();
  byte month = now.month();
  byte day = now.day();
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  display_clock(hour, minute, second);
  if (current_minute == minute) {
    return;
  }
  current_minute = minute;
  byte gd1 = day / 10;
  byte gd2 = day  % 10;
  byte gm1 = month / 10;
  byte gm2 = month  % 10;
  GregorianDate gdate = GregorianDate(year, month, day);
  LunarDate ldate = gdate.toLunarDate();
  int lunar_dd = ldate.d;
  int lunar_mm = ldate.m;
  byte ld1 = lunar_dd / 10;
  byte ld2 = lunar_dd  % 10;
  byte lm1 = lunar_mm / 10;
  byte lm2 = lunar_mm  % 10;
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  int int_temp = (int)(temperature);
  byte t1 = int_temp / 10;
  byte t2 = int_temp % 10;
  byte w = now.dayOfTheWeek() + 1;
  w = (w == 1 ? 8 : w);
  display_date(gd1, gd2, gm1, gm2, ld1, ld2, lm1, lm2, t1, t2, w);
}

void display_clock(byte h, byte m, byte s) {
  uint32_t color = COLORS[now.dayOfTheWeek()];
  byte h0 = h / 10;
  byte h1 = h % 10;
  byte m0 = m / 10;
  byte m1 = m  % 10;
  write_neo_digit(color, h0, 23);
  write_neo_digit(color, h1, 16);
  if (s % 2 == 0) {
    strip.setPixelColor(14, color);
    strip.setPixelColor(15, color);
  } else {
    strip.setPixelColor(14, NEO_OFF);
    strip.setPixelColor(15, NEO_OFF);
  }
  write_neo_digit(color, m0, 7);
  write_neo_digit(color, m1, 0);
  strip.show();
}

void write_neo_digit(uint32_t color, byte digit, byte start_index) {
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
      strip.setPixelColor(start_index + 0, NEO_OFF);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
  }
}

void display_date(byte gd1, byte gd2, byte gm1, byte gm2, byte ld1, byte ld2, byte lm1, byte lm2, byte t1, byte t2, byte wd) {
  calendar.batchWriteBegin();
  write_74595_digit_bit(0, wd);
  write_74595_digit_bit(1, t2);
  write_74595_digit_bit(2, t1);
  write_74595_digit_bit(3, lm2);
  write_74595_digit_bit(4, lm1);
  write_74595_digit_bit(5, ld2);
  write_74595_digit_bit(6, ld1);
  write_74595_digit_bit(7, gm2);
  write_74595_digit_bit(8, gm1);
  write_74595_digit_bit(9, gd2);
  write_74595_digit_bit(10, gd1);
  calendar.batchWriteEnd();
}

void write_74595_digit_bit(byte p, byte d) {
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
  } else {
    calendar.writeBit(i + 0, LOW);
    calendar.writeBit(i + 1, LOW);
    calendar.writeBit(i + 2, LOW);
    calendar.writeBit(i + 3, LOW);
    calendar.writeBit(i + 4, LOW);
    calendar.writeBit(i + 5, LOW);
    calendar.writeBit(i + 6, LOW);
    calendar.writeBit(i + 7, LOW);
  }
}
