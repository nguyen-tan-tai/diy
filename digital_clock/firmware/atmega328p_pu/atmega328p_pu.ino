#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include "RTClib.h"
RTC_DS3231 rtc;

#include <FastLED_NeoPixel.h>
// How many LEDs are attached to the Arduino?
#define NUM_LEDS 30
// Which pin on the Arduino is connected to the LEDs?
#define DATA_PIN 9

#define VOL_PIN A3
#define BRIGHTNESS_PIN A2
#define QUICK_ADJ_PIN A1

FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;      // <- FastLED NeoPixel version
#define NEO_OFF strip.Color(0, 0, 0)

SoftwareSerial softSerial(10, 11);
DFRobotDFPlayerMini mp3;

int vol = 10;
int brightness = 100;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  softSerial.begin(9600);
  if (!mp3.begin(softSerial, false, false)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {}
  }
  Serial.println(F("DFPlayer Mini online."));
  mp3.setTimeOut(500); //Set serial communictaion time out 500ms
  mp3.volume(vol);
  mp3.playMp3Folder(1);
  mp3.volume(20);
  mp3.playMp3Folder(2);

  // DS3231
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

  // NEO
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
    } else if (command.startsWith("DANCE")) {
      String s = command.substring(5, command.length());
      s.trim();
      int seconds = s.toInt();
      Serial.println("Dancing for " + String(seconds) + " seconds");
      dance(seconds);
    }
  }
  if (digitalRead(QUICK_ADJ_PIN) == LOW) {
    quick_adjust();
  }
  set_volume();
  set_brightness();
  display_clock();
  fire_alarm();
}

void quick_adjust() {
  Serial.println("adjust");
  DateTime now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), 8, 15, 1));
  delay(500);
}

void set_volume() {
  int newVal = map(analogRead(VOL_PIN), 0, 1023, 0, 30);
  if (abs(vol - newVal) < 2) {
    return;
  }
  if (newVal < 2) {
    newVal = 0;
  }
  Serial.print("volume ");
  Serial.print(vol);
  Serial.print("->");
  Serial.println(newVal);
  vol = (uint8_t) newVal;
  uint8_t uint8tVol = (uint8_t) vol;
  mp3.volume(vol);
  delay(500);
}

void set_brightness() {
  int newVal = map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 255);
  if (abs(brightness - newVal) < 10) {
    return;
  }
  if (newVal < 3) {
    newVal = 0;
  }
  Serial.print("brightness ");
  Serial.print(brightness);
  Serial.print("->");
  Serial.println(newVal);
  brightness = newVal;
  strip.setBrightness(brightness);
  delay(500);
}

void fire_alarm() {
  DateTime now = rtc.now();
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  if (now.dayOfTheWeek() == 0 || now.dayOfTheWeek() == 6) {
    return;
  }
  if (hour == 7 && minute == 50 && second == 0) {
    mp3.playMp3Folder(750);
    dance(10);
  } else if (hour == 7 && minute == 58 && second == 0) {
    mp3.playMp3Folder(758);
  } else if (hour == 8 && minute == 0 && second == 0) {
    mp3.playMp3Folder(800);
    dance(10);
  } else if (hour == 9 && minute == 0 && second == 0) {
    mp3.playMp3Folder(900);
    dance(10);
  } else if (hour == 10 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1000);
    dance(10);
  } else if (hour == 11 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1100);
    dance(10);
  } else if (hour == 12 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1200);
    dance(10);
  } else if (hour == 13 && minute == 28 && second == 0) {
    mp3.playMp3Folder(1328);
  } else if (hour == 13 && minute == 30 && second == 0) {
    mp3.playMp3Folder(1330);
    dance(10);
  } else if (hour == 14 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1400);
    dance(10);
  } else if (hour == 15 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1500);
    dance(10);
  } else if (hour == 16 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1600);
    dance(10);
  } else if (hour == 17 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1700);
    dance(10);
  } else if (hour == 17 && minute == 30 && second == 0) {
    mp3.playMp3Folder(1730);
    dance(10);
  } else if (hour == 18 && minute == 0 && second == 0) {
    mp3.playMp3Folder(1800);
    dance(10);
  }
}

long stop_dance_millis = 0;

void dance(int seconds) {
  stop_dance_millis = millis() + seconds * 1000;
}

const uint32_t COLORS[7] = {
  strip.Color(255, 0, 0),
  strip.Color(255, 127, 0),
  strip.Color(255, 255, 0),
  strip.Color(0, 255, 0),
  strip.Color(0, 0, 255),
  strip.Color(75, 0, 130),
  strip.Color(148, 0, 211)
};

byte current_color = 0;
long last_change_color = 0;

uint32_t getColor(int dayOfTheWeek) {
  long current_millis = millis();
  if (current_millis < stop_dance_millis) {
    if (current_millis - last_change_color >= 250) {
      last_change_color = current_millis;
      current_color = current_color >= 6 ? 0 : current_color + 1;
    }
    return COLORS[current_color];
  }
  return COLORS[dayOfTheWeek];
}

void display_clock() {
  DateTime now = rtc.now();
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  uint32_t color = getColor(now.dayOfTheWeek());
  byte h0 = hour / 10;
  byte h1 = hour % 10;
  byte m0 = minute / 10;
  byte m1 = minute  % 10;
  display_7_segment(color, h0, 0);
  display_7_segment(color, h1, 7);
  if (second % 2 == 0) {
    strip.setPixelColor(14, color);
    strip.setPixelColor(15, color);
  } else {
    strip.setPixelColor(14, NEO_OFF);
    strip.setPixelColor(15, NEO_OFF);
  }
  display_7_segment(color, m0, 16);
  display_7_segment(color, m1, 23);
  strip.show();
}

void display_7_segment(uint32_t color, byte digit, byte start_index) {
  switch (digit) {
    case 0:
      strip.setPixelColor(start_index, NEO_OFF);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 1:
      strip.setPixelColor(start_index, NEO_OFF);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 2:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, NEO_OFF);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 3:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 4:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, NEO_OFF);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 5:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 6:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, NEO_OFF);
      break;
    case 7:
      strip.setPixelColor(start_index, NEO_OFF);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, NEO_OFF);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, NEO_OFF);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 8:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, color);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    case 9:
      strip.setPixelColor(start_index, color);
      strip.setPixelColor(start_index + 1, color);
      strip.setPixelColor(start_index + 2, color);
      strip.setPixelColor(start_index + 3, NEO_OFF);
      strip.setPixelColor(start_index + 4, color);
      strip.setPixelColor(start_index + 5, color);
      strip.setPixelColor(start_index + 6, color);
      break;
    default:
      break;
  }
}

void rainbow(unsigned long wait, unsigned int numLoops) {
  for (unsigned int count = 0; count < numLoops; count++) {
    // iterate through all 8-bit hues, using 16-bit values for granularity
    for (unsigned long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
      for (unsigned int i = 0; i < strip.numPixels(); i++) {
        unsigned long pixelHue = firstPixelHue + (i * 65536UL / strip.numPixels()); // vary LED hue based on position
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));  // assign color, using gamma curve for a more natural look
      }
      strip.show();
      delay(wait);
    }
  }
}
