# Main script generating the web page and managing video tracking and manual mode #

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

override = 1 # Variable containing manual (1) / automatic (0) configuration
app = Flask(__name__)
camera = VideoCamera(flip=False) # Possibility to rotate the image if inverted

@app.route('/') # Main page of Flask server
def index():
    """Video streaming home page."""
    return render_template('index.html',state=override)

## Get tracking box
def init(param):
	bounding = (0,0,0,0) # bounding will contain the upper part of the detected body 
	face_cascade = cv2.CascadeClassifier('../haarcascade_frontalface_default.xml')
	# Convert to gray scale to ease detection
	gray = cv2.cvtColor(param, cv2.COLOR_BGR2GRAY )
	faces = face_cascade.detectMultiScale(gray, 1.1 , 4)
	# Get coordinates, width and heigth of the detected face
	for (x,y,w,h) in faces:
		#cv2.rectangle(param, (x-w,y), (x+2*w, y+4*h), (12,150,100),2)
		bounding = (x-w,y,3*w,4*h)  # bounding contains the coordinates and size of the upper part of the body
		print("visage trouvé")
	return(bounding)


def gen():
    #setFocus(camera)
    """Video streaming generator function."""
    while True:
        if not override:
            tracker = cv2.TrackerKCF_create() # Create tracker
            frame = camera.get_frame() # Read frame
            bbox = init(frame) # Try to find someone
            while bbox == (0,0,0,0) : # If none, loop
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                bbox=init(frame)
            ok = tracker.init(frame, bbox) # When found, launch tracking
            a=0
            while(not override): # While not manual control
                frame = camera.get_frame()
                encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                io_buf = io.BytesIO(image_buffer)
                yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n') # Web page get video stream
                ok, bbox = tracker.update(frame) # Update tracker
                if ok:   # Someone is still detected
                    p1 = (int(bbox[0]), int(bbox[1])) # Get tracker rectangle coordinates
                    p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3])) # Get tracker rectangle width and length
                    cam_width = camera.get(3)
                    cam_height = camera.get(4) # Get video resolution
                    xpos = int(cam_width/2-(bbox[0]+bbox[2]/2)) # Get position from center (x axis)
                    ypos = -int(cam_height/2-(bbox[1]+bbox[3]/2)) # Get position from center (y axis)
                    zoom = 0
                    if(bbox[3]>3/4*cam_height): # If tracker rectangle is greater than 3/4 of height
                        zoom=-1 # We need to unzoom
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2]) # We recover the percentage of the screen occupied by the person (not used subsequently)
                    elif(bbox[3]<1/2*cam_height): # If tracker rectangle is less than 1/2 of heigth
                        zoom=1 # We need to zoom
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2])
                    else:
                        zoom=0
                        zoom_percent = (cam_width*cam_height-bbox[3]*bbox[2])/(bbox[3]*bbox[2])
                    
                    trame = [255,255,255,255,255] # Contains the UART frame sent to the servos [x, y, zoom, focus1, focus2]. The default (no movement) is 255.
                    if (xpos>20) : # If the person is too much on the right
                         trame[0]=5 
                    elif (xpos<-20) :  # If the person is too much on the left
                         trame[0]=4

                    if (ypos>20) : # If the camera is pointing up too much
                          trame[1]=2
                    elif (ypos<-20) : # If the camera is pointing down too much
                         trame[1]=3

                    if (zoom==1) : # If zoom is needed
                         trame[2]=1
                         sendData(trame) # We sent the x, y and zoom movement
                         #setFocus(camera) # We make the focus which must have changed with the zoom (commented here because poorly implemented)
                    elif (zoom==-1) : # If unzoom is needed
                         trame[2]=0
                         sendData(trame)
                         #setFocus(camera)
                    else :
                         sendData(trame)

                t0=time.time()
                while (not ok): # Lost person
                    frame = camera.get_frame()
                    encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
                    io_buf = io.BytesIO(image_buffer)
                    yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')
                    if(time.time()-t0)>5: # If 5s and still no one, relaunch tracking
                        tracker = cv2.TrackerKCF_create() # Create a tracker once again
                        bbox =init(frame)
                        while bbox == (0,0,0,0) : # While not detected
                            if override: # If manual override
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
                        ok = tracker.init(frame, bbox) # Old tracker get replaced
                    ok, bbox = tracker.update(frame) # Update tracker
                    if override: # Manual override
                        break

        while override: # In manual mode, we just send the video stream
            frame = camera.get_frame()
            encode_return_code, image_buffer = cv2.imencode('.jpg', frame)
            io_buf = io.BytesIO(image_buffer)
            yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + io_buf.read() + b'\r\n')


@app.route('/video_feed') # Video stream path
def video_feed():
    """Video streaming route."""
    return Response(
        gen(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

@app.route('/m_override/',methods=['GET']) # API to switch from manual to auto back and forth
def m_override():
    global override
    etat = request.args.get('state') # We get arg of the HTTP request : /m_override/?state=
    if (etat == "1") :
        override = 1
        return("Manual override activated")
    else :
        override = 0
        return("Manual override off")

@app.route('/m_control/',methods=['GET']) # API to control the camera movements
def m_control():
    xpos=request.args.get('x')
    ypos=request.args.get('y')
    zoom=request.args.get('zoom') # We get args of the HTTP request x,y & zoom : /mcontrol/?x=&y=&zoom=
    print(xpos,ypos,zoom)
    if(override == 1): # While manual override, command get treated and send
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


