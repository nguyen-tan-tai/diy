int S74HC595_DATA_PIN = 5;
int S74HC595_SHIFT_PIN = 7;
int S74HC595_LATCH_PIN = 6;

int digits[10][8] {
  {0, 0, 0, 1, 0, 1, 0, 0}, // digit 0
  {0, 1, 1, 1, 0, 1, 1, 1}, // digit 1
  {1, 0, 0, 0, 0, 1, 0, 1}, // digit 2
  {0, 0, 1, 0, 0, 1, 0, 1}, // digit 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // digit 4
  {0, 0, 1, 0, 1, 1, 0, 0}, // digit 5
  {0, 0, 0, 0, 1, 1, 0, 0}, // digit 6
  {0, 1, 1, 1, 0, 1, 0, 1}, // digit 7
  {0, 0, 0, 0, 0, 1, 0, 0}, // digit 8
  {0, 0, 1, 0, 0, 1, 0, 0}  // digit 9
};

void setup() {
  pinMode(S74HC595_DATA_PIN, OUTPUT);
  pinMode(S74HC595_SHIFT_PIN, OUTPUT);
  pinMode(S74HC595_LATCH_PIN,  OUTPUT);
  shift(8);
  shift(5);
}

void loop() {
//  for (int i = 0; i < 10; i++) {
//    display_digit(i);
//    delay(1000);
//  }
}

void shift(int d) {
  digitalWrite(S74HC595_SHIFT_PIN, LOW);
  for (int i = 7; i >= 0; i--)  {
    digitalWrite(S74HC595_LATCH_PIN, LOW);
    if (digits[d][i] == 1) {
      digitalWrite(S74HC595_DATA_PIN,  LOW);
    }
    if (digits[d][i] == 0) {
      digitalWrite(S74HC595_DATA_PIN, HIGH);
    }
    digitalWrite(S74HC595_LATCH_PIN, HIGH);
  }
  digitalWrite(S74HC595_SHIFT_PIN, HIGH);
}
