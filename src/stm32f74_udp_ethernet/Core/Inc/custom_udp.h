/*
 * custom_udp.h
 *
 *  Created on: Jun 1, 2021
 *      Author: Samudra Gupta
 *
 *  Description - This header file consists of MACRO definitions of the
 *  communication ports and function prototypes for setting up raw UDP Ethernet connectivity
 *  between micro controller and computer.
 */

/* ************************************************************************************* */

#ifndef INC_CUSTOM_UDP_H_
#define INC_CUSTOM_UDP_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "lwip.h"
#include "lwip/udp.h"
#include "ip_addr.h"

// USART1 HandleType
extern UART_HandleTypeDef huart1;

#define MICRO_CTRL_PORT 7000
#define RX_PORT_pc 8000
#define RX_PORT_db 8089

// IP address of the client (PC in this case)
#define RX_IPADDRESS_0 192
#define RX_IPADDRESS_1 168
#define RX_IPADDRESS_2 0 //1
#define RX_IPADDRESS_3 101 //38

//extern uint8_t rx_IPADDR[4];
extern ip_addr_t rx_IPADDR;

// Size of packet received by the micro controller
#define SIZE_OF_RX_BUFFER 80
// Size of packet sent by the micro controller
#define SIZE_OF_TX_PACKET_BUFFER 150

extern volatile bool btn_interrupt_flag;

// IP address of STM32
extern uint8_t IP_ADDRESS[4];

// Custom message sent by the micro controller
extern char tx_packet[SIZE_OF_TX_PACKET_BUFFER];
extern struct netif gnetif;

/*
 * Function prototype to create a UDP socket to initiate UDP Ethernet connection.
 */
void udp_socket_init();

/*
 * Function prototype to send packets back to remote(computer) IP address.
 */
//void udp_packet_send(char *, uint8_t *, uint16_t);
void udp_packet_send(char *, ip_addr_t *, uint16_t);

/*
 * Function prototype for UDP receive callback.
 */
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);


#endif /* INC_CUSTOM_UDP_H_ */
