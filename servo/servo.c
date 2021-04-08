#include "servo.h"


void sendPacket(unsigned char* packet, int lenPacket, int lenFeedback, UART_HandleTypeDef* huart, unsigned char* feedback){
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Transmit(huart, packet, lenPacket, 1000);
	HAL_HalfDuplex_EnableReceiver(huart);
	HAL_UART_Receive(huart, feedback, lenFeedback, 1000);
}

int checksum(unsigned char* packet, int lenPacket){
	int check = 0;
	for(int i=2; i<lenPacket-1; i++){
		check += packet[i];
		}
	check = ~(check) & 0xFF;
	return check;
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
	unsigned char feedback[9];

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
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ID;
	packet[6] = ID_goal;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, 8, 6, huart, NULL);
	}

void setBaudRate(unsigned char ID, int BRate, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_COM_S;
	packet[6] = BRate;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@Delay entre 0 et 254, multiplier par 2us
 */
void setReturnDelayTime(unsigned char ID, int Delay, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_RET_D;
	packet[6] = Delay;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void setMinAngleLimit(unsigned char ID, float angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MinPos, MinPos1, MinPos2;

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
	packet[8] = checksum(packet, sizeof(packet));

	sendPacket(packet, 9, 6, huart, NULL);
}

void setMaxAngleLimit(unsigned char ID, float angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MaxPos, MaxPos1, MaxPos2;

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
	packet[8] = checksum(packet, sizeof(packet));

	sendPacket(packet, 9, 6, huart, NULL);
}

/**
 *@Temp Range : 0 - 100, 1°C par unité
 */
void setTemperatureLimit(unsigned char ID, int Temp, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_TLIM;
	packet[6] = Temp;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@V 50 ~ 160	5.0 ~ 16.0V
 */
void setMinimunVolt(unsigned char ID, float volt, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	int volt2 = volt*10;
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_MINV;
	packet[6] = volt2;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

/**
 *@V 50 ~ 160	5.0 ~ 16.0V
 */
void setMaximunVolt(unsigned char ID, int volt, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	int volt2 = volt;
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_MAXV;
	packet[6] = volt;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void setTorque(unsigned char ID, float Torque, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	int MaxTor, MaxTor1, MaxTor2;

	MaxTor = Torque*T_STEP;
	MaxTor1 = MaxTor%256;
	MaxTor2 = MaxTor/256;

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_MAXTOR;
	packet[6] = MaxTor1;
	packet[7] = MaxTor2;
	packet[8] = checksum(packet, sizeof(packet));

	sendPacket(packet, 9, 6, huart, NULL);
}

void setStatutsReturn(unsigned char ID, int Statuts, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_STATRETRL;
	packet[6] = Statuts;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void enableTorque(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_TORQ;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void enableLED(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_LED;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

/*
 * @compliance between 0 - 255
 */
void setMinAngleComplianceMargin(unsigned char ID, int compliance, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_CW_COM;
	packet[6] = compliance;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

/*
 * @compliance between 0 - 255
 */
void setMaxAngleComplianceMargin(unsigned char ID, int compliance, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ACW_COM;
	packet[6] = compliance;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void setMinAngleComplianceSlope(unsigned char ID, int slope, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_CW_COS;
	packet[6] = compliance;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}

void setMaxAngleComplianceSlope(unsigned char ID, int slope, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ACW_COS;
	packet[6] = slope;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
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


void setMovingSpeed(unsigned char ID, int movingspeed, int mode, UART_HandleTypeDef* huart){
	
	unsigned char packet[9];
	int movingspeed1, movingspeed2, movingspeed3;
	movingspeed1 = movingspeed*SPEED_STEP+ mode*1024
	movingspeed2 = movingspeed1%256;
	movingspeed3 = movingspeed1/256;
	
	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_GOAL;
	packet[6] = movingspeed2;
	packet[7] = movingspeed3;
	packet[8] = checksum(packet, sizeof(packet));
}

/*
 * @torquelimite between 0 - 100
 */
void setTorqueLimite(unsigned char ID, int torquelimite, UART_HandleTypeDef* huart){
		
	unsigned char packet[9];
	int MaxTor, MaxTor1, MaxTor2;

	MaxTor = torquelimite*T_STEP;
	MaxTor1 = MaxTor%256;
	MaxTor2 = MaxTor/256;

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = 0x05;
	packet[4] = WRITE;
	packet[5] = A_TORQ_LIM;
	packet[6] = MaxTor1;
	packet[7] = MaxTor2;
	packet[8] = checksum(packet, sizeof(packet));

	sendPacket(packet, 9, 6, huart, NULL);
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

unsigned char readSpeed(unsigned char ID, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];
	unsigned char feedback[8];
	unsigned char ret[2];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = servo_ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_SPEED;
	packet[6] = 2;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0){
		ret(301);
	}
	else{
		if (feedback[5]+feedback[6]*256 >1023){
			ret[0] = 1;
			ret[1] = (feedback[5]+feedback[6]*256-1024)*SPEED_STEP
		}
		else{
			ret[0] = 0;
			ret[1] = (feedback[5]+feedback[6]*256)*SPEED_STEP
		}
		return(ret);
	}
}
}


float readLoad(unsigned char ID, UART_HandleTypeDef* huart){
	
	unsigned char packet[8];
	unsigned char feedback[8];
	unsigned char ret[2];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = servo_ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_LOAD;
	packet[6] = 2;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0){
		ret(301);
	}
	else{
		if (feedback[5]+feedback[6]*256 >1023){
			ret[0] = 1;
			ret[1] = feedback[5]+feedback[6]*256-1024
		}
		else{
			ret[0] = 0;
			ret[1] = feedback[5]+feedback[6]*256
		}
		return(ret);
	}
}


float readVolt(unsigned char ID, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_VOLT;
	packet[6] = 1;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else return (feedback[5]/10);

}

float readTemp(unsigned char ID, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_TEMP;
	packet[6] = 1;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else return (feedback[5]);

}

float checkRegisterInstruction(unsigned char ID, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_REGISTRE;
	packet[6] = 1;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else{
		if(feedbackk[5]==0){
			return(0);
		}
		else{
			return(1);
		}
	}
}

float checkMoving(unsigned char ID, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_READ;
	packet[4] = READ;
	packet[5] = A_MOV;
	packet[6] = 1;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, sizeof(packet), sizeof(feedback), huart, feedback);
	if(feedback[4]!=0) return(301);
	else{
		if(feedbackk[5]==0){
			return(0);
		}
		else{
			return(1);
		}
	}
}

void enableEEPROMLock(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_LOCK;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, NULL);
}
void MoveRelatif(int ID, int angle, int sens, UART_HandleTypeDef* huart){
	float pos;
	pos = readPosition(ID, huart) + sens*angle;
	if (posd > 299){
			setGoalPosition( ID, 299, huart);
			return(0);
		}
	if (posd < 1){
			setGoalPosition( ID, 1, huart);
			return(0);
		}
	setGoalPosition( ID, posd, huart);
}

