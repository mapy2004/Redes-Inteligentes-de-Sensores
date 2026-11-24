#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
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
  char rx_buf[16];
  char tx_buf[16];
  int temp_c, temp_f;

  /* Asegurar terminación nula de la cadena recibida antes de usar atoi() */
  if(datalen >= sizeof(rx_buf)) {
    datalen = sizeof(rx_buf) - 1;
  }
  memcpy(rx_buf, data, datalen);
  rx_buf[datalen] = '\0';

  temp_c = atoi(rx_buf);
  temp_f = (temp_c * 9 + 2) / 5 + 32;

  LOG_INFO("Info recibida del nodo: '%s' grados\n", rx_buf);
  LOG_INFO("Direccion ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");
  LOG_INFO("TEMPERATURA RECIBIDA = %d\n", temp_c);
  LOG_INFO("Enviando Temperatura en Fahrenheit = %d\n", temp_f);
  LOG_INFO("Destino ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");

#if WITH_SERVER_REPLY
  snprintf(tx_buf, sizeof(tx_buf), "%d", temp_f);
  simple_udp_sendto(&udp_conn, tx_buf, strlen(tx_buf), sender_addr);
#endif /* WITH_SERVER_REPLY */
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/