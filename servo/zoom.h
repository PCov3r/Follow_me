/*
 * zoom.h
 *
 *  Created on: 27 mai 2021
 *      Author: xavier
 */

#include "main.h"

#ifndef SRC_ZOOM_H_
#define SRC_ZOOM_H_

// Adresses

#define Slave_Adress  		  0xc
//adresse de la caméra décalée d'un bit vers la gauche
#define Slave_Adress_7b		  0x18
//adresse des registres de la caméra
#define Slave_Zoom_Adress	  0x00
#define Slave_Focus_Adress	0x01

#define Mem_Size			      0x01

// Incréments

#define Zoom_Increment_1	  0x01
#define Zoom_Increment_2	  0x00
#define Focus_Increment_1	  0x01
#define Focus_Increment_2	  0x00

// Définition des tableaux servant à la transmition

#define BUFFER_i2c_tx_size  3
#define BUFFER_i2c_rx_size  2

// Fonctions

void zoomrel( I2C_HandleTypeDef* i2c, int sens);
void zoom(I2C_HandleTypeDef* i2c, int val12, int val34);

void focusrel( I2C_HandleTypeDef* i2c, int sens);
void focus( I2C_HandleTypeDef* i2c, int val12, int val34);

#endif /* SRC_ZOOM_H_ */
