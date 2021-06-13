# Script to launch the stream via YT
# For that you have to create a stream key from your account and paste it in KEY
# /!\ This script uses the picamera library and is therefore not currently usable with the Flask script generating the web page: main.py /!\
# Script from https://learn.adafruit.com/ultimate-youtube-live-camera/streaming-to-youtube

import subprocess 
import picamera 
import time 
YOUTUBE="rtmp://a.rtmp.youtube.com/live2/" 
KEY= # Key goes here #
# Launching live streaming
stream_cmd = 'ffmpeg -f h264 -r 25 -i - -itsoffset 5.5 -fflags nobuffer -f alsa -ac 1 -i hw:1,0 -vcodec copy -acodec aac -ac 1 -ar 8000 -ab 32k -map 0:0 -map 1:0 -strict experimental -f flv ' + YOUTUBE + KEY 
stream_pipe = subprocess.Popen(stream_cmd, shell=True, stdin=subprocess.PIPE) 
camera = picamera.PiCamera(resolution=(640, 480), framerate=25) 
try: # Launching camera
  now = time.strftime("%Y-%m-%d-%H:%M:%S") 
  camera.framerate = 25 
  camera.vflip = True 
  camera.hflip = True 
  camera.start_recording(stream.stdin, format='h264', bitrate = 2000000) 
  while True: 
     camera.wait_recording(1) 
except KeyboardInterrupt: # Wait for crtl+C to stop camera
     camera.stop_recording() 
finally: # Closing live stream
  camera.close() 
  stream.stdin.close() 
  stream.wait() 
  print("Live stream shutdown") 
