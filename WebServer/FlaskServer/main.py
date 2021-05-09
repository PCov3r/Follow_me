#!/usr/bin/env python
from flask import Flask, render_template, Response, request
from camera import VideoCamera
import time
import threading
import io
import os
import cv2

override = 1
app = Flask(__name__)
camera = VideoCamera(flip=False) # flip pi camera if upside down.


@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')


def init(param):
    bounding = (0,0,0,0)
    face_cascade = cv2.CascadeClassifier('/home/pi/camera/flaskr/haarcascade_frontalface_default.xml')
    gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )
    faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
    for (x,y,w,h) in faces:
        #cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
        bounding = (x-w,y,3*w,4*h) # Si visage, on renvoie une boite contenant les coordonnÃ©es et dimensions du haut du corps
        print("visage trouvÃ©")
    return(bounding)


def gen():
    """Video streaming generator function."""
    while True:
        if not override:
            tracker = cv2.TrackerKCF_create() # On crÃ©e un tracker
            frame = camera.get_frame() # On lit une frame
            bbox = init(frame) # On essaye d'y trouver une personne
            while bbox == (0,0,0,0) : # Si on a aucune personne, on recommence la recherche en boucle
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                bbox=init(frame)
            ok = tracker.init(frame, bbox) # Une fois la personne trouvÃ©, on lance le tracking
            a=0
            while(not override): # Tant que l'utilisateur n'a pas pressÃ© Echap, on recommence
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                ok, bbox = tracker.update(frame) # On met Ã  jour le tracker avec le flux video
                if ok:   # La personne est bien dÃ©tectÃ©e
                    p1 = (int(bbox[0]), int(bbox[1])) # On rÃ©cupÃ¨re les coordonnÃ©es du rectangle de tracking
                    p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # On rÃ©cupÃ¨re la largeur et hauteur du rectangle
                    cam_width = camera.get(3)
                    cam_height = camera.get(4) # On rÃ©cupÃ¨re les dimensions de la vidÃ©o
                    xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) #RÃ©cupÃ¨re la position par rapport au centre (largeur)
                    ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2)) #RÃ©cupÃ¨re la position par rapport au centre (hauteur)
                    zoom = 0
                    if(bbox[3]>3/4*cam_height): # Si le rectangle de dÃ©tection occupe plus des 3/4 de la hauteur
                        zoom=-1
                    elif(bbox[3]<1/2*cam_height): # Si le rectangle de dÃ©tection occupe moins des 1/2 de la hauteur
                        zoom=1
                    else:
                        zoom=0

                t0=time.time()
                while (not ok): # On a perdu la personne
                    frame = camera.get_frame()
                    encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                    io_buf = io.BytesIO(image_buffer)
                    yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                    if(time.time()-t0)>5: # Si cela fait plus de 5s, il faut relancer le tracking
                        tracker = cv2.TrackerKCF_create() # On recrÃ©e un tracker
                        bbox =init(frame)
                        while bbox == (0,0,0,0) :
                            if override: # q for quitting
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
                        ok = tracker.init(frame, bbox) # On rÃ©initialise le tracker lorsqu'on a trouvÃ© la personne
                    ok, bbox = tracker.update(frame) # On met Ã  jour le tracker
                    if override: # q for quitting
                        break

        while override:
            frame = camera.get_frame()
            encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
            io_buf = io.BytesIO(image_buffer)
            yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')


@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    return Response(
        gen(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

@app.route('/m_override',methods=['GET'])
def m_override():
    # Send some command to camera script to enable manual override
    global override
    override = not override
    if override :
        return("Manual override activated")
    else :
        return("Manual override off")

@app.route('/m_control/',methods=['GET'])
def m_control():
    xpos=request.args.get('x')
    ypos=request.args.get('y')
    zoom=request.args.get('zoom')
    print(xpos,ypos,zoom)
    return("Got "+xpos+", "+ypos+", "+zoom)


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=False, threaded=True)


