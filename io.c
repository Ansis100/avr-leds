#include "./io.h"

void on(pin out) { *out.port |= (1 << out.index); }

void off(pin out) { *out.port &= ~(1 << out.index); }

int check(pin in) { return *in.pin & (1 << in.index) ? 0 : 1; }