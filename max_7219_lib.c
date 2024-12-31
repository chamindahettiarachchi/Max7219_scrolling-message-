#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "max7219_lib.h"

// SPI initialization for ATmega32A
void spi_init(void) {
	// Set MOSI (PB5), SCK (PB7), and SS (PB4) as output
	DDRB |= (1 << PORTB5) | (1 << PORTB7) | (1 << PORTB4);
	
	// Enable SPI, Set as Master, Prescaler: Fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

// SPI send byte
void spi_send_byte(uint8_t data) {
	SPDR = data;  // Load data into the buffer
	while (!(SPSR & (1 << SPIF)));  // Wait until transmission complete
}

// Send a command to MAX7219
void max7219_send(uint8_t address, uint8_t data) {
	PORTB &= ~(1 << PORTB4);  // Bring CS (Chip Select) low (PB4)
	spi_send_byte(address);  // Send register address
	spi_send_byte(data);     // Send the data
	PORTB |= (1 << PORTB4);     // Bring CS (Chip Select) high (PB4)
}

// Initialize MAX7219
void max7219_init(void) {
	max7219_send(MAX7219_SHUTDOWN, 0x01);    // Exit shutdown mode
	max7219_send(MAX7219_DECODE_MODE, 0x00); // No decode for 8x8 matrix
	max7219_send(MAX7219_SCAN_LIMIT, 0x07);  // Display all 8 digits
	max7219_send(MAX7219_INTENSITY, 0x08);   // Set medium brightness
	max7219_send(MAX7219_DISPLAY_TEST, 0x00); // Disable display test
}

// Write one column of data to MAX7219
void max7219_write_column(uint8_t column, uint8_t value) {
	max7219_send(column + 1, value);  // Send to the corresponding digit register
}

// Scroll the message on the display
void scroll_message(const uint8_t* message, const uint8_t char_map[][8], uint16_t msg_len) {
	uint8_t buffer[8] = {0};  // Temporary display buffer

	for (uint16_t i = 0; i < msg_len; i++) {
		for (int v = 0; v < 8; v++) {
			// Shift the buffer to the left by 1 column
			for (int b = 0; b < 7; b++) {
				buffer[b] = buffer[b + 1];
			}

			// Load the next column from the current character
			uint8_t store = 0;
			uint8_t letter = message[i];

			for (uint8_t b = 0; b < 8; b++) {
				if (pgm_read_byte(&(char_map[letter][b])) & (1 << (7 - v))) {
					store |= (1 << b);
				}
			}

			buffer[7] = store;

			// Display the buffer
			for (int c = 0; c < 8; c++) {
				max7219_write_column(c, buffer[c]);
			}
			_delay_ms(150);  // Delay for smooth scrolling
		}
	}
}
