#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* 1. Nuevo PAN ID y Canal para el Ejercicio 2 */
#define IEEE802154_CONF_PANID           0xBBBB
#define IEEE802154_CONF_DEFAULT_CHANNEL 8

/* 2. Reducción de memoria RAM para que quepa en la mota Z1 */
#define UIP_CONF_BUFFER_SIZE            140
#define NBR_TABLE_CONF_MAX_NEIGHBORS    4
#define NETSTACK_MAX_ROUTE_ENTRIES      4
#define QUEUEBUF_CONF_NUM               4

#endif /* PROJECT_CONF_H_ */