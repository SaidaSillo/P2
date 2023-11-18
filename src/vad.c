#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */
const int N_TRAMAS = 9;         // Trames que agafem

// float p1= -30;
/*
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
    "UNDEF", "S", "V", "INIT", "MV", "MS"};

const char *state2str(VAD_STATE st)
{
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct
{
  float zcr;
  float p;
  float am;
} Features;

/*
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N)
{
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
  feat.p = compute_power(x, N);
  feat.zcr = compute_zcr(x, N, 16000);
  feat.am = compute_am(x, N);
  return feat;
}

/*
 * TODO: Init the values of vad_data
 */

VAD_DATA *vad_open(float rate, float alfa1, float alfa2)
{
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->alfa1 = alfa1;
  vad_data->alfa2 = alfa2;
  vad_data->n = 0;
  vad_data->p = 0;
  vad_data->timer = 0;
  vad_data->cont = 0;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data)
{
  /*
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;
  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data)
{
  return vad_data->frame_length;
}

/*
 * TODO: Implement the Voice Activity Detection
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x)
{

  /*
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p;

  switch (vad_data->state)
  {
  case ST_INIT:
    vad_data->cont++;
    if (vad_data->n < N_TRAMAS)
    {
      vad_data->p = vad_data->p + f.p;
      vad_data->n++;
    }
    else
    {
      vad_data->k0 = vad_data->p / N_TRAMAS;
      vad_data->k1 = vad_data->k0 + vad_data->alfa1;
      vad_data->k2 = vad_data->k1 + vad_data->alfa2;
      // printf("k0 = %f\n", vad_data->k0);
      // printf("k1 = %f\n", vad_data->k1);
      // printf("k2 = %f\n\n\n", vad_data->k2);
      vad_data->state = ST_SILENCE;
    }
    break;

  case ST_SILENCE:
    if (f.p > vad_data->k1)
    { /*Si estamos en el estado SILENCE y la potencia supera el umbral1, entonces el estado futuro es MAYBE VOICE*/
      vad_data->state = ST_MV;
      vad_data->cont = 0;
    }
    else
    {
      vad_data->cont++;
    }
    break;

  case ST_VOICE:
    if (f.p > vad_data->k0 && f.p < vad_data->k1)
    { /*Si estamos en el estado VOICE y la potencia NO supera el umbral2, entonces el estado futuro es MAYBE SILENCE*/
      vad_data->state = ST_MS;
      vad_data->cont = 0;
    }
    else
    {
      vad_data->cont++;
    }
    break;

  case ST_MV:
    vad_data->timer = vad_data->timer + FRAME_TIME;
    if (f.p < vad_data->k0 && vad_data->timer > MIN_SILENCE)
    {
      vad_data->state = ST_SILENCE;
      vad_data->timer = 0;
    }
    else if (f.p > vad_data->k2 && vad_data->timer > MIN_VOICE)
    {
      vad_data->state = ST_VOICE;
      vad_data->timer = 0;
    }
    else
      vad_data->cont++;
    break;

  case ST_MS:
    vad_data->timer = vad_data->timer + FRAME_TIME;
    if (f.p < vad_data->k0 && vad_data->timer > MIN_SILENCE)
    {
      vad_data->state = ST_SILENCE;
      vad_data->timer = 0;
    }
    else if (f.p > vad_data->k2 && vad_data->timer > MIN_VOICE)
    {
      vad_data->state = ST_VOICE;
      vad_data->timer = 0;
    }
    else
      vad_data->cont++;
    break;

  case ST_UNDEF:
    break;
  }

  if (vad_data->state == ST_SILENCE || vad_data->state == ST_VOICE)
  {
    // printf("%s\n", state2str(vad_data->state));
    return vad_data->state;
  }
  else
  {
    /*printf(" %d\n", vad_data->state);*/
    return ST_UNDEF;
  }
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out)
{
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
