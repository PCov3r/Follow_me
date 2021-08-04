
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

#cam = cv2.VideoCapture(0)
#tracker = cv2.TrackerKCF_create()
#ok, frame = cam.read()


## Get tracking box
def init(param):
	bounding = (0,0,0,0) # bounding will contain the upper part of the detected body 

	face_cascade = cv2.CascadeClassifier('C:/Users/pauld/Documents/Machine_virtuelle/Projet/Camera/haarcascade_frontalface_default.xml')
	# Convert to gray scale to ease detection
	gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )
	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
	# Get coordinates, width and heigth of the detected face
	for (x,y,w,h) in faces:
		#cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)  # bounding contains the coordinates and size of the upper part of the body
		print("visage trouvé")

	return(bounding)

## Main function

def tracking():
	cam = cv2.VideoCapture(0) # Start camera
	tracker = cv2.TrackerKCF_create() # Create tracker
	ok, frame = cam.read() # Read frame
	bbox = init(frame) # Try to find face
	print(bbox)
	while bbox == (0,0,0,0) : # If none, loop
		ok, frame = cam.read()
		bbox=init(frame)
		cv2.imshow("Tracking", frame) # Continue to show video
		k = cv2.waitKey(1) & 0xFF
		if k == 27: # esc for quitting
			print("ESC hit, closing...")
			cam.release()
			cv2.destroyAllWindows()
	ok = tracker.init(frame, bbox) # Face found, launch tracking
	exit=0
	a=0

	while(not exit): # While esc not pressed
        	ok, frame = cam.read()
		ok, bbox = tracker.update(frame) # Update tracker
		if ok:   # Still detected
			p1 = (int(bbox[0]), int(bbox[1])) # Get coordinates of bounding box
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # Get width and length of box
			cam_width = cam.get(3)
			cam_height = cam.get(4) # Get camera resolution
			xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) # Get position from center (x axis)
			ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2)) # Get position from center (y axis)
			zoom = 0
			if(bbox[3]>3/4*cam_height): # If rectangle greater than 3/4 of the screen, we need to unzoom
				zoom=-1
			elif(bbox[3]<1/2*cam_height): # If rectangle less than 1/2 of the screen, we need to zoom
				zoom=1
			else:
				zoom=0
			print(xpos,ypos,zoom) # Print all the data
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2) # Show bounding box
			cv2.imshow("Tracking", frame) # Show video

		t0=time.time()
		while (not ok): # Face lost
			ok, frame = cam.read()

			if(time.time()-t0)>5: # If face lost for more than 5s, we need to relaunch tracking
				tracker = cv2.TrackerKCF_create() # Recreate a tracker
				bbox =init(frame)
				while bbox == (0,0,0,0) :
					k = cv2.waitKey(1) & 0xFF
					if k == 27: # esc for quitting 
						print("ESC hit, closing...")
						a=1
						exit=1
						break
					ok, frame = cam.read()
					bbox=init(frame)
					cv2.imshow("Tracking", frame) # Continue to show video
				if(a) :
					break
				ok = tracker.init(frame, bbox) # Reinitialize tracker

			ok, bbox = tracker.update(frame) # Update tracker
			cv2.imshow("Tracking", frame)
			k = cv2.waitKey(1) & 0xFF
			if k == 27: # esc for quitting
				print("ESC hit, closing...")
				exit=1
				break


		k = cv2.waitKey(1) & 0xFF
		if k == 27: # esc for quitting
			print("ESC hit, closing...")
			exit=1

	cam.release()
	cv2.destroyAllWindows()
