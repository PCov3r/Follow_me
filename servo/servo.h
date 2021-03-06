#ifndef SRC_SERVO_H_
#define SRC_SERVO_H_

#include "main.h"

void sendPacket(unsigned char*, int, int, UART_HandleTypeDef*, unsigned char*);
void setGoalPosition(unsigned char, float, UART_HandleTypeDef* huart);
float readPosition(unsigned char, UART_HandleTypeDef*);
int checksum(unsigned char*, int);
float readSimple(unsigned char, int, UART_HandleTypeDef*);
float readDouble(unsigned char, int, UART_HandleTypeDef*);
void setID( unsigned char, unsigned char, UART_HandleTypeDef*);
void setBaudRate(unsigned char, int, UART_HandleTypeDef*);
void setReturnDelayTime(unsigned char, int, UART_HandleTypeDef*);
void setMinAngleLimit(unsigned char, float, UART_HandleTypeDef*);
void setMaxAngleLimit(unsigned char, float, UART_HandleTypeDef*);
void setTemperatureLimit(unsigned char, int, UART_HandleTypeDef*);
void setMinimunVolt(unsigned char, int, UART_HandleTypeDef*);
void setMaximunVolt(unsigned char, int, UART_HandleTypeDef*);
void setTorque(unsigned char, float, UART_HandleTypeDef*);
void setStatutsReturn(unsigned char, int, UART_HandleTypeDef*);
void enableTorque(unsigned char, int, UART_HandleTypeDef*);
void enableLED(unsigned char, int, UART_HandleTypeDef*);
void setMinAngleComplianceMargin(unsigned char, int, UART_HandleTypeDef*);
void setMaxAngleComplianceMargin(unsigned char, int, UART_HandleTypeDef*);
void setMinAngleComplianceSlope(unsigned char, int, UART_HandleTypeDef*);
void setMaxAngleComplianceSlope(unsigned char, int, UART_HandleTypeDef*);
void setGoalPosition(unsigned char, float, UART_HandleTypeDef*);
void setMovingSpeed(unsigned char, int, int mode, UART_HandleTypeDef*);
void setTorqueLimite(unsigned char, int, UART_HandleTypeDef*);
float readPosition(unsigned char, UART_HandleTypeDef*);
float readVolt(unsigned char, UART_HandleTypeDef*);
float readTemp(unsigned char, UART_HandleTypeDef*);
float checkRegisterInstruction(unsigned char, UART_HandleTypeDef*);
float checkMoving(unsigned char, UART_HandleTypeDef*);
void enableEEPROMLock(unsigned char, int, UART_HandleTypeDef*);
void moveStep(unsigned char, int, UART_HandleTypeDef*);




#define 	ON			1
#define 	OFF 		0
#define 	LEFT		0
#define 	RIGHT		1

#define		ID_X		0
#define		ID_Y		1
#define		ID_ALL		254

// ADRESS

// EEPROM
#define		A_MODNU		0
#define		A_FIRMV		2
#define		A_ID		3
#define		A_COM_S		4
#define		A_RET_D		5
#define		A_CW_AL		6
#define		A_ACW_AL	8
#define		A_TLIM		11
#define		A_MINV		12
#define		A_MAXV		13
#define		A_MAXTOR	14
#define		A_STATRETRL	16
#define		A_ALRM8LED	17
#define		A_SHUT_ER	18


// RAM
#define		A_TORQ		24
#define		A_LED		25
#define		A_CW_COM	26
#define		A_ACW_COM	27
#define		A_CW_COS	28
#define		A_ACW_COS	29
#define		A_GOAL		30
#define		A_MS		32
#define		A_TORQ_LIM	34
#define		A_POS		36
#define		A_SPEED		38
#define		A_LOAD		40
#define		A_VOLT		42
#define		A_TEMP		44
#define		A_REGISTRE	46
#define		A_MOV		48
#define		A_LOCK		50
#define		A_PUNCH		52

// INSTRUCTIONS

#define		PING		1
#define		READ		2
#define		WRITE		3
#define		R_WRITE		4
#define		ACTION		5
#define		F_RESET		6
#define		REBOOT		8
#define		SYNC_WRITE	83
#define		BULK_READ	92

// COMMUNICATIONS

#define		HEADER		255
#define 	ERROR 		9999

// LENGTH

#define		L_WRITE		4
#define		L_WRITE2P	6
#define		L_READ		4

// Steps

#define 	T_STEP		0.09765625
#define 	SPEED_STEP	0.09765625
#define 	MOTOR_STEP  30

// Step Angle

#define		STEP		0.29296875

#endif /* SRC_SERVO_H_ */
