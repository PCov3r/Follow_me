/*
 * zoom.c
 *
 *  Created on: 27 mai 2021
 *      Author: xavier
 */

#include "zoom.h"

/**fonction servant modifier le zoom dans un sens définit
 *@sens +1 ou -1, sert à définir le zoom in ou out
 *@i2c pointeur pour les pin de communication i2c
 */
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

/**fonction servant modifier le focus dans un sens définit
 *@sens +1 ou -1, sert à définir le zoom in ou out
 *@i2c pointeur pour les pin de communication i2c
 */
void focusrel(I2C_HandleTypeDef* i2c, int sens){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];
	uint8_t buffer_i2c_rx[BUFFER_i2c_rx_size];
	int a;

	HAL_I2C_Mem_Read(i2c, Slave_Adress_7b, Slave_Focus_Adress, Mem_Size, buffer_i2c_rx, BUFFER_i2c_rx_size,HAL_MAX_DELAY);

	buffer_i2c_tx[1] = buffer_i2c_rx[0];
	buffer_i2c_tx[2] = buffer_i2c_rx[1];


	if (sens>0){
		a =  buffer_i2c_tx[2] + sens * Focus_Increment_2;
		if (a >0xFF){
			b= 0x01;
			a= a- 0x100;
		}
		buffer_i2c_tx[1] = buffer_i2c_tx[1] + sens * b;
		buffer_i2c_tx[2] = a;
		if (a < 0xFF) {
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Focus_Increment_2;
		}
	}
	if (sens <0){
		if (buffer_i2c_tx[2] > Focus_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1];
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Focus_Increment_2;
		}
		if (buffer_i2c_tx[2] < Focus_Increment_2){
			buffer_i2c_tx[1] = buffer_i2c_tx[1] - 0x01;
			buffer_i2c_tx[2] = buffer_i2c_tx[2] + sens * Focus_Increment_2 + 0x100;
		}
	}
	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

/** fonction permettant de régler le zoom sur une valeur définit entre 0x0 et 0x4E20
 *@i2c pointeur pour les pin de communication i2c
 *@val12 valeur, en héxadécimal allant de 0x0 à 0x4E, correspondant aux 2 bits de poids fort
 *@val34 valeur, en héxadécimal allant de 0x0 à 0xFF, correspondant aux 2 bits de poids fort
 */
void zoom(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Zoom_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}

/** fonction permettant de régler le focus sur une valeur définit entre 0x0 et 0x4E20
 *@i2c pointeur pour les pin de communication i2c
 *@val12 valeur, en héxadécimal allant de 0x0 à 0x4E, correspondant aux 2 bits de poids fort
 *@val34 valeur, en héxadécimal allant de 0x0 à 0xFF, correspondant aux 2 bits de poids fort
 */
void focus(I2C_HandleTypeDef* i2c, int val12, int val34){

	uint8_t buffer_i2c_tx[BUFFER_i2c_tx_size];

	buffer_i2c_tx[1] = val12;
	buffer_i2c_tx[2] = val34;

	buffer_i2c_tx[0] = Slave_Focus_Adress;

	HAL_I2C_Master_Transmit(i2c, 0x18, buffer_i2c_tx, BUFFER_i2c_tx_size, HAL_MAX_DELAY);
}
