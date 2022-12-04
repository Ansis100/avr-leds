#define __AVR_ATmega328P__  // Processor

#include <avr/interrupt.h>

#include "./io.h"

// Pin definitions
pin ledPins[9] = {
    [0] = {.port = &PORTD, .index = PORTD0},
    [1] = {.port = &PORTD, .index = PORTD1},
    [2] = {.port = &PORTD, .index = PORTD2},
    [3] = {.port = &PORTD, .index = PORTD3},
    [4] = {.port = &PORTD, .index = PORTD4},
    [5] = {.port = &PORTD, .index = PORTD5},
    [6] = {.port = &PORTD, .index = PORTD6},
    [7] = {.port = &PORTD, .index = PORTD7},
    [8] = {.port = &PORTB, .index = PORTB0},
};
pin indicatorPin = {.port = &PORTB, .index = PORTB1};
pin btnPin = {.port = &PORTC, .pin = &PINC, .index = PORTC0};

volatile int currLED = 0;
int btnPressed = 0;
int btnCtr = 0;
int dirFlag = 0;

int main(void) {
  // Setup the LED clock
  cli();                            // Disable global interrupts
  TCCR1B |= 1 << CS11 | 1 << CS10;  // Divide 16MHz by 1024 = 250000
  OCR1A = 25000;                    // Interrupt every 100ms
  TCCR1B |= 1 << WGM12;             // Put Timer/Counter1 in CTC mode
  TIMSK1 |= 1 << OCIE1A;            // Enable timer compare interrupt
  sei();                            // Enable global interrupts

  // Set LED pins as output
  DDRD |= 0xFF;  // All of D register
  DDRB |= 0x1;   // First of B register

  // Pull up button
  on(btnPin);

  while (1) {
    if (check(btnPin)) {
      // Debounce
      btnCtr++;
      if (btnCtr >= 10000) {
        // Toggle direction flag only once
        if (!btnPressed) dirFlag = dirFlag == 0 ? 1 : 0;
        btnPressed = 1;
        btnCtr = 0;
      }
    } else {
      btnCtr = 0;
      btnPressed = 0;
    }
  }
}

// LED timer interrupt
ISR(TIMER1_COMPA_vect) {
  off(ledPins[currLED]);
  if (dirFlag) {
    if (currLED >= 8)
      currLED = 0;
    else
      ++currLED;
  } else {
    if (currLED <= 0)
      currLED = 8;
    else
      --currLED;
  }
  on(ledPins[currLED]);
}