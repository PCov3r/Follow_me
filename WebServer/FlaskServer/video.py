import cv2
from imutils.video.pivideostream import PiVideoStream
import imutils
import time
import numpy as np

class VideoCamera(object):
    def __init__(self, flip = False): #On lance l'aquisition du flux vidéo sans retournement d'image. 
        self.vs = PiVideoStream().start()
        self.flip = flip
        time.sleep(2.0)

    def __del__(self): #On stop l'aquisition du flux vidéo.
        self.vs.stop()

    def flip_if_needed(self, frame):  #On retourne l'image si nécessaire. 
        if self.flip:
            return np.flip(frame, 0)
        return frame

    def get_frame(self):    #On récupère le flux vidéo. 
        frame = self.flip_if_needed(self.vs.read())
        ret, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()
