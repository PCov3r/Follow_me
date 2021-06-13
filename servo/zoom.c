#include "zoom.h"


/**
 * @brief	moving the zoom in and out by an predefine increment
 * @param	direction Integer +1 or -1, defining the direction of the zoom : in or out
 * @param	i2c Pointer to an I2C_HandleTypeDef structure that contains the configuration information for the specified I2C
 */
void zoomrel( I2C_HandleTypeDef* i2c, int direction)
{
	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];
	int a, b;

	HAL_I2C_Mem_Read(i2c, Slave_Adress_7b, Slave_Zoom_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];


	if (direction>0){
		a =  buffer_i2c_tx[2] + direction * Zoom_Increment_2;
		if (a >0xFF){
			b= 0x01;
			a= a- 0x100;
		}
		buffer_i2c_tx[1] = buffer_i2c_tx[1] + direction * b;
		buffer_i2c_tx[2] = a;
		if (a < 0xFF) {
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Zoom_Increment_2;
		}
	}
	if (direction <0){
		if (buffer_i2c_tx[2] > Zoom_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Zoom_Increment_2;
		}
		if (buffer_i2c_tx[2] < Zoom_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1] - 0x01;
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Zoom_Increment_2 + 0x100;
		}
	}
	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(i2c, Slave_Adress_7b, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

/**
 * @brief	moving the focus in and out by an predefine increment
 * @param	direction Integer +1 or -1, defining the direction of the focus : in or out
 * @param	i2c Pointer to an I2C_HandleTypeDef structure that contains the configuration information for the specified I2C
 */
void focusrel(I2C_HandleTypeDef* i2c, int direction){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];
	int a, b;

	HAL_I2C_Mem_Read(i2c, Slave_Adress_7b, Slave_Focus_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];


	if (direction>0){
		a =  buffer_i2c_tx[2] + direction * Focus_Increment_2;
		if (a >0xFF){
			b= 0x01;
			a= a- 0x100;
		}
		buffer_i2c_tx[1] = buffer_i2c_tx[1] + direction * b;
		buffer_i2c_tx[2] = a;
		if (a < 0xFF) {
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Focus_Increment_2;
		}
	}
	if (direction <0){
		if (buffer_i2c_tx[2] > Focus_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Focus_Increment_2;
		}
		if (buffer_i2c_tx[2] < Focus_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1] - 0x01;
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + direction * Focus_Increment_2 + 0x100;
		}
	}
	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, Slave_Adress_7b, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

/**
 * @brief Set the zoom to a target value
 * @note the value range is between 0x0 and 0x4E20
 * @param i2c Pointer to an I2C_HandleTypeDef structure that contains the configuration information for the specified I2C
 * @param val12 Integer corresponding to the MSB
 * @param val34 Integer corresponding to the LSB
 */
void zoom(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(i2c, Slave_Adress_7b, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

/**
 * @brief Set the focus to a target value
 * @note the value range is between 0x0 and 0x4E20
 * @param i2c Pointer to an I2C_HandleTypeDef structure that contains the configuration information for the specified I2C
 * @param val12 Integer corresponding to the MSB
 * @param val34 Integer corresponding to the LSB
 */
void focus(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, Slave_Adress_7b, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}
