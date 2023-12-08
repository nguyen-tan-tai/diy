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
#define ADJUST_INCREASE 10
#define ADJUST_DECREASE 12

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
#define BIT_CNT 88 // 11 seven-segment digits

void setup() {
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

  // Neopixel
  strip.begin();
  strip.setBrightness(brightness);

  // 74HC595
  calendar.setBitCount(BIT_CNT);
  calendar.setPins(SHIFT_DATA, SHIFT_CLOCK, SHIFT_LATCH);
}

#define MODE_NORMAL 1
#define MODE_ADJ_YEAR 2
#define MODE_ADJ_MONTH 3
#define MODE_ADJ_DAY 4
#define MODE_ADJ_HOUR 5
#define MODE_ADJ_MINUTE 6

byte mode = MODE_NORMAL;

void loop() {
  do_serial_command();
  if (digitalRead(MODE_PIN) == HIGH) {
    mode = (mode == MODE_ADJ_MINUTE) ? 1 : (mode + 1);
    delay(250);
  }
  if (mode == MODE_NORMAL) {
    display_calendar();
    return;
  }
  display_adjust();
}

void display_adjust() {
  boolean inc = digitalRead(ADJUST_INCREASE);
  boolean dec = digitalRead(ADJUST_DECREASE);
  DateTime now = rtc.now();
  byte year = now.year();
  byte month = now.month();
  byte day = now.day();
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  if (inc || dec) {
    byte value = (inc ? 1 : 0) + (dec ? -1 : 0);
    if (mode == MODE_ADJ_YEAR) {
      rtc.adjust(DateTime(year + value, month, day, hour, minute, second));
    } else if (mode == MODE_ADJ_MONTH) {
      rtc.adjust(DateTime(year, month + value, day, hour, minute, second));
    } else if (mode == MODE_ADJ_DAY) {
      rtc.adjust(DateTime(year, month, day + value, hour, minute, second));
    } else if (mode == MODE_ADJ_HOUR) {
      rtc.adjust(DateTime(year, month, day, hour + value, minute, second));
    } else if (mode == MODE_ADJ_MINUTE) {
      rtc.adjust(DateTime(year, month, day, hour, minute + value, 0));
    }
    delay(250);
  }
  if (mode == MODE_ADJ_YEAR) {
    display_clock(0, -1, -1, -1);
    display_date(year / 1000, year / 100, year / 10, y % 10, -1, -1, -1, -1, -1, -1, -1);
  } else if (mode == MODE_ADJ_MONTH) {
    display_clock(0, -1, -1, -1);
    display_date(-1, -1, month / 10, m % 10, -1, -1, -1, -1, -1, -1, -1);
  } else if (mode == MODE_ADJ_DAY) {
    display_clock(0, -1, -1, -1);
    display_date(day / 10, day % 10, -1, -1, -1, -1, -1, -1, -1, -1, -1);
  } else if (mode == MODE_ADJ_HOUR) {
    display_clock(0, hour, -1, -1);
    display_date(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
  } else if (mode == MODE_ADJ_MINUTE) {
    display_clock(0, -1, minute, -1);
    display_date(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
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
  byte dayofweek = now.dayOfTheWeek();
  display_clock(dayofweek, hour, minute, second);
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
  display_date(gd1, gd2, gm1, gm2, ld1, ld2, lm1, lm2, t1, t2, (dayofweek == 0 ? 8 : dayofweek + 1));
}

void display_clock(byte w, byte h, byte m, byte s) {
  uint32_t color = COLORS[w];
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
