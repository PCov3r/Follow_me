# Camera functions #

import cv2
from imutils.video.pivideostream import PiVideoStream
import imutils
import time
import numpy as np

class VideoCamera(object):
    def __init__(self, flip = False): # Initiate video stream and flip image if needed (default: False) 
        self.vs = PiVideoStream().start()
        self.flip = flip
        time.sleep(2.0)

    def __del__(self): # Stop video stream
        self.vs.stop()

    def flip_if_needed(self, frame):  # Flip camera
        if self.flip:
            return np.flip(frame, 0)
        return frame

    def get_frame(self):    # Get camera frame
        frame = self.flip_if_needed(self.vs.read())
        ret, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()
