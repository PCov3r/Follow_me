#include "servo.h"


void sendPacket(unsigned char* packet, int lenPacket, int lenFeedback, UART_HandleTypeDef* huart, unsigned char* feedback){
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Transmit(huart, packet, lenPacket, 1000);
	HAL_HalfDuplex_EnableReceiver(huart);
	HAL_UART_Receive(huart, feedback, lenFeedback, 1000);
}

float readSimple(unsigned char ID, int A_target, UART_HandleTypeDef* huart){
	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_target;
	packet[6] = 1;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else return (feedback[5]);

}

float readDouble(unsigned char ID, int A_target, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_target;
	packet[6] = 2;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else return (feedback[5]+feedback[6]*256);

}

void setID( unsigned char ID_target, unsigned char ID_goal, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID_target;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_ID;
	packet[6] = ID_goal;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
	}

void setBaudRate ( unsigned char ID, int BRate, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_COM_S;
	packet[6] = BRate;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@Delay entre 0 et 254, multiplier par 2us
 */
void setReturnDelayTime ( unsigned char ID, int Delay, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_RET_D;
	packet[6] = Delay;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

void setMinAngleLimit ( unsigned char ID, int Angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MinPos,int MinPos1,int MinPos2;
	
	MinPos = angle/STEP;
	MinPos1 = MinPos%256;
	MinPos2 = MinPos/256;	
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_CW_AL;
	packet[6] = MinPos1;
	packet[7] = MinPos2;
	packet[8] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 9, 6, huart, NULL);
}

void setMaxAngleLimit ( unsigned char ID, int Angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MaxPos,int MaxPos1,int MaxPos2;
	
	MaxPos = angle/STEP;
	MaxPos1 = MaxPos%256;
	MaxPos2 = MaxPos/256;	
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_CW_AL;
	packet[6] = MaxPos1;
	packet[7] = MaxPos2;
	packet[8] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 9, 6, huart, NULL);
}

/**
 *@Temp Range : 0 - 100, 1°C par unité
 */
void setTemperatureLimit ( unsigned char ID, int Temp, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_TLIM;
	packet[6] = Temp;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@V 50 ~ 160	5.0 ~ 16.0V
 */
void setMinimunVolt ( unsigned char ID, int V, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_MINV;
	packet[6] = V;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@V 50 ~ 160	5.0 ~ 16.0V 
 */
void setMaximunVolt ( unsigned char ID, int V, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_MAXV;
	packet[6] = V;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

void setTorque ( unsigned char ID, float Torque, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MaxTor,int MaxTor1,int MaxTor2;
	
	Maxtor = Torque/T_Step;
	MaxTor1 = Maxtor%256;
	MaxTor2 = Maxtor/256;	
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_MAXTOR;
	packet[6] = MaxTor1;
	packet[7] = MaxTor2;
	packet[8] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 9, 6, huart, NULL);
}

void setStatutReturn ( unsigned char ID, int Statut, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_STATRETRL;
	packet[6] = Statut;
	packet[7] = checksum(packet, sizeof(packet);
	
	sendPacket(packet, 8, 6, huart, NULL);
}

float readPosition(unsigned char servo_ID, UART_HandleTypeDef* huart){
	unsigned char packet[8];
	unsigned char feedback[8];

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

void setGoalPosition(unsigned char ID, float angle, UART_HandleTypeDef* huart){

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



void MoveRelatif(int ID, int angle, int sens, UART_HandleTypeDef* huart){
	float pos;
	pos = readPosition(ID, huart) + sens*angle;
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

