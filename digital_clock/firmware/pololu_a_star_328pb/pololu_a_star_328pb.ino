#include "OneWire.h"
#include "DallasTemperature.h"
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


#include "RTClib.h"
RTC_DS3231 rtc;

#include <FastLED_NeoPixel.h>
// How many LEDs are attached to the Arduino?
#define NUM_LEDS 60
// Which pin on the Arduino is connected to the LEDs?
#define DATA_PIN 9
// LED brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 100
FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;      // <- FastLED NeoPixel version
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

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin();
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
  strip.setBrightness(BRIGHTNESS);
}

float temperature = 0.0;
long last_fetch_temperature = -60000;

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    if (command.startsWith("SET_TIME")) {
      //SET_TIME 2023-01-03T03:04:05
      String isoTime = command.substring(8, command.length());
      isoTime.trim();
      int y = isoTime.substring(0, 4).toInt();
      int mo = isoTime.substring(5, 7).toInt();
      int d = isoTime.substring(8, 10).toInt();
      int h = isoTime.substring(11, 13).toInt();
      int mi = isoTime.substring(14, 16).toInt();
      int s = isoTime.substring(17, 19).toInt();
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
      rtc.adjust(DateTime(y, mo, d, h, mi, s));
    } else if (command.startsWith("GET_TEMP")) {
      Serial.println(String(temperature, 2));
    } else if (command.startsWith("THEATER_CHASE")) {
      theaterChase(strip.Color(255, 255, 0), 100, 3, 5);
    } else if (command.startsWith("RAINBOW")) {
      rainbow(10, 3);
    }
  }
  display_clock();
  fetch_temperature();
  delay(1000);
}

// re-fetch temperature after 1 minute
void fetch_temperature() {
  if (millis() - last_fetch_temperature < 30000) {
    return;
  }
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  last_fetch_temperature = millis();
}

void display_clock() {
  strip.clear();
  DateTime now = rtc.now();
  uint32_t color = COLORS[now.dayOfTheWeek()];
  byte hour = now.hour();
  byte minute = now.minute();
  byte second = now.second();
  byte h0 = hour / 10;
  byte h1 = hour % 10;
  byte m0 = minute / 10;
  byte m1 = minute  % 10;
  byte s0 = second  / 10;
  byte s1 = second  % 10;
  int int_temp = (int)(temperature * 100);
  byte t0 = (byte)(int_temp / 1000);
  byte t1 = (byte)((int_temp - (t0 * 1000)) / 100);
  byte t2 = (byte)(int_temp - (t0 * 1000) - (t1 * 100));
  display_7_segment(color, h0, 0);
  display_7_segment(color, h1, 7);
  display_7_segment(color, m0, 14);
  display_7_segment(color, m1, 21);
  display_7_segment(color, s0, 28);
  display_7_segment(color, s1, 35);
  display_7_segment(get_alert_color(t0 * 10 + t1), t0, 42);
  display_7_segment(get_float_color(t2), t1, 49);
  strip.show();
}

uint32_t get_alert_color(int temp) {
  if (temp >= 34) {
    return COLORS[0];
  } else if (temp >= 32) {
    return COLORS[1];
  } else if (temp >= 30) {
    return COLORS[2];
  } else if (temp >= 28) {
    return COLORS[3];
  } else if (temp >= 26) {
    return COLORS[4];
  } else {
    return COLORS[5];
  }
}

uint32_t get_float_color(int float_temp) {
  if (float_temp >= 80) {
    return COLORS[0];
  } else if (float_temp >= 60) {
    return COLORS[1];
  } else if (float_temp >= 40) {
    return COLORS[2];
  } else if (float_temp >= 20) {
    return COLORS[3];
  } else {
    return COLORS[4];
  }
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

void theaterChase(uint32_t color, unsigned long wait, unsigned int groupSize, unsigned int numChases) {
  for (unsigned int chase = 0; chase < numChases; chase++) {
    for (unsigned int pos = 0; pos < groupSize; pos++) {
      strip.clear();  // turn off all LEDs
      for (unsigned int i = pos; i < strip.numPixels(); i += groupSize) {
        strip.setPixelColor(i, color);  // turn on the current group
      }
      strip.show();
      delay(wait);
    }
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
