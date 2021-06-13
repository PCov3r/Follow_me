#include "servo.h"

/**
 * 	@brief Send a series of hex values to motors, and store the feedback.
 * 	@note This function is only used in the other functions of the library.
 * 	@param packet Pointer to sending data buffer (unsigned char*).
 * 	@param lenPacket Amount of data to be transfered.
 * 	@param lenFeedback Amount of data to be received.
 * 	@param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 * 	@param feedback Pointer to receiving data buffer (unsigned char*).
 */
void sendPacket(unsigned char* packet, int lenPacket, int lenFeedback, UART_HandleTypeDef* huart, unsigned char* feedback){
	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_UART_Transmit(huart, packet, lenPacket, 1000);
	HAL_HalfDuplex_EnableReceiver(huart);
	HAL_UART_Receive(huart, feedback, lenFeedback, 10);
}
/**
 * @brief Change the ID of the motor.
 * @param ID_target ID of the motor to be changed.
 * @param ID_goal New ID of the motor (0-255).
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setID(unsigned char ID_target, unsigned char ID_goal, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID_target;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ID;
	packet[6] = ID_goal;
	packet[7] = checksum(packet, sizeof(packet));
	sendPacket(packet, 8, 6, huart, feedback);
	}

/**
 * @brief Change the communication speed of the motors.
 * @note Base speed is 1 million bps.
 * @param ID ID of the motor.
 * @param Brate New baud rate (int), check documentation for speed values.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setBaudRate(unsigned char ID, int BRate, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_COM_S;
	packet[6] = BRate;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the time between receiving a packet and returning the feedback (from the motor)
 * @note The real return delay time is given by delay*2us.
 * @note The base delay value is 250 (500us).
 * @param ID ID of the motor.
 * @param delay New delay value, between 0 and 254.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setReturnDelayTime(unsigned char ID, int delay, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_RET_D;
	packet[6] = delay;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the minimum angle possible on the motor.
 * @note The base value is 0.
 * @param ID ID of the motor.
 * @param angle New minimum angle, in degrees.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setMinAngleLimit(unsigned char ID, float angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	unsigned char feedback[6];
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

	sendPacket(packet, 9, 6, huart, feedback);
}

/**
 * @brief Change the maximum angle possible on the motor.
 * @note The base value is 300.
 * @param ID ID of the motor.
 * @param angle New minimum angle, in degrees.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setMaxAngleLimit(unsigned char ID, float angle, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	unsigned char feedback[6];
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

	sendPacket(packet, 9, 6, huart, feedback);
}

/**
 *@brief Change the maximum working temperature of the motor.
 *@note If the internal temperature of the motor is higher than the limit, the motor will shut down automatically.
 *@param ID ID of the motor.
 *@param temp Range : 0 - 100, 1°C per unit.
 *@param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setTemperatureLimit(unsigned char ID, int temp, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_TLIM;
	packet[6] = temp;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 *@brief Change the minimum voltage threshold for the motor to start.
 *@note If the voltage supply is lower than this value, the motor won't start.
 *@note The actual voltage threshold is given by V/10 Volts.
 *@param ID ID of the motor.
 *@param V Int value between 50 and 160.
 *@param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setMinimunVolt(unsigned char ID, int V, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_MINV;
	packet[6] = V;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 *@brief Change the maximum voltage threshold for the motor to start.
 *@note If the voltage supply is higher than this value, the motor won't start.
 *@note The actual voltage threshold is given by V/10 Volts.
 *@param ID ID of the motor.
 *@param V Int value between 50 and 160.
 *@param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setMaximunVolt(unsigned char ID, int V, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_MAXV;
	packet[6] = V;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the maximum torque of the motor.
 * @note The actual torque value is given by (torque*0.1)% of the max possible torque value.
 * @param ID ID of the motor.
 * @param torque Value of the desired torque, between 0 and 1023.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setTorque(unsigned char ID, float torque, UART_HandleTypeDef* huart){

	unsigned char packet[9];
	unsigned char feedback[6];
	int MaxTor, MaxTor1, MaxTor2;

	MaxTor = torque/T_STEP;
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

	sendPacket(packet, 9, 6, huart, feedback);
}

/**
 * @brief Decides when to return a status packet.
 * @note 0 means return a status packet only for ping instruction.
 * @note 1 means return a status packet only for ping or read instruction.
 * @note 2 means return a status packet only for all instructions.
 * @param ID ID of the motor.
 * @param status The desired behavior of the motor (0, 1 or 2).
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void setStatusReturn(unsigned char ID, int status, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_STATRETRL;
	packet[6] = status;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Enables the torque of the motor.
 * @note By default, the value is 0 (Torque off).
 * @param ID ID of the motor.
 * @param state Desired torque mode (0 or 1).
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void enableTorque(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_TORQ;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Lights on/off the LED.
 * @param ID ID of the motor.
 * @param state Defines the state of the LED (0=OFF, 1=ON).
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void enableLED(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_LED;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Enables the writing in the ROM area of the motor memory.
 * @param ID ID of the motor.
 * @param state Defines the state of the ROM memory (0=no access, 1=free access).
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 */
void enableEEPROMLock(unsigned char ID, int state, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_LOCK;
	packet[6] = state;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the clockwise angle margin.
 * @note There are 7 margins possible given by the closest inferior power of 2 of compliance value.
 * @param ID Integer giving the id of the motor.
 * @param compliance Value of desired margin between 0 and 255.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 *
 */
void setMinAngleComplianceMargin(unsigned char ID, int compliance, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_CW_COM;
	packet[6] = compliance;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the counter-clockwise angle margin.
 * @note There are 7 margins possible given by the closest inferior power of 2 of compliance value.
 * @param ID Integer giving the id of the motor.
 * @param compliance Value of desired margin between 0 and 255.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 *
 */
void setMaxAngleComplianceMargin(unsigned char ID, int compliance, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ACW_COM;
	packet[6] = compliance;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the clockwise slope margin.
 * @note There are 7 margins possible given by the closest inferior power of 2 of compliance value.
 * @param ID Integer giving the id of the motor.
 * @param compliance Value of desired margin between 0 and 255.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 *
 */
void setMinAngleComplianceSlope(unsigned char ID, int slope, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_CW_COS;
	packet[6] = slope;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}

/**
 * @brief Change the counter-clockwise slope margin.
 * @note There are 7 margins possible given by the closest inferior power of 2 of compliance value.
 * @param ID Integer giving the id of the motor.
 * @param compliance Value of desired margin between 0 and 255.
 * @param huart Pointer to an UART_HandleTypeDef structure that contains the configuration information for the specified UART.
 *
 */
void setMaxAngleComplianceSlope(unsigned char ID, int slope, UART_HandleTypeDef* huart){

	unsigned char packet[8];
	unsigned char feedback[6];

	packet[0] = HEADER;
	packet[1] = HEADER;
	packet[2] = ID;
	packet[3] = L_WRITE;
	packet[4] = WRITE;
	packet[5] = A_ACW_COS;
	packet[6] = slope;
	packet[7] = checksum(packet, sizeof(packet));

	sendPacket(packet, 8, 6, huart, feedback);
}
