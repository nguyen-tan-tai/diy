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
