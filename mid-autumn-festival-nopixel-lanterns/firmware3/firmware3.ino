#include <avr/io.h>

#define NEO_PIN       PB0
#define NEO_PIXELS    5
uint8_t NEO_brightness = 0;
#define NEO_latch()   _delay_us(281)

void setup() {
  DDRB |= (1 << NEO_PIN);
}

uint8_t start = 0;
uint8_t speed = 3;
uint8_t dense = 2;
uint8_t current = start;

void loop() {
  start += speed;
  if (start >= 192) start -= 192;
  for (uint8_t i = NEO_PIXELS; i; i--) {
    NEO_writeHue(current);
    current += dense;
    if (current >= 192) current -= 192;
  }
  _delay_ms(80);
  if (digitalRead(PB4) == LOW) {
    for (uint8_t i = NEO_PIXELS; i; i--) {
      NEO_writeColor(0, 0, 255);
    }
    delay(2000);
  } else if (digitalRead(PB3) == LOW) {
    for (uint8_t i = NEO_PIXELS; i; i--) {
      NEO_writeColor(255, 0, 0);
    }
    delay(2000);
  } else if (digitalRead(PB1) == LOW) {
    for (uint8_t i = NEO_PIXELS; i; i--) {
      NEO_writeColor(0, 255, 0);
    }
    delay(2000);
  }
}

void NEO_writeHue(uint8_t hue) {
  uint8_t phase = hue >> 6;
  uint8_t step  = (hue & 63) << NEO_brightness;
  uint8_t nstep = (63 << NEO_brightness) - step;
  switch (phase) {
    case 0:   NEO_writeColor(nstep,  step,     0); break;
    case 1:   NEO_writeColor(    0, nstep,  step); break;
    case 2:   NEO_writeColor( step,     0, nstep); break;
    default:  break;
  }
}

void NEO_writeColor(uint8_t r, uint8_t g, uint8_t b) {
  NEO_sendByte(r);
  NEO_sendByte(g);
  NEO_sendByte(b);
}

void NEO_sendByte(uint8_t byte) {               // CLK  comment
  for (uint8_t bit = 8; bit; bit--) asm volatile( //  3   8 bits, MSB first
      "sbi  %[port], %[pin]   \n\t"               //  2   DATA HIGH
      "sbrs %[byte], 7        \n\t"               // 1-2  if "1"-bit skip next instruction
      "cbi  %[port], %[pin]   \n\t"               //  2   "0"-bit: DATA LOW after 3 cycles
      "rjmp .+0               \n\t"               //  2   delay 2 cycles
      "add  %[byte], %[byte]  \n\t"               //  1   byte <<= 1
      "cbi  %[port], %[pin]   \n\t"               //  2   "1"-bit: DATA LOW after 7 cycles
      ::
      [port]  "I"   (_SFR_IO_ADDR(PORTB)),
      [pin]   "I"   (NEO_PIN),
      [byte]  "r"   (byte)
    );
}

void NEO_clear(void) {
  for (uint8_t i = NEO_PIXELS; i; i--) {
    NEO_writeColor(0, 0, 0);
  }
}
