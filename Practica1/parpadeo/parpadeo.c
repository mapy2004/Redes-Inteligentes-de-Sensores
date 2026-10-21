
#include "contiki.h"
#include "dev/leds.h" 

/* Declaracion de los tres procesos */
PROCESS(parpadeo_1_process, "Parpadeo 1 process");
PROCESS(parpadeo_2_process, "Parpadeo 2 process");
PROCESS(timer_process, "Timer process");

/* Arrancar los tres procesos */
AUTOSTART_PROCESSES(&parpadeo_1_process,
                    &parpadeo_2_process,
                    &timer_process);

/* Evento que inicia el parpadeo */
static process_event_t start_event;


/* Proceso que conmuta el LED 1 cada 2 segundos */
PROCESS_THREAD(parpadeo_1_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Esperar el evento inicial */
  PROCESS_WAIT_EVENT_UNTIL(ev == start_event);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_toggle(LEDS_GREEN);
  }

  PROCESS_END();
}


/* Proceso que conmuta el LED 2 cada 3 segundos */
PROCESS_THREAD(parpadeo_2_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Esperar el evento inicial */
  PROCESS_WAIT_EVENT_UNTIL(ev == start_event);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 3);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_toggle(LEDS_RED);
  }

  PROCESS_END();
}


/* Proceso que inicia ambos parpadeos tras 5 segundos */
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Reservar un identificador de evento */
  start_event = process_alloc_event();

  /* Esperar 5 segundos */
  etimer_set(&timer, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  /* Enviar el evento inicial a ambos procesos */
  process_post(&parpadeo_1_process, start_event, NULL);
  process_post(&parpadeo_2_process, start_event, NULL);

  PROCESS_END();
}

