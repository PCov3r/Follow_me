# Script gérant le focus automatique #
# /!\ Ce dernier n'est pour le moment pas fonctionnel car utilise la librairie picamera au lieu de la librairie créée camera.py
# Largement inspiré de https://www.arducam.com/raspberry-pi-camera/autofocus/#next-level-raspberry-pi-camera-autofocus-with-ptz-pan-tilt-zoom-cameras

import cv2 
import numpy as py
import time
from uartcom import sendData
from camera import VideoCamera
from picamera.array import PiRGBArray
	
def focusing(val): # Fonction générant les données en hexa à envoyer à la carte gérant la caméra
	value = (val << 4) & 0x3ff0
	data1 = (value >> 8) & 0x3f
	data2 = value & 0xf0
	return([data1,data2])  


def laplacian(img): # Application du Laplacien sur l'image (détection de contour)
	img_gray = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
	img_sobel = cv2.Laplacian(img_gray,cv2.CV_16U)
	return cv2.mean(img_sobel)[0]
	

def calculation(camera): # Acquisition d'une frame de la caméra et recherche des contours
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
	    # Ajustement du focus
		[F1,F2] = focusing(focal_distance) # On sa place tout au bout de la distance focale
		sendData([255,255,255,F1,F2]) # On envoie à la carte gérant les servos la commande de la distance focale (et donc on ne touche pas à la postion en x, y ou au zoom)
		#Take image and calculate image clarity
		frame = camera.get_frame()
		val = calculation(frame)
		#Recherche du focus en cherchant la valeur pour laquelle on distingue un maximum de contours
		if val > max_value:
			max_index = focal_distance
			max_value = val
			
		# Si la clarté de l'image diminue
		if val < last_value:
			dec_count += 1
		else:
			dec_count = 0
		# Si la clarté diminue de manière consécutive 6 fois, on a trouvé le focus idéal
		if dec_count > 6:
			break
		last_value = val
		
		# Augmentation de la distance focale
		focal_distance += 15
		if focal_distance > 1000:
			break

    # Réglage sur le focus
	[F1,F2] = focusing(max_index)
	sendData([255,255,255,F1,F2])
	return(0)
