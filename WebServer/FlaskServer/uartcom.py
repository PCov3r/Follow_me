#!/usr/bin/env python
# Script contenant la fonction relative à l'UART

import serial
port= "/dev/ttyAMA0" #Port de communication UART à changer selon les cas


def sendData(trame):
    try :
        serialPort = serial.Serial(port, 115200, timeout = 2) #Ouverture du port série au baud rate 115200
        bytes_sent = serialPort.write(trame) #Envoi de la trame via UART
        serialPort.close() 
    except IOError:
        print ("Erreur sur", port,"\n")

