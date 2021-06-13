# Auto focus script #
# /!\ The latter is currently not functional because it uses the picamera library instead of the custom camera.py library
# Largely inspired by https://www.arducam.com/raspberry-pi-camera/autofocus/#next-level-raspberry-pi-camera-autofocus-with-ptz-pan-tilt-zoom-cameras

import cv2 
import numpy as py
import time
from uartcom import sendData
from camera import VideoCamera
from picamera.array import PiRGBArray
	
def focusing(val): # Function generating the data in hexa to send to the card managing the camera
	value = (val << 4) & 0x3ff0
	data1 = (value >> 8) & 0x3f
	data2 = value & 0xf0
	return([data1,data2])  


def laplacian(img): # Use laplacian on image to detect edges
	img_gray = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
	img_sobel = cv2.Laplacian(img_gray,cv2.CV_16U)
	return cv2.mean(img_sobel)[0]
	

def calculation(camera): # Search for edges in frame
	rawCapture = PiRGBArray(camera) 
	camera.capture(rawCapture,format="bgr", use_video_port=True)
	image = rawCapture.array
	rawCapture.truncate(0)
	return laplacian(image)


def setFocus(camera) :
	print("Start focusing")
	
	max_index = 10
	max_value = 0.0
	last_value = 0.0
	dec_count = 0
	focal_distance = 10

	while True:
	    # Adjust focus
		[F1,F2] = focusing(focal_distance) # Focus on maximum distance
		sendData([255,255,255,F1,F2]) # Send focus to STM32 via Uart (x, y and zoom untouched)
		#Take image and calculate image clarity
		frame = camera.get_frame()
		val = calculation(frame)
		# Searching for best focus, ie the one whose camera frame has clear edges
		if val > max_value:
			max_index = focal_distance
			max_value = val
			
		# Image clarity is decreasing
		if val < last_value:
			dec_count += 1
		else:
			dec_count = 0
		# Image clarity decrease 6 times in a row, meaning we found the best focus
		if dec_count > 6:
			break
		last_value = val
		
		# Testing other focal distances
		focal_distance += 15
		if focal_distance > 1000:
			break

    # Set focus
	[F1,F2] = focusing(max_index)
	sendData([255,255,255,F1,F2])
	return(0)
