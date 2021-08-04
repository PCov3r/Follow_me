#This is a face tracking script not suitable for the raspberry. We display an image seen by the camera with the position of the tracker. We validate it if we
# find if convenient by pressing enter. Then, we display the video stream from the computer camera with the position of the tracker.
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

time.sleep(3)
cam = cv2.VideoCapture(0)
tracker = cv2.TrackerKCF_create()
ok, frame = cam.read()

# Tracking file
face_cascade = cv2.CascadeClassifier('./haarcascade_frontalface_default.xml')
# Image get converted to gray scale to ease detection
gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY ) 
faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
# A face has been detected
for (x,y,w,h) in faces:
    cv2.rectangle(frame, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
    bbox = (x-w,y,3*w,4*h)  # bbox contains the upper part of the body

cv2.imshow('image',frame) # Showing detected face
cv2.waitKey(0)  # User must hit enter to validate tracker position
time.sleep(3)
ok = tracker.init(frame, bbox) 
cv2.destroyAllWindows() # We close this "validation" window 

while(True):
	ok, frame = cam.read() # Frame reading
	ok, bbox = tracker.update(frame) # Tracker updating
	if ok:   # If a face is detected, we print the bounding box
		p1 = (int(bbox[0]), int(bbox[1]))
		p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3]))
		cv2.rectangle(frame, p1,p2, (0,0,255),2,2)
	cv2.imshow("Tracking", frame) # Show video
	k = cv2.waitKey(1) & 0xFF
	if k == 27: # esc to quit
			print("ESC hit, closing...")
			break

cam.release()
cv2.destroyAllWindows()
