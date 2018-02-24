#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include <delay.h>

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Blink LED Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * Application entry point
 */
int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Configure the console uart for debug information */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Enable PWM peripheral clock */

	pmc_enable_periph_clk(ID_PIOB);
	pio_set_output(PIOB, PIO_PB27, LOW, DISABLE, ENABLE);

	/* Infinite loop */
	while (1) {
		delay_ms(500);
		pio_clear(PIOB, PIO_PB27);
		delay_ms(500);
		pio_set(PIOB, PIO_PB27);
	}
}
