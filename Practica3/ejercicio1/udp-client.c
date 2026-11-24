#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT    8765
#define UDP_SERVER_PORT    5678

#define SEND_INTERVAL      (10 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static uint32_t rx_count = 0;

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  LOG_INFO("Temperatura en Fahrenheit calculada por el servidor primera fila '%.*s' grados (desde ",
           datalen, (char *) data);
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_(" | PANID: 0x%04X, CH: %d).\n",
            IEEE802154_CONF_PANID, IEEE802154_CONF_DEFAULT_CHANNEL);
#if LLSEC802154_CONF_ENABLED
  LOG_INFO_(" LLSEC LV:%d", uipbuf_get_attr(UIPBUF_ATTR_LLSEC_LEVEL));
#endif
  rx_count++;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static char str[32];
  uip_ipaddr_t dest_ipaddr;
  static uint32_t tx_count = 1;
  static uint32_t missed_tx_count = 0;
  int temp_celsius;

  PROCESS_BEGIN();

  LOG_INFO("Cliente iniciado en PANID: 0x%04X | Canal: %d\n",
           IEEE802154_CONF_PANID, IEEE802154_CONF_DEFAULT_CHANNEL);

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, random_rand() % SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if(NETSTACK_ROUTING.node_is_reachable() &&
        NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

      if(tx_count % 10 == 0) {
        LOG_INFO("Tx/Rx/MissedTx: %" PRIu32 "/%" PRIu32 "/%" PRIu32 "\n",
                 tx_count, rx_count, missed_tx_count);
      }

      /* Valores propios para distinguirlos en el servidor: 35 C (impar) y 18 C (par) */
      if(tx_count % 2 != 0) {
        temp_celsius = 35;
        LOG_INFO("Enviando temperatura interna en Celsius = %d a ", temp_celsius);
      } else {
        temp_celsius = 18;
        LOG_INFO("Enviando temperatura externa en Celsius = %d a ", temp_celsius);
      }
      LOG_INFO_6ADDR(&dest_ipaddr);
      LOG_INFO_("\n");

      snprintf(str, sizeof(str), "%d", temp_celsius);
      simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
      tx_count++;
    } else {
      LOG_INFO("Not reachable yet\n");
      if(tx_count > 1) {
        missed_tx_count++;
      }
    }

    /* Add some jitter */
    etimer_set(&periodic_timer, SEND_INTERVAL
      - CLOCK_SECOND + (random_rand() % (2 * CLOCK_SECOND)));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/