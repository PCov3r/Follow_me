import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

#cam = cv2.VideoCapture(0)
#tracker = cv2.TrackerKCF_create()
#ok, frame = cam.read()

## Cette fonction initialise le tracker

def init(param):
	bounding = (0,0,0,0) #On initialise le tableau contenant les coordonnées du haut du corps. 

	face_cascade = cv2.CascadeClassifier('C:/Users/pauld/Documents/Machine_virtuelle/Projet/Camera/haarcascade_frontalface_default.xml')
	#On récupère le fichier de tracking du visage 
	gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )
	#On convertit les couleurs de l'image en niveau de gris pour faciliter la détection du corps du professeur. 
	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
	#On récupère les coordonnées du visage du professeur.
	for (x,y,w,h) in faces:
		#cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)  # Si visage, on renvoie un tableau contenant les coordonnées et dimensions du haut du corps
		print("visage trouvé")

	return(bounding)

## Fonction principale de tracking 

def tracking():
    cam = cv2.VideoCapture(0) #On démarre la caméra
    tracker = cv2.TrackerKCF_create() # On crée un tracker
	ok, frame = cam.read() # On lit une frame. 
	bbox = init(frame) # On essaye de trouver une personne dans la frame.
	print(bbox)
	while bbox == (0,0,0,0) : # Tant qu'on ne trouve personne, on recommence la recherhe.
        ok, frame = cam.read()
        bbox=init(frame)
        cv2.imshow("Tracking", frame) # En même temps, on affiche l'image de la caméra pour avoir un retour.
	ok = tracker.init(frame, bbox)

	while(True):
        ok, frame = cam.read()
		ok, bbox = tracker.update(frame) # On met à jour le tracker avec le flux vidéo
		if ok:   # La personne est bien détectée
			print("c ok")
			p1 = (int(bbox[0]), int(bbox[1])) # On récupère les coordonnées du rectangle de tracking
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # On récupère la largeur et hauteur du rectangle
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2) # On affiche le rectangle autour de la personne détectée
			cv2.imshow("Tracking", frame)  # On affiche la vidéo

		t0=time.time()
		while not ok: # On a perdu la personne
			ok, frame = cam.read()
			cv2.imshow("Tracking", frame)
			if(time.time()-t0)>5: # Si cela fait plus de 5s, il faut relancer le tracking
				print("process time")
				tracker = cv2.TrackerKCF_create() # On recrée un tracker
				bbox =init(frame)
				while bbox == (0,0,0,0) :
					print("bloqué")
					ok, frame = cam.read()
					bbox=init(frame)
					cv2.imshow("Tracking", frame) # On essaie de retrouver la personne
				ok = tracker.init(frame, bbox)  # On réinitialise le tracker lorsqu'on a trouvé la personne
			ok, bbox = tracker.update(frame) # On met à jour le tracker


		k = cv2.waitKey(1) & 0xFF
		if k == 27: # echap pour quitter le programme. 
				print("ESC hit, closing...")
				break

	cam.release()
	cv2.destroyAllWindows()
