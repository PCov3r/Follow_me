#include "servo.h"

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

void checksum( unsigned char* packet){
	int n = sizeof(packet)
	int check = 0;
	for ( int i=2, i<n-1, i++){
		check += packet[i];
		}
	check = ~(check) & 0xFF;
	packet[n] = check;
}
