#include "servo.h"


void sendPacket(unsigned char* packet, int lenPacket, int lenFeedback, UART_HandleTypeDef* huart, unsigned char* feedback){
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Transmit(huart, packet, lenPacket, 1000);
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Receive(huart, feedback, lenFeedback, 1000);
}

float ReadPosition( int servo_ID ){
	unsigned char packet[];
	unsigned char feedback[];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = servo_ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_POS;
	packet[6] = 1;
	checksum(packet);
	feedback = sendpacket(packet);
	if feedback[4] != 0{
		return( 255);
		}
	p= feedback[5];
	Pa= p*STEAP;
	return(Pa);
	
}

void setGoalPosition(float angle, unsigned char ID, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int goalPosition, goalPosition1, goalPosition2;

	goalPosition = angle/STEP;
	goalPosition1 = goalPosition%256;
	goalPosition2 = goalPosition/256;

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_GOAL;
	packet[6] = goalPosition1;
	packet[7] = goalPosition2;
	packet[8] = checksum(packet, sizeof(packet));

	sendPacket(packet, 9, 6, huart, NULL);
}

int checksum(unsigned char* packet, int lenPacket){
	int check = 0;
	for(int i=2; i<lenPacket-1; i++){
		check += packet[i];
		}
	check = ~(check) & 0xFF;
	return check;
}
