#include "servo.h"


void sendPacket(unsigned char* packet, int lenPacket, int lenFeedback, UART_HandleTypeDef* huart, unsigned char* feedback){
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Transmit(huart, packet, lenPacket, 1000);
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Receive(huart, feedback, lenFeedback, 1000);
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

float readPosition(int servo_ID, UART_HandleTypeDef* huart){
	unsigned char packet[8];
	unsigned char feedback[7];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = servo_ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_POS;
	packet[6] = 2;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else return (feedback[5]+feedback[6]*256)*STEP;

}

int checksum(unsigned char* packet, int lenPacket){
	int check = 0;
	for(int i=2; i<lenPacket-1; i++){
		check += packet[i];
		}
	check = ~(check) & 0xFF;
	return check;
}

void MoveRelatif(int ID, int angle, int sens, UART_HandleTypeDef* huart){
	float pos;
	float posd;
	pos = readPosition(ID, huart);
	posd = pos + sens*angle;
	if posd > 299{
			setGoalPosition( ID, 299, huart);
			return(0);
		}
	if posd < 1{
			setGoalPosition( ID, 1, huart);
			return(0);
		}
	setGoalPosition( ID, posd, huart);
}
