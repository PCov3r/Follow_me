/*
 * zoom.c
 *
 *  Created on: 27 mai 2021
 *      Author: xavie
 */


void zoomrel(sens){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];

	HAL_I2C_Mem_Read(&hi2c2, Slave_Adress_7b, Slave_Zoom_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];

	buffer_i2c_tx[1] = buffer_i2c_tx[1] + sens * Zoom_Increment_1;
	buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Zoom_Increment_2;

	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(&hi2c2, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

void focusrel(sens, valeur){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];

	HAL_I2C_Mem_Read(&hi2c2, Slave_Adress_7b, Slave_Focus_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];

	buffer_i2c_tx[1] = buffer_i2c_tx[1] + sens * Focus_Increment_1;
	buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Focus_Increment_2;

	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(&hi2c2, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

