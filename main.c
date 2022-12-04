#define __AVR_ATmega328P__

#include <avr/interrupt.h>
#include <avr/io.h>

typedef struct pin_t {
  volatile uint8_t* port;
  volatile uint8_t* pin;
  uint8_t index;
} pin;

pin outPins[10] = {
    [0] = {.port = &PORTD, .index = PORTD0},
    [1] = {.port = &PORTD, .index = PORTD1},
    [2] = {.port = &PORTD, .index = PORTD2},
    [3] = {.port = &PORTD, .index = PORTD3},
    [4] = {.port = &PORTD, .index = PORTD4},
    [5] = {.port = &PORTD, .index = PORTD5},
    [6] = {.port = &PORTD, .index = PORTD6},
    [7] = {.port = &PORTD, .index = PORTD7},
    [8] = {.port = &PORTB, .index = PORTB0},
    [9] = {.port = &PORTB, .index = PORTB1},
};
volatile int currLED = 0;
int btnPressed = 0;
int btnCtr = 0;
int dirFlag = 0;

int main(void) {
  // Setup the LED clock
  cli();                            // Disable global interrupts
  TCCR1B |= 1 << CS11 | 1 << CS10;  // Divide by 1024
  OCR1A = 15624;                    // Count 15624 cycles for 1 second interrupt
  TCCR1B |= 1 << WGM12;             // Put Timer/Counter1 in CTC mode
  TIMSK1 |= 1 << OCIE1A;            // enable timer compare interrupt
  sei();                            // Enable global interrupts

  // Set output pins
  DDRD |= 0xFF;
  DDRB |= 0x3;

  pin btn = {.port = &PORTC, .pin = &PINC, .index = PORTC0};
  on(btn);  // Pull up button

  while (1) {
    // if (!(*btn.pin & (1 << btn.index))) currLED++;
    if (*btn.pin & (1 << btn.index)) {
      if (btnPressed == 0) {
        ++btnCtr;

        dirFlag = dirFlag == 0 ? 1 : 0;  // Toggle direction flag
        btnPressed = 1;
      }
    } else {
      if (btnPressed == 1) {
        btnPressed = 0;
      }
    }
    if (dirFlag)
      PORTB |= (1 << 1);
    else
      PORTB &= ~(1 << 1);
  }
}

void on(pin out) { *out.port |= (1 << out.index); }

void off(pin out) { *out.port &= ~(1 << out.index); }

ISR(TIMER1_COMPA_vect) {
  off(outPins[currLED]);
  if (dirFlag)
    if (currLED == 8)
      currLED = 0;
    else
      ++currLED;
  else if (currLED == 0)
    currLED = 8;
  else
    --currLED;
  on(outPins[currLED]);
}