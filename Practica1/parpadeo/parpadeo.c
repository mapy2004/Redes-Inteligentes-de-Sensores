#include "contiki.h"
#include "dev/leds.h"
//#include "nrf_gpio.h"

/* Pin del LED verde independiente LD1: P0.06 */
//#define LED_VERDE_PIN NRF_GPIO_PIN_MAP(0, 6)

/* Declaracion de los tres procesos */
PROCESS(parpadeo_1_process, "Parpadeo RGB");
PROCESS(parpadeo_2_process, "Parpadeo verde");
PROCESS(timer_process, "Timer process");

/* Arrancar los tres procesos */
AUTOSTART_PROCESSES(&parpadeo_1_process,
                    &parpadeo_2_process,
                    &timer_process);

/* Evento de inicio */
static process_event_t start_event;

/*--------------------------------------------------*/
/* PROCESO 1: LED RGB cada 2 segundos                */
/*--------------------------------------------------*/
PROCESS_THREAD(parpadeo_1_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  PROCESS_WAIT_EVENT_UNTIL(ev == start_event);

  /* Encender inmediatamente al recibir el evento */
  leds_on(LEDS_LED2);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_toggle(LEDS_LED2);
  }

  PROCESS_END();
}

/*--------------------------------------------------*/
/* PROCESO 2: LED verde independiente cada 3 segundos*/
/*--------------------------------------------------*/
PROCESS_THREAD(parpadeo_2_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Configurar el LED verde independiente */
  //nrf_gpio_cfg_output(LED_VERDE_PIN);
  //nrf_gpio_pin_set(LED_VERDE_PIN); /* Apagado */

  PROCESS_WAIT_EVENT_UNTIL(ev == start_event);

  /* Encender inmediatamente al recibir el evento */
  //nrf_gpio_pin_clear(LED_VERDE_PIN);
  leds_on(LEDS_NUM_TO_MASK(LEDS_LED1));

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 3);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_toggle(LEDS_NUM_TO_MASK(LEDS_LED1));
  }

  PROCESS_END();
}

/*--------------------------------------------------*/
/* PROCESO 3: iniciar ambos parpadeos tras 5 s       */
/*--------------------------------------------------*/
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  start_event = process_alloc_event();

  /* Esperar 5 segundos */
  etimer_set(&timer, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  /* Arrancar ambos procesos */
  process_post(&parpadeo_1_process, start_event, NULL);
  process_post(&parpadeo_2_process, start_event, NULL);

  PROCESS_END();
}
