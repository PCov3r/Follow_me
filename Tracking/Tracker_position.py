##Script permettant de connaitre la position de la personne par rapport au centre de l'image. 
##Selon x, on se trouve dans les valeurs négatives si la camera est trop sur la gauche (sa gauche)
##Selon y, on se trouve dans les valeurs négatives si la camera est trop basse.
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

#cam = cv2.VideoCapture(0)
#tracker = cv2.TrackerKCF_create()
#ok, frame = cam.read()

def init(param):
	bounding = (0,0,0,0)

	face_cascade = cv2.CascadeClassifier('C:/Users/pauld/Documents/Machine_virtuelle/Projet/Camera/haarcascade_frontalface_default.xml')

	gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )

	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)

	for (x,y,w,h) in faces:
		#cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)
		print("visage trouvé")

	return(bounding)


def tracking():
    cam = cv2.VideoCapture(0)
    tracker = cv2.TrackerKCF_create()
	ok, frame = cam.read()
	bbox = init(frame)
	print(bbox)
	while bbox == (0,0,0,0) :
        ok, frame = cam.read()
        bbox=init(frame)
        cv2.imshow("Tracking", frame)
	ok = tracker.init(frame, bbox)

	while(True):
        ok, frame = cam.read()
		ok, bbox = tracker.update(frame)
		if ok:   # frame is valid
			# show the frame
			print("c ok")
			p1 = (int(bbox[0]), int(bbox[1]))
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3]))
			cam_width = cam.get(3)
			cam_height = cam.get(4)
			xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) #Récupère la position par rapport au centre (largeur)
			ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2))
			print(xpos,ypos);
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2)
			cv2.imshow("Tracking", frame)

		t0=time.time()
		while not ok:
			ok, frame = cam.read()
			cv2.imshow("Tracking", frame)
			if(time.time()-t0)>5:
				print("process time")
				tracker = cv2.TrackerKCF_create()
				bbox =init(frame)
				while bbox == (0,0,0,0) :
					k = cv2.waitKey(1) & 0xFF
					if k == 27: # q for quitting
						print("ESC hit, closing...")
						break
					print("bloqué")
					ok, frame = cam.read()
					bbox=init(frame)
					cv2.imshow("Tracking", frame)
				ok = tracker.init(frame, bbox)
			ok, bbox = tracker.update(frame)


		k = cv2.waitKey(1) & 0xFF
		if k == 27: # q for quitting
				print("ESC hit, closing...")
				break

	cam.release()
	cv2.destroyAllWindows()
