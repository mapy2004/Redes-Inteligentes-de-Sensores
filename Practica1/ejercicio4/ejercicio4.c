#include "contiki.h"
#include "arch/platform/nrf52840/common/temperature-sensor.h"
#include <stdio.h>

/* Declaración de los dos procesos independientes */
PROCESS(timer_process, "Proceso Temporizador (3s)");
PROCESS(temperature_process, "Proceso Lectura Temperatura");

/* Ambos procesos se inician automáticamente al arrancar */
AUTOSTART_PROCESSES(&timer_process, &temperature_process);

/* Identificador único para el evento de sincronización inter-proceso */
static process_event_t event_read_temp;

/*---------------------------------------------------------------------------*/
/* PROCESO 1: Generación de temporización de 3 segundos                      */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Asignación dinámica de un identificador de evento único */
  event_read_temp = process_alloc_event();

  /* Configuración del temporizador a 3 segundos (CLOCK_SECOND * 3) */
  etimer_set(&timer, CLOCK_SECOND * 3);

  while(1) {
    /* Espera a que expire el temporizador de 3 segundos */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /* Envía el evento asíncrono al proceso de lectura de temperatura */
    process_post(&temperature_process, event_read_temp, NULL);

    /* Reinicia el temporizador manteniendo el periodo de 3s */
    etimer_reset(&timer);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/* PROCESO 2: Lectura del sensor e impresión por pantalla                   */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(temperature_process, ev, data)
{
  PROCESS_BEGIN();

  while(1) {
    /* Bloquea la ejecución hasta recibir el evento desde timer_process */
    PROCESS_WAIT_EVENT_UNTIL(ev == event_read_temp);

    /* Encendido/Activación del sensor de temperatura interno */
    SENSORS_ACTIVATE(temperature_sensor);

    /* Lectura del valor medido */
    int temp_val = temperature_sensor.value(0);
    
    int intemp = temp_val / 4;
    int fractemp = (temp_val % 4) *25;

    /* Imprime el valor en grados centígrados */
    printf("Temperatura interna nRF52840: %d.%02d ºC\n", intemp,fractemp);

    /* Desactivación del sensor tras la lectura */
    SENSORS_DEACTIVATE(temperature_sensor);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
