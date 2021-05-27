/*
 * zoom.h
 *
 *  Created on: 27 mai 2021
 *      Author: xavie
 */

#ifndef SRC_ZOOM_H_
#define SRC_ZOOM_H_


#define Slave_Adress  		0xc
#define Slave_Adress_7b		0x18
#define Slave_Zoom_Adress	0x00
#define Mem_Size			0x01
#define Zoom_Increment_1	0x01
#define Zoom_Increment_2	0x00


#define BUFFER_i2c_tx_size 3
#define BUFFER_i2c_rx_size 2

void zoomrel( int sens);

#endif /* SRC_ZOOM_H_ */
