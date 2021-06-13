# Script des fonctions l'acquisition du flux vidéo #

import cv2
from imutils.video.pivideostream import PiVideoStream
import imutils
import time
import numpy as np

class VideoCamera(object):
    def __init__(self, flip = False): #On lance l'aquisition du flux vidéo avec possibilité d'inverser l'image (valeur par défaut : False) 
        self.vs = PiVideoStream().start()
        self.flip = flip
        time.sleep(2.0)

    def __del__(self): #On stoppe l'aquisition du flux vidéo.
        self.vs.stop()

    def flip_if_needed(self, frame):  #On retourne l'image si nécessaire. 
        if self.flip:
            return np.flip(frame, 0)
        return frame

    def get_frame(self):    #On récupère le flux vidéo. 
        frame = self.flip_if_needed(self.vs.read())
        return frame