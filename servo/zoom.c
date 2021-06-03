/*
 * zoom.c
 *
 *  Created on: 27 mai 2021
 *      Author: xavier
 */
#include "zoom.h"

void zoomrel( I2C_HandleTypeDef* i2c, int sens)
{

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];

	HAL_I2C_Mem_Read(i2c, Slave_Adress_7b, Slave_Zoom_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];

	buffer_i2c_tx[1] = buffer_i2c_tx[1] + sens * Zoom_Increment_1;
	buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Zoom_Increment_2;

	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

void focusrel(I2C_HandleTypeDef* i2c, int sens){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];

	HAL_I2C_Mem_Read(i2c, Slave_Adress_7b, Slave_Focus_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];

	buffer_i2c_tx[1] = buffer_i2c_tx[1] + sens * Focus_Increment_1;
	buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Focus_Increment_2;

	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

void zoom(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

void focus(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}
