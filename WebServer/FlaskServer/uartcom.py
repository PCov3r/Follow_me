#!/usr/bin/env python
# Script for Uart communication with STM32 board #

import serial
port= "/dev/ttyAMA0" # Change UART port 


def sendData(trame):
    try :
        serialPort = serial.Serial(port, 115200, timeout = 2) # Open port with baudrate 115200
        bytes_sent = serialPort.write(trame) # Send data to STM32 board
        serialPort.close() 
    except IOError:
        print ("Erreur sur", port,"\n")

