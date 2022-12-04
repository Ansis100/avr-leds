#include <avr/io.h>

typedef struct pin_t {
  volatile uint8_t* port;
  volatile uint8_t* pin;
  uint8_t index;
} pin;

// Set output pin high
void on(pin out);

// Set output pin low
void off(pin out);

// Check if button pin pressed
// Returns 1 for true and 0 for false
int check(pin in);