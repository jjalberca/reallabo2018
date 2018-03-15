/** File Name : fsm.c
* Description : Implementación de "fsm.h"
* Author	  : Alejandro Vicario Espinosa
* Date		  : 04-06-2017
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "fsm.h"

/* Definición de funciones----------------------------------------------------*/
fsm_t*
fsm_new (int state, fsm_trans_t* tt)
{
	fsm_t* this = (fsm_t*) malloc (sizeof (fsm_t)); //Se almacena espacio en memoria
	fsm_init (this, state, tt);          //Se inicializa la fsm
	return this;
}

void
fsm_init (fsm_t* this, int state, fsm_trans_t* tt)
{
	this->current_state = state;
	this->tt = tt;
}

void
fsm_destroy (fsm_t* this)
{
	free(this); //Se libera el espacio en memoria
}

void
fsm_fire (fsm_t* this)
{
    /*
    El bucle recorre la tabla ade transiciones y comprueba
    si el evento correspondiente se cumple
    */
	fsm_trans_t* t;
	for (t = this->tt; t->orig_state >= 0; ++t) {
		if ((this->current_state == t->orig_state) && t->in(this)) {
		    //Si se cumple se cambia el estado
			this->current_state = t->dest_state;
			if (t->out) //Si existiese la funcion de salida se ejecuta
				t->out(this);
			break;
		}
	}
}

int fsm_true(fsm_t* this){
	return 1;
}
int fsm_false(fsm_t* this){
	return 0;
}
