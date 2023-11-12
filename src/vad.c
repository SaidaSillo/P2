#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */
float p1= -30;
/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  feat.p = compute_power(x,N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate, float umbral1, float umbral2, float tvoice, float tsilence) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->umbral1 = umbral1;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;
  if(state == ST_MS) 
    state = ST_VOICE;
  if(state == ST_MV) 
    state = ST_SILENCE;
  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

<<<<<<< HEAD

=======
>>>>>>> 97b9f7e3ca6bf47695ad1fe01e17b41f44c5fab8
  switch (vad_data->state) {
    
    case ST_INIT:
      vad_data->umbral1 = f.p + vad_data->umbral1;
      vad_data->umbral2 = f.p + vad_data->umbral2;
      vad_data->state = ST_SILENCE;
      break;

    case ST_SILENCE:
      if (f.p > vad_data->umbral2){     /*Si estamos en el estado SILENCE y la potencia supera el umbral2, entonces el estado futuro es MAYBE VOICE*/
        vad_data->state = ST_MV;      
        vad_data->mv++;
      }
      break;

   case ST_VOICE:
      if (f.p < vad_data->umbral1){     /*Si estamos en el estado VOICE y la potencia NO supera el umbral 1, entonces el estado futuro es MAYBE SILENCE*/
        vad_data->state = ST_SILENCE;
        vad_data->ms++;
      }
      break;


    case ST_MV:
      if(f.p>vad_data->umbral1){      /*Si estamos en el estado MAYBE VOICE y la potencia supera el umbral 1*/
        if(vad_data->mv*vad_data->frame_length/vad_data->sampling_rate > vad_data->tvoice){ /*duracion de la trama = frame_length/samplig_rate*/
          vad_data->state = ST_VOICE; /*Si el tiempo en MAYBE VOICE supera el tiempo minimo para considerar que hay voz  */
          vad_data->mv=0;             /*El estado futuro sera VOICE y reinciamos el tiempo en MAYBE VOICE*/
        }
        else{
          vad_data->mv++;           /*Si no lo supera, se queda en MAYBE VOICE*/
        }
      }
      else if(f.p < vad_data->umbral2){   /*Si estamos en el estado MAYBE VOICE y la potencia no supera el umbral 2*/
        vad_data->state=ST_SILENCE;     /*El futuro estado sera silencio*/
        vad_data->ms=0;                 /*Reiniciamos el tiempo en MAYBE SILENCE a cero*/
      }
      break;

    case ST_MS:
      if(f.p < vad_data->umbral2){
        if(vad_data->ms*vad_data->frame_length/vad_data->sampling_rate > vad_data->tsilence){ /*duracion de la trama = frame_length/samplig_rate*/
          vad_data->state = ST_SILENCE; /*Si el tiempo en MAYBE SILENCE supera el tiempo minimo para considerar que hay silencio  */
          vad_data->ms=0;             /*El estado futuro sera SILENCE y reinciamos el tiempo en MAYBE SILENCE*/
        }
        else{
          vad_data->ms++;     /*Si no lo supera, se queda en MAYBE SILENCE*/
        }
      }
      else if (f.p > vad_data->umbral1){   /*Si estamos en el estado MAYBE SILENCE y la potencia supera el umbral 1*/
        vad_data->state=ST_VOICE;         /*El futuro estado sera VOICE*/
        vad_data->mv=0;                   /*Reiniciamos el tiempo en MAYBE VOICE a cero*/
      }

    break;

    case ST_UNDEF:
      break;
<<<<<<< HEAD

 
=======
>>>>>>> 97b9f7e3ca6bf47695ad1fe01e17b41f44c5fab8
  }

  if (vad_data->state == ST_SILENCE || vad_data->state == ST_VOICE)
    return vad_data->state;
  else
    return ST_UNDEF;
}


void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
<<<<<<< HEAD
}

=======
}
>>>>>>> 97b9f7e3ca6bf47695ad1fe01e17b41f44c5fab8
