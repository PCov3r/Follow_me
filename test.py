import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

cam = cv2.VideoCapture(0)
tracker = cv2.TrackerKCF_create()
ok, frame = cam.read()

def init(param):
	#time.sleep(3)


	face_cascade = cv2.CascadeClassifier('C:/Users/jeremy/Documents/project/haarcascade_frontalface_default.xml')

	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY )

	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)

	for (x,y,w,h) in faces:
		cv2.rectangle(frame, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)

	cv2.imshow('image',frame)
	#cv2.waitKey(0)
	#time.sleep(3)
	#ok = tracker.init(frame, bbox)
	cv2.destroyAllWindows()
	return(bounding)


def tracking():
	ok, frame = cam.read()
	bbox = init(frame)
	ok = tracker.init(frame, bbox)
	while(True):
		ok, frame = cam.read()
		ok, bbox = tracker.update(frame)
		if ok:   # frame is valid
			# show the frame
			p1 = (int(bbox[0]), int(bbox[1]))
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3]))
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2)
			cv2.imshow("Tracking", frame)
		if not ok:
			bbox =init()
		k = cv2.waitKey(1) & 0xFF
		if k == 27: # q for quitting
				print("ESC hit, closing...")
				break

	cam.release()
	cv2.destroyAllWindows()
