import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

#cam = cv2.VideoCapture(0)
#tracker = cv2.TrackerKCF_create()
#ok, frame = cam.read()

## Function to initialize tracker

def init(param):
	bounding = (0,0,0,0) # bounding will contain the upper part of the detected body 

	face_cascade = cv2.CascadeClassifier('./haarcascade_frontalface_default.xml')
	# Convert to gray scale to ease detection
	gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )
	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
	# Get coordinates, width and heigth of the detected face
	for (x,y,w,h) in faces:
		#cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)  # bounding contains the coordinates and size of the upper part of the body
		print("visage trouvé")

	return(bounding)

## Main tracking function 

def tracking():
    	cam = cv2.VideoCapture(0) # Starting camera
    	tracker = cv2.TrackerKCF_create() # Create tracker
	ok, frame = cam.read() # Read frame 
	bbox = init(frame) # Find face in the frame
	print(bbox)
	while bbox == (0,0,0,0) : # While no face detected
            ok, frame = cam.read()
            bbox=init(frame)
            cv2.imshow("Tracking", frame) # Keep showing video stream while searching
	ok = tracker.init(frame, bbox)

	while(True):
        	ok, frame = cam.read()
		ok, bbox = tracker.update(frame) # Update tracker position in frame
		if ok:   # Face is still detected
			print("ok")
			p1 = (int(bbox[0]), int(bbox[1])) # Get tracking rectangle coordinates
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # Get width and length of tracking rectangle
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2) # Print tracking rectangle on stream
			cv2.imshow("Tracking", frame)  # Show stream

		t0=time.time()
		while not ok: # Face lost
			ok, frame = cam.read()
			cv2.imshow("Tracking", frame)
			if(time.time()-t0)>5: # If 5s has passed and still no face detected
				print("process time")
				tracker = cv2.TrackerKCF_create() # We recreate a tracker
				bbox =init(frame)
				while bbox == (0,0,0,0) :
					print("no face detected")
					ok, frame = cam.read()
					bbox=init(frame)
					cv2.imshow("Tracking", frame) # Get stream whil searching
				ok = tracker.init(frame, bbox)  # Tracker reinitialization on detection
			ok, bbox = tracker.update(frame) # Tracker update


		k = cv2.waitKey(1) & 0xFF
		if k == 27: # esc to quit. 
				print("ESC hit, closing...")
				break

	cam.release()
	cv2.destroyAllWindows()
