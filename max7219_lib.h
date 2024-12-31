#ifndef MAX7219_LIB_H
#define MAX7219_LIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// Define MAX7219 register addresses
#define MAX7219_NO_OP        0x00
#define MAX7219_DIGIT0       0x01
#define MAX7219_DIGIT1       0x02
#define MAX7219_DIGIT2       0x03
#define MAX7219_DIGIT3       0x04
#define MAX7219_DIGIT4       0x05
#define MAX7219_DIGIT5       0x06
#define MAX7219_DIGIT6       0x07
#define MAX7219_DIGIT7       0x08
#define MAX7219_DECODE_MODE  0x09
#define MAX7219_INTENSITY    0x0A
#define MAX7219_SCAN_LIMIT   0x0B
#define MAX7219_SHUTDOWN     0x0C
#define MAX7219_DISPLAY_TEST 0x0F

// Function declarations
void spi_init(void);
void max7219_init(void);
void max7219_write_column(uint8_t column, uint8_t value);
void scroll_message(const uint8_t* message, const uint8_t char_map[][8], uint16_t msg_len);

#endif
