import pycurl
import time
import serial
from io import BytesIO

def activateOverride(i) :
    # RPi adress goes here
    ip = "192.168.0.215:5000"
    url = ip+"/m_override/?state="
    url = url+str(i)
    b_obj = BytesIO()
    crl = pycurl.Curl()

    # Set URL value
    crl.setopt(crl.URL, url)

    # Write bytes that are utf-8 encoded
    crl.setopt(crl.WRITEDATA, b_obj)

    # Perform a file transfer
    crl.perform()

    # End curl session
    crl.close()

    # Get the content stored in the BytesIO object (in byte characters)
    get_body = b_obj.getvalue()

def sendCmd(x,y,zoom) :
    url="192.168.0.215:5000/m_control/?x="+x+"&y="+y+"&zoom="+z
    b_obj = BytesIO()
    crl = pycurl.Curl()

    # Set URL value
    crl.setopt(crl.URL, url)

    # Write bytes that are utf-8 encoded
    crl.setopt(crl.WRITEDATA, b_obj)

    # Perform a file transfer
    crl.perform()

    # End curl session
    crl.close()

    # Get the content stored in the BytesIO object (in byte characters)
    get_body = b_obj.getvalue()

#Open serial port
ser = serial.Serial(
 port='/dev/serial0',
 baudrate = 115200,
 parity=serial.PARITY_NONE,
 stopbits=serial.STOPBITS_ONE,
 bytesize=serial.EIGHTBITS,
 timeout=1)
counter=0

#Read the start value
x=ser.read(4)
y=ser.read(4)
z=ser.read(4)
i=ser.read(4)

#Set Auto or Manual Mode
if i[3]==48:
 #48 is ASCII for '0'
 activateOverride(1)
if i[3]==49:
 #49 is ASCII for '1'
 activateOverride(0)

while(1):
 #Read port value with x horizontal value, y vertical value, z zoom and ifu manual or auto mode
 x=ser.read(4)
 y=ser.read(4)
 z=ser.read(4)
 ifu=ser.read(4)

 #Extract only needed information -> only 3 possible values: -1,0,1
 if x[0]==51 or x[0]==52:
  #51 is ASCII for '3' and 52 is ASCII for '4' ex: 3545 becomes 1
  x="1"
 elif x[0]==32:
  #32 is ASCII for ' ' ex: ' 845 becomes -1
  x="-1"
 else:
  x="0"
 if y[0]==51 or y[0]==52:
  y="1"
 elif y[0]==32:
  y="-1"
 else:
  y="0"
 if z[0]==51 or z[0]==52:
  z="1"
 elif z[0]==32:
  z="-1"
 else:
  z="0"

 #Set Auto or Manuel Mode at rising edge
 if i[3]==48 and ifu[3]==49:
  activateOverride(0)
 elif i[3]==49 and ifu[3]==48:
  activateOverride(1)
 i=ifu

 #Send position and zoom values
 sendCmd(x,y,z)
 
