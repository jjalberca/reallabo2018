#include "encoder.h"
#include "stdio_serial.h"
#include "asf.h"
#include "fsm.h"


static void enc_isr(const uint32_t id, const uint32_t index);

static volatile int pos;
static volatile int fire;
static volatile int status_a;
static volatile int status_b;

static fsm_t fsm;
static int init = 0;

/*******************************************************************************
 * FUNCIONES FSM
*******************************************************************************/

enum {
	ESTADO_00,
	ESTADO_01,
	ESTADO_10,
	ESTADO_11
};

static void subir(fsm_t* this){
	++pos;
	fire = 0;
}

static void bajar(fsm_t* this){
	--pos;
	fire = 0;
}

static int estado00(fsm_t* this){
	if(!fire) return 0;
	return !status_a && !status_b;
}

static int estado01(fsm_t* this){
	if(!fire) return 0;
	return !status_a && status_b;
}

static int estado10(fsm_t* this){
	if(!fire) return 0;
	return status_a && !status_b;
}

static int estado11(fsm_t* this){
	if(!fire) return 0;
	return status_a && status_b;
}




static fsm_trans_t transiciones[] = {
	{ESTADO_00, estado01, ESTADO_01, bajar},
	{ESTADO_00, estado10, ESTADO_10, subir},
	{ESTADO_01, estado00, ESTADO_00, subir},
	{ESTADO_01, estado11, ESTADO_11, bajar},
	{ESTADO_10, estado00, ESTADO_00, bajar},
	{ESTADO_10, estado11, ESTADO_11, subir},
	{ESTADO_11, estado01, ESTADO_01, subir},
	{ESTADO_11, estado10, ESTADO_10, bajar},
	{-1, NULL, -1, NULL}
};



static void enc_isr(const uint32_t id, const uint32_t index){
	status_a = pio_get(ENC_A_PORT, PIO_TYPE_PIO_INPUT, ENC_A_PIN);
	status_b = pio_get(ENC_B_PORT, PIO_TYPE_PIO_INPUT, ENC_B_PIN);
	fire = 1;
	encoder_fire();
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
		PIO_PULLUP);// |
		//PIO_DEBOUNCE);
	//pio_set_debounce_filter(ENC_A_PORT,ENC_A_PIN, 100);
	pio_handler_set(ENC_A_PORT, ENC_A_ID, ENC_A_PIN, PIO_IT_EDGE, enc_isr);
	pio_enable_interrupt(ENC_A_PORT, ENC_A_PIN);

	// Config pin B
	NVIC_ClearPendingIRQ(ENC_B_IRQ);
	NVIC_SetPriority(ENC_B_IRQ, ENC_PRIORITY);

	pmc_enable_periph_clk(ENC_B_ID);
	pio_set_input(ENC_B_PORT, ENC_B_PIN, 0 |
		PIO_PULLUP);// |
		//PIO_DEBOUNCE);
	//pio_set_debounce_filter(ENC_B_PORT,ENC_B_PIN, 100);
	pio_handler_set(ENC_B_PORT, ENC_B_ID, ENC_B_PIN, PIO_IT_EDGE, enc_isr);
	pio_enable_interrupt(ENC_B_PORT, ENC_B_PIN);

	// Enable interrupts
	NVIC_EnableIRQ(ENC_A_IRQ);
	NVIC_EnableIRQ(ENC_B_IRQ);

	encoder_clear();


	fsm_init(&fsm, ESTADO_00, transiciones);
	init = 1;
}

int encoder_clear(void){
	pos = 0;
	return 0;
}

void encoder_fire(void){
	if(init) fsm_fire(&fsm);
}

int encoder_getpos(void){
	return pos;
}
