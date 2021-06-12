
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
		bounding = (x-w,y,3*w,4*h) # Si visage, on renvoie un tableau contenant les coordonnées et dimensions du haut du corps
		print("visage trouvé")

	return(bounding)

## Fonction principale de tracking

def tracking():
    cam = cv2.VideoCapture(0) # On démarre la caméra
    tracker = cv2.TrackerKCF_create() # On crée un tracker
	ok, frame = cam.read() # On lit une frame
	bbox = init(frame) # On essaye d'y trouver une personne
	print(bbox)
	while bbox == (0,0,0,0) : # Si on a aucune personne, on recommence la recherche en boucle
        ok, frame = cam.read()
        bbox=init(frame)
        cv2.imshow("Tracking", frame) # En même temps, on affiche l'image de la caméra pour avoir un retour
		k = cv2.waitKey(1) & 0xFF
		if k == 27: # echap for quitting
			print("ESC hit, closing...")
			cam.release()
			cv2.destroyAllWindows()
	ok = tracker.init(frame, bbox) # Une fois la personne trouvé, on lance le tracking
	exit=0
	a=0

	while(not exit): # Tant que l'utilisateur n'a pas pressé Echap, on recommence
        ok, frame = cam.read()
		ok, bbox = tracker.update(frame) # On met à jour le tracker avec le flux video
		if ok:   # La personne est bien détectée
			p1 = (int(bbox[0]), int(bbox[1])) # On récupère les coordonnées du rectangle de tracking
			p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # On récupère la largeur et hauteur du rectangle
			cam_width = cam.get(3)
			cam_height = cam.get(4) # On récupère les dimensions de la vidéo
			xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) #Récupère la position par rapport au centre (largeur)
			ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2)) #Récupère la position par rapport au centre (hauteur)
			zoom = 0
			if(bbox[3]>3/4*cam_height): # Si le rectangle de détection occupe plus des 3/4 de la hauteur, on doit dézoomer
				zoom=-1
			elif(bbox[3]<1/2*cam_height): # Si le rectangle de détection occupe moins des 1/2 de la hauteur, on doit zoomer
				zoom=1
			else:
				zoom=0
			print(xpos,ypos,zoom) # On récupère toutes ces données afin de les envoyer à la STM32
			cv2.rectangle(frame, p1,p2, (0,0,255),2,2) # On affiche le rectangle autour de la personne détectée
			cv2.imshow("Tracking", frame) # On affiche la vidéo

		t0=time.time()
		while (not ok): # On a perdu la personne
			ok, frame = cam.read()

			if(time.time()-t0)>5: # Si cela fait plus de 5s, il faut relancer le tracking
				tracker = cv2.TrackerKCF_create() # On recrée un tracker
				bbox =init(frame)
				while bbox == (0,0,0,0) :
					k = cv2.waitKey(1) & 0xFF
					if k == 27: # si l'utilisateur appuie sur ESC, on quitte le progamme 
						print("ESC hit, closing...")
						a=1
						exit=1
						break
					ok, frame = cam.read()
					bbox=init(frame)
					cv2.imshow("Tracking", frame) # On essaie de retrouver la personne
				if(a) :
					break
				ok = tracker.init(frame, bbox) # On réinitialise le tracker lorsqu'on a trouvé la personne

			ok, bbox = tracker.update(frame) # On met à jour le tracker
			cv2.imshow("Tracking", frame)
			k = cv2.waitKey(1) & 0xFF
			if k == 27: # q for quitting
				print("ESC hit, closing...")
				exit=1
				break


		k = cv2.waitKey(1) & 0xFF
		if k == 27: # si l'utilisateur appuie sur ESC, on quitte le progamme
			print("ESC hit, closing...")
			exit=1

	cam.release()
	cv2.destroyAllWindows()
