#include "encoder.h"
#include "stdio_serial.h"
#include "asf.h"


static void enc_isr(const uint32_t id, const uint32_t index);

volatile static int pos;

static void enc_isr(const uint32_t id, const uint32_t index){
	int pin_a = pio_get(ENC_A_PORT, PIO_TYPE_PIO_INPUT, ENC_A_PIN);
	int pin_b = pio_get(ENC_B_PORT, PIO_TYPE_PIO_INPUT, ENC_B_PIN);
	printf("Interrupt: a=%d, b=%d\r\n", pin_a, pin_b);
}

void encoder_init(void){
	// Disable interrutps
	NVIC_DisableIRQ(ENC_A_IRQ);
	NVIC_DisableIRQ(ENC_B_IRQ);

	// Config pin A
	NVIC_ClearPendingIRQ(ENC_A_IRQ);
	NVIC_SetPriority(ENC_A_IRQ, ENC_PRIORITY);

	pmc_enable_periph_clk(ENC_A_ID);
	pio_set_input(ENC_A_PORT, ENC_A_PIN, 0 |
		PIO_PULLUP |
		PIO_DEBOUNCE);
	pio_set_debounce_filter(ENC_A_PORT,ENC_A_PIN, 100);
	pio_handler_set(ENC_A_PORT, ENC_A_ID, ENC_A_PIN, PIO_IT_EDGE, enc_isr);
	pio_enable_interrupt(ENC_A_PORT, ENC_A_PIN);

	// Config pin B
	NVIC_ClearPendingIRQ(ENC_B_IRQ);
	NVIC_SetPriority(ENC_B_IRQ, ENC_PRIORITY);

	pmc_enable_periph_clk(ENC_B_ID);
	pio_set_input(ENC_B_PORT, ENC_B_PIN, 0 |
		PIO_PULLUP |
		PIO_DEBOUNCE);
	pio_set_debounce_filter(ENC_B_PORT,ENC_B_PIN, 100);
	pio_handler_set(ENC_B_PORT, ENC_B_ID, ENC_B_PIN, PIO_IT_EDGE, enc_isr);
	pio_enable_interrupt(ENC_B_PORT, ENC_B_PIN);

	// Enable interrupts
	NVIC_EnableIRQ(ENC_A_IRQ);
	NVIC_EnableIRQ(ENC_B_IRQ);
}
