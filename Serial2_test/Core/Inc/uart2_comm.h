#ifndef __UART2_COMM_H
#define __UART2_COMM_H

#include <stdint.h>

void uart2_comm_init(void);
void uart2_tx_send(uint8_t *buf, uint16_t len);
void uart2_rx_task(void const *arg);
void uart2_tx_task(void const *arg);
void uart2_idle_callback(void);

#endif
