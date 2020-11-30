#include <avr/pgmspace.h>
#include <avr/io.h>

#define BAUD 9600
#include <util/setbaud.h>

#include "microvisor.h"
#include "serial.h"



int main(void) {
  uart_init();
  uart_puts("test\n");

  // Never return from main()
  while(1);
}
