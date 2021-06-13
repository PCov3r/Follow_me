#!/bin/bash

sudo apt-get update && sudo apt-get upgrade
wget https://bootstrap.pypa.io/get-pip.py
sudo python3 get-pip.py
sudo pip3 install pyserial
sudo pip3 install pycurl
