#include "main.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

volatile uint32_t sys_ticks = 0;

void SysTick_Handler(void)
{
	sys_ticks++;
}

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

void due_init(void){

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Configure the console uart for debug information */
	configure_console();

	if (SysTick_Config(84000000U / 1000U)) {
		puts("-F- Systick configuration error\r");
		while (1);
	}

}
