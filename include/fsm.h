/** File Name : fsm.h
* Description : Permite crear maquinas de estados finitos
* Author	  : Alejandro Vicario Espinosa
* Date		  : 04-06-2017
*/

#ifndef FSM_H_
#define FSM_H_

/* Definiciones de tipos------------------------------------------------------*/
/*
 Definición del tipo que va a corresponder al "objeto" fsm
 */
typedef struct fsm_t fsm_t;

/*
 Definición de las funciones de entrada y salida de los eventos
 de la máquina de estados
 */
typedef int (*fsm_input_func_t) (fsm_t*);
typedef void (*fsm_output_func_t) (fsm_t*);
/*
 Estructura de una entrada en la tabla de transiciones entre estados
 */
typedef struct fsm_trans_t {
  int orig_state;
  fsm_input_func_t in;
  int dest_state;
  fsm_output_func_t out;
} fsm_trans_t;
/*
 Variables del "objeto" fsm
 */
struct fsm_t {
  int current_state;
  fsm_trans_t* tt;
};

/* Prototipos de funciones----------------------------------------------------*/
/**
 * @brief   Almacena en memoria al fsm e inicializa los valores
 * @param   state       Estado inicial
 * @param   tt          Tabla de transiciones
 * @return Un puntero hacía la struct de la fsm
 */
fsm_t* fsm_new (int state, fsm_trans_t* tt);
/**
 * @brief   Inicializa los valores de un struct fsm_t
 * @param   this        Struct de la fsm que se quiere inicializar
 * @param   state       Estado inicial
 * @param   tt          Tabla de transiciones
 */
void fsm_init (fsm_t* this, int state, fsm_trans_t* tt);
/**
 * @brief   Ejecuta las transiciones y actualiza el estado de la fsm
 * @param   this    Puntero hacía la fsm que se quiere ejecutar
 */
void fsm_fire (fsm_t* this);
/**
 * @brief   Destruye y libera la memoria ocupada por una fsm
 * @param   this    Puntero hacía la fsm que se quiere destruir
 */
void fsm_destroy (fsm_t* this);


int fsm_true(fsm_t* this);
int fsm_false(fsm_t* this);

#endif /* FSM_H_ */
