#include <avr/pgmspace.h>
#include <avr/io.h>

#define BAUD 9600
#include <util/setbaud.h>

#include "serial.h"


void uart_init(void) {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}

char uart_getchar() {
  char c;
  loop_until_bit_is_set(UCSR0A, RXC0);
  c = UDR0;
  return c;
}

void uart_putchar(char c) {
  if (c == '\n') {
    uart_putchar('\r');
  }
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

void uart_puts(char *c) {
  while(*c) {
    uart_putchar(*c++);
  }
}