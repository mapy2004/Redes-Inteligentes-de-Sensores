

#include "contiki.h"
#include <stdio.h> 

/* Declaracion de los dos procesos */
PROCESS(periodic_process, "periodic process");
PROCESS(hello_world_process, "Hello world process");

/* Arrancar ambos procesos */
AUTOSTART_PROCESSES(&periodic_process, &hello_world_process);

/* Identificador del evento */
static process_event_t reading_event;

/* Proceso que recibe los eventos e imprime el contador */
PROCESS_THREAD(hello_world_process, ev, data)
{
  static int contador = 0;

  PROCESS_BEGIN();

  while(1) {
    /* Esperar a recibir el evento */
    PROCESS_WAIT_EVENT_UNTIL(ev == reading_event);

    printf("Hello, world numero %d\n", contador);

    /* Incrementar y reiniciar el contador */
    if(contador >= 20) {
      contador = 0;
    } else {
      contador++;
    }
  }

  PROCESS_END();
}

/* Proceso que genera un evento cada 5 segundos */
PROCESS_THREAD(periodic_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Reservar un identificador para el evento */
  reading_event = process_alloc_event();

  while(1) {
    /* Esperar 5 segundos */
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /* Enviar el evento al proceso hello_world_process */
    process_post(&hello_world_process, reading_event, NULL);
  }

  PROCESS_END();
}
