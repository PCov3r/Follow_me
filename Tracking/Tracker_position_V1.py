#Il s'agit d'un script de tracking de visage non adapté à la raspberry. Dans lequel, on affiche une image vue par la caméra avec la position du tracker. ON la valide si elle nous 
#convient en appuyant sur entrer. Ensuite, on affiche le flux video de la caméra de l'ordinateur avec la position du tracker. 
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

time.sleep(3)
cam = cv2.VideoCapture(0)
tracker = cv2.TrackerKCF_create()
ok, frame = cam.read()

face_cascade = cv2.CascadeClassifier('E:/Projet/Camera/haarcascade_frontalface_default.xml')
#On récupère le fichier de tracking du visage
gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY )
#On convertit les couleurs de l'image en niveau de gris pour faciliter la détection du corps du professeur. 
faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
#On récupère les coordonnées du visage du professeur.
for (x,y,w,h) in faces:
    cv2.rectangle(frame, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
    bbox = (x-w,y,3*w,4*h)  # Si visage, on renvoie un tableau contenant les coordonnées et dimensions du haut du corps

cv2.imshow('image',frame) #On affiche la frame qui est une fenêtre de "validation" pour  voir si le tracking à bien été réalisé.
cv2.waitKey(0)  #On doit appuyer sur entrer pour valider la position du rectangle.
time.sleep(3)
ok = tracker.init(frame, bbox) 
cv2.destroyAllWindows() #On écrase cette fenêtre de "validation"  

while(True):
	ok, frame = cam.read() # On lit une frame
	ok, bbox = tracker.update(frame) # On met à jour le tracker
	if ok:   # Si on détecte un visage on affiche le rectangle correspondant. 
		p1 = (int(bbox[0]), int(bbox[1]))
		p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3]))
		cv2.rectangle(frame, p1,p2, (0,0,255),2,2)
	cv2.imshow("Tracking", frame) # En même temps, on affiche l'image de la caméra pour avoir un retour
	k = cv2.waitKey(1) & 0xFF
	if k == 27: # echap pour quitter
			print("ESC hit, closing...")
			break

cam.release()
cv2.destroyAllWindows()
