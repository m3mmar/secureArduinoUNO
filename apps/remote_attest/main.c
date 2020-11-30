#include <avr/io.h>

#define BAUD 9600
#include <util/setbaud.h>

#include "microvisor.h"
#include "serial.h"

int main(void) {

  uint8_t buf[20];
  uint16_t i;

  uart_init();

  while(1) {
    // Receive nonce
    for(i=0; i<20; i++) {
      buf[i] = uart_getchar();
    }
    uart_putchar('o');

    // Do remote attest
    remote_attestation(buf);

    // Send attestation reponse to verifier over serial!
    for(i=0; i<20; i++) {
      uart_putchar(buf[i]);
    }
  }
}
