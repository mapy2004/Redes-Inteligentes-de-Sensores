#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

/* Compatibilidad para LEDS_YELLOW en nRF52840 Dongle */
#ifndef LEDS_YELLOW
#define LEDS_YELLOW LEDS_RED
#endif

/* Declaración de los tres procesos independientes */
PROCESS(tarea3_process, "Tarea 3 - Temporizador Evento (3s)");
PROCESS(tarea1_process, "Tarea 1 - Parpadeo LED Verde (2s)");
PROCESS(tarea2_process, "Tarea 2 - Parpadeo LED Amarillo (4s)");

/* Tarea 3 arranca primero para asignar el evento antes que Tarea 1 y Tarea 2 */
AUTOSTART_PROCESSES(&tarea3_process, &tarea1_process, &tarea2_process);

/* Identificador único para el evento de sincronización inter-proceso */
static process_event_t event_start_blink;

/*---------------------------------------------------------------------------*/
/* TAREA 3: Generación de temporización de 3 segundos y envío de evento      */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tarea3_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Asignación dinámica de un identificador de evento único */
  event_start_blink = process_alloc_event();

  /* Configuración del temporizador a 3 segundos (CLOCK_SECOND * 3) */
  etimer_set(&timer, CLOCK_SECOND * 3);
  printf("Tarea 3: Esperando 3 segundos para iniciar el parpadeo...\n");

  /* Espera a que expire el temporizador de 3 segundos */
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  printf("Tarea 3: Timer expirado. Lanzando evento a Tarea 1 y Tarea 2\n");

  /* Envía el evento asíncrono a las tareas 1 y 2 */
  process_post(&tarea1_process, event_start_blink, NULL);
  process_post(&tarea2_process, event_start_blink, NULL);

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/* TAREA 1: Parpadeo de LEDS_GREEN cada 2 segundos tras recibir evento       */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tarea1_process, ev, data)
{
  static struct etimer timer_green;

  PROCESS_BEGIN();

  /* Bloquea la ejecución hasta recibir el evento desde tarea3_process */
  PROCESS_WAIT_EVENT_UNTIL(ev == event_start_blink);
  printf("Tarea 1: Evento recibido. Iniciando parpadeo LED_GREEN (2s)\n");

  /* Configura el temporizador periódico de 2 segundos */
  etimer_set(&timer_green, CLOCK_SECOND * 2);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_green));
    leds_toggle(LEDS_GREEN);
    printf("Tarea 1: Toggle LED_GREEN\n");
    etimer_reset(&timer_green);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/* TAREA 2: Parpadeo de LEDS_YELLOW cada 4 segundos tras recibir evento      */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tarea2_process, ev, data)
{
  static struct etimer timer_yellow;

  PROCESS_BEGIN();

  /* Bloquea la ejecución hasta recibir el evento desde tarea3_process */
  PROCESS_WAIT_EVENT_UNTIL(ev == event_start_blink);
  printf("Tarea 2: Evento recibido. Iniciando parpadeo LED_YELLOW (4s)\n");

  /* Configura el temporizador periódico de 4 segundos */
  etimer_set(&timer_yellow, CLOCK_SECOND * 4);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer_yellow));
    leds_toggle(LEDS_YELLOW);
    printf("Tarea 2: Toggle LED_YELLOW\n");
    etimer_reset(&timer_yellow);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/