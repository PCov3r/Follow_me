#include "main.h"

#ifndef SRC_ZOOM_H_
#define SRC_ZOOM_H_

// Addresses

#define Slave_Address		0xc
//address of the camera shifted to the left by one bit
#define Slave_Address_7b	0x18
//addresses of the register in the camera
#define Slave_Zoom_Address	0x00
#define Slave_Focus_Address	0x01

// Memory Size in byte
#define Mem_Size			0x01

// Increments

#define Zoom_Increment_1	0x01
#define Zoom_Increment_2	0x00
#define Focus_Increment_1	0x01
#define Focus_Increment_2	0x00

// definition of the tables use for transmission

#define BUFFER_i2c_tx_size 	3
#define BUFFER_i2c_rx_size 	2

// Functions

void zoomrel( I2C_HandleTypeDef* i2c, int sens);
void zoom(I2C_HandleTypeDef* i2c, int val12, int val34);

void focusrel( I2C_HandleTypeDef* i2c, int sens);
void focus( I2C_HandleTypeDef* i2c, int val12, int val34);

#endif /* SRC_ZOOM_H_ */
