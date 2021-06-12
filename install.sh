#!/bin/bash

sudo apt-get update && sudo apt-get upgrade
mkdir ~/src && cd ~/src
wget https://bootstrap.pypa.io/get-pip.py
sudo python3 get-pip.py
sudo pip3 install opencv-contrib-python==4.4.0.46
sudo pip3 install Flask
sudo pip3 install imutils
sudo apt-get install python-picamera python3-picamera
