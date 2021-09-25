/*
 * custom_udp.c
 *
 *  Created on: Jun 1, 2021
 *      Author: Samudra Gupta
 *
 *  Description - This C file consists of APIs to setup a raw UDP Ethernet connectivity
 *  between the micro controller and the computer.
 *  The micro controller sends the UDP packet back to the client whenever an
 *  external interrupt is generated.
 */

/* ************************************************************************************* */

#include "custom_udp.h"
struct udp_pcb *upcb_1;
volatile bool btn_interrupt_flag = false;
uint8_t rx_buffer[SIZE_OF_RX_BUFFER] = "";
//uint8_t rx_IPADDR[4] = {RX_IPADDRESS_0,RX_IPADDRESS_1,RX_IPADDRESS_2,RX_IPADDRESS_3};
ip_addr_t rx_IPADDR;
char tx_packet[SIZE_OF_TX_PACKET_BUFFER] = "";

/*
 * Description - This API creates a UDP socket and initialize UDP connection
 * @param - None
 * @return - None
 */
void udp_socket_init() {
	err_t err_i;
	ip_addr_t uC_ipaddr;

	upcb_1 = udp_new();

	if(upcb_1 != NULL) {
		//IP4_ADDR(&uC_ipaddr,IP_ADDRESS[0],IP_ADDRESS[1],IP_ADDRESS[2],IP_ADDRESS[3]); // STM32 IP

		memcpy(&uC_ipaddr, &gnetif.ip_addr, sizeof(ip_addr_t));

		err_i = udp_bind(upcb_1, &uC_ipaddr, MICRO_CTRL_PORT);
		//err_i = udp_bind(upcb_1, IP4_ADDR_ANY, MICRO_CTRL_PORT);
		if(err_i != ERR_OK) {
			Error_Handler();
		}
		udp_recv(upcb_1, udp_receive_callback, 0);
	}
	// Assign IP of the receiver/client/pc allocated by DHCP server to the rx_IPADDR array
	//IP4_ADDR(&rx_IPADDR, RX_IPADDRESS_0, RX_IPADDRESS_1, RX_IPADDRESS_2, RX_IPADDRESS_3);
	IP4_ADDR(&rx_IPADDR,192,168,0,101);
}


/*
 * Description - This API sends the requested packet back to the remote(client/pc) IP address
 * @param - character pointer to the UDP packet
 * @param - 8-bit integer pointer to receiver IP address array
 * @param - 16-bit integer receiver port value
 * @return - None
 */
void udp_packet_send(char *udp_tx_packet, ip_addr_t *ptr_rx_ipaddr, uint16_t rx_port) {
	err_t err_s;
	//ip_addr_t rx_ipaddr;

	//IP4_ADDR(&rx_ipaddr,ptr_rx_ipaddr[0],ptr_rx_ipaddr[1],ptr_rx_ipaddr[2],ptr_rx_ipaddr[3]);

	//err_s = udp_connect(upcb_1, &rx_ipaddr, rx_port);
	err_s = udp_connect(upcb_1, ptr_rx_ipaddr, rx_port);
	if(err_s != ERR_OK) {
		Error_Handler();
	}

	struct pbuf *pb = pbuf_alloc(PBUF_TRANSPORT, strlen(udp_tx_packet), PBUF_POOL);
	if(pb != NULL) {
		memcpy(pb->payload, udp_tx_packet, strlen(udp_tx_packet));

		err_s = udp_send(upcb_1, pb);
		//err_s = udp_sendto(upcb_1, pb, ptr_rx_ipaddr, rx_port);
		if(err_s != ERR_OK) {
			Error_Handler();
		}
		pbuf_free(pb);
	}
	udp_disconnect(upcb_1);
}


/*
 * Description - This API is called when a packet/request is received from the remote(client) IP address
 * @param arg - user supplied argument
 * @param pcb - the udp_pcb that receive data
 * @param p - packet buffer that is received
 * @param addr - remote IP address from where the packet is received
 * @param port - remote port from where the packet is received
 * @return - None
 */
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
	/*if(strcmp(p->payload,"any string") == 0) { // Strings match
		// Any code here...
	}
	else {
	}*/
	sprintf(rx_buffer, "Packet received by micro controller: %s\n\r",(char *)p->payload);
	HAL_UART_Transmit(&huart1, rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
}


/*
 * Description - This API is called when an external interrupt is generated.(user button in this case)
 * @param - GPIO PIN
 * @return - None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	btn_interrupt_flag = true;
}
