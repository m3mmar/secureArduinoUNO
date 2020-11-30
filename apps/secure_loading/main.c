#include <avr/pgmspace.h>
#include <avr/io.h>

#define BAUD 9600
#include <util/setbaud.h>
#include "serial.h"
#include "microvisor.h"



void clear_buf(uint8_t *buf) {
  uint16_t i;
  for(i=0; i<PAGE_SIZE; i++)
    buf[i]=0xFF;
}

int main(void) {
  uint8_t buffer[PAGE_SIZE];
  uint16_t i;
  uint16_t j;

  uint16_t total_size;
  uint16_t nr_2ndwords;

  uart_init();



  while(1) {
    // No memset support!
    clear_buf(buffer);

    // Total size (Arrives little endian, NOT network order!)
    buffer[0] = uart_getchar();
    buffer[1] = uart_getchar();
    total_size = buffer[1]<<8 | buffer[0];

    // Data start
    buffer[2] = uart_getchar();
    buffer[3] = uart_getchar();

    // Nr 2nd words (Arrives little endian, NOT network order!)
    buffer[4] = uart_getchar();
    buffer[5] = uart_getchar();
    nr_2ndwords = buffer[5]<<8 | buffer[4];

    // Unsafe 2nd words
    for(i=0; i<nr_2ndwords; i++) {
      buffer[6+i*2] = uart_getchar();
      buffer[7+i*2] = uart_getchar();
    }

    // HMAC digest (20 bytes)
    for(i=0; i<20; i++) {
      buffer[6+nr_2ndwords*2+i] = uart_getchar();
    }

    // Write to flash + ready to receive more
    load_image(buffer, METADATA_OFFSET);
    uart_putchar('o');

    // Round down total_size/PAGE_SIZE to get amount of full pages to write
    for(j=0; j < total_size/PAGE_SIZE; j++) {
      for(i=0; i<PAGE_SIZE; i++) {
        buffer[i] = uart_getchar();
      }
      load_image(buffer, PAGE_SIZE*j);
      uart_putchar('o');
    }

    // Receive last (incomplete) page
    if(total_size%PAGE_SIZE) {
      // No memset support!
      clear_buf(buffer);
      // Write possible last incomplete page..
      for(i=0; i<(total_size%PAGE_SIZE); i++) {
        buffer[i] = uart_getchar();
      }
      load_image(buffer, PAGE_SIZE*j);
      uart_putchar('o');
    }

    // Everything received, done
    uart_putchar('d');

    // Verify and activate if secure
    verify_activate_image();
  }
}
