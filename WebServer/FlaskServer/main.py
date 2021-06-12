# Script principal générant la page web et gérant le tracking vidéo et le mode manuel #

#!/usr/bin/env python
# -*-coding:Latin-1 -* 
from flask import Flask, render_template, Response, request
from camera import VideoCamera
from uartcom import sendData
from focus_lib import setFocus
import time
import threading
import io
import os
import cv2

override = 1 #Variable contenant la configuration manuelle (1)/automatique (0)
app = Flask(__name__)
camera = VideoCamera(flip=False) #Possibilité de tourner l'image si cette dernière est inversée


@app.route('/') #Page d'accueil du serveur Flask
def index():
    """Video streaming home page."""
    return render_template('index.html',state=override)


def init(param): #Fonction d'initialisation du tracking
    bounding = (0,0,0,0) #Initialisation de la boite de tracking
    face_cascade = cv2.CascadeClassifier('../haarcascade_frontalface_default.xml') #Récupération du fichier de tracking du visage
    gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY ) #passe en noir et blanc de l'image donnée en paramètre
    faces = face_cascade.detectMultiScale(gray, 1.1 , 4) #Recherche d'un visage dans l'image
    for (x,y,w,h) in faces:
        #cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2) #Décommentez cette ligne si vous voulez faire apparaitre le rectangle de détection
        bounding = (x-w,y,3*w,4*h) # Si visage, on renvoie une boite contenant les coordonnées et dimensions du haut du corps
        print("visage trouvé")
    return(bounding)


def gen():
    #setFocus(camera)
    """Video streaming generator function."""
    while True:
        if not override:
            tracker = cv2.TrackerKCF_create() # On crée un tracker
            frame = camera.get_frame() # On lit une frame
            bbox = init(frame) # On essaye d'y trouver une personne
            while bbox == (0,0,0,0) : # Si on a aucune personne, on recommence la recherche en boucle
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                bbox=init(frame)
            ok = tracker.init(frame, bbox) # Une fois la personne trouvée, on lance le tracking
            a=0
            while(not override): # Tant que l'utilisateur n'est pas passé en manuel, on continue le tracking
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n') #On renvoie le flux vidéo au serveur
                ok, bbox = tracker.update(frame) # On met à  jour le tracker avec le flux video
                if ok:   # La personne est bien détectée
                    p1 = (int(bbox[0]), int(bbox[1])) # On récupère les coordonnées du rectangle de tracking
                    p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # On récupère la largeur et hauteur du rectangle
                    cam_width = camera.get(3)
                    cam_height = camera.get(4) # On récupère les dimensions de la vidéo
                    xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) #Récupère la position par rapport au centre (largeur)
                    ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2)) #Récupère la position par rapport au centre (hauteur)
                    zoom = 0
                    if(bbox[3]>3/4*cam_height): # Si le rectangle de détection occupe plus des 3/4 de la hauteur
                        zoom=-1 #Il faut dézoomer
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2]) # On récupère le pourcentage de l'écran occupé par la personne (non utilisé par la suite)
                    elif(bbox[3]<1/2*cam_height): # Si le rectangle de détection occupe moins des 1/2 de la hauteur
                        zoom=1 #Il faut zoomer
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2])
                    else:
                        zoom=0
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2])
                    
                    trame = [255,255,255,255,255] # Contient la trame UART envoyée aux servos [x,y,zoom,focus1,focus2]. La valeur par défaut (aucun mouvement) est 255.
                    if (xpos>20) : #Si la personne est trop sur la droite
                         trame[0]=5 
                    elif (xpos<-20) :  #Si la personne est trop sur la gauche
                         trame[0]=4

                    if (ypos>20) : #Si la caméra pointe trop vers le haut
                          trame[1]=2
                    elif (ypos<-20) : #Si la caméra pointe trop vers le bas
                         trame[1]=3

                    if (zoom==1) : #Si on doit zoomer
                         trame[2]=1
                         sendData(trame) #On envoie la trame en UART
                         #setFocus(camera) #On fait le focus qui a du changer avec le zoom (commenté ici car mal implémenté)
                    elif (zoom==-1) : #Si on doit dézoomer
                         trame[2]=0
                         sendData(trame)
                         #setFocus(camera)
                    else :
                         sendData(trame)

                t0=time.time()
                while (not ok): # On a perdu la personne
                    frame = camera.get_frame()
                    encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                    io_buf = io.BytesIO(image_buffer)
                    yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                    if(time.time()-t0)>5: # Si cela fait plus de 5s, il faut relancer le tracking
                        tracker = cv2.TrackerKCF_create() # On recrée un tracker
                        bbox =init(frame)
                        while bbox == (0,0,0,0) : # Si on a aucune personne de détecté, on recommence la recherche en boucle
                            if override: # On passe en manuel
                                a=1
                                break
                            frame = camera.get_frame()
                            encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                            io_buf = io.BytesIO(image_buffer)
                            yield (b'--frame\r\n'
                                b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                            bbox=init(frame)
                        if(a) :
                            break
                        ok = tracker.init(frame, bbox) # On remplace le tracker précédent par le nouveau lorsqu'on a trouvé la personne
                    ok, bbox = tracker.update(frame) # On met à  jour le tracker
                    if override: # passage en manuel
                        break

        while override: #En mode manuel, on se contente de renvoyer le flux vidéo
            frame = camera.get_frame()
            encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
            io_buf = io.BytesIO(image_buffer)
            yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')


@app.route('/video_feed') #Chemin du flux vidéo
def video_feed():
    """Video streaming route."""
    return Response(
        gen(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

@app.route('/m_override/',methods=['GET']) #Chemin permettant de basculer de manuel à automatique et inversement
def m_override():
    global override
    etat = request.args.get('state') #On récupère l'argument state de la requête : /m_override/?state=
    if (etat == "1") :
        override = 1
        return("Manual override activated")
    else :
        override = 0
        return("Manual override off")

@app.route('/m_control/',methods=['GET']) #Chemin d'acquisition des commandes en manuel
def m_control():
    xpos=request.args.get('x')
    ypos=request.args.get('y')
    zoom=request.args.get('zoom') #On récupère tous les arguments x,y et zoom : /mcontrol/?x=&y=&zoom=
    print(xpos,ypos,zoom)
    if(override == 1): #Si on est en mode manuel, on traite les commandes entrantes
        trame = [255,255,255,255,255]
        if (xpos=="1") :
            trame[0]=4
        elif (xpos=="-1") :
            trame[0]=5

        if (ypos=="-1") : 
            trame[1]=3
        elif (ypos=="1") :
            trame[1]=2

        if (zoom=="1") :
            trame[2]=1
            sendData(trame)
            #setFocus(camera)
        elif (zoom=="-1") : 
            trame[2]=0
            sendData(trame)
            #setFocus(camera)
        else :
            sendData(trame)

    return("Got "+xpos+", "+ypos+", "+zoom)


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=False, threaded=True)


