# Script permettant de lancer le stream via YT
# Pour cela il faut créer une clé de stream depuis son compte et la coller dans KEY
# /!\ Ce script utilise la librairie picamera et n'est donc pas pour le moment utilisable avec le script Flask générant la page web : main.py /!\

import subprocess 
import picamera 
import time 
YOUTUBE="rtmp://a.rtmp.youtube.com/live2/" 
KEY= #La clé va ici# 
# On lance le stream
stream_cmd = 'ffmpeg -f h264 -r 25 -i - -itsoffset 5.5 -fflags nobuffer -f alsa -ac 1 -i hw:1,0 -vcodec copy -acodec aac -ac 1 -ar 8000 -ab 32k -map 0:0 -map 1:0 -strict experimental -f flv ' + YOUTUBE + KEY 
stream_pipe = subprocess.Popen(stream_cmd, shell=True, stdin=subprocess.PIPE) 
camera = picamera.PiCamera(resolution=(640, 480), framerate=25) 
try: # On lance l'acquisition caméra
  now = time.strftime("%Y-%m-%d-%H:%M:%S") 
  camera.framerate = 25 
  camera.vflip = True 
  camera.hflip = True 
  camera.start_recording(stream.stdin, format='h264', bitrate = 2000000) 
  while True: 
     camera.wait_recording(1) 
except KeyboardInterrupt: # ctrl+C pour arrêter la caméra
     camera.stop_recording() 
finally: # Fermeture du stream 
  camera.close() 
  stream.stdin.close() 
  stream.wait() 
  print("Caméra éteinte et stream coupé") 
