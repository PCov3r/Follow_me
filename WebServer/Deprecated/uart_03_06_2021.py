#!/usr/bin/env python
# Test du port sÃ©rie

import serial
port= "/dev/ttyAMA0"


def sendData(type,data):
    try :
        hex_type = format(type,"02X")
        hex_data = format(data,"04X")
        cmd=[hex_type,hex_data]
        serialPort = serial.Serial(port, 115200, timeout = 2)
        bytes_sent = serialPort.write(cmd)
        loopback = serialPort.read(1)
        serialPort.close()
    except IOError:
        print ("Erreur sur", port,"\n")
